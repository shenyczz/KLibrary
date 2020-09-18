// KBlackboardView.cpp : 实现文件
//

#include "stdafx.h"
#include "KBlackboardView.h"
#include "KBlackboardDocument.h"

#include "KLayerDockablePane.h"
#include "KPropertyDockablePane.h"


namespace Framework
{

	// KBlackboardView

	IMPLEMENT_DYNCREATE(KBlackboardView, KAgmrsDataView)

	KBlackboardView::KBlackboardView()
	{

	}

	KBlackboardView::~KBlackboardView()
	{
	}

	BEGIN_MESSAGE_MAP(KBlackboardView, KAgmrsDataView)
		// 黑板操作
		ON_COMMAND_RANGE(KID_BLACK_BOARD_BEG,KID_BLACK_BOARD_END,OnBlackBoard)
		ON_UPDATE_COMMAND_UI_RANGE(KID_BLACK_BOARD_BEG,KID_BLACK_BOARD_END,OnBlackBoardUpdate)

		// 向黑板图层添加点
		ON_MESSAGE(WM_USER_ADD_POINT,OnAddPoint)
		// 向黑板图层添加线段
		ON_MESSAGE(WM_USER_ADD_LINE,OnAddLine)
		// 向黑板图层添加矩形
		ON_MESSAGE(WM_USER_ADD_RECT,OnAddRect)
		// 向黑板图层添加椭圆
		ON_MESSAGE(WM_USER_ADD_ELLIPSE,OnAddEllipse)
		// 向黑板图层添加折线
		ON_MESSAGE(WM_USER_ADD_POLYLINE,OnAddPolyline)
		// 向黑板图层添加多边形
		ON_MESSAGE(WM_USER_ADD_POLYGON,OnAddPolygon)
		// 向黑板图层添加曲线
		ON_MESSAGE(WM_USER_ADD_CURVE,OnAddCurve)
		// 向黑板图层添加闭合曲线
		ON_MESSAGE(WM_USER_ADD_CLOSE_CURVE,OnAddCloseCurve)

		// 添加标注到黑板图层
		ON_MESSAGE(WM_USER_ADD_LABEL,OnAddLabel)
		// 添加图层图例到黑板图层
		ON_MESSAGE(WM_USER_ADD_LAYER_LEGEND,OnAddLayerLegend)
		// 添加自定义图例到黑板图层
		ON_MESSAGE(WM_USER_ADD_CUSTOM_LEGEND,OnAddCustomLegend)

		// 选择了对象
		ON_MESSAGE(WM_USER_SELECT_OBJECT,OnSelectObject)

	END_MESSAGE_MAP()


	// KBlackboardView 绘图

	void KBlackboardView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KBlackboardView 诊断

