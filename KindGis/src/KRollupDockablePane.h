#pragma once

namespace DockablePanes
{

	// KRollupDockablePane - ���Զ��Ͼ��ɶ������

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

