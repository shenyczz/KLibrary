#pragma once


// KGisMdiFrameWnd ���

namespace Framework
{

	class KIND_EXT_CLASS KGisMdiFrameWnd : public KMDIFrameWndEx
	{
		DECLARE_DYNCREATE(KGisMdiFrameWnd)
	protected:
		KGisMdiFrameWnd();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KGisMdiFrameWnd();

	protected:
		DECLARE_MESSAGE_MAP()

	protected:
		// �����û�������(��Ҫʱ������������)
		virtual bool CreateUserToolbars();
		// ����ͣ������(��Ҫʱ������������)
		virtual bool CreateDockingWindows();
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

