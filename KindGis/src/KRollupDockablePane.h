#pragma once

namespace DockablePanes
{

	// KRollupDockablePane - 可自动上卷、可定靠面板

	class KIND_EXT_CLASS KRollupDockablePane : public KEditCtrlDockablePane
	{
		DECLARE_DYNAMIC(KRollupDockablePane)

	public:
		KRollupDockablePane();
		virtual ~KRollupDockablePane();

	public:
		virtual BOOL Create(LPCTSTR lpszCaption, CWnd* pParentWnd, UINT nID);

	protected:
		DECLARE_MESSAGE_MAP()
	};

}// namespace DockablePanes - end

