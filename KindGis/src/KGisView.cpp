// KGisView.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KGisView.h"

#include "KGisDocument.h"

#include "KLayerDockablePane.h"
#include "KPropertyDockablePane.h"

// KGisView

namespace Framework
{

	IMPLEMENT_DYNCREATE(KGisView, KFinalView)

	KGisView::KGisView()
	{
	}

	KGisView::~KGisView()
	{
	}

	BEGIN_MESSAGE_MAP(KGisView, KFinalView)
		// ��������Ϣ
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_LBUTTONDBLCLK()
		// ����Ҽ���Ϣ
		ON_WM_RBUTTONDOWN()
		ON_WM_RBUTTONUP()
		ON_WM_RBUTTONDBLCLK()
		// ����ƶ���Ϣ
		ON_WM_MOUSEMOVE()
	END_MESSAGE_MAP()

	// KGisView ��ͼ

	void KGisView::OnDraw(CDC* pDC)
	{
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == NULL )
			return;

		if(NULL == pMapEngine->GetHwnd())
			return;

		CRect clientRect;
		this->GetClientRect( &clientRect );

		int x = clientRect.left;
		int y = clientRect.top;
		int w = clientRect.Width();
		int h = clientRect.Height();

		if(!pMapEngine->HasDataBound())
		{
			pMapEngine->SetViewBound(x,y,w,h);
			pMapEngine->Reset();
			pMapEngine->EnableRedraw();
		}

		pMapEngine->SetViewBound(x,y,w,h);
		pMapEngine->DrawMap();

