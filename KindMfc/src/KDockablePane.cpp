// KDockablePane.cpp : 实现文件
//

#include "stdafx.h"
#include "KDockablePane.h"


// KDockablePane

namespace DockablePanes
{

	IMPLEMENT_DYNAMIC(KDockablePane, CDockablePane)

	KDockablePane::KDockablePane()
	{
		m_lpvData = NULL;
	}

	KDockablePane::~KDockablePane()
	{
		m_Font.DeleteObject();
	}


	BEGIN_MESSAGE_MAP(KDockablePane, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
	END_MESSAGE_MAP()



	// KDockablePane 消息处理程序


	afx_msg int KDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		if(this->CreateCtrl())
		{
			this->SetCtrlFont();
			this->Refresh(NULL);
			return 0;
		}

		return -1;
	}

	afx_msg void KDockablePane::OnSize(UINT nType, int cx, int cy)
	{
		CDockablePane::OnSize(nType, cx, cy);
		this->AdjustLayout();
	}

	// 建立停靠窗口内部控件
	BOOL KDockablePane::CreateCtrl()
	{
		// 没有代码
		// 在派生类中重载实现具体功能
		return FALSE;
	}

	// 设置控件字体
	void KDockablePane::SetCtrlFont()
	{
		//m_Font.DeleteObject();
		//m_Font.CreateStockObject(DEFAULT_GUI_FONT);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);
		afxGlobalData.GetNonClientMetrics(info);

		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;

		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);
	}

	// 调整停靠窗口布局
	void KDockablePane::AdjustLayout()
	{
		// 没有代码
		// 在派生类中重载实现具体功能
	}

	// 刷新停靠窗口
	void KDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		// 没有代码
		// 在派生类中重载实现具体功能
	}

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------


