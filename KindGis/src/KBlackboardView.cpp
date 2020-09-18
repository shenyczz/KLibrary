// KBlackboardView.cpp : ʵ���ļ�
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
		// �ڰ����
		ON_COMMAND_RANGE(KID_BLACK_BOARD_BEG,KID_BLACK_BOARD_END,OnBlackBoard)
		ON_UPDATE_COMMAND_UI_RANGE(KID_BLACK_BOARD_BEG,KID_BLACK_BOARD_END,OnBlackBoardUpdate)

		// ��ڰ�ͼ����ӵ�
		ON_MESSAGE(WM_USER_ADD_POINT,OnAddPoint)
		// ��ڰ�ͼ������߶�
		ON_MESSAGE(WM_USER_ADD_LINE,OnAddLine)
		// ��ڰ�ͼ����Ӿ���
		ON_MESSAGE(WM_USER_ADD_RECT,OnAddRect)
		// ��ڰ�ͼ�������Բ
		ON_MESSAGE(WM_USER_ADD_ELLIPSE,OnAddEllipse)
		// ��ڰ�ͼ���������
		ON_MESSAGE(WM_USER_ADD_POLYLINE,OnAddPolyline)
		// ��ڰ�ͼ����Ӷ����
		ON_MESSAGE(WM_USER_ADD_POLYGON,OnAddPolygon)
		// ��ڰ�ͼ���������
		ON_MESSAGE(WM_USER_ADD_CURVE,OnAddCurve)
		// ��ڰ�ͼ����ӱպ�����
		ON_MESSAGE(WM_USER_ADD_CLOSE_CURVE,OnAddCloseCurve)

		// ��ӱ�ע���ڰ�ͼ��
		ON_MESSAGE(WM_USER_ADD_LABEL,OnAddLabel)
		// ���ͼ��ͼ�����ڰ�ͼ��
		ON_MESSAGE(WM_USER_ADD_LAYER_LEGEND,OnAddLayerLegend)
		// ����Զ���ͼ�����ڰ�ͼ��
		ON_MESSAGE(WM_USER_ADD_CUSTOM_LEGEND,OnAddCustomLegend)

		// ѡ���˶���
		ON_MESSAGE(WM_USER_SELECT_OBJECT,OnSelectObject)

	END_MESSAGE_MAP()


	// KBlackboardView ��ͼ

	void KBlackboardView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KBlackboardView ���

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


	// KBlackboardView ��Ϣ�������

	///========================================================
	///�Զ�����Ϣ - Beg
	///-----------------
	// ��ڰ�ͼ����ӵ� WM_USER_ADD_POINT
	sfx_msg LRESULT KBlackboardView::OnAddPoint(WPARAM wParam, LPARAM lParam)
	{
		AddPoint();
		return 0L;
	}
	// ��ڰ�ͼ������߶� WM_USER_ADD_LINE
	sfx_msg LRESULT KBlackboardView::OnAddLine(WPARAM wParam, LPARAM lParam)
	{
		AddLine();
		return 0L;
	}
	// ��ڰ�ͼ����Ӿ���
	sfx_msg LRESULT KBlackboardView::OnAddRect(WPARAM wParam, LPARAM lParam)
	{
		AddRect();
		return 0L;
	}
	// ��ڰ�ͼ�������Բ
	sfx_msg LRESULT KBlackboardView::OnAddEllipse(WPARAM wParam, LPARAM lParam)
	{
		AddEllipse();
		return 0L;
	}
	// ��ڰ�ͼ���������
	sfx_msg LRESULT KBlackboardView::OnAddPolyline(WPARAM wParam, LPARAM lParam)
	{
		AddPolyline();
		return 0L;
	}
	// ��ڰ�ͼ����Ӷ����
	sfx_msg LRESULT KBlackboardView::OnAddPolygon(WPARAM wParam, LPARAM lParam)
	{
		AddPolygon();
		return 0L;
	}
	// ��ڰ�ͼ���������
	sfx_msg LRESULT KBlackboardView::OnAddCurve(WPARAM wParam, LPARAM lParam)
	{
		AddCurve();
		return 0L;
	}
	// ��ڰ�ͼ����ӱպ�����
	sfx_msg LRESULT KBlackboardView::OnAddCloseCurve(WPARAM wParam, LPARAM lParam)
	{
		AddCloseCurve();
		return 0L;
	}
	// ��ӱ�ע���ڰ�ͼ��
	sfx_msg LRESULT KBlackboardView::OnAddLabel(WPARAM wParam, LPARAM lParam)
	{
		AddLabel();
		return 0L;
	}
	// ���ͼ��ͼ�����ڰ�ͼ��
	sfx_msg LRESULT KBlackboardView::OnAddLayerLegend(WPARAM wParam, LPARAM lParam)
	{
		AddLayerLegend();
		return 0L;
	}
	// ����Զ���ͼ�����ڰ�ͼ��
	sfx_msg LRESULT KBlackboardView::OnAddCustomLegend(WPARAM wParam, LPARAM lParam)
	{
		AddCustomLegend();
		return 0L;
	}
	// ѡ���˶���
	sfx_msg LRESULT KBlackboardView::OnSelectObject(WPARAM wParam, LPARAM lParam)
	{
		SelectObject((LPVOID)lParam);
		return 0L;
	}
	///-----------------
	///�Զ�����Ϣ - End
	///========================================================

	//=====================================================
	// �ڰ���� - beg
	//---------------
	sfx_msg void KBlackboardView::OnBlackBoard(UINT id)
	{
		// �ĵ�
		KBlackboardDocument* pDoc = (KBlackboardDocument*)GetDocument();
		if (!pDoc)
			return;

		// ��ͼ����
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == NULL )
			return;

		// ������������
		KMouseListenerCollection* pMouseListenerCollection = pMapEngine->GetMouseListenerCollection();
		if(!pMouseListenerCollection)
			return;

		// ���õ�ǰ����������ֵ = BlackboardController
		pMouseListenerCollection->SetCurrentMouseListenerKey(GisTool_BlackboardController,GisTool_None);

		switch(id)
		{
		case KID_BLACK_BOARD_SELECT:			// ѡ��Ŀ��
			BlackBoard_Select_Object();
			break;
		case KID_BLACK_BOARD_MOVE:				// �ƶ�Ŀ��
			BlackBoard_Move_Object();
			break;
		case KID_BLACK_BOARD_DELETE:			// ɾ��Ŀ��
			BlackBoard_Delete_Object();
			break;

		case KID_BLACK_BOARD_POINT:				// ���Ƶ�
			Blackboard_Draw_Point();
			break;
		case KID_BLACK_BOARD_LINE:				// ������
			Blackboard_Draw_Line();
			break;
		case KID_BLACK_BOARD_RECT:				// ���ƾ���
			Blackboard_Draw_Rect();
			break;
		case KID_BLACK_BOARD_ELLIPSE:			// ������Բ
			Blackboard_Draw_Ellipse();
			break;
		case KID_BLACK_BOARD_POLYLINE:			// ��������
			Blackboard_Draw_Polyline();
			break;
		case KID_BLACK_BOARD_POLYGON:			// ���ƶ����
			Blackboard_Draw_Polygon();
			break;
		case KID_BLACK_BOARD_CURVE:				// ���ƹ⻬����
			Blackboard_Draw_Curve();
			break;
		case KID_BLACK_BOARD_CLOSE_CURVE:		// ���ƱպϹ⻬����
			Blackboard_Draw_Close_Curve();
			break;

		case KID_BLACK_BOARD_LABEL:				// ��ע
			Blackboard_Draw_Label();
			break;
		case KID_BLACK_BOARD_LEGEND_CURRENT:	// ��ǰѡ��ͼ��ͼ��
			Blackboard_Draw_Layer_Legend();
			break;
		case KID_BLACK_BOARD_LEGEND_CUSTOM:		// �Զ���ͼ��
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
		case KID_BLACK_BOARD_SELECT:			// ѡ��Ŀ��
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Select_Object);
			break;
		case KID_BLACK_BOARD_MOVE:				// �ƶ�Ŀ��
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Move_Object);
			break;
		case KID_BLACK_BOARD_DELETE:			// ɾ��Ŀ��
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Delete_Object);
			break;

		case KID_BLACK_BOARD_POINT:				// ���Ƶ�
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Point);
			break;
		case KID_BLACK_BOARD_LINE:				// ������
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Line);
			break;
		case KID_BLACK_BOARD_RECT:				// ���ƾ���
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Rect);
			break;
		case KID_BLACK_BOARD_ELLIPSE:			// ������Բ
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Ellipse);
			break;
		case KID_BLACK_BOARD_POLYLINE:			// ��������
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Polyline);
			break;
		case KID_BLACK_BOARD_POLYGON:			// ���ƶ����
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Polygon);
			break;
		case KID_BLACK_BOARD_CURVE:				// ���ƹ⻬����
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Curve);
			break;
		case KID_BLACK_BOARD_CLOSE_CURVE:		// ���ƱպϹ⻬����
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Close_Curve);
			break;

		case KID_BLACK_BOARD_LABEL:				// ��ע
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Label);
			break;
		case KID_BLACK_BOARD_LEGEND_CURRENT:	// ��ǰѡ��ͼ��ͼ��
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Layer_Legend);
			break;
		case KID_BLACK_BOARD_LEGEND_CUSTOM:		// �Զ���ͼ��
			pCmdUI->SetCheck(m_iGisTool == GisTool_Blackboard_Draw_Custom_Legend);
			break;
		}

		return;
	}
	// ѡ��Ŀ��
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
	// �ƶ�Ŀ��
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
	// ɾ��Ŀ��
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
	// ���Ƶ�
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
	// ������
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
	// ���ƾ���
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
	// ������Բ
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
	// ��������
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
	// ���ƶ����
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
	// ���ƹ⻬����
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
	// ���ƹ⻬�պ�����
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
	// ��ע
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
	//  ���Ƶ�ǰѡ��ͼ��ͼ��
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
	// �Զ���ͼ��
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
	// �ڰ���� - end
	//=====================================================

	//=====================================================
	// �����������͵���Ϣ - beg
	//--------------------------
	// ��ڰ�ͼ����ӵ�
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
	// ��ڰ�ͼ������߶�
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
	// ��ڰ�ͼ����Ӿ���
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
	// ��ڰ�ͼ�������Բ
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
	// ��ڰ�ͼ���������
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
	// ��ڰ�ͼ����Ӷ����
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
	// ��ڰ�ͼ���������
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
	// ��ڰ�ͼ����ӱպ�����
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
	// ��ӱ�ע
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
	// ���ͼ��ͼ��
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
	// ����Զ���ͼ��
	void KBlackboardView::AddCustomLegend()
	{
		AfxMessageBox(_T("����Զ���ͼ��"));
	}

	// ѡ���˶���
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

		// �������� KPoint,KLine,KLabel, ����
		//if( TRUE
		//	&& pGeometry->GetType() != KGeometry::GeometryType::Point
		//	&& pGeometry->GetType() != KGeometry::GeometryType::Line
		//	&& pGeometry->GetType() != KGeometry::GeometryType::Label
		//	)
		//	return;

		KLayer* pFirstBlackboardLayer = pMapEngine->GetBlackboardLayer(0);
		if(!pFirstBlackboardLayer)
			return;

		// ѡ��ڰ�ͼ��
		((KLayerDockablePane*)pWndLayer)->SelectLayer(pFirstBlackboardLayer);

		// ǿ��ˢ�����Դ���
		pPropertyWnd->Refresh(pFirstBlackboardLayer,TRUE);

		return;
	}
	//--------------------------
	// �����������͵���Ϣ - end
	//=====================================================

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
