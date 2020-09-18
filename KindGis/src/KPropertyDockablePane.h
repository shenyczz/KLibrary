/******************************************************************************
 ** KPropertyDockablePane.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPropertyDockablePane - 属性可停靠窗格
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

namespace DockablePanes
{

	// KPropertyDockablePane

	class KIND_EXT_CLASS KPropertyDockablePane : public KDockablePane
	{
		DECLARE_DYNAMIC(KPropertyDockablePane)

	public:
		KPropertyDockablePane();
		virtual ~KPropertyDockablePane();

	protected:
		// 注册消息 AFX_WM_PROPERTY_CHANGED
		afx_msg LRESULT OnPropertyChanged(WPARAM wParam,LPARAM lParam);
		// 来自 LayerWnd WM_USER_REFRESH_PROPERTY_WND 消息
		sfx_msg LRESULT OnRefreshPropertyWnd(WPARAM wParam,LPARAM lParam);

		DECLARE_MESSAGE_MAP()

	public:
		//=================================================
		// 刷新停靠窗口(必要时在派生类重载)
		virtual void Refresh(LPVOID lpvData, BOOL bForceRefresh = FALSE);
		protected:
			// 建立停靠窗口内部控件(必要时在派生类重载)
			virtual BOOL CreateCtrl();
			// 设置控件字体(必要时在派生类重载)
			virtual void SetCtrlFont();
			// 调整停靠窗口布局(必要时在派生类重载)
			virtual void AdjustLayout();
		//=================================================

	protected:
		//-------------------------------------------------
		// 添加属性组
		virtual void AddPropertyGroup();
		//-----------
		// 添加属性组_杂项
		virtual void addPropertyGroup_Misce();
		// 添加组_经度
		virtual void addPropertyGroup_Lon();
		// 添加属性组_纬度
		virtual void addPropertyGroup_Lat();
		// 添加属性组_数据尺寸
		virtual void addPropertyGroup_DataSize();
		// 添加属性组_站点信息
		virtual void addPropertyGroup_Station();
		// 添加属性组_字体信息
		virtual void addPropertyGroup_Font();
		// 添加属性组_通道
		virtual void addPropertyGroup_Channel();
		// 添加属性组_地理信息
		virtual void addPropertyGroup_GeoInfo();
		// 添加属性组_控制
		virtual void addPropertyGroup_Control();
		// 添加属性组_图例
		virtual void addPropertyGroup_Legend();

	protected:
		//-------------------------------------------------
		// 属性改变(有注册消息调用)
		//------------------------
		virtual void PropertyChanged(LPVOID lpvData);
		// 属性改变_标注文本字体
		virtual void propertyChanged_Label(LPVOID lpvData);
		// 属性改变_通道
		virtual void propertyChanged_Channel(LPVOID lpvData);
		// 属性改变_控制
		virtual void propertyChanged_Control(LPVOID lpvData);
		// 属性改变_地理信息
		virtual void propertyChanged_GeoInfo(LPVOID lpvData);
		// 属性改变_图例
		virtual void propertyChanged_Legend(LPVOID lpvData);
		//-------------------------------------------------

	private:
		// 添加控制 - 公共属性
		virtual void addPropertyGroup_Control_Common(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// 添加控制 - ShapeLayer 属性
		virtual void addPropertyGroup_Control_ShapeLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// 添加控制 - StationLayer 属性
		virtual void addPropertyGroup_Control_StationLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// 添加控制 - GridContourLayer 属性
		virtual void addPropertyGroup_Control_GridContourLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// 添加控制 - WindVectorLayer 属性
		virtual void addPropertyGroup_Control_WindVectorLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// 添加控制 - RpgLayer 属性
		virtual void addPropertyGroup_Control_RpgLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// 添加控制 - BlackboardLayer 属性
		virtual void addPropertyGroup_Control_BlackboardLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
	
		// 添加控制 - BlackboardLayer 的 Point 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Point(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 Line 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Line(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 Rect 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Rect(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 Ellipse 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Ellipse(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 Polyline 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Polyline(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 Polygon 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Polygon(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 Curve 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Curve(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加属性组_字体信息(黑板)
		virtual void addPropertyGroup_Control_BlackboardLayer_Label(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 Lengend 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_Legend(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);

		// 添加控制 - BlackboardLayer 的 LinearGeometry_Common 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// 添加控制 - BlackboardLayer 的 LinearGeometry_CanFill 对象属性
		virtual void addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);

	protected:
		// 刷新活动视图
		void RefreshActiveView();

	protected:
		// 当前属性拥有者
		LPVOID m_lpvPropertyOwner;
		// 属性网格控件
		CMFCPropertyGridCtrl* m_pMfcPropertyGridCtrl;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

