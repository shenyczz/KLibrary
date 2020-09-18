// src\KTreeCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KTreeCtrl.h"

namespace Gui
{

	// KTreeCtrl

	IMPLEMENT_DYNAMIC(KTreeCtrl, CTreeCtrl)

	KTreeCtrl::KTreeCtrl()
	{
		m_bColorFont = FALSE;		// �Ƿ��ɫ�ı�
		m_bMultiSelect = FALSE;		// �Ƿ��ѡ

		m_cxBitmap = 0;			//����λͼ�Ŀ��
		m_cyBitmap = 0;			//����λͼ�ĸ߶�

		m_mapColorFont.RemoveAll();
		m_uTimerID = 0;
		m_hItemFirstSel = NULL;		// �ڶ�ѡ״̬��ѡ�еĵ�һ����Ŀ

		m_pTreeCtrlChecked = new TreeCtrlChecked();

		m_pOwner = NULL;

		m_uMenuID = 0;
	}

	KTreeCtrl::~KTreeCtrl()
	{
		m_ImageListState.DeleteImageList();
		m_mapColorFont.RemoveAll();
		delete m_pTreeCtrlChecked;
		m_pTreeCtrlChecked = NULL;
	}


	BEGIN_MESSAGE_MAP(KTreeCtrl, CTreeCtrl)
		ON_WM_PAINT()
		ON_WM_ERASEBKGND()
		ON_WM_TIMER()
		ON_WM_QUERYNEWPALETTE()
		ON_WM_HSCROLL()
		ON_WM_VSCROLL()
		ON_WM_LBUTTONDOWN()
		ON_WM_RBUTTONDOWN()
		ON_WM_CONTEXTMENU()
		ON_NOTIFY_REFLECT(NM_CLICK, &KTreeCtrl::OnNMClick)
		ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, &KTreeCtrl::OnTvnItemexpanding)
		ON_NOTIFY_REFLECT(TVN_SELCHANGED, &KTreeCtrl::OnTvnSelchanged)
	END_MESSAGE_MAP()



	// KTreeCtrl ��Ϣ�������

	// �ڴ��ڱ����໯֮ǰ�ɿ�ܵ���,����������Ҫ�����໯
	// �ڶ��󸽼ӵ����д��ں����
	void KTreeCtrl::PreSubclassWindow()
	{
		// TODO: �ڴ����ר�ô����/����û���

		DWORD dwAdd= 0
			| TVS_HASLINES
			| TVS_HASBUTTONS
			| TVS_SHOWSELALWAYS
			| TVS_LINESATROOT
			| TVS_CHECKBOXES
			;

		DWORD dwRemove = 0
			//| TVS_DISABLEDRAGDROP
			;
	 
		ModifyStyle(dwRemove,dwAdd);

		SetBkColor(::GetSysColor(COLOR_WINDOW));
		SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));

		CTreeCtrl::PreSubclassWindow();
	}

	// ��Windows��������֮ǰ����,���ڸ��� CWnd
	BOOL KTreeCtrl::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CTreeCtrl::PreCreateWindow(cs))
			return FALSE;

		cs.style = cs.style
			| TVS_HASLINES
			| TVS_HASBUTTONS
			| TVS_SHOWSELALWAYS
			| TVS_LINESATROOT
			| TVS_CHECKBOXES
			;

		cs.style &= ~TVS_DISABLEDRAGDROP;

		return TRUE;
	}

	void KTreeCtrl::OnPaint()
	{
		CPaintDC dc(this); // device context for painting
		// TODO: �ڴ˴������Ϣ����������
		// ��Ϊ��ͼ��Ϣ���� CTreeCtrl::OnPaint()
		//-----------------------------
		//�ͻ�������
		CRect rcClient;
		GetClientRect(&rcClient);
		//-----------------------------
		//�����ڴ���� DC ���ڴ����λͼ
		CDC memDC;
		CBitmap bitmap;
		if(memDC.GetSafeHdc()==NULL)
		{
			memDC.CreateCompatibleDC(&dc);
			bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
		}
		memDC.SetBkMode(TRANSPARENT);
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

		//���ÿؼ�ִ��Ĭ�ϻ���
		CWnd::DefWindowProc(WM_PAINT,(WPARAM)memDC.m_hDC,(LPARAM)0 );
		//-----------------------------
		CRect rcClip;
		dc.GetClipBox(&rcClip);	//�ؼ��ļ�����(�����ɼ�item�ľ���)
		//-----------------------------
		//��ɫ�ı�
		if(m_bColorFont)
		{
			this->DrawColorFont(&memDC);

			dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
				&memDC, rcClip.left, rcClip.top, SRCCOPY);
		}
		//-----------------------------
		//���Ʊ���ͼƬ
		if(m_bitmap.m_hObject != NULL)
		{
			this->DrawBackImage(&dc,&memDC);
		}
		else
		{
			dc.BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
				&memDC, rcClip.left, rcClip.top, SRCCOPY );
		}
		//-----------------------------
		//Cleanup GDI
		memDC.SelectObject(pOldBitmap);
		memDC.DeleteDC();
		bitmap.DeleteObject();
	}
	//���Ʋ�ɫ�ı�
	void KTreeCtrl::DrawColorFont(CDC* pDC)
	{
		int iOffsetOfTop=GetImageList(TVSIL_NORMAL) ? 2 : 1;
		//if(GetImageList())
		//-----------------------------
		//�ɼ� item ����
		int n = GetVisibleCount()+1;

		//ȡ�õ�һ���ɼ���item
		HTREEITEM hItem = GetFirstVisibleItem();
		//-----------------------------
		CFont *pFontDC;
		CFont fontDC;
		LOGFONT logfont;
		while(hItem && n--)
		{
			CRect rect;	//item �ľ���

			//������ѡ�еĻ������ item ���ı���ɫ
			UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
			ColorFont colorFont;

			if(!(GetItemState( hItem, selflag ) & selflag )	//���Ǳ�ѡ�еĻ����item
				&& m_mapColorFont.Lookup( hItem,colorFont))		//�����ҵ���
			{
				//ȡ���߼�����
				if( colorFont.logfont.lfFaceName[0] != '\0' ) 
				{
					//ָ��������
					logfont = colorFont.logfont;
				}
				else
				{
					//û��ָ������,ʹ��windowsϵͳ����
					CFont *pFont = GetFont();
					pFont->GetLogFont(&logfont);
				}

				CString sItem = GetItemText( hItem );	//ȡ�� item ���ı�

				BOOL bTextOnly = TRUE;
				GetItemRect( hItem,&rect,bTextOnly);		//ȡ�� item ���ı�ʹ�õľ���
				
				//����ԭ���ı���
				CFont* pOldFont = pDC->SelectObject(GetFont());
				//�����ı���ɫ�ͱ�����ɫһ��
				pDC->SetTextColor(GetBkColor());
				pDC->TextOut( rect.left+2, rect.top+iOffsetOfTop, sItem );	//why plus 2 ???
				pDC->SelectObject(pOldFont);

				//�ж� item �Ƿ����
				if(IsItemBold(hItem))
				{
					logfont.lfWeight = FW_BOLD;
				}
				else
				{
					logfont.lfWeight = FW_NORMAL;
				}

				//�����ڴ����岢ѡ���ڴ� DC
				fontDC.DeleteObject();
				fontDC.CreateFontIndirect(&logfont);
				pFontDC = pDC->SelectObject(&fontDC);

				if(colorFont.color != (COLORREF)(-1))
				{
					//memDC.SetBkColor(GetBkColor());
					pDC->SetTextColor(colorFont.color);
				}
				else
				{
					//memDC.SetBkColor(GetBkColor());
					pDC->SetTextColor(GetTextColor());
				}

				pDC->SetBkMode(TRANSPARENT);
				pDC->TextOut( rect.left+2, rect.top+iOffsetOfTop,sItem);
				
				//Cleanup GDI
				pDC->SelectObject(pFontDC);
				fontDC.DeleteObject();
			}//end if

			hItem = GetNextVisibleItem(hItem);
		}//end while
		//-----------------------------
		//Cleanup GDI
		fontDC.DeleteObject();
	}
	//���Ʊ���ͼƬ
	void KTreeCtrl::DrawBackImage(CDC* pDevDC,CDC* pMemDC)
	{
		int i,j;

		//�ͻ�������
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcClip;
		pDevDC->GetClipBox(&rcClip);	//�ؼ��ļ�����(�����ɼ�item�ľ���)

		// Now create a mask
		CDC maskDC;
		//maskDC.CreateCompatibleDC(&dc);
		maskDC.CreateCompatibleDC(pDevDC);
		CBitmap maskBitmap,bmpImage;

		// Create monochrome bitmap for the mask
		maskBitmap.CreateBitmap( rcClient.Width(), rcClient.Height(), 
						1, 1, NULL );
		maskDC.SelectObject( &maskBitmap );
		//pMemDC->SetBkColor(::GetSysColor(COLOR_WINDOW));
		pMemDC->SetBkColor(GetBkColor());

		// Create the mask from the memory DC
		maskDC.BitBlt( 0, 0, rcClient.Width(), rcClient.Height(),pMemDC, 
					rcClient.left, rcClient.top, SRCCOPY );

		
		CDC tempDC;
		//tempDC.CreateCompatibleDC(&dc);
		tempDC.CreateCompatibleDC(pDevDC);
		tempDC.SelectObject(&m_bitmap);

		CDC imageDC;
		//CBitmap bmpImage;
		imageDC.CreateCompatibleDC(pDevDC);
		bmpImage.CreateCompatibleBitmap(pDevDC, rcClient.Width(), 
						rcClient.Height() );
		imageDC.SelectObject( &bmpImage );

		if((pDevDC->GetDeviceCaps(RASTERCAPS) & RC_PALETTE)
			&&m_pal.m_hObject != NULL )
		{
			pDevDC->SelectPalette( &m_pal, FALSE );
			pDevDC->RealizePalette();

			imageDC.SelectPalette( &m_pal, FALSE );
		}

		// Get x and y offset
		CRect rcRoot(0,0,0,0);
		HTREEITEM hRoot = GetRootItem();
		TRACE("hRoot = %p\n",hRoot);
		if(hRoot)
		{
			GetItemRect( hRoot, rcRoot, FALSE );
		}
		rcRoot.left = -GetScrollPos(SB_HORZ);

		TRACE("hRoot = %p\n",hRoot);
		TRACE("rcRoot = %d %d\n",rcRoot.left,rcRoot.top);

		// Draw bitmap in tiled manner to imageDC
		for(i=rcRoot.left; i<rcClient.right; i+=m_cxBitmap )
		{
			for(j=rcRoot.top; j<rcClient.bottom; j+=m_cyBitmap )
			{
				imageDC.BitBlt( i,j,m_cxBitmap,m_cyBitmap,&tempDC,0,0,SRCCOPY );
			}
		}

		// Set the background in memDC to black. Using SRCPAINT with black and any other
		// color results in the other color, thus making black the transparent color
		pMemDC->SetTextColor(GetSysColor(COLOR_WINDOW));
		pMemDC->SetBkColor(GetSysColor(COLOR_WINDOWTEXT));
		pMemDC->BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &maskDC, 
				rcClip.left, rcClip.top, SRCAND);

		// Set the foreground to black. See comment above.
		pMemDC->SetBkColor(GetSysColor(COLOR_WINDOW));
		pMemDC->SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
		imageDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), &maskDC, 
				rcClip.left, rcClip.top, SRCAND);

		// Combine the foreground with the background
		imageDC.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), 
					pMemDC, rcClip.left, rcClip.top,SRCPAINT);

		// Draw the final image to the screen		
		pDevDC->BitBlt( rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(), 
					&imageDC, rcClip.left, rcClip.top, SRCCOPY );
		//--------------------------------
		//Cleanup GDI
		maskDC.DeleteDC();
		tempDC.DeleteDC();
		imageDC.DeleteDC();

		maskBitmap.DeleteObject();
		bmpImage.DeleteObject();
		
	}

	// ��ֹ��˸
	BOOL KTreeCtrl::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
		//return CTreeCtrl::OnEraseBkgnd(pDC);
		return TRUE;
	}

	// �����ڽ��ս���ʱ,������ʵ�����߼���ɫ��
	BOOL KTreeCtrl::OnQueryNewPalette()
	{
		CClientDC dc(this);

		if( (dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)	//Ӳ���豸֧�ֵ�ɫ��
			&& m_pal.m_hObject != NULL )				//�����е�ɫ��
		{
			dc.SelectPalette( &m_pal, FALSE );
			BOOL result = dc.RealizePalette();

			if( result ) Invalidate();

			return result;
		}

		return CTreeCtrl::OnQueryNewPalette();
	}

	// ˮƽ������
	void KTreeCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		if(m_bitmap.m_hObject != NULL )
		{
			InvalidateRect(NULL,false);
		}

		//�������
		CTreeCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	}
	// ��ֱ������
	void KTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		if(m_bitmap.m_hObject != NULL )
		{
			InvalidateRect(NULL,false);
		}

		//�������
		CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	}

	// ������
	void KTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
	{
		//-----------------------------
		//�����ѡ
		if(m_bMultiSelect)	//�����ѡ
		{
			if(nFlags & MK_CONTROL ) //ͬʱ���� Ctrl ��
			{
				UINT flag;

				//Returns the current position of the cursor related to the CTreeCtrl object
				HTREEITEM hItem = HitTest(point,&flag );

				if(hItem)
				{
					//��Ŀ��ѡ��״̬
					UINT uItemSelState = GetItemState(hItem, TVIS_SELECTED);

					//��Ŀ����״̬(ԭ��ûѡ����������Ϊѡ��,ԭ��ѡ����������Ϊûѡ��)
					UINT uNewSelState = (uItemSelState & TVIS_SELECTED) ? 0 : TVIS_SELECTED;

					//ȡ��ԭ����ѡ�е���Ŀ��״̬
					HTREEITEM hItemOld = GetSelectedItem();
					UINT uOldSelState  = hItemOld ? GetItemState(hItemOld, TVIS_SELECTED) : 0;

					//ѡ������Ŀ
					if( GetSelectedItem() == hItem )
						SelectItem(NULL);	//��ֹ�༭����
					CTreeCtrl::OnLButtonDown(nFlags, point);

					//����ѡ����Ŀ���ʵ�״̬(ԭ��ûѡ����������Ϊѡ��,ԭ��ѡ����������Ϊûѡ��)
					SetItemState(hItem, uNewSelState,  TVIS_SELECTED);

					// Restore state of old selected item
					//�ָ�ԭ����ѡ�е���Ŀ��״̬
					if (hItemOld && (hItemOld != hItem))
						SetItemState(hItemOld, uOldSelState, TVIS_SELECTED);

					//???
					m_hItemFirstSel = NULL;
					
					return;
				}
			}
			else if(nFlags & MK_SHIFT)	//ͬʱ���� Shift��
			{
				UINT flag;

				//Returns the current position of the cursor related to the CTreeCtrl object
				HTREEITEM hItem = HitTest(point,&flag );

				//�õ���һ��ѡ�е� item
				if(!m_hItemFirstSel)
					m_hItemFirstSel = GetSelectedItem();

				//ѡ���µ� item
				if( GetSelectedItem() == hItem )
					SelectItem( NULL );			// to prevent edit
				CTreeCtrl::OnLButtonDown(nFlags, point);

				if(m_hItemFirstSel)
				{
					//SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo)
					SelectItems(m_hItemFirstSel, hItem );
					return;
				}
			}
			else
			{
				// Normal - remove all selection and let default 
				// handler do the rest
				ClearSelection();
				m_hItemFirstSel = NULL;
			}
		}
		//-----------------------------
		CTreeCtrl::OnLButtonDown(nFlags, point);
	}
	// ����Ҽ�
	void KTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
	{
		UINT flag;
		HTREEITEM hItem = HitTest( point, &flag );
		if(hItem)
		{
			ClearSelection();
			SelectItem(hItem);
		}

		this->ClientToScreen(&point);
		OnContextMenu(this->GetParent(),point);

		CTreeCtrl::OnRButtonDown(nFlags, point);
	}

	void KTreeCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
	{
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		*pResult = 0;

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		TRACE("[CSfxTreeCtrl::OnNMClick] x=%d y=%d\n",point.x,point.y);

		UINT uFlag;
		HTREEITEM htCurrent = HitTest(point,&uFlag);
		TRACE("[CSfxTreeCtrl::OnNMClick] uFlag=%d\n",uFlag);

		// ����Ŀ��״̬ͼ���ϵ���(��Ҫ���ڸı�ѡ��״̬)
		if(uFlag & TVHT_ONITEMSTATEICON)
		{
			m_point = point;
			if(!m_uTimerID)
			{
				m_uTimerID = (UINT)SetTimer(TREE_CTRL_CHECKBOX_TIMERID,10,NULL);
			}
		}
		// ����Ŀ�ı�ǩ�ϵ���(��Ҫ���ڸı�ѡ��״̬)
		else if(uFlag & TVHT_ONITEMLABEL)
		{
			m_point = point;
			if(!m_uTimerID)
			{
				m_uTimerID = (UINT)SetTimer(TREE_CTRL_LABEL_TIMERID,10,NULL);
			}
		}

		return;
	}

	void KTreeCtrl::OnTimer(UINT_PTR nIDEvent)
	{
		if(nIDEvent != m_uTimerID)
			return;
		//-----------------------------
		KillTimer(m_uTimerID);
		//-----------------------------
		HTREEITEM htCurrent;
		UINT uFlag;
		htCurrent=HitTest(m_point,&uFlag);
		//-----------------------------
		BOOL bCheck = GetCheck(htCurrent);		//�Ƿ�ѡ��
		SetAllChildCheck(htCurrent,bCheck);		//����������������check
		SetParentCheck(htCurrent);				//���ø�����check
		//-----------------------------
		m_pTreeCtrlChecked->pTreeCtrl = this;
		m_pTreeCtrlChecked->hTreeItem = htCurrent;
		m_pTreeCtrlChecked->point = m_point;
		m_pTreeCtrlChecked->dwItemData = (DWORD)GetItemData(htCurrent);

		WPARAM wParam = (WPARAM)0;
		LPARAM lParam = (LPARAM)(m_pTreeCtrlChecked);
		//-----------------------------
		CWnd* pWndParent = GetParent(); //ȡ�ø��״��ڣ�����CTreeView
		CWnd* pWndOwner = GetOwner();	//ȡ��ӵ���ߴ��ڣ�����CTreeView

		if(m_uTimerID==TREE_CTRL_CHECKBOX_TIMERID)
		{
			if(pWndParent->GetSafeHwnd())
			{
				pWndParent->PostMessage(WM_KIND_TREE_CTRL_CHECKBOX,wParam,lParam);	//���״��ڷ�����Ϣ
			}
			if(pWndOwner->GetSafeHwnd())
			{
				pWndOwner->PostMessage(WM_KIND_TREE_CTRL_CHECKBOX,wParam,lParam);	//��ӵ���ߴ��ڷ�����Ϣ
			}
		}
		else if(m_uTimerID==TREE_CTRL_LABEL_TIMERID)
		{
			if(pWndParent->GetSafeHwnd())
			{
				pWndParent->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//���״��ڷ�����Ϣ
			}
			if(pWndOwner->GetSafeHwnd())
			{
				pWndOwner->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//��ӵ���ߴ��ڷ�����Ϣ
			}
		}

 		//-----------------------------
		m_uTimerID = 0;

		return;
	}

	void KTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		*pResult = 0;

		if( m_bitmap.m_hObject != NULL )
		{
			InvalidateRect(NULL,FALSE);
		}

		return;
	}


	//���ñ���ͼ��
	BOOL KTreeCtrl::SetBkImage(LPCTSTR lpszResourceName)
	{
		int i;
		//-----------------------------
		//������ǵ�һ�ε���,ɾ�� GDI ����
		if(m_bitmap.m_hObject != NULL) m_bitmap.DeleteObject();
		if(m_pal.m_hObject != NULL) m_pal.DeleteObject();
		//-----------------------------
		HBITMAP hBitmap = (HBITMAP)::LoadImage(AfxGetInstanceHandle(), 
				lpszResourceName, IMAGE_BITMAP, 0,0, LR_CREATEDIBSECTION );

		if(hBitmap == NULL)return FALSE;

		m_bitmap.Attach(hBitmap);

		BITMAP bm;
		m_bitmap.GetBitmap( &bm );
		m_cxBitmap = bm.bmWidth;
		m_cyBitmap = bm.bmHeight;
		//-----------------------------
		//Ϊλͼ�����߼���ɫ��
		DIBSECTION ds;
		m_bitmap.GetObject(sizeof(DIBSECTION),&ds );

		BITMAPINFOHEADER bmInfo = ds.dsBmih;
		int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

		//ȡ������DC
		CClientDC dc(NULL);

		//�����ɫ������256,������ɫ���߼���ɫ��
		if( nColors > 256 )
		{
			m_pal.CreateHalftonePalette(&dc);
		}
		else
		{
			//��ȡλͼ��ɫ��
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);
			memDC.SelectObject(&m_bitmap);
			RGBQUAD *pRgbQuad = new RGBQUAD[nColors];
			::GetDIBColorTable(memDC,0,nColors,pRgbQuad);

			//�߼���ɫ��ߴ�
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY)*nColors);
			LOGPALETTE* pLogPal = (LOGPALETTE*)new BYTE[nSize];

			pLogPal->palVersion = 0x300;	//����
			pLogPal->palNumEntries = nColors;

			for(i=0; i<nColors; i++)
			{
				pLogPal->palPalEntry[i].peRed = pRgbQuad[i].rgbRed;
				pLogPal->palPalEntry[i].peGreen = pRgbQuad[i].rgbGreen;
				pLogPal->palPalEntry[i].peBlue = pRgbQuad[i].rgbBlue;
				pLogPal->palPalEntry[i].peFlags = 0;
			}

			//������ɫ�� CPalette
			m_pal.CreatePalette(pLogPal);

			//�ͷ��ڴ�
			delete[] pLogPal;
			delete[] pRgbQuad;
		}

		Invalidate(FALSE);
		//-----------------------------
		return TRUE;
	}
	BOOL KTreeCtrl::SetBkImage(UINT nIDResource)
	{
		return SetBkImage(MAKEINTRESOURCE(nIDResource));
	}

	//����/��ȡ item ��ɫ
	void KTreeCtrl::SetItemColor(HTREEITEM hItem, COLORREF color)
	{
		ColorFont tSfxColorFont;

		if(!m_mapColorFont.Lookup(hItem,tSfxColorFont))
		{
			tSfxColorFont.logfont.lfFaceName[0] = '\0';
		}

		tSfxColorFont.color = color;
		m_mapColorFont[hItem] = tSfxColorFont;
	}
	COLORREF KTreeCtrl::GetItemColor(HTREEITEM hItem)
	{
		COLORREF clrValue;
		//-----------------------------
		ColorFont tSfxColorFont;
		if( !m_mapColorFont.Lookup( hItem, tSfxColorFont ) )
		{
			clrValue = (COLORREF)-1;
		}
		else
		{
			clrValue = tSfxColorFont.color;
		}
		//-----------------------------
		return clrValue;
	}

	//����/��ȡ item �߼�����
	void KTreeCtrl::SetItemFont(HTREEITEM hItem, LOGFONT* pLogFont)
	{
		ColorFont tSfxColorFont;
		if(!m_mapColorFont.Lookup( hItem,tSfxColorFont))
		{
			tSfxColorFont.color = (COLORREF)-1;
		}
		tSfxColorFont.logfont = *pLogFont;
		m_mapColorFont[hItem] = tSfxColorFont;
	}
	BOOL KTreeCtrl::GetItemFont(HTREEITEM hItem, LOGFONT* pLogFont)
	{
		ColorFont tSfxColorFont;

		if(!m_mapColorFont.Lookup(hItem,tSfxColorFont)) return FALSE;

		if(tSfxColorFont.logfont.lfFaceName[0] == '\0') return FALSE;

		*pLogFont = tSfxColorFont.logfont;
		//---
		return TRUE;
	}

	//����״̬ͼƬ
	void KTreeCtrl::SetStateImage(UINT UnselectID,UINT SelectID,COLORREF clrMask)
	{
		m_ImageListState.DeleteImageList();
		m_ImageListState.Create(16,16,ILC_COLOR|ILC_MASK,3,1);

		CBitmap bmp[2];
		bmp[0].LoadBitmap(UnselectID);
		bmp[1].LoadBitmap(SelectID);

		m_ImageListState.Add(&bmp[0],clrMask);
		m_ImageListState.Add(&bmp[0],clrMask);	//why???
		m_ImageListState.Add(&bmp[1],clrMask);

		SetImageList(&m_ImageListState,TVSIL_STATE );
	}
	void KTreeCtrl::SetStateImage(UINT nBitmapID,COLORREF clrMask)
	{
		CImageList ImageListTemp;
		ImageListTemp.Create(nBitmapID, 16, 2,clrMask);
		//-----------------------------
		HICON hIcon[2];
		hIcon[0]=ImageListTemp.ExtractIcon(0);
		hIcon[1]=ImageListTemp.ExtractIcon(1);
		//-----------------------------
		m_ImageListState.DeleteImageList();
		m_ImageListState.Create(16,16,ILC_COLOR|ILC_MASK,3,1);

		m_ImageListState.Add(hIcon[0]);
		m_ImageListState.Add(hIcon[0]);	//why???
		m_ImageListState.Add(hIcon[1]);

		ImageListTemp.DeleteImageList();
		//-----------------------------
		SetImageList(&m_ImageListState, TVSIL_STATE);
	}
	void KTreeCtrl::SetStateImage(HBITMAP hBitmap,COLORREF clrMask)
	{
		//-----------------------------
		CImageList ImageListTemp;
		ImageListTemp.Create(16,16,ILC_COLOR|ILC_MASK,2,2);

		CBitmap bitmap;
		bitmap.Attach(hBitmap);
		ImageListTemp.Add(&bitmap,clrMask);
		bitmap.DeleteObject();
		//-----------------------------
		HICON hIcon[2];
		hIcon[0]=ImageListTemp.ExtractIcon(0);
		hIcon[1]=ImageListTemp.ExtractIcon(1);
		//-----------------------------
		m_ImageListState.DeleteImageList();
		m_ImageListState.Create(16,16,ILC_COLOR|ILC_MASK,3,1);

		m_ImageListState.Add(hIcon[0]);
		m_ImageListState.Add(hIcon[0]);	//why???
		m_ImageListState.Add(hIcon[1]);

		ImageListTemp.DeleteImageList();
		//-----------------------------
		SetImageList(&m_ImageListState, TVSIL_STATE);
	}

	// �ı�item״̬(��ʱû�е���)
	void KTreeCtrl::ChangeItemState()
	{
		UINT OldState;
		HTREEITEM AItem;
		AItem = GetSelectedItem();

		if (AItem!=NULL)
		{
			// ... calculate the new state, use a bitmask
			OldState = GetItemState(AItem, TVIS_STATEIMAGEMASK);
			if ((OldState >> 12) & 1)
				SetItemState(AItem,INDEXTOSTATEIMAGEMASK (2), TVIS_STATEIMAGEMASK);
			else
				SetItemState(AItem,INDEXTOSTATEIMAGEMASK (1), TVIS_STATEIMAGEMASK);
		}
	}

	// ȡ�õ�һ��ѡ�е� item �ľ��
	HTREEITEM KTreeCtrl::GetFirstSelectedItem()
	{
		if(!m_bMultiSelect) return NULL;
		//-----------------------------
		for (HTREEITEM hItem= GetRootItem(); hItem!=NULL; hItem = GetNextItem(hItem))
		{
			if (GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED)
			{
				return hItem;
			}
		}
		//-----------------------------
		return NULL;
	}
	//ȡ����һ��ѡ�е� item �ľ��
	HTREEITEM KTreeCtrl::GetPrevSelectedItem(HTREEITEM hItem)
	{
		if(m_bMultiSelect)
		{
			for ( hItem = GetPrevItem( hItem ); hItem!=NULL; hItem = GetPrevItem( hItem ) )
			{
				if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
				{
					return hItem;
				}
			}
		}

		return NULL;
	}
	// ȡ����һ��ѡ�е� item �ľ��
	HTREEITEM KTreeCtrl::GetNextSelectedItem(HTREEITEM hItem)
	{
		if(m_bMultiSelect)
		{
			for ( hItem = GetNextItem( hItem ); hItem!=NULL; hItem = GetNextItem( hItem ) )
			{
				if ( GetItemState( hItem, TVIS_SELECTED ) & TVIS_SELECTED )
				{
					return hItem;
				}
			}
		}

		return NULL;
	}
	//ȡ��ĩһ��ѡ�е� item �ľ��
	HTREEITEM KTreeCtrl::GetLastSelectedItem()
	{
		ASSERT(FALSE);
		return NULL;
	}

	//ȡ����һ�� item �ľ��
	HTREEITEM KTreeCtrl::GetPrevItem(HTREEITEM hItem)
	{
		//ȡ���ϸ��ֵ� item
		HTREEITEM hti = GetPrevSiblingItem(hItem);

		if(hti == NULL)
		{
			//���û����һ���ֵܽڵ�,�򷵻��丸�ڵ�
			hti = GetParentItem(hItem);

			if(hti == NULL)
			{
				//���û�и��ڵ�,˵���Ǹ��ڵ�
				hti = GetLastItem(hti);
			}
		}

		return hti;
	}
	//ȡ����һ�� item �ľ��
	HTREEITEM KTreeCtrl::GetNextItem(HTREEITEM hItem)
	{
		HTREEITEM hti = NULL;
		//-----------------------------
		//���ӽڵ�
		if(ItemHasChildren(hItem))
		{
			//�õ��ӽڵ�
			hti = GetChildItem(hItem);
		}
		
		//û���ӽڵ�
		if(hti == NULL)
		{
			//���û���ӽڵ�,ȡ����һ���ֵܽڵ�
			while((hti = GetNextSiblingItem(hItem)) == NULL)
			{
				//���û���ֵܽ��,ȡ�ø����
				if((hItem = GetParentItem(hItem)) == NULL)
				{
					//���û�и����,˵����ֻ�иýڵ�һ��
					return NULL;
				}
			}
		}
		//-----------------------------
		return hti;
	}
	//ȡ�����һ���� item �ľ��
	HTREEITEM KTreeCtrl::GetLastItem(HTREEITEM hItem)
	{
		HTREEITEM	htiNext;
		
		if( hItem == NULL )	hItem = GetRootItem();
		
		while( ItemHasChildren( hItem ) != NULL ) 
		{
			//	Find the last child of hItem
			htiNext = GetChildItem( hItem );
			while( htiNext != NULL ) 
			{
				hItem = htiNext;
				htiNext = GetNextSiblingItem(htiNext);
			}
		}
		
		return hItem;
	}

	//---------------------------------------------------------
	// ��    ��: FindItem
	// ��    ��: �����ı����� item,����������
	// ��    ��: public
	// ��    ��:
	//			[i] lpszSearch - Ҫ���ҵ��ı�
	//			[i] bCaseSensitive - ��Сд����,Ĭ��=FALSE
	//			[i] bDownDir - ���²���,Ĭ��=TRUE
	//			[i] bWholeWord - ȫ�ַ�ƥ��,Ĭ��=FALSE
	//			[i] hItemFrom - ��ʼ���ҵĽڵ���,Ĭ��=NULL,��ʾ�ӵ�ǰѡ�еĽڵ㿪ʼ
	//
	// ��    ��: 
	// ע    ��: 
	//---------------------------------------------------------
	HTREEITEM KTreeCtrl::FindItem(LPCTSTR lpszSearch,BOOL bCaseSensitive /*= FALSE*/,BOOL bDownDir /*= TRUE*/,BOOL bWholeWord /*= FALSE*/,HTREEITEM hItemFrom /*=NULL*/)
	{
		CString str = lpszSearch;
		//-----------------------------
		int lenSearchStr = str.GetLength();
		if(lenSearchStr == 0) return NULL;
		//-----------------------------
		// For the first pass, set the current item equal to the selection
		HTREEITEM htiSel = hItemFrom ? hItemFrom : GetSelectedItem();
		HTREEITEM htiCur = htiSel;

		CString sSearch = str;

		// make sure it ends if we started with no selection
		if((htiCur==NULL) && (htiSel!=NULL))
		{
			if( bDownDir )
				htiCur = GetRootItem();
			else
				htiCur = GetLastItem(NULL);
		}

		if(!bCaseSensitive ) sSearch.MakeLower();

		// For the first pass only, we check to see if it 
		// is the item we're looking for.
		BOOL bFirstPass = TRUE;

		while(htiCur && (htiCur!=htiSel || bFirstPass))
		{
			bFirstPass = FALSE;

			CString sItemText = GetItemText( htiCur );

			if(!bCaseSensitive) sItemText.MakeLower();

			int n;
			while((n=sItemText.Find(sSearch)) != -1)
			{
				// Search string found
				if(bWholeWord)
				{
					// Check preceding char
					if(n!=0)
					{
						if(isalpha(sItemText[n-1]) || (sItemText[n-1]=='_'))
						{
							// Not whole word
							sItemText = sItemText.Right(sItemText.GetLength() - n - lenSearchStr );
							continue;
						}
					}
					// Check succeeding char
					if((sItemText.GetLength()>n+lenSearchStr) && (isalpha(sItemText[n+lenSearchStr]) ||	(sItemText[n+lenSearchStr]=='_')))
					{
						// Not whole word
						sItemText = sItemText.Right( sItemText.GetLength() - n - sSearch.GetLength());
						continue;
					}
				}//endif(bWholeWord)

				if(IsFindValid(htiCur)) return htiCur;
				else break;
			}//end while 1

			htiCur = bDownDir ? GetNextItem(htiCur) : GetPrevItem(htiCur);
			if(htiCur == NULL)
			{
				if( bDownDir )  htiCur = GetRootItem();
				else htiCur = GetLastItem( NULL );
			}
		}//end while 0
		//-----------------------------
		return NULL;
	}

	BOOL KTreeCtrl::IsFindValid(HTREEITEM hti)
	{
		//return hti==NULL ? false : true;
		return TRUE;
	}

	//ȡ������ѡ��
	void KTreeCtrl::ClearSelection()
	{
		if(!m_bMultiSelect) return;
		//-----------------------------
		HTREEITEM hItem=GetRootItem();
		while(hItem)
		{
			if (GetItemState(hItem, TVIS_SELECTED ) & TVIS_SELECTED)
			{
				SetItemState( hItem, 0, TVIS_SELECTED );
			}

			hItem=GetNextItem(hItem);
		}
		//-----------------------------
	}

	//ѡ��ĳһ��Χ�ڵ� item
	void KTreeCtrl::SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo)
	{
		if(!m_bMultiSelect) return;
		//-----------------------------
		HTREEITEM hItem = GetRootItem();

		// Clear selection upto the first item
		while (hItem && (hItem!=hItemFrom) && (hItem!=hItemTo))
		{
			hItem = GetNextVisibleItem(hItem);
			SetItemState( hItem, 0, TVIS_SELECTED );
		}

		if (!hItem ) return;	// Item is not visible

		SelectItem(hItemTo);

		// Rearrange hItemFrom and hItemTo so that hItemFirst is at top
		if( hItem == hItemTo )
		{
			hItemTo = hItemFrom;
			hItemFrom = hItem;
		}

		// Go through remaining visible items
		BOOL bSelect = TRUE;
		while (hItem)
		{
			// Select or remove selection depending on whether item
			// is still within the range.
			SetItemState( hItem, bSelect ? TVIS_SELECTED : 0, TVIS_SELECTED );

			// Do we need to start removing items from selection
			if( hItem == hItemTo ) bSelect = FALSE;

			hItem = GetNextVisibleItem( hItem );
		}
		//-----------------------------
		return;
	}

	//�������Checked��������������Ů������Ů��Checked,��֮��Ȼ
	void KTreeCtrl::SetAllChildCheck(HTREEITEM hTreeItemParent,BOOL bCheck)
	{
		HTREEITEM hTreeItemChild=GetChildItem(hTreeItemParent);
		while(hTreeItemChild)
		{
			SetCheck(hTreeItemChild,bCheck);

			//�ݹ����
			SetAllChildCheck(hTreeItemChild,bCheck);

			hTreeItemChild=GetNextSiblingItem(hTreeItemChild);
		}
	}

	// �������Checked�������丸���游��Checked
	void KTreeCtrl::SetParentCheck(HTREEITEM hTreeItemChild)
	{
		HTREEITEM hTreeItemParent=GetParentItem(hTreeItemChild);
		//---
		while(hTreeItemParent)
		{
			BOOL bChildChecked=IsChildChecked(hTreeItemParent);
			SetCheck(hTreeItemParent,bChildChecked);
			//---
			//---
			hTreeItemParent=GetParentItem(hTreeItemParent);
		}
	}

	//��鱾��������Ƿ�Checked
	BOOL KTreeCtrl::IsChildChecked(HTREEITEM hTreeItemParent)
	{
		BOOL bValue=FALSE;
		//---
		HTREEITEM hTreeItemChild=GetChildItem(hTreeItemParent);
		while(hTreeItemChild && !bValue)
		{
			BOOL bTemp=GetCheck(hTreeItemChild);
			bValue |= bTemp;
			hTreeItemChild=GetNextSiblingItem(hTreeItemChild);
		}
		//---
		return bValue;
	}

	void KTreeCtrl::OnContextMenu(CWnd* pWnd, CPoint point)
	{
		if(m_uMenuID == 0)
			return;

		//CWinApp* pWinApp = AfxGetApp();
		//if(pWinApp->IsKindOf(RUNTIME_CLASS(CWinAppEx)))
		//{
		//	CContextMenuManager* pContextMenuManager = ((CWinAppEx*)pWinApp)->GetContextMenuManager();
		//	pContextMenuManager->ShowPopupMenu(m_uMenuID, point.x, point.y, this, TRUE);

		//	return;
		//}

		CMenu menu;
		menu.LoadMenu(m_uMenuID);

		CMenu* pSumMenu = menu.GetSubMenu(0);

		if (AfxGetMainWnd()->IsKindOf(RUNTIME_CLASS(CFrameWndEx)))
		{
			CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;

			if (!pPopupMenu->Create(pWnd, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
				return;

			((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
			UpdateDialogControls(this, FALSE);
		}

		SetFocus();
	}

	void KTreeCtrl::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		// TODO: �ڴ���ӿؼ�֪ͨ����������
		//-----------------------------------------------------
		HTREEITEM htCurrent = this->GetSelectedItem();

		m_pTreeCtrlChecked->pTreeCtrl = this;
		m_pTreeCtrlChecked->hTreeItem = htCurrent;
		m_pTreeCtrlChecked->point = m_point;
		m_pTreeCtrlChecked->dwItemData = (DWORD)GetItemData(htCurrent);

		WPARAM wParam = (WPARAM)0;
		LPARAM lParam = (LPARAM)(m_pTreeCtrlChecked);

		CWnd* pWndParent = GetParent(); //ȡ�ø��״��ڣ�����CTreeView
		CWnd* pWndOwner = GetOwner();	//ȡ��ӵ���ߴ��ڣ�����CTreeView

		if(pWndParent->GetSafeHwnd())
		{
			pWndParent->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//���״��ڷ�����Ϣ
		}
		if(pWndOwner->GetSafeHwnd())
		{
			pWndOwner->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//��ӵ���ߴ��ڷ�����Ϣ
		}
		//-----------------------------------------------------
		*pResult = 0;
	}

//---------------------------------------------------------
}// namespace Gui - end
//---------------------------------------------------------
