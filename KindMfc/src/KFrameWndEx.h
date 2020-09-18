#pragma once

namespace Framework
{

	// KFrameWndEx ���

	class KIND_EXT_CLASS KFrameWndEx : public CFrameWndEx
	{
		DECLARE_DYNCREATE(KFrameWndEx)
	protected:
		KFrameWndEx();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KFrameWndEx();

	protected:
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		// ȡ��ͼ�㴰��
		virtual CWnd* GetLayerWnd() { return m_pLayerWnd; }
		// ȡ���������
		virtual CWnd* GetOutputWnd() { return m_pOutputWnd; }
		// ȡ�����Դ���
		virtual CWnd* GetPropertyWnd() { return m_pPropertyWnd; }

	protected:
		// �����û�������(��Ҫʱ������������)
		virtual BOOL CreateUserToolbars() { return TRUE; }
		// ����ͣ������(��Ҫʱ������������)
		virtual BOOL CreateDockingWindows() { return TRUE; }

	protected:
		// ͼ�㴰��ָ��
		CWnd* m_pLayerWnd;
		// �������ָ��
		CWnd* m_pOutputWnd;
		// ���Դ���ָ��
		CWnd* m_pPropertyWnd;
	};


//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