	#ifdef _DEBUG
	void KBlackboardView::AssertValid() const
	{
		KAgmrsDataView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KBlackboardView::Dump(CDumpContext& dc) const
	{
		KAgmrsDataView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KBlackboardView 消息处理程序

	///========================================================
	///自定义消息 - Beg
	///-----------------
	// 向黑板图层添加点 WM_USER_ADD_POINT
	sfx_msg LRESULT KBlackboardView::OnAddPoint(WPARAM wParam, LPARAM lParam)
	{
		AddPoint();
		return 0L;
	}
	// 向黑板图层添加线段 WM_USER_ADD_LINE
	sfx_msg LRESULT KBlackboardView::OnAddLine(WPARAM wParam, LPARAM lParam)
	{
		AddLine();
		return 0L;
	}
	// 向黑板图层添加矩形
	sfx_msg LRESULT KBlackboardView::OnAddRect(WPARAM wParam, LPARAM lParam)
	{
		AddRect();
		return 0L;
	}
	// 向黑板图层添加椭圆
	sfx_msg LRESULT KBlackboardView::OnAddEllipse(WPARAM wParam, LPARAM lParam)
	{
		AddEllipse();
		return 0L;
	}
	// 向黑板图层添加折线
	sfx_msg LRESULT KBlackboardView::OnAddPolyline(WPARAM wParam, LPARAM lParam)
	{
		AddPolyline();
		return 0L;
	}
	// 向黑板图层添加多边形
	sfx_msg LRESULT KBlackboardView::OnAddPolygon(WPARAM wParam, LPARAM lParam)
	{
		AddPolygon();
		return 0L;
	}
	// 向黑板图层添加曲线
	sfx_msg LRESULT KBlackboardView::OnAddCurve(WPARAM wParam, LPARAM lParam)
	{
		AddCurve();
		return 0L;
	}
	// 向黑板图层添加闭合曲线
	sfx_msg LRESULT KBlackboardView::OnAddCloseCurve(WPARAM wParam, LPARAM lParam)
	{
		AddCloseCurve();
		return 0L;
	}
	// 添加标注到黑板图层
	sfx_msg LRESULT KBlackboardView::OnAddLabel(WPARAM wParam, LPARAM lParam)
	{
		AddLabel();
		return 0L;
	}
	// 添加图层图例到黑板图层
	sfx_msg LRESULT KBlackboardView::OnAddLayerLegend(WPARAM wParam, LPARAM lParam)
	{
		AddLayerLegend();
		return 0L;
	}
	// 添加自定义图例到黑板图层
	sfx_msg LRESULT KBlackboardView::OnAddCustomLegend(WPARAM wParam, LPARAM lParam)
	{
		AddCustomLegend();
		return 0L;
	}
	// 选择了对象
	sfx_msg LRESULT KBlackboardView::OnSelectObject(WPARAM wParam, LPARAM lParam)
	{
		SelectObject((LPVOID)lParam);
		return 0L;
	}
	///-----------------
	///自定义消息 - End
	///========================================================

	//=====================================================
	// 黑板操作 - beg
	//---------------
	sfx_msg void KBlackboardView::OnBlackBoard(UINT id)
	{
		// 文档
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
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

		// 设置当前鼠标监听器键值 = BlackboardController
		pMouseListenerCollection->SetCurrentMouseListenerKey(GisTool_BlackboardController,GisTool_None);

		switch(id)
		{
		case KID_BLACK_BOARD_SELECT:			// 选择目标
			BlackBoard_Select_Object();
			break;
		case KID_BLACK_BOARD_MOVE:				// 移动目标
			BlackBoard_Move_Object();
			break;
		case KID_BLACK_BOARD_DELETE:			// 删除目标
			BlackBoard_Delete_Object();
			break;

		case KID_BLACK_BOARD_POINT:				// 绘制点
			Blackboard_Draw_Point();
			break;
		case KID_BLACK_BOARD_LINE:				// 绘制线
			Blackboard_Draw_Line();
			break;
		case KID_BLACK_BOARD_RECT:				// 绘制矩形
			Blackboard_Draw_Rect();
			break;
		case KID_BLACK_BOARD_ELLIPSE:			// 绘制椭圆
			Blackboard_Draw_Ellipse();
			break;
		case KID_BLACK_BOARD_POLYLINE:			// 绘制折线
			Blackboard_Draw_Polyline();
			break;
		case KID_BLACK_BOARD_POLYGON:			// 绘制多边形
			Blackboard_Draw_Polygon();
			break;
		case KID_BLACK_BOARD_CURVE:				// 绘制光滑曲线
			Blackboard_Draw_Curve();
			break;
		case KID_BLACK_BOARD_CLOSE_CURVE:		// 绘制闭合光滑曲线
			Blackboard_Draw_Close_Curve();
			break;

		case KID_BLACK_BOARD_LABEL:				// 标注
			Blackboard_Draw_Label();
			break;
		case KID_BLACK_BOARD_LEGEND_CURRENT:	// 当前选中图层图例
			Blackboard_Draw_Layer_Legend();
			break;
		case KID_BLACK_BOARD_LEGEND_CUSTOM:		// 自定义图例
			Blackboard_Draw_Custom_Legend();
			break;
		}

		return;
	}
	sfx_msg void KBlackboardView::OnBlackBoardUpdate(CCmdUI* pCmdUI)
	{
		UINT id = pCmdUI->m_nID;

		switch(id)
		{
		case KID_BLACK_BOARD_SELECT:			// 选择目标
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Select_Object);
			break;
		case KID_BLACK_BOARD_MOVE:				// 移动目标
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Move_Object);
			break;
		case KID_BLACK_BOARD_DELETE:			// 删除目标
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Delete_Object);
			break;

		case KID_BLACK_BOARD_POINT:				// 绘制点
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Point);
			break;
		case KID_BLACK_BOARD_LINE:				// 绘制线
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Line);
			break;
		case KID_BLACK_BOARD_RECT:				// 绘制矩形
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Rect);
			break;
		case KID_BLACK_BOARD_ELLIPSE:			// 绘制椭圆
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Ellipse);
			break;
		case KID_BLACK_BOARD_POLYLINE:			// 绘制折线
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Polyline);
			break;
		case KID_BLACK_BOARD_POLYGON:			// 绘制多边形
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Polygon);
			break;
		case KID_BLACK_BOARD_CURVE:				// 绘制光滑曲线
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Curve);
			break;
		case KID_BLACK_BOARD_CLOSE_CURVE:		// 绘制闭合光滑曲线
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Close_Curve);
			break;

		case KID_BLACK_BOARD_LABEL:				// 标注
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Label);
			break;
		case KID_BLACK_BOARD_LEGEND_CURRENT:	// 当前选中图层图例
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Layer_Legend);
			break;
		case KID_BLACK_BOARD_LEGEND_CUSTOM:		// 自定义图例
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Custom_Legend);
			break;
		}

		return;
	}
	// 选择目标
	void KBlackboardView::BlackBoard_Select_Object()
	{
		m_iGisTool = GisTool_Blackboard_Select_Object;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_SELECT);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 移动目标
	void KBlackboardView::BlackBoard_Move_Object()
	{
		m_iGisTool = GisTool_Blackboard_Move_Object;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = ::LoadCursor(NULL,IDC_SIZEALL);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 删除目标
	void KBlackboardView::BlackBoard_Delete_Object()
	{
		m_iGisTool = GisTool_Blackboard_Delete_Object;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_DELETE);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制点
	void KBlackboardView::Blackboard_Draw_Point()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Point;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_POINT);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制线
	void KBlackboardView::Blackboard_Draw_Line()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Line;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_LINE);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制矩形
	void KBlackboardView::Blackboard_Draw_Rect()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Rect;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_RECT);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制椭圆
	void KBlackboardView::Blackboard_Draw_Ellipse()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Ellipse;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_ELLIPSE);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制折线
	void KBlackboardView::Blackboard_Draw_Polyline()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Polyline;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_POLYLINE);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制多边形
	void KBlackboardView::Blackboard_Draw_Polygon()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Polygon;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_POLYGON);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制光滑曲线
	void KBlackboardView::Blackboard_Draw_Curve()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Curve;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_CURVE);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 绘制光滑闭合曲线
	void KBlackboardView::Blackboard_Draw_Close_Curve()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Close_Curve;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_CLOSE_CURVE);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 标注
	void KBlackboardView::Blackboard_Draw_Label()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Label;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_LABEL);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	//  绘制当前选中图层图例
	void KBlackboardView::Blackboard_Draw_Layer_Legend()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Layer_Legend;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_LAYER_LEGEND);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	// 自定义图例
	void KBlackboardView::Blackboard_Draw_Custom_Legend()
	{
		m_iGisTool = GisTool_Blackboard_Draw_Custom_Legend;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_DRAW_CUSTOM_LEGEND);
		}

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}
	//---------------
	// 黑板操作 - end
	//=====================================================

	//=====================================================
	// 鼠标监听器发送的消息 - beg
	//--------------------------
	// 向黑板图层添加点
	void KBlackboardView::AddPoint()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddPoint(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 向黑板图层添加线段
	void KBlackboardView::AddLine()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddLine(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 向黑板图层添加矩形
	void KBlackboardView::AddRect()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddRect(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 向黑板图层添加椭圆
	void KBlackboardView::AddEllipse()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddEllipse(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 向黑板图层添加折线
	void KBlackboardView::AddPolyline()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		if(pDoc->AddPolyline(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 向黑板图层添加多边形
	void KBlackboardView::AddPolygon()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddPolygon(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 向黑板图层添加曲线
	void KBlackboardView::AddCurve()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddCurve(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 向黑板图层添加闭合曲线
	void KBlackboardView::AddCloseCurve()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddCloseCurve(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 添加标注
	void KBlackboardView::AddLabel()
	{
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		if(pDoc->AddLabel(NULL))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 添加图层图例
	void KBlackboardView::AddLayerLegend()
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

		KLayer* pLayer = ((KLayerDockablePane*)pWndLayer)->GetSelectedLayer();
		if(!pLayer)
			return;

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		if(pDoc->AddLayerLegend(pLayer))
		{
			this->ZoomRefresh();
		}

		return;
	}
	// 添加自定义图例
	void KBlackboardView::AddCustomLegend()
	{
		AfxMessageBox(_T("添加自定义图例"));
	}

	// 选择了对象
	void KBlackboardView::SelectObject(LPVOID lpvData)
	{
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(!pFrame)
			return;

		CWnd* pWndLayer = NULL;
		KPropertyDockablePane* pPropertyWnd = NULL;
		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndLayer = ((KFrameWndEx*)pFrame)->GetLayerWnd();
			pPropertyWnd = (KPropertyDockablePane*)((KFrameWndEx*)pFrame)->GetPropertyWnd();
		}
		else if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			pWndLayer = ((KMDIFrameWndEx*)pFrame)->GetLayerWnd();
			pPropertyWnd = (KPropertyDockablePane*)((KMDIFrameWndEx*)pFrame)->GetPropertyWnd();
		}
		if(!pWndLayer)
			return;
		if(!pPropertyWnd)
			return;

		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		KGeometry* pGeometry = (KGeometry*)lpvData;
		if(!pGeometry)
			return;

		// 现在允许 KPoint,KLine,KLabel, 对象
		//if( TRUE
		//	&& pGeometry->GetType() != KGeometry::GeometryType::Point
		//	&& pGeometry->GetType() != KGeometry::GeometryType::Line
		//	&& pGeometry->GetType() != KGeometry::GeometryType::Label
		//	)
		//	return;

		KLayer* pFirstBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pFirstBlackboardLayer)
			return;

		// 选择黑板图层
		((KLayerDockablePane*)pWndLayer)->SelectLayer(pFirstBlackboardLayer);

		// 强制刷新属性窗口
		pPropertyWnd->Refresh(pFirstBlackboardLayer,TRUE);

		return;
	}
	//--------------------------
	// 鼠标监听器发送的消息 - end
	//=====================================================

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
