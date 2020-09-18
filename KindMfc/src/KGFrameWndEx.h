#pragma once

#include "KLayerDockablePane.h"
#include "KPropertyDockablePane.h"

// KGFrameWndEx ���

namespace Framework
{

	class KIND_EXT_CLASS KGFrameWndEx : public KFrameWndEx
	{
		DECLARE_DYNCREATE(KGFrameWndEx)
	protected:
		KGFrameWndEx();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KGFrameWndEx();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		// ȡ��ͼ�㴰��
		virtual CWnd* GetLayerWnd() { return &m_LayerDockablePane; }
		// ȡ�����Դ���
		virtual CWnd* GetPropertyWnd() { return &m_PropertyDockablePane; }

	private:
		KLayerDockablePane m_LayerDockablePane;
		KPropertyDockablePane m_PropertyDockablePane;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