		return;
	}// OnDraw(CDC* pDC) - end

	// KGisView ���

	#ifdef _DEBUG
	void KGisView::AssertValid() const
	{
		KFinalView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KGisView::Dump(CDumpContext& dc) const
	{
		KFinalView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KGisView ��Ϣ�������

	void KGisView::OnContextMenu(CWnd* pWnd, CPoint point)
	{
		CMenu menu;
		// װ����Դ���е����β˵�
		//if(!menu.Attach(KResource::LoadMenu(KIND_IDR_MENU_ROAM)))
			return;

		// ȡ���Ӳ˵�
		CMenu* pSumMenu = menu.GetSubMenu(0);

		//CBitmap bitmap;
		//if(bitmap.LoadBitmap(IDB_TOOLBAR_ZOOM))
		//{
		//	pSumMenu->SetMenuItemBitmaps(0,MF_BYPOSITION,&bitmap,&bitmap);
		//	pSumMenu->SetMenuItemBitmaps(1,MF_BYPOSITION,&bitmap,&bitmap);
		//	pSumMenu->SetMenuItemBitmaps(2,MF_BYPOSITION,&bitmap,&bitmap);
		//	pSumMenu->SetMenuItemBitmaps(3,MF_BYPOSITION,&bitmap,&bitmap);
		//	pSumMenu->SetMenuItemBitmaps(4,MF_BYPOSITION,&bitmap,&bitmap);
		//}
		//pPopupMenu->TrackPopupMenu(TPM_TOPALIGN,point.x,point.y,this,NULL);

		CMFCPopupMenu* pPopupMenu = new CMFCPopupMenu;
		if (!pPopupMenu->Create(pWnd, point.x, point.y, (HMENU)pSumMenu->m_hMenu, FALSE, TRUE))
			return;

		((CFrameWndEx*)AfxGetMainWnd())->OnShowPopupMenu(pPopupMenu);
		UpdateDialogControls(this, FALSE);
	}

	// �������
	void KGisView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// ���ø���ͬ������
		KFinalView::OnLButtonDown(nFlags, point);

		// �������(Ҫ�� OnLButtonUp ���ͷŲ���)
		::SetCapture(this->GetSafeHwnd());

		if(m_iGisTool == GisTool_ZoomPan)  //�ƶ�
		{
			m_hCurOld = m_hCurNew;	// m_hCurOld ԭ�������ι��,��OnLButtonUp�лָ�
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_HAND_HOLD);
			SetCursor(m_hCurNew);
		}

		// ��ȡ�ĵ�
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// ȡ�õ�ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// ����λ��
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);
		// ������������
		KPoint kpoint = KPoint(x,y);

		// ���Һڰ�ͼ���еļ���ͼ�ζ���
		KObject* pObject = NULL;
		if(m_iGisTool & GisTool_BlackboardController)	// �Ǻڰ��������
		{
			// ���Һڰ�ͼ���ҵ�����ͼ�ζ���
			KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
			if(pBlackboardLayer)
			{
				// �����ṩ��
				KProvider* pProvider = pBlackboardLayer->GetProvider();
				// �ڰ����ݶ���
				KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
				if(pBlackboardData && !pBlackboardData->IsEmpty())
				{
					// ȡ�����λ�õĶ���
					pObject = pBlackboardData->GetObject(point);
				}
			}// if(pBlackboardLayer)

		}// if(m_iGisTool & BlackboardController)

		// ����������
		if( nFlags & MK_LBUTTON )
		{
			// ����������
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// ȡ�õ�ǰ����������
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonDown(&mouseEvent);
			}
		}
	}
	// ����ſ�
	void KGisView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// ���ø���ͬ������
		KFinalView::OnLButtonUp(nFlags, point);

		// ��λ����϶����
		m_bMouseDraging = false;

		// �ͷ����(�� OnLButtonDown �в����)
		::ReleaseCapture();

		// �ƶ�
		if(m_iGisTool == GisTool_ZoomPan)
		{
			m_hCurOld = m_hCurNew;
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_HAND_ARROW);
			SetCursor(m_hCurNew);
		}

		// ��ȡ�ĵ�
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// ȡ�õ�ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return;

		// ���Һڰ�ͼ���еļ���ͼ�ζ���
		KObject* pObject = NULL;
		// ...

		// �������ſ�
		if( !(nFlags & MK_LBUTTON) )
		{
			// ����������
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// ȡ�õ�ǰ����������
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonUp(&mouseEvent);
			}
		}

		return;
	}
	// ���˫��
	void KGisView::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		// ���ø���ͬ������
		KFinalView::OnLButtonDblClk(nFlags, point);

		// ��ȡ�ĵ�
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// ȡ�õ�ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// ����λ��
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);
		// ������������
		KPoint kpoint = KPoint(x,y);

		// ���Һڰ�ͼ���еļ���ͼ�ζ���
		KObject* pObject = NULL;
		if(m_iGisTool & GisTool_BlackboardController)
		{
			// ���Һڰ�ͼ���ҵ�����ͼ�ζ���
			KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
			if(pBlackboardLayer)
			{
				// �����ṩ��
				KProvider* pProvider = pBlackboardLayer->GetProvider();
				if(pProvider)
				{
					// �ڰ����ݶ���
					KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
					if(pBlackboardData && !pBlackboardData->IsEmpty())
					{
						// ȡ�����λ�õĶ���
						pObject = pBlackboardData->GetObject(point);
					}
				}// if(pProvider)
			}// if(pBlackboardLayer)
		}// if(m_iGisTool & GisTool_BlackboardController)

		// ����������
		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		// �����������������Ŀ���ǵ�ͼ�ؼ�,
		// ���ҵ�ͼ�ؼ���ǰû�����ò����������β���,
		// ��ô����˫��������зŴ�,�������õ�ǰ����Ϊ����
		int key = pMouseListeners->GetCurrentMouseListenerKey();
		int subKey = pMouseListeners->GetCurrentMouseListenerSubKey();
		if( key==GisTool_MapController && (subKey==GisTool_ZoomPan || subKey==GisTool_None) )
		{
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonDBLClick(&mouseEvent);
			}
		}
		else if( key==GisTool_BlackboardController && (subKey==GisTool_Blackboard_Select_Object) )
		{
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonDBLClick(&mouseEvent);
			}
		}

		// FUN_END
	}

	// �Ҽ�����
	void KGisView::OnRButtonDown(UINT nFlags, CPoint point)
	{
		// ���ø���ͬ������
		KFinalView::OnRButtonDown(nFlags, point);
		// FUN_END
	}
	// �Ҽ��ſ�
	void KGisView::OnRButtonUp(UINT nFlags, CPoint point)
	{
		// ���ø���ͬ������
		KFinalView::OnRButtonUp(nFlags, point);
		// ���ø���ͬ������(...)
		KFinalView::OnLButtonDblClk(nFlags, point);

		// ��ȡ�ĵ�
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// ȡ�õ�ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// ����Ҽ��ſ�
		if( !(nFlags & MK_RBUTTON) )
		{
			// ����������
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// ȡ�õ�ǰ����������
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),NULL);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnRButtonUp(&mouseEvent);
			}
		}

		return;
	}
	// �Ҽ�˫��
	void KGisView::OnRButtonDblClk(UINT nFlags, CPoint point)
	{
		// ���ø���ͬ������
		KFinalView::OnRButtonDblClk(nFlags, point);

		// ��ȡ�ĵ�
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// ȡ�õ�ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// ����������
		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		// �����������������Ŀ���ǵ�ͼ�ؼ�,
		// ���ҵ�ͼ�ؼ���ǰû�����ò����������β���,
		// ��ô����˫��������зŴ�,�������õ�ǰ����Ϊ����
		int key = pMouseListeners->GetCurrentMouseListenerKey();
		int subKey = pMouseListeners->GetCurrentMouseListenerSubKey();
		if( key==GisTool_MapController && (subKey==GisTool_ZoomPan || subKey==GisTool_None) )
		{
			pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,GisTool_ZoomPan);
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),NULL);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnRButtonDBLClick(&mouseEvent);
			}
		}

		return;
	}
	// ����ƶ�
	void KGisView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// ���ø���ͬ������
		KFinalView::OnMouseMove(nFlags, point);

		if(!m_hCurNew)
		{
			m_hCurNew = m_hCurDefault;	//ȱʡ���
		}

		// ��ȡ�ĵ�
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// ȡ�õ�ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// ����λ��
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);
		// ������������
		KPoint kpoint = KPoint(x,y);

		static KObject* s_pObject = NULL;
		// ���Һڰ�ͼ���еļ���ͼ�ζ���
		KObject* pObject = NULL;
		if(m_iGisTool & GisTool_BlackboardController)
		{
			// ���Һڰ�ͼ���ҵ�����ͼ�ζ���
			KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
			if(pBlackboardLayer)
			{
				// �����ṩ��
				KProvider* pProvider = pBlackboardLayer->GetProvider();
				if(pProvider)
				{
					// �ڰ����ݶ���
					KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
					if(pBlackboardData && !pBlackboardData->IsEmpty())
					{
						// ȡ�����λ�õĶ���
						pObject = pBlackboardData->GetObject(point);
					}
				}
			}
		}

		// ��������϶����
		m_bMouseDraging = false;

		// ����������(�϶�)
		if( (nFlags & MK_LBUTTON) )
		{
			// ��������϶����
			m_bMouseDraging = true;
			if(!s_pObject)
			{
				s_pObject = pObject;
			}

			// ����������
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// ȡ�õ�ǰ����������
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),s_pObject);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnMouseDrag(&mouseEvent);
			}
		}
		else
		{
			// ��������϶����
			m_bMouseDraging = false;

			s_pObject = NULL;

			// ����������
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// ȡ�õ�ǰ����������
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// ������
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// ��������¼�
				KMouseEvent mouseEvent(mouseEventArgs);
				// ע��˳��(���ƶ�������)
				pMouseListener->OnMouseMove(&mouseEvent);
				pMouseListener->OnMouseHover(&mouseEvent);
			}
		}

		// ��ʾ��γ��
		double xx = point.x;
		double yy = point.y;
		pMapEngine->ClientToWorld(xx,yy);
		// ��γ����Ч
		bool bInvalid = (xx>180 || xx<-180) || (yy>90  || yy<-90);
		if(bInvalid)
			return;

		int iPaneIndex = 0;
		TCHAR szPicInfo[MAX_PATH] = _T("");
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(!pMFCStatusBar)
			return;

		_stprintf(szPicInfo,_T("[����:%8.3f  γ��:%8.3f]"),x,y);
		pMFCStatusBar->SetPaneText(iPaneIndex,szPicInfo);

		return;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
