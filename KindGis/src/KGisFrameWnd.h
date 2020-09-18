#pragma once

// KGFrameWndEx ���

namespace Framework
{

	class KIND_EXT_CLASS KGisFrameWnd : public KFrameWndEx
	{
		DECLARE_DYNCREATE(KGisFrameWnd)
	public:
		KGisFrameWnd();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KGisFrameWnd();

	protected:
		DECLARE_MESSAGE_MAP()

	protected:
		void EnableToolbarBarRoam(bool bEnable = true) { m_bToolbarBarRoam = bEnable; }
		void EnableToolbarBarBlackboard(bool bEnable = true) { m_bToolbarBarBlackboard = bEnable; }

	protected:
		// �����û�������(��Ҫʱ������������)
		virtual BOOL CreateUserToolbars();
		// ����ͣ������(��Ҫʱ������������)
		virtual BOOL CreateDockingWindows();

	protected:
		// ���ι�����
		CMFCToolBar			m_wndToolBarRoam;
		CMFCToolBar			m_wndToolBarBlackboard;

	protected:
		BOOL m_bToolbarBarRoam;
		BOOL m_bToolbarBarBlackboard;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

