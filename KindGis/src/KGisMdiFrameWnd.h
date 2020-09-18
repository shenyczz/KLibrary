#pragma once


// KGisMdiFrameWnd 框架

namespace Framework
{

	class KIND_EXT_CLASS KGisMdiFrameWnd : public KMDIFrameWndEx
	{
		DECLARE_DYNCREATE(KGisMdiFrameWnd)
	protected:
		KGisMdiFrameWnd();           // 动态创建所使用的受保护的构造函数
		virtual ~KGisMdiFrameWnd();

	protected:
		DECLARE_MESSAGE_MAP()

	protected:
		// 创建用户工具栏(需要时在派生类重载)
		virtual bool CreateUserToolbars();
		// 创建停靠窗口(需要时在派生类重载)
		virtual bool CreateDockingWindows();
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

