#pragma once

// KOutputDockablePane

namespace DockablePanes
{

	class KIND_EXT_CLASS KOutputDockablePane : public KDockablePane
	{
		DECLARE_DYNAMIC(KOutputDockablePane)

	public:
		KOutputDockablePane();
		virtual ~KOutputDockablePane();

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

	protected:
		KEditCtrl* m_pEditCtrl;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

