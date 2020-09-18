#pragma once

#include "KLayerDockablePane.h"
#include "KPropertyDockablePane.h"

// KGFrameWndEx 框架

namespace Framework
{

	class KIND_EXT_CLASS KGFrameWndEx : public KFrameWndEx
	{
		DECLARE_DYNCREATE(KGFrameWndEx)
	protected:
		KGFrameWndEx();           // 动态创建所使用的受保护的构造函数
		virtual ~KGFrameWndEx();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		// 取得图层窗口
		virtual CWnd* GetLayerWnd() { return &m_LayerDockablePane; }
		// 取得属性窗口
		virtual CWnd* GetPropertyWnd() { return &m_PropertyDockablePane; }

	private:
		KLayerDockablePane m_LayerDockablePane;
		KPropertyDockablePane m_PropertyDockablePane;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

