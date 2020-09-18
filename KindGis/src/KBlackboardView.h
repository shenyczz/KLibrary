#pragma once

#include "KAgmrsDataView.h"

namespace Framework
{

	// KBlackboardView ��ͼ

	class KIND_EXT_CLASS KBlackboardView : public KAgmrsDataView
	{
		DECLARE_DYNCREATE(KBlackboardView)

	protected:
		KBlackboardView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KBlackboardView();

	public:
		virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		// �ڰ����
		sfx_msg void OnBlackBoard(UINT id);
		sfx_msg void OnBlackBoardUpdate(CCmdUI* pCmdUI);

		// ��ڰ�ͼ����ӵ� WM_USER_ADD_POINT
		sfx_msg LRESULT OnAddPoint(WPARAM wParam, LPARAM lParam);
		// ��ڰ�ͼ������߶� WM_USER_ADD_LINE
		sfx_msg LRESULT OnAddLine(WPARAM wParam, LPARAM lParam);
		// ��ڰ�ͼ����Ӿ��� WM_USER_ADD_RECT
		sfx_msg LRESULT OnAddRect(WPARAM wParam, LPARAM lParam);
		// ��ڰ�ͼ�������Բ WM_USER_ADD_ELLIPSE
		sfx_msg LRESULT OnAddEllipse(WPARAM wParam, LPARAM lParam);
		// ��ڰ�ͼ��������� WM_USER_ADD_POLYLINE
		sfx_msg LRESULT OnAddPolyline(WPARAM wParam, LPARAM lParam);
		// ��ڰ�ͼ����Ӷ���� WM_USER_ADD_POLYGON
		sfx_msg LRESULT OnAddPolygon(WPARAM wParam, LPARAM lParam);
		//// ��ڰ�ͼ��������� WM_USER_ADD_CURVE
		sfx_msg LRESULT OnAddCurve(WPARAM wParam, LPARAM lParam);
		//// ��ڰ�ͼ����ӱպ����� WM_USER_ADD_CLOSE_CURVE
		sfx_msg LRESULT OnAddCloseCurve(WPARAM wParam, LPARAM lParam);
		// ��ӱ�ע���ڰ�ͼ�� WM_USER_ADD_LABEL
		sfx_msg LRESULT OnAddLabel(WPARAM wParam, LPARAM lParam);
		// ���ͼ��ͼ�����ڰ�ͼ�� WM_USER_ADD_LAYER_LEGEND
		sfx_msg LRESULT OnAddLayerLegend(WPARAM wParam, LPARAM lParam);
		// ����Զ���ͼ�����ڰ�ͼ�� WM_USER_ADD_CUSTOM_LEGEND
		sfx_msg LRESULT OnAddCustomLegend(WPARAM wParam, LPARAM lParam);

		//// ѡ���˶��� WM_USER_SELECT_OBJECT
		sfx_msg LRESULT OnSelectObject(WPARAM wParam, LPARAM lParam);
		//	

		DECLARE_MESSAGE_MAP()

	protected:
		// ѡ��Ŀ��
		void BlackBoard_Select_Object();
		// �ƶ�Ŀ��
		void BlackBoard_Move_Object();
		// ɾ��Ŀ��
		void BlackBoard_Delete_Object();

		// ���Ƶ�
		void Blackboard_Draw_Point();
		// ������
		void Blackboard_Draw_Line();
		// ���ƾ���
		void Blackboard_Draw_Rect();
		// ������Բ
		void Blackboard_Draw_Ellipse();
		// ��������
		void Blackboard_Draw_Polyline();
		// ���ƶ����
		void Blackboard_Draw_Polygon();
		// ���ƹ⻬����
		void Blackboard_Draw_Curve();
		// ���ƹ⻬�պ�����
		void Blackboard_Draw_Close_Curve();

		// ��ע
		void Blackboard_Draw_Label();
		//  ���Ƶ�ǰѡ��ͼ��ͼ��
		void Blackboard_Draw_Layer_Legend();
		// �Զ���ͼ��
		void Blackboard_Draw_Custom_Legend();

	protected:
		// ��ڰ�ͼ����ӵ�
		virtual void AddPoint();
		// ��ڰ�ͼ������߶�
		virtual void AddLine();
		// ��ڰ�ͼ����Ӿ���
		virtual void AddRect();
		// ��ڰ�ͼ�������Բ
		virtual void AddEllipse();
		// ��ڰ�ͼ���������
		virtual void AddPolyline();
		// ��ڰ�ͼ����Ӷ����
		virtual void AddPolygon();
		// ��ڰ�ͼ���������
		virtual void AddCurve();
		// ��ڰ�ͼ����ӱպ�����
		virtual void AddCloseCurve();

		// ��ӱ�ע
		virtual void AddLabel();
		// ���ͼ��ͼ��
		virtual void AddLayerLegend();
		// ����Զ���ͼ��
		virtual void AddCustomLegend();

		// ѡ���˶���
		virtual void SelectObject(LPVOID lpvData);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

