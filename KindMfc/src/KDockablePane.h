#pragma once

#include "KDockablePaneToolBar.h"

namespace DockablePanes
{
}// namespace DockablePanes - end

// KDockablePane

namespace DockablePanes
{

	class KIND_EXT_CLASS KDockablePane : public CDockablePane
	{
		DECLARE_DYNAMIC(KDockablePane)

	public:
		KDockablePane();
		virtual ~KDockablePane();

	protected:
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
		afx_msg void OnSize(UINT nType, int cx, int cy);

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
		// ����
		CFont m_Font;
		// ��ǩ�ؼ�
		CMFCTabCtrl m_wndTabs;
		// ������
		KDockablePaneToolBar m_wndToolBar;
		// ��ָ������
		LPVOID m_lpvData;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------
