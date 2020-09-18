#pragma once

// KMDIFrameWndEx ���

namespace Framework
{

	class KIND_EXT_CLASS KMDIFrameWndEx : public CMDIFrameWndEx
	{
		DECLARE_DYNCREATE(KMDIFrameWndEx)
	protected:
		KMDIFrameWndEx();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KMDIFrameWndEx();

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
		virtual bool CreateUserToolbars() { return true; }
		// ����ͣ������(��Ҫʱ������������)
		virtual bool CreateDockingWindows() { return true; }

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
