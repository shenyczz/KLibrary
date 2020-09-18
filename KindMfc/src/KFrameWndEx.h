#pragma once

namespace Framework
{

	// KFrameWndEx 框架

	class KIND_EXT_CLASS KFrameWndEx : public CFrameWndEx
	{
		DECLARE_DYNCREATE(KFrameWndEx)
	protected:
		KFrameWndEx();           // 动态创建所使用的受保护的构造函数
		virtual ~KFrameWndEx();

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
		virtual BOOL CreateUserToolbars() { return TRUE; }
		// 创建停靠窗口(需要时在派生类重载)
		virtual BOOL CreateDockingWindows() { return TRUE; }

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
