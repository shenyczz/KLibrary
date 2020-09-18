// KGisView.cpp : 实现文件
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
		// 鼠标左键消息
		ON_WM_LBUTTONDOWN()
		ON_WM_LBUTTONUP()
		ON_WM_LBUTTONDBLCLK()
		// 鼠标右键消息
		ON_WM_RBUTTONDOWN()
		ON_WM_RBUTTONUP()
		ON_WM_RBUTTONDBLCLK()
		// 鼠标移动消息
		ON_WM_MOUSEMOVE()
	END_MESSAGE_MAP()

	// KGisView 绘图

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

	// KGisView 诊断

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


	// KGisView 消息处理程序

	void KGisView::OnContextMenu(CWnd* pWnd, CPoint point)
	{
		CMenu menu;
		// 装载资源库中的漫游菜单
		//if(!menu.Attach(KResource::LoadMenu(KIND_IDR_MENU_ROAM)))
			return;

		// 取得子菜单
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

	// 左键按下
	void KGisView::OnLButtonDown(UINT nFlags, CPoint point)
	{
		// 调用父类同名函数
		KFinalView::OnLButtonDown(nFlags, point);

		// 捕获鼠标(要在 OnLButtonUp 中释放捕获)
		::SetCapture(this->GetSafeHwnd());

		if(m_iGisTool == GisTool_ZoomPan)  //移动
		{
			m_hCurOld = m_hCurNew;	// m_hCurOld 原来的手形光标,在OnLButtonUp中恢复
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_HAND_HOLD);
			SetCursor(m_hCurNew);
		}

		// 获取文档
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// 取得地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// 坐标位置
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);
		// 世界坐标鼠标点
		KPoint kpoint = KPoint(x,y);

		// 查找黑板图层中的几何图形对象
		KObject* pObject = NULL;
		if(m_iGisTool & GisTool_BlackboardController)	// 是黑板操作工具
		{
			// 查找黑板图层找到集合图形对象
			KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
			if(pBlackboardLayer)
			{
				// 数据提供者
				KProvider* pProvider = pBlackboardLayer->GetProvider();
				// 黑板数据对象
				KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
				if(pBlackboardData && !pBlackboardData->IsEmpty())
				{
					// 取得鼠标位置的对象
					pObject = pBlackboardData->GetObject(point);
				}
			}// if(pBlackboardLayer)

		}// if(m_iGisTool & BlackboardController)

		// 鼠标左键按下
		if( nFlags & MK_LBUTTON )
		{
			// 鼠标监听器集
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// 取得当前的鼠标监听器
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonDown(&mouseEvent);
			}
		}
	}
	// 左键放开
	void KGisView::OnLButtonUp(UINT nFlags, CPoint point)
	{
		// 调用父类同名函数
		KFinalView::OnLButtonUp(nFlags, point);

		// 复位鼠标拖动标记
		m_bMouseDraging = false;

		// 释放鼠标(在 OnLButtonDown 中捕获的)
		::ReleaseCapture();

		// 移动
		if(m_iGisTool == GisTool_ZoomPan)
		{
			m_hCurOld = m_hCurNew;
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_HAND_ARROW);
			SetCursor(m_hCurNew);
		}

		// 获取文档
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// 取得地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return;

		// 查找黑板图层中的几何图形对象
		KObject* pObject = NULL;
		// ...

		// 鼠标左键放开
		if( !(nFlags & MK_LBUTTON) )
		{
			// 鼠标监听器集
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// 取得当前的鼠标监听器
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonUp(&mouseEvent);
			}
		}

		return;
	}
	// 左键双击
	void KGisView::OnLButtonDblClk(UINT nFlags, CPoint point)
	{
		// 调用父类同名函数
		KFinalView::OnLButtonDblClk(nFlags, point);

		// 获取文档
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// 取得地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// 坐标位置
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);
		// 世界坐标鼠标点
		KPoint kpoint = KPoint(x,y);

		// 查找黑板图层中的几何图形对象
		KObject* pObject = NULL;
		if(m_iGisTool & GisTool_BlackboardController)
		{
			// 查找黑板图层找到集合图形对象
			KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
			if(pBlackboardLayer)
			{
				// 数据提供者
				KProvider* pProvider = pBlackboardLayer->GetProvider();
				if(pProvider)
				{
					// 黑板数据对象
					KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
					if(pBlackboardData && !pBlackboardData->IsEmpty())
					{
						// 取得鼠标位置的对象
						pObject = pBlackboardData->GetObject(point);
					}
				}// if(pProvider)
			}// if(pBlackboardLayer)
		}// if(m_iGisTool & GisTool_BlackboardController)

		// 鼠标监听器集
		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		// 如果鼠标监听器监听的目标是地图控件,
		// 并且地图控件当前没有设置操作或是漫游操作,
		// 那么允许双击左键进行放大,并且设置当前操作为漫游
		int key = pMouseListeners->GetCurrentMouseListenerKey();
		int subKey = pMouseListeners->GetCurrentMouseListenerSubKey();
		if( key==GisTool_MapController && (subKey==GisTool_ZoomPan || subKey==GisTool_None) )
		{
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonDBLClick(&mouseEvent);
			}
		}
		else if( key==GisTool_BlackboardController && (subKey==GisTool_Blackboard_Select_Object) )
		{
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnLButtonDBLClick(&mouseEvent);
			}
		}

		// FUN_END
	}

	// 右键按下
	void KGisView::OnRButtonDown(UINT nFlags, CPoint point)
	{
		// 调用父类同名函数
		KFinalView::OnRButtonDown(nFlags, point);
		// FUN_END
	}
	// 右键放开
	void KGisView::OnRButtonUp(UINT nFlags, CPoint point)
	{
		// 调用父类同名函数
		KFinalView::OnRButtonUp(nFlags, point);
		// 调用父类同名函数(...)
		KFinalView::OnLButtonDblClk(nFlags, point);

		// 获取文档
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// 取得地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// 鼠标右键放开
		if( !(nFlags & MK_RBUTTON) )
		{
			// 鼠标监听器集
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// 取得当前的鼠标监听器
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),NULL);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnRButtonUp(&mouseEvent);
			}
		}

		return;
	}
	// 右键双击
	void KGisView::OnRButtonDblClk(UINT nFlags, CPoint point)
	{
		// 调用父类同名函数
		KFinalView::OnRButtonDblClk(nFlags, point);

		// 获取文档
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// 取得地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// 鼠标监听器集
		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		// 如果鼠标监听器监听的目标是地图控件,
		// 并且地图控件当前没有设置操作或是漫游操作,
		// 那么允许双击左键进行放大,并且设置当前操作为漫游
		int key = pMouseListeners->GetCurrentMouseListenerKey();
		int subKey = pMouseListeners->GetCurrentMouseListenerSubKey();
		if( key==GisTool_MapController && (subKey==GisTool_ZoomPan || subKey==GisTool_None) )
		{
			pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,GisTool_ZoomPan);
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),NULL);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnRButtonDBLClick(&mouseEvent);
			}
		}

		return;
	}
	// 鼠标移动
	void KGisView::OnMouseMove(UINT nFlags, CPoint point)
	{
		// 调用父类同名函数
		KFinalView::OnMouseMove(nFlags, point);

		if(!m_hCurNew)
		{
			m_hCurNew = m_hCurDefault;	//缺省光标
		}

		// 获取文档
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return;

		// 取得地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// 坐标位置
		double x = point.x;
		double y = point.y;
		pMapEngine->ClientToWorld(x,y);
		// 世界坐标鼠标点
		KPoint kpoint = KPoint(x,y);

		static KObject* s_pObject = NULL;
		// 查找黑板图层中的几何图形对象
		KObject* pObject = NULL;
		if(m_iGisTool & GisTool_BlackboardController)
		{
			// 查找黑板图层找到集合图形对象
			KLayer* pBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
			if(pBlackboardLayer)
			{
				// 数据提供者
				KProvider* pProvider = pBlackboardLayer->GetProvider();
				if(pProvider)
				{
					// 黑板数据对象
					KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
					if(pBlackboardData && !pBlackboardData->IsEmpty())
					{
						// 取得鼠标位置的对象
						pObject = pBlackboardData->GetObject(point);
					}
				}
			}
		}

		// 设置鼠标拖动标记
		m_bMouseDraging = false;

		// 如果按下左键(拖动)
		if( (nFlags & MK_LBUTTON) )
		{
			// 设置鼠标拖动标记
			m_bMouseDraging = true;
			if(!s_pObject)
			{
				s_pObject = pObject;
			}

			// 鼠标监听器集
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// 取得当前的鼠标监听器
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),s_pObject);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				pMouseListener->OnMouseDrag(&mouseEvent);
			}
		}
		else
		{
			// 设置鼠标拖动标记
			m_bMouseDraging = false;

			s_pObject = NULL;

			// 鼠标监听器集
			KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
			// 取得当前的鼠标监听器
			KMouseListener* pMouseListener = pMouseListeners->GetCurrentMouseListener();
			if( pMouseListener != 0 )
			{
				// 鼠标参数
				KMouseEventArgs mouseEventArgs
					= KMouseEventArgs(pMapEngine,nFlags,point,pMouseListeners->GetCurrentMouseListenerSubKey(),pObject);
				// 构造鼠标事件
				KMouseEvent mouseEvent(mouseEventArgs);
				// 注意顺序(先移动再盘旋)
				pMouseListener->OnMouseMove(&mouseEvent);
				pMouseListener->OnMouseHover(&mouseEvent);
			}
		}

		// 显示经纬度
		double xx = point.x;
		double yy = point.y;
		pMapEngine->ClientToWorld(xx,yy);
		// 经纬度无效
		bool bInvalid = (xx>180 || xx<-180) || (yy>90  || yy<-90);
		if(bInvalid)
			return;

		int iPaneIndex = 0;
		TCHAR szPicInfo[MAX_PATH] = _T("");
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(!pMFCStatusBar)
			return;

		_stprintf(szPicInfo,_T("[经度:%8.3f  纬度:%8.3f]"),x,y);
		pMFCStatusBar->SetPaneText(iPaneIndex,szPicInfo);

		return;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
