#pragma once

#include "KDockablePane.h"
#include "KEditCtrl.h"

namespace DockablePanes
{

	// KEditCtrlDockablePane - 文本控件可停靠面板

	class KIND_EXT_CLASS KEditCtrlDockablePane : public KDockablePane
	{
		DECLARE_DYNAMIC(KEditCtrlDockablePane)

	public:
		KEditCtrlDockablePane();
		virtual ~KEditCtrlDockablePane();

	protected:
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

	public:
		KEditCtrl& EditCtrl() { return *(&m_EditCtrl); }

	protected:
		KEditCtrl m_EditCtrl;
	};

}// namespace DockablePanes - end

