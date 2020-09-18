#pragma once

#include "KAgmrsDataView.h"

namespace Framework
{

	// KBlackboardView 视图

	class KIND_EXT_CLASS KBlackboardView : public KAgmrsDataView
	{
		DECLARE_DYNCREATE(KBlackboardView)

	protected:
		KBlackboardView();           // 动态创建所使用的受保护的构造函数
		virtual ~KBlackboardView();

	public:
		virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		// 黑板操作
		sfx_msg void OnBlackBoard(UINT id);
		sfx_msg void OnBlackBoardUpdate(CCmdUI* pCmdUI);

		// 向黑板图层添加点 WM_USER_ADD_POINT
		sfx_msg LRESULT OnAddPoint(WPARAM wParam, LPARAM lParam);
		// 向黑板图层添加线段 WM_USER_ADD_LINE
		sfx_msg LRESULT OnAddLine(WPARAM wParam, LPARAM lParam);
		// 向黑板图层添加矩形 WM_USER_ADD_RECT
		sfx_msg LRESULT OnAddRect(WPARAM wParam, LPARAM lParam);
		// 向黑板图层添加椭圆 WM_USER_ADD_ELLIPSE
		sfx_msg LRESULT OnAddEllipse(WPARAM wParam, LPARAM lParam);
		// 向黑板图层添加折线 WM_USER_ADD_POLYLINE
		sfx_msg LRESULT OnAddPolyline(WPARAM wParam, LPARAM lParam);
		// 向黑板图层添加多边形 WM_USER_ADD_POLYGON
		sfx_msg LRESULT OnAddPolygon(WPARAM wParam, LPARAM lParam);
		//// 向黑板图层添加曲线 WM_USER_ADD_CURVE
		sfx_msg LRESULT OnAddCurve(WPARAM wParam, LPARAM lParam);
		//// 向黑板图层添加闭合曲线 WM_USER_ADD_CLOSE_CURVE
		sfx_msg LRESULT OnAddCloseCurve(WPARAM wParam, LPARAM lParam);
		// 添加标注到黑板图层 WM_USER_ADD_LABEL
		sfx_msg LRESULT OnAddLabel(WPARAM wParam, LPARAM lParam);
		// 添加图层图例到黑板图层 WM_USER_ADD_LAYER_LEGEND
		sfx_msg LRESULT OnAddLayerLegend(WPARAM wParam, LPARAM lParam);
		// 添加自定义图例到黑板图层 WM_USER_ADD_CUSTOM_LEGEND
		sfx_msg LRESULT OnAddCustomLegend(WPARAM wParam, LPARAM lParam);

		//// 选择了对象 WM_USER_SELECT_OBJECT
		sfx_msg LRESULT OnSelectObject(WPARAM wParam, LPARAM lParam);
		//	

		DECLARE_MESSAGE_MAP()

	protected:
		// 选择目标
		void BlackBoard_Select_Object();
		// 移动目标
		void BlackBoard_Move_Object();
		// 删除目标
		void BlackBoard_Delete_Object();

		// 绘制点
		void Blackboard_Draw_Point();
		// 绘制线
		void Blackboard_Draw_Line();
		// 绘制矩形
		void Blackboard_Draw_Rect();
		// 绘制椭圆
		void Blackboard_Draw_Ellipse();
		// 绘制折线
		void Blackboard_Draw_Polyline();
		// 绘制多边形
		void Blackboard_Draw_Polygon();
		// 绘制光滑曲线
		void Blackboard_Draw_Curve();
		// 绘制光滑闭合曲线
		void Blackboard_Draw_Close_Curve();

		// 标注
		void Blackboard_Draw_Label();
		//  绘制当前选中图层图例
		void Blackboard_Draw_Layer_Legend();
		// 自定义图例
		void Blackboard_Draw_Custom_Legend();

	protected:
		// 向黑板图层添加点
		virtual void AddPoint();
		// 向黑板图层添加线段
		virtual void AddLine();
		// 向黑板图层添加矩形
		virtual void AddRect();
		// 向黑板图层添加椭圆
		virtual void AddEllipse();
		// 向黑板图层添加折线
		virtual void AddPolyline();
		// 向黑板图层添加多边形
		virtual void AddPolygon();
		// 向黑板图层添加曲线
		virtual void AddCurve();
		// 向黑板图层添加闭合曲线
		virtual void AddCloseCurve();

		// 添加标注
		virtual void AddLabel();
		// 添加图层图例
		virtual void AddLayerLegend();
		// 添加自定义图例
		virtual void AddCustomLegend();

		// 选择了对象
		virtual void SelectObject(LPVOID lpvData);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

