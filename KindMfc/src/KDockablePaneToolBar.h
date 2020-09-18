#pragma once


// KDockablePaneToolBar

namespace DockablePanes
{

	class KIND_EXT_CLASS KDockablePaneToolBar : public CMFCToolBar
	{
		DECLARE_DYNAMIC(KDockablePaneToolBar)

	public:
		KDockablePaneToolBar();
		virtual ~KDockablePaneToolBar();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		virtual void OnUpdateCmdUI(CFrameWnd* /*pTarget*/, BOOL bDisableIfNoHndler)
		{
			CMFCToolBar::OnUpdateCmdUI((CFrameWnd*) GetOwner(), bDisableIfNoHndler);
		}

		virtual BOOL AllowShowOnList() const { return FALSE; }
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

