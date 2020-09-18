// KRollupDockablePane.cpp : 实现文件
//

#include "stdafx.h"
#include "KRollupDockablePane.h"

namespace DockablePanes
{

	// KRollupDockablePane

	IMPLEMENT_DYNAMIC(KRollupDockablePane, KEditCtrlDockablePane)

	KRollupDockablePane::KRollupDockablePane()
	{
		//this->Create()
		//if (!m_wndInfoBar.Create (_T("Rollup Pane"), this, CRect (0, 0, 100, 100),
		//	TRUE , 
		//	ID_VIEW_INFOBAR, 
		//	WS_CHILD | WS_VISIBLE,
		//	AFX_CBRS_REGULAR_TABS,
		//	AFX_CBRS_CLOSE | AFX_CBRS_FLOAT | AFX_CBRS_AUTO_ROLLUP))
		//{
		//	TRACE0("Failed to create infor bar bar\n");
		//	return -1;      // fail to create
		//}
	}

	KRollupDockablePane::~KRollupDockablePane()
	{
	}


	BEGIN_MESSAGE_MAP(KRollupDockablePane, KEditCtrlDockablePane)
	END_MESSAGE_MAP()


	// KRollupDockablePane 消息处理程序

	BOOL KRollupDockablePane::Create(LPCTSTR lpszCaption, CWnd* pParentWnd, UINT nID)
	{
		return KEditCtrlDockablePane::Create
			(
				lpszCaption,
				pParentWnd,
				CRect(0,0,200,200),
				TRUE,
				nID,
				WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI,
				AFX_CBRS_REGULAR_TABS,
				AFX_DEFAULT_DOCKING_PANE_STYLE | AFX_CBRS_AUTO_ROLLUP, // AFX_CBRS_CLOSE | AFX_CBRS_FLOAT | AFX_CBRS_AUTO_ROLLUP
				NULL
			);
	}//CBRS_ALIGN_ANY

}// namespace DockablePanes - end
