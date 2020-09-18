// KGisViewBase.cpp : 实现文件
//

#include "stdafx.h"
#include "KGisViewBase.h"

#include "KGisFrameWnd.h"
#include "KGisDocument.h"
#include "KLayerDockablePane.h"

namespace Framework
{

	// KGisViewBase

	IMPLEMENT_DYNCREATE(KGisViewBase, KPrintView)

	KGisViewBase::KGisViewBase()
	{
		// 地理信息工具
		m_iGisTool = GisTool_None;
		// 显示地理信息
		m_bShowGisInfo = true;
		// 鼠标拖动
		m_bMouseDraging = false;

		//光标
		m_hCurDefault   = ::LoadCursor(NULL,IDC_ARROW);	//缺省光标
		m_hCurNew       = m_hCurDefault;			//新光标
		m_hCurOld       = m_hCurDefault;  			//原来光标

		//动画光标
		m_hCurAnimate = NULL;
		//m_hCurAnimate = KResource::LoadAnimateCursor(KIND_IDR_ANI_HAND_APP_START,_T("ANI"));
	}

	KGisViewBase::~KGisViewBase()
	{
		DestroyCursor(m_hCurDefault);
		DestroyCursor(m_hCurOld);
		DestroyCursor(m_hCurNew);
		DestroyCursor(m_hCurAnimate);
	}

	BEGIN_MESSAGE_MAP(KGisViewBase, KPrintView)
		ON_WM_SETCURSOR()
		ON_WM_KEYDOWN()
		ON_WM_KEYUP()

		// 漫游操作
		ON_COMMAND_RANGE(KID_ROAM_BEG,KID_ROAM_END,OnRoamOperate)
		ON_UPDATE_COMMAND_UI_RANGE(KID_ROAM_BEG,KID_ROAM_END,OnRoamOperateUpdate)

		// 添加图层 OnAddLayer
		ON_MESSAGE(WM_USER_LAYER_ADD,OnAddLayer)
		// 移除图层 OnRemoveLayer
		ON_MESSAGE(WM_USER_LAYER_REMOVE,OnRemoveLayer)

		// WM_USER_LANDMARK_CALIBRATE : WPARAM = bDraging, LPARAM = &m_sizeDraging
		ON_MESSAGE(WM_USER_LANDMARK_CALIBRATE,OnLandmarkCalibrate)

		// WM_USER_LANDMARK_CALIBRATE_STEP : WPARAM = 0, LPARAM = &m_sizeDraging
		ON_MESSAGE(WM_USER_LANDMARK_CALIBRATE_STEP,OnLandmarkCalibrateStep)
	END_MESSAGE_MAP()


	// KGisViewBase 绘图

	void KGisViewBase::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KGisViewBase 诊断

	#ifdef _DEBUG
	void KGisViewBase::AssertValid() const
	{
		KPrintView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KGisViewBase::Dump(CDumpContext& dc) const
	{
		KPrintView::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	// KGisViewBase 消息处理程序

	void KGisViewBase::OnInitialUpdate()
	{
		KPrintView::OnInitialUpdate();

		// TODO: 在此添加专用代码和/或调用基类

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// 地图引擎
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == NULL )
			return;

		// 设置使用地图空间的视图窗口句柄
		pMapEngine->SetHwnd(this->GetSafeHwnd());

		// 同步地图引擎和视图的背景颜色
		pMapEngine->SetBackColor(this->GetBackColor());
	}
	void KGisViewBase::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
	{
		KPrintView::OnActivateView(bActivate, pActivateView, pDeactiveView);

		// TODO: 在此添加专用代码和/或调用基类
		CWnd* pWndLayer = NULL;
		BOOL bKindOfMDIFrameWnd = FALSE;
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndLayer = ((KFrameWndEx*)pFrame)->GetLayerWnd();
		}
		else if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			bKindOfMDIFrameWnd = TRUE;
			pWndLayer = ((KMDIFrameWndEx*)pFrame)->GetLayerWnd();
		}

