#pragma once

// KMDIFrameWndEx 框架

namespace Framework
{

	class KIND_EXT_CLASS KMDIFrameWndEx : public CMDIFrameWndEx
	{
		DECLARE_DYNCREATE(KMDIFrameWndEx)
	protected:
		KMDIFrameWndEx();           // 动态创建所使用的受保护的构造函数
		virtual ~KMDIFrameWndEx();

	protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		// 取得图层窗口
		virtual CWnd* GetLayerWnd() { return m_pLayerWnd; }
		// 取得输出窗口
		virtual CWnd* GetOutputWnd() { return m_pOutputWnd; }
		// 取得属性窗口
		virtual CWnd* GetPropertyWnd() { return m_pPropertyWnd; }

	protected:
		// 创建用户工具栏(需要时在派生类重载)
		virtual bool CreateUserToolbars() { return true; }
		// 创建停靠窗口(需要时在派生类重载)
		virtual bool CreateDockingWindows() { return true; }

	protected:
		// 图层窗口指针
		CWnd* m_pLayerWnd;
		// 输出窗口指针
		CWnd* m_pOutputWnd;
		// 属性窗口指针
		CWnd* m_pPropertyWnd;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
