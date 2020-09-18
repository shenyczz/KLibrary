#pragma once

// KGFrameWndEx 框架

namespace Framework
{

	class KIND_EXT_CLASS KGisFrameWnd : public KFrameWndEx
	{
		DECLARE_DYNCREATE(KGisFrameWnd)
	public:
		KGisFrameWnd();           // 动态创建所使用的受保护的构造函数
		virtual ~KGisFrameWnd();

	protected:
		DECLARE_MESSAGE_MAP()

	protected:
		void EnableToolbarBarRoam(bool bEnable = true) { m_bToolbarBarRoam = bEnable; }
		void EnableToolbarBarBlackboard(bool bEnable = true) { m_bToolbarBarBlackboard = bEnable; }

	protected:
		// 创建用户工具栏(需要时在派生类重载)
		virtual BOOL CreateUserToolbars();
		// 创建停靠窗口(需要时在派生类重载)
		virtual BOOL CreateDockingWindows();

	protected:
		// 漫游工具栏
		CMFCToolBar			m_wndToolBarRoam;
		CMFCToolBar			m_wndToolBarBlackboard;

	protected:
		BOOL m_bToolbarBarRoam;
		BOOL m_bToolbarBarBlackboard;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