		if(pWndLayer && pWndLayer->GetSafeHwnd())
		{
			if(bActivate)
			{
				// TRUE - 是否强制刷新
				pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)bKindOfMDIFrameWnd,(LPARAM)this);
			}
			else
			{
				pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)FALSE,(LPARAM)this);
			}
		}
		TRACE(_T("OnActivateView - %s\n"),bActivate ? _T("bActivate") : _T("Deactive"));
	}

	BOOL KGisViewBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
		::SetCursor(m_hCurNew);
		return TRUE;
	}

	void KGisViewBase::Refresh(BOOL bErase)
	{
		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		pMapEngine->EnableRedraw();
		Invalidate(bErase);

		return;
	}

	// 添加图层 WM_USER_LAYER_ADD
	sfx_msg LRESULT KGisViewBase::OnAddLayer(WPARAM wParam, LPARAM lParam)
	{
		// 添加图层事件发生
		AddLayerHappened();
		return 0L;
	}
	// 移除图层 WM_USER_LAYER_REMOVE
	sfx_msg LRESULT KGisViewBase::OnRemoveLayer(WPARAM wParam, LPARAM lParam)
	{
		// 移除图层事件发生
		RemoveLayerHappened();
		return 0L;
	}
	// 添加图层发生
	void KGisViewBase::AddLayerHappened()
	{
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(!pFrame)
			return;

		CWnd* pWndLayer = NULL;

		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndLayer = ((KFrameWndEx*)pFrame)->GetLayerWnd();
		}
		else if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			pWndLayer = ((KMDIFrameWndEx*)pFrame)->GetLayerWnd();
		}

		if(!pWndLayer)
			return;

		// 活动则向图层窗口发送消息
		if(pWndLayer->GetSafeHwnd())
		{
			// 刷新图层管理窗口
			pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)TRUE,(LPARAM)this);
		}

		return;
	}
	// 移除图层发生
	void KGisViewBase::RemoveLayerHappened()
	{
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(!pFrame)
			return;

		CWnd* pWndLayer = NULL;

		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndLayer = ((KFrameWndEx*)pFrame)->GetLayerWnd();
		}
		else if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			pWndLayer = ((KMDIFrameWndEx*)pFrame)->GetLayerWnd();
		}

		if(!pWndLayer)
			return;

		// 活动则向图层窗口发送消息
		if(pWndLayer->GetSafeHwnd())
		{
			// 刷新图层管理窗口
			pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)TRUE,(LPARAM)this);
		}

		return;
	}

	///========================================================
	///漫游操作 - Beg
	///---------------
	sfx_msg void KGisViewBase::OnRoamOperate(UINT id)
	{
		// 文档
		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		// 地图引擎
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == NULL )
			return;

		// 鼠标监听器集合
		KMouseListenerCollection* pMouseListenerCollection = pMapEngine->GetMouseListenerCollection();
		if(!pMouseListenerCollection)
			return;

		// 设置当前鼠标监听器键值 = MapController
		//pMouseListenerCollection->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		switch(id)
		{
		case KID_ROAM_CANCEL:		// 取消
			ZoomCancel();
			break;

		case KID_ROAM_RESET:		// 复位
			ZoomReset();
			break;

		case KID_ROAM_REFRESH:		// 刷新
			ZoomRefresh();
			break;

		case KID_ROAM_ZOOM_IN:		// 放大
			ZoomIn();
			break;

		case KID_ROAM_ZOOM_OUT:		// 缩小
			ZoomOut();
			break;

		case KID_ROAM_ZOOM_PAN:		// 漫游
			ZoomPan();
			break;
		}

		return;
	}
	sfx_msg void KGisViewBase::OnRoamOperateUpdate(CCmdUI* pCmdUI)
	{
		UINT id = pCmdUI->m_nID;

		switch(id)
		{
		case KID_ROAM_CANCEL:		// 取消
			pCmdUI->SetCheck(m_iGisTool == GisTool_Cancel);
			break;

		case KID_ROAM_RESET:		// 复位
			pCmdUI->SetCheck(m_iGisTool == GisTool_Reset);
			break;

		case KID_ROAM_REFRESH:	// 刷新
			pCmdUI->SetCheck(m_iGisTool == GisTool_Refresh);
			break;

		case KID_ROAM_ZOOM_IN:		// 放大
			pCmdUI->SetCheck(m_iGisTool == GisTool_ZoomIn);
			break;

		case KID_ROAM_ZOOM_OUT:	// 缩小
			pCmdUI->SetCheck(m_iGisTool == GisTool_ZoomOut);
			break;

		case KID_ROAM_ZOOM_PAN:	// 漫游
			pCmdUI->SetCheck(m_iGisTool == GisTool_ZoomPan);
			break;
		}

		return;
	}
	// 放大
	void KGisViewBase::ZoomIn()
	{
		m_iGisTool = GisTool_ZoomIn;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_ZOOM_IN);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		return;
	}
	// 缩小
	void KGisViewBase::ZoomOut()
	{
		m_iGisTool = GisTool_ZoomOut;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_ZOOM_OUT);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		return;
	}
	// 漫游
	void KGisViewBase::ZoomPan()
	{
		m_iGisTool = GisTool_ZoomPan;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_HAND_ARROW);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		return;
	}
	// 复位
	void KGisViewBase::ZoomReset()
	{
		// 不改变 m_iGisTool

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		pMapEngine->Reset();
		CRect clientRect;
		GetClientRect(&clientRect);
		pMapEngine->ReDraw(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height());
		Invalidate(false);

		return;
	}
	// 刷新(根据需要在派生类中修改)
	void KGisViewBase::ZoomRefresh()
	{
		// 不改变 m_iGisTool

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		pMapEngine->EnableRedraw();
		Invalidate(false);

		return;
	}
	//	取消
	void KGisViewBase::ZoomCancel()
	{
		m_iGisTool = GisTool_None;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = m_hCurDefault;
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,GisTool_ZoomPan);

		return;
	}
	///----------------
	///漫游操作 - End
	///========================================================

	///========================================================
	///键盘操作 - Beg
	///---------------
	void KGisViewBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		//-------------------------------------------------
		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;
		//-------------------------------------------------
		// 地标校准
		if(m_iGisTool == GisTool_Misce_Landmark_Calibrate)
		{
			int dx = 0;
			int dy = 0;
			switch(nChar)
			{
			case VK_LEFT:
				dx = -1; dy = 0;
				break;

			case VK_RIGHT:
				dx = +1; dy = 0;
				break;

			case VK_UP:
				dx = 0; dy = -1;
				break;

			case VK_DOWN:
				dx = 0; dy = +1;
				break;

			default:
				break;
			}

			pMapEngine->LandmarkCalibrate(dx,dy);
		}
		//-------------------------------------------------
		KPrintView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void KGisViewBase::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		//-------------------------------------------------
		//-------------------------------------------------
		KPrintView::OnKeyUp(nChar, nRepCnt, nFlags);
	}
	///----------------
	///键盘操作 - End
	///========================================================

	///========================================================
	// 地标校准
	// 当GisTool = GisTool_Misce_Landmark_Calibrate
	void KGisViewBase::LandmarkCalibrate()
	{
		if(m_iGisTool == GisTool_Misce_Landmark_Calibrate)
		{
			this->ZoomCancel();
			return;
		}

		m_iGisTool = GisTool_Misce_Landmark_Calibrate;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = ::LoadCursor(NULL,IDC_SIZEALL);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// 启动鼠标监听器
		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}

	// 响应 WM_USER_LANDMARK_CALIBRATE
	sfx_msg LRESULT KGisViewBase::OnLandmarkCalibrate(WPARAM wParam,LPARAM lParam)
	{
		//-----------------------------------------------------
		KGisFrameWnd* pMainFrame = (KGisFrameWnd*)AfxGetMainWnd();
		KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();
		KLayer* pLayer = pLayerWnd->GetSelectedLayer();
		if(!pLayer)
		{
			AfxMessageBox(_T("没有选择数据图层"));
			return -1;
		}

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return -1;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return -1;
		//-----------------------------------------------------
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return -1;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if (!pMapEngine)
			return -1;
		//-----------------------------------------------------
		KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;
		if(!pAgmrsData->IsAgmrsData())
		{
			AfxMessageBox(_T("选择数据图层不是遥感数据!"));
			return -1;
		}
		//-----------------------------------------------------
		BOOL bDraging = (BOOL)wParam;
		SIZE size = *(SIZE*)lParam;
		int dx = (int)size.cx;
		int dy = (int)size.cy;
		// 如果是卫星遥感数据,平移
		// dx<0 -> 向左, dx>0 -> 向右
		// dy<0 -> 向上, dy>0 -> 向下
		//-----------------------------------------------------
		static int flag = 0;
		static float fLonMin0 = 0;	// 
		static float fLonMax0 = 0;	// 
		static float fLatMin0 = 0;	// 
		static float fLatMax0 = 0;	// 
		//-----------------------------------------------------
		LDFHeaderCommon* pFileHeader = NULL;

		if(flag==0 && bDraging)		// 第一次
		{
			flag = 1;

			// 保存原来数据
			pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();
			fLonMin0 = pFileHeader->fLonMin;
			fLonMax0 = pFileHeader->fLonMax;
			fLatMin0 = pFileHeader->fLatMin;
			fLatMax0 = pFileHeader->fLatMax;
		}
		else if(flag==1 && bDraging)
		{
			pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();
			pFileHeader->fLonMin = fLonMin0;
			pFileHeader->fLonMax = fLonMax0;
			pFileHeader->fLatMin = fLatMin0;
			pFileHeader->fLatMax = fLatMax0;
		}
		else if(!bDraging)
		{
			flag = 0;
			pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();
			return 0;
			//pFileHeader->fLonMin = fLonMin0;
			//pFileHeader->fLonMax = fLonMax0;
			//pFileHeader->fLatMin = fLatMin0;
			//pFileHeader->fLatMax = fLatMax0;
		}

		// 修改
		pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();

		// 修改经度
		pFileHeader->fLonMin += pFileHeader->fLonResolution * dx;
		pFileHeader->fLonMax += pFileHeader->fLonResolution * dx;
		// 修改纬度
		pFileHeader->fLatMin += pFileHeader->fLatResolution * -dy;
		pFileHeader->fLatMax += pFileHeader->fLatResolution * -dy;

		// 修改数据信息
		pAgmrsData->FillDataInfo(pAgmrsData->FileHeader());
		// 修改包围盒
		pAgmrsData->SetExtents(KExtents(pFileHeader->fLonMin,pFileHeader->fLatMin,pFileHeader->fLonMax,pFileHeader->fLatMax));
		//-----------------------------------------------------
		pMapEngine->Refresh(TRUE);
		//-----------------------------------------------------
		return 0;
	}

	sfx_msg LRESULT KGisViewBase::OnLandmarkCalibrateStep(WPARAM wParam,LPARAM lParam)
	{
		//-----------------------------------------------------
		KGisFrameWnd* pMainFrame = (KGisFrameWnd*)AfxGetMainWnd();
		KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();
		KLayer* pLayer = pLayerWnd->GetSelectedLayer();
		if(!pLayer)
		{
			AfxMessageBox(_T("没有选择数据图层"));
			return -1;
		}

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return -1;

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return -1;
		//-----------------------------------------------------
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return -1;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if (!pMapEngine)
			return -1;
		//-----------------------------------------------------
		KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;
		if(!pAgmrsData->IsAgmrsData())
		{
			AfxMessageBox(_T("选择数据图层不是遥感数据!"));
			return -1;
		}
		//-----------------------------------------------------
		BOOL bDraging = (BOOL)wParam;
		SIZE size = *(SIZE*)lParam;
		//-----------------------------------------------------
		LDFHeaderCommon* pFileHeader = NULL;
		int dx = (int)size.cx;
		int dy = (int)size.cy;
		// 如果是卫星遥感数据,平移
		// dx<0 -> 向左, dx>0 -> 向右
		// dy<0 -> 向上, dy>0 -> 向下
		//-----------------------------------------------------
		// 修改
		pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();

		// 修改经度
		pFileHeader->fLonMin += pFileHeader->fLonResolution * dx;
		pFileHeader->fLonMax += pFileHeader->fLonResolution * dx;
		// 修改纬度
		pFileHeader->fLatMin += pFileHeader->fLatResolution * -dy;
		pFileHeader->fLatMax += pFileHeader->fLatResolution * -dy;

		// 修改数据信息
		pAgmrsData->FillDataInfo(pAgmrsData->FileHeader());
		// 修改包围盒
		pAgmrsData->SetExtents(KExtents(pFileHeader->fLonMin,pFileHeader->fLatMin,pFileHeader->fLonMax,pFileHeader->fLatMax));
		//-----------------------------------------------------
		pMapEngine->Refresh(TRUE);
		//-----------------------------------------------------
		return 0;
	}
	///========================================================

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
