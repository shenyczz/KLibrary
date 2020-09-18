// src\KTreeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "KTreeCtrl.h"

namespace Gui
{

	// KTreeCtrl

	IMPLEMENT_DYNAMIC(KTreeCtrl, CTreeCtrl)

	KTreeCtrl::KTreeCtrl()
	{
		m_bColorFont = FALSE;		// 是否彩色文本
		m_bMultiSelect = FALSE;		// 是否多选

		m_cxBitmap = 0;			//背景位图的宽度
		m_cyBitmap = 0;			//背景位图的高度

		m_mapColorFont.RemoveAll();
		m_uTimerID = 0;
		m_hItemFirstSel = NULL;		// 在多选状态下选中的第一个条目

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



	// KTreeCtrl 消息处理程序

	// 在窗口被子类化之前由框架调用,允许其他必要的子类化
	// 在对象附加到现有窗口后调用
	void KTreeCtrl::PreSubclassWindow()
	{
		// TODO: 在此添加专用代码和/或调用基类

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

	// 在Windows创建窗口之前调用,窗口附加 CWnd
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
		// TODO: 在此处添加消息处理程序代码
		// 不为绘图消息调用 CTreeCtrl::OnPaint()
		//-----------------------------
		//客户区矩形
		CRect rcClient;
		GetClientRect(&rcClient);
		//-----------------------------
		//创建内存兼容 DC 和内存兼容位图
		CDC memDC;
		CBitmap bitmap;
		if(memDC.GetSafeHdc()==NULL)
		{
			memDC.CreateCompatibleDC(&dc);
			bitmap.CreateCompatibleBitmap( &dc, rcClient.Width(), rcClient.Height() );
		}
		memDC.SetBkMode(TRANSPARENT);
		CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

		//先让控件执行默认绘制
		CWnd::DefWindowProc(WM_PAINT,(WPARAM)memDC.m_hDC,(LPARAM)0 );
		//-----------------------------
		CRect rcClip;
		dc.GetClipBox(&rcClip);	//控件的剪切区(包含可见item的矩形)
		//-----------------------------
		//彩色文本
		if(m_bColorFont)
		{
			this->DrawColorFont(&memDC);

			dc.BitBlt(rcClip.left, rcClip.top, rcClip.Width(), rcClip.Height(),
				&memDC, rcClip.left, rcClip.top, SRCCOPY);
		}
		//-----------------------------
		//绘制背景图片
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
	//绘制彩色文本
	void KTreeCtrl::DrawColorFont(CDC* pDC)
	{
		int iOffsetOfTop=GetImageList(TVSIL_NORMAL) ? 2 : 1;
		//if(GetImageList())
		//-----------------------------
		//可见 item 数量
		int n = GetVisibleCount()+1;

		//取得第一个可见的item
		HTREEITEM hItem = GetFirstVisibleItem();
		//-----------------------------
		CFont *pFontDC;
		CFont fontDC;
		LOGFONT logfont;
		while(hItem && n--)
		{
			CRect rect;	//item 的矩形

			//不管理选中的或高亮的 item 的文本颜色
			UINT selflag = TVIS_DROPHILITED | TVIS_SELECTED;
			ColorFont colorFont;

			if(!(GetItemState( hItem, selflag ) & selflag )	//不是被选中的或高亮item
				&& m_mapColorFont.Lookup( hItem,colorFont))		//并且找到了
			{
				//取得逻辑字体
				if( colorFont.logfont.lfFaceName[0] != '\0' ) 
				{
					//指定了字体
					logfont = colorFont.logfont;
				}
				else
				{
					//没有指定字体,使用windows系统字体
					CFont *pFont = GetFont();
					pFont->GetLogFont(&logfont);
				}

				CString sItem = GetItemText( hItem );	//取得 item 的文本

				BOOL bTextOnly = TRUE;
				GetItemRect( hItem,&rect,bTextOnly);		//取得 item 的文本使用的矩形
				
				//覆盖原来文本上
				CFont* pOldFont = pDC->SelectObject(GetFont());
				//设置文本颜色和背景颜色一致
				pDC->SetTextColor(GetBkColor());
				pDC->TextOut( rect.left+2, rect.top+iOffsetOfTop, sItem );	//why plus 2 ???
				pDC->SelectObject(pOldFont);

				//判断 item 是否粗体
				if(IsItemBold(hItem))
				{
					logfont.lfWeight = FW_BOLD;
				}
				else
				{
					logfont.lfWeight = FW_NORMAL;
				}

				//创建内存字体并选入内存 DC
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
	//绘制背景图片
	void KTreeCtrl::DrawBackImage(CDC* pDevDC,CDC* pMemDC)
	{
		int i,j;

		//客户区矩形
		CRect rcClient;
		GetClientRect(&rcClient);
		CRect rcClip;
		pDevDC->GetClipBox(&rcClip);	//控件的剪切区(包含可见item的矩形)

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

	// 防止闪烁
	BOOL KTreeCtrl::OnEraseBkgnd(CDC* pDC)
	{
		// TODO: 在此添加消息处理程序代码和/或调用默认值
		//return CTreeCtrl::OnEraseBkgnd(pDC);
		return TRUE;
	}

	// 当窗口接收焦点时,允许其实现其逻辑调色板
	BOOL KTreeCtrl::OnQueryNewPalette()
	{
		CClientDC dc(this);

		if( (dc.GetDeviceCaps(RASTERCAPS) & RC_PALETTE)	//硬件设备支持调色板
			&& m_pal.m_hObject != NULL )				//并且有调色板
		{
			dc.SelectPalette( &m_pal, FALSE );
			BOOL result = dc.RealizePalette();

			if( result ) Invalidate();

			return result;
		}

		return CTreeCtrl::OnQueryNewPalette();
	}

	// 水平滚动条
	void KTreeCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		if(m_bitmap.m_hObject != NULL )
		{
			InvalidateRect(NULL,false);
		}

		//必须调用
		CTreeCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
	}
	// 垂直滚动条
	void KTreeCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
	{
		if(m_bitmap.m_hObject != NULL )
		{
			InvalidateRect(NULL,false);
		}

		//必须调用
		CTreeCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
	}

	// 鼠标左键
	void KTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
	{
		//-----------------------------
		//允许多选
		if(m_bMultiSelect)	//允许多选
		{
			if(nFlags & MK_CONTROL ) //同时按下 Ctrl 键
			{
				UINT flag;

				//Returns the current position of the cursor related to the CTreeCtrl object
				HTREEITEM hItem = HitTest(point,&flag );

				if(hItem)
				{
					//条目的选择状态
					UINT uItemSelState = GetItemState(hItem, TVIS_SELECTED);

					//条目的新状态(原来没选中现在设置为选中,原来选中现在设置为没选中)
					UINT uNewSelState = (uItemSelState & TVIS_SELECTED) ? 0 : TVIS_SELECTED;

					//取得原来被选中的条目和状态
					HTREEITEM hItemOld = GetSelectedItem();
					UINT uOldSelState  = hItemOld ? GetItemState(hItemOld, TVIS_SELECTED) : 0;

					//选择新条目
					if( GetSelectedItem() == hItem )
						SelectItem(NULL);	//防止编辑？？
					CTreeCtrl::OnLButtonDown(nFlags, point);

					//设置选中条目的适当状态(原来没选中现在设置为选中,原来选中现在设置为没选中)
					SetItemState(hItem, uNewSelState,  TVIS_SELECTED);

					// Restore state of old selected item
					//恢复原来被选中的条目和状态
					if (hItemOld && (hItemOld != hItem))
						SetItemState(hItemOld, uOldSelState, TVIS_SELECTED);

					//???
					m_hItemFirstSel = NULL;
					
					return;
				}
			}
			else if(nFlags & MK_SHIFT)	//同时按下 Shift键
			{
				UINT flag;

				//Returns the current position of the cursor related to the CTreeCtrl object
				HTREEITEM hItem = HitTest(point,&flag );

				//得到第一个选中的 item
				if(!m_hItemFirstSel)
					m_hItemFirstSel = GetSelectedItem();

				//选择新的 item
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
	// 鼠标右键
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
		// TODO: 在此添加控件通知处理程序代码
		*pResult = 0;

		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);
		TRACE("[CSfxTreeCtrl::OnNMClick] x=%d y=%d\n",point.x,point.y);

		UINT uFlag;
		HTREEITEM htCurrent = HitTest(point,&uFlag);
		TRACE("[CSfxTreeCtrl::OnNMClick] uFlag=%d\n",uFlag);

		// 在条目的状态图标上单击(主要用于改变选中状态)
		if(uFlag & TVHT_ONITEMSTATEICON)
		{
			m_point = point;
			if(!m_uTimerID)
			{
				m_uTimerID = (UINT)SetTimer(TREE_CTRL_CHECKBOX_TIMERID,10,NULL);
			}
		}
		// 在条目的标签上单击(主要用于改变选中状态)
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
		BOOL bCheck = GetCheck(htCurrent);		//是否选中
		SetAllChildCheck(htCurrent,bCheck);		//先设置所有子树的check
		SetParentCheck(htCurrent);				//设置父树的check
		//-----------------------------
		m_pTreeCtrlChecked->pTreeCtrl = this;
		m_pTreeCtrlChecked->hTreeItem = htCurrent;
		m_pTreeCtrlChecked->point = m_point;
		m_pTreeCtrlChecked->dwItemData = (DWORD)GetItemData(htCurrent);

		WPARAM wParam = (WPARAM)0;
		LPARAM lParam = (LPARAM)(m_pTreeCtrlChecked);
		//-----------------------------
		CWnd* pWndParent = GetParent(); //取得父亲窗口，比如CTreeView
		CWnd* pWndOwner = GetOwner();	//取得拥有者窗口，比如CTreeView

		if(m_uTimerID==TREE_CTRL_CHECKBOX_TIMERID)
		{
			if(pWndParent->GetSafeHwnd())
			{
				pWndParent->PostMessage(WM_KIND_TREE_CTRL_CHECKBOX,wParam,lParam);	//向父亲窗口发送消息
			}
			if(pWndOwner->GetSafeHwnd())
			{
				pWndOwner->PostMessage(WM_KIND_TREE_CTRL_CHECKBOX,wParam,lParam);	//向拥有者窗口发送消息
			}
		}
		else if(m_uTimerID==TREE_CTRL_LABEL_TIMERID)
		{
			if(pWndParent->GetSafeHwnd())
			{
				pWndParent->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//向父亲窗口发送消息
			}
			if(pWndOwner->GetSafeHwnd())
			{
				pWndOwner->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//向拥有者窗口发送消息
			}
		}

 		//-----------------------------
		m_uTimerID = 0;

		return;
	}

	void KTreeCtrl::OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult)
	{
		LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
		// TODO: 在此添加控件通知处理程序代码
		*pResult = 0;

		if( m_bitmap.m_hObject != NULL )
		{
			InvalidateRect(NULL,FALSE);
		}

		return;
	}


	//设置背景图像
	BOOL KTreeCtrl::SetBkImage(LPCTSTR lpszResourceName)
	{
		int i;
		//-----------------------------
		//如果不是第一次调用,删除 GDI 对象
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
		//为位图创建逻辑调色板
		DIBSECTION ds;
		m_bitmap.GetObject(sizeof(DIBSECTION),&ds );

		BITMAPINFOHEADER bmInfo = ds.dsBmih;
		int nColors = bmInfo.biClrUsed ? bmInfo.biClrUsed : 1 << bmInfo.biBitCount;

		//取得桌面DC
		CClientDC dc(NULL);

		//如果颜色数大于256,创建半色调逻辑调色板
		if( nColors > 256 )
		{
			m_pal.CreateHalftonePalette(&dc);
		}
		else
		{
			//获取位图颜色表
			CDC memDC;
			memDC.CreateCompatibleDC(&dc);
			memDC.SelectObject(&m_bitmap);
			RGBQUAD *pRgbQuad = new RGBQUAD[nColors];
			::GetDIBColorTable(memDC,0,nColors,pRgbQuad);

			//逻辑调色板尺寸
			UINT nSize = sizeof(LOGPALETTE) + (sizeof(PALETTEENTRY)*nColors);
			LOGPALETTE* pLogPal = (LOGPALETTE*)new BYTE[nSize];

			pLogPal->palVersion = 0x300;	//必须
			pLogPal->palNumEntries = nColors;

			for(i=0; i<nColors; i++)
			{
				pLogPal->palPalEntry[i].peRed = pRgbQuad[i].rgbRed;
				pLogPal->palPalEntry[i].peGreen = pRgbQuad[i].rgbGreen;
				pLogPal->palPalEntry[i].peBlue = pRgbQuad[i].rgbBlue;
				pLogPal->palPalEntry[i].peFlags = 0;
			}

			//创建调色板 CPalette
			m_pal.CreatePalette(pLogPal);

			//释放内存
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

	//设置/获取 item 颜色
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

	//设置/获取 item 逻辑字体
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

	//设置状态图片
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

	// 改变item状态(暂时没有调用)
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

	// 取得第一个选中的 item 的句柄
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
	//取得上一个选中的 item 的句柄
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
	// 取得下一个选中的 item 的句柄
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
	//取得末一个选中的 item 的句柄
	HTREEITEM KTreeCtrl::GetLastSelectedItem()
	{
		ASSERT(FALSE);
		return NULL;
	}

	//取得上一个 item 的句柄
	HTREEITEM KTreeCtrl::GetPrevItem(HTREEITEM hItem)
	{
		//取得上个兄弟 item
		HTREEITEM hti = GetPrevSiblingItem(hItem);

		if(hti == NULL)
		{
			//如果没有上一个兄弟节点,则返回其父节点
			hti = GetParentItem(hItem);

			if(hti == NULL)
			{
				//如果没有父节点,说明是根节点
				hti = GetLastItem(hti);
			}
		}

		return hti;
	}
	//取得下一个 item 的句柄
	HTREEITEM KTreeCtrl::GetNextItem(HTREEITEM hItem)
	{
		HTREEITEM hti = NULL;
		//-----------------------------
		//有子节点
		if(ItemHasChildren(hItem))
		{
			//得到子节点
			hti = GetChildItem(hItem);
		}
		
		//没有子节点
		if(hti == NULL)
		{
			//如果没有子节点,取得下一个兄弟节点
			while((hti = GetNextSiblingItem(hItem)) == NULL)
			{
				//如果没有兄弟结点,取得父结点
				if((hItem = GetParentItem(hItem)) == NULL)
				{
					//如果没有父结点,说明树只有该节点一个
					return NULL;
				}
			}
		}
		//-----------------------------
		return hti;
	}
	//取得最后一个的 item 的句柄
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
	// 名    称: FindItem
	// 功    能: 根据文本查找 item,并返回其句柄
	// 访    问: public
	// 参    数:
	//			[i] lpszSearch - 要查找的文本
	//			[i] bCaseSensitive - 大小写敏感,默认=FALSE
	//			[i] bDownDir - 向下查找,默认=TRUE
	//			[i] bWholeWord - 全字符匹配,默认=FALSE
	//			[i] hItemFrom - 开始查找的节点句柄,默认=NULL,表示从当前选中的节点开始
	//
	// 返    回: 
	// 注    释: 
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

	//取消所有选择
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

	//选择某一范围内的 item
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

	//如果本层Checked则设置其所有子女、孙子女等Checked,反之依然
	void KTreeCtrl::SetAllChildCheck(HTREEITEM hTreeItemParent,BOOL bCheck)
	{
		HTREEITEM hTreeItemChild=GetChildItem(hTreeItemParent);
		while(hTreeItemChild)
		{
			SetCheck(hTreeItemChild,bCheck);

			//递归调用
			SetAllChildCheck(hTreeItemChild,bCheck);

			hTreeItemChild=GetNextSiblingItem(hTreeItemChild);
		}
	}

	// 如果本层Checked则设置其父、祖父等Checked
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

	//检查本层的子树是否Checked
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
		// TODO: 在此添加控件通知处理程序代码
		//-----------------------------------------------------
		HTREEITEM htCurrent = this->GetSelectedItem();

		m_pTreeCtrlChecked->pTreeCtrl = this;
		m_pTreeCtrlChecked->hTreeItem = htCurrent;
		m_pTreeCtrlChecked->point = m_point;
		m_pTreeCtrlChecked->dwItemData = (DWORD)GetItemData(htCurrent);

		WPARAM wParam = (WPARAM)0;
		LPARAM lParam = (LPARAM)(m_pTreeCtrlChecked);

		CWnd* pWndParent = GetParent(); //取得父亲窗口，比如CTreeView
		CWnd* pWndOwner = GetOwner();	//取得拥有者窗口，比如CTreeView

		if(pWndParent->GetSafeHwnd())
		{
			pWndParent->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//向父亲窗口发送消息
		}
		if(pWndOwner->GetSafeHwnd())
		{
			pWndOwner->PostMessage(WM_KIND_TREE_CTRL_LABEL,wParam,lParam);	//向拥有者窗口发送消息
		}
		//-----------------------------------------------------
		*pResult = 0;
	}

//---------------------------------------------------------
}// namespace Gui - end
//---------------------------------------------------------
