// KGMDIFrameWndEx.cpp : 实现文件
//

#include "stdafx.h"
#include "KGisMdiFrameWnd.h"


// KGMDIFrameWndEx

namespace Framework
{

	IMPLEMENT_DYNCREATE(KGisMdiFrameWnd, KMDIFrameWndEx)

	KGisMdiFrameWnd::KGisMdiFrameWnd()
	{
		
	}

	KGisMdiFrameWnd::~KGisMdiFrameWnd()
	{
	}


	BEGIN_MESSAGE_MAP(KGisMdiFrameWnd, KMDIFrameWndEx)
	END_MESSAGE_MAP()


	// KGisMdiFrameWnd 消息处理程序

	// 创建用户工具栏(需要时在派生类重载)
	bool KGisMdiFrameWnd::CreateUserToolbars()
	{ 
		return true;
	}
	// 创建停靠窗口(需要时在派生类重载)
	bool KGisMdiFrameWnd::CreateDockingWindows()
	{
		return true;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
