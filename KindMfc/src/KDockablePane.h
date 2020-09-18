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
		// 字体
		CFont m_Font;
		// 标签控件
		CMFCTabCtrl m_wndTabs;
		// 工具栏
		KDockablePaneToolBar m_wndToolBar;
		// 空指针数据
		LPVOID m_lpvData;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------
