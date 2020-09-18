/******************************************************************************
 ** KPropertyDockablePane.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPropertyDockablePane - ���Կ�ͣ������
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
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
		// ע����Ϣ AFX_WM_PROPERTY_CHANGED
		afx_msg LRESULT OnPropertyChanged(WPARAM wParam,LPARAM lParam);
		// ���� LayerWnd WM_USER_REFRESH_PROPERTY_WND ��Ϣ
		sfx_msg LRESULT OnRefreshPropertyWnd(WPARAM wParam,LPARAM lParam);

		DECLARE_MESSAGE_MAP()

	public:
		//=================================================
		// ˢ��ͣ������(��Ҫʱ������������)
		virtual void Refresh(LPVOID lpvData, BOOL bForceRefresh = FALSE);
		protected:
			// ����ͣ�������ڲ��ؼ�(��Ҫʱ������������)
			virtual BOOL CreateCtrl();
			// ���ÿؼ�����(��Ҫʱ������������)
			virtual void SetCtrlFont();
			// ����ͣ�����ڲ���(��Ҫʱ������������)
			virtual void AdjustLayout();
		//=================================================

	protected:
		//-------------------------------------------------
		// ���������
		virtual void AddPropertyGroup();
		//-----------
		// ���������_����
		virtual void addPropertyGroup_Misce();
		// �����_����
		virtual void addPropertyGroup_Lon();
		// ���������_γ��
		virtual void addPropertyGroup_Lat();
		// ���������_���ݳߴ�
		virtual void addPropertyGroup_DataSize();
		// ���������_վ����Ϣ
		virtual void addPropertyGroup_Station();
		// ���������_������Ϣ
		virtual void addPropertyGroup_Font();
		// ���������_ͨ��
		virtual void addPropertyGroup_Channel();
		// ���������_������Ϣ
		virtual void addPropertyGroup_GeoInfo();
		// ���������_����
		virtual void addPropertyGroup_Control();
		// ���������_ͼ��
		virtual void addPropertyGroup_Legend();

	protected:
		//-------------------------------------------------
		// ���Ըı�(��ע����Ϣ����)
		//------------------------
		virtual void PropertyChanged(LPVOID lpvData);
		// ���Ըı�_��ע�ı�����
		virtual void propertyChanged_Label(LPVOID lpvData);
		// ���Ըı�_ͨ��
		virtual void propertyChanged_Channel(LPVOID lpvData);
		// ���Ըı�_����
		virtual void propertyChanged_Control(LPVOID lpvData);
		// ���Ըı�_������Ϣ
		virtual void propertyChanged_GeoInfo(LPVOID lpvData);
		// ���Ըı�_ͼ��
		virtual void propertyChanged_Legend(LPVOID lpvData);
		//-------------------------------------------------

	private:
		// ��ӿ��� - ��������
		virtual void addPropertyGroup_Control_Common(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// ��ӿ��� - ShapeLayer ����
		virtual void addPropertyGroup_Control_ShapeLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// ��ӿ��� - StationLayer ����
		virtual void addPropertyGroup_Control_StationLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// ��ӿ��� - GridContourLayer ����
		virtual void addPropertyGroup_Control_GridContourLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// ��ӿ��� - WindVectorLayer ����
		virtual void addPropertyGroup_Control_WindVectorLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// ��ӿ��� - RpgLayer ����
		virtual void addPropertyGroup_Control_RpgLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
		// ��ӿ��� - BlackboardLayer ����
		virtual void addPropertyGroup_Control_BlackboardLayer(CMFCPropertyGridProperty* pGroupProperty,void* lpvData);
	
		// ��ӿ��� - BlackboardLayer �� Point ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Point(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� Line ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Line(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� Rect ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Rect(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� Ellipse ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Ellipse(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� Polyline ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Polyline(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� Polygon ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Polygon(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� Curve ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Curve(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ���������_������Ϣ(�ڰ�)
		virtual void addPropertyGroup_Control_BlackboardLayer_Label(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� Lengend ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_Legend(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);

		// ��ӿ��� - BlackboardLayer �� LinearGeometry_Common ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_LinearGeometry_Common(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);
		// ��ӿ��� - BlackboardLayer �� LinearGeometry_CanFill ��������
		virtual void addPropertyGroup_Control_BlackboardLayer_LinearGeometry_CanFill(CMFCPropertyGridProperty* pGroupProperty,KGeometry* pGeometry);

	protected:
		// ˢ�»��ͼ
		void RefreshActiveView();

	protected:
		// ��ǰ����ӵ����
		LPVOID m_lpvPropertyOwner;
		// ��������ؼ�
		CMFCPropertyGridCtrl* m_pMfcPropertyGridCtrl;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

