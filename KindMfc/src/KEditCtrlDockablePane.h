#pragma once

#include "KDockablePane.h"
#include "KEditCtrl.h"

namespace DockablePanes
{

	// KEditCtrlDockablePane - �ı��ؼ���ͣ�����

	class KIND_EXT_CLASS KEditCtrlDockablePane : public KDockablePane
	{
		DECLARE_DYNAMIC(KEditCtrlDockablePane)

	public:
		KEditCtrlDockablePane();
		virtual ~KEditCtrlDockablePane();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		//=================================================
		// ˢ��ͣ������(��Ҫʱ������������)
		virtual void Refresh(LPVOID lpvData, BOOL bForceRefresh = FALSE);
	protected:
		// ����ͣ�������ڲ��ؼ�(��Ҫʱ������������)
		virtual BOOL CreateCtrl();
		// ���ÿؼ�����(��Ҫʱ������������)
		virtual void SetCtrlFont();
		// ����ͣ�����ڲ���(��Ҫʱ������������)
		virtual void AdjustLayout();
		//=================================================

	public:
		KEditCtrl& EditCtrl() { return *(&m_EditCtrl); }

	protected:
		KEditCtrl m_EditCtrl;
	};

}// namespace DockablePanes - end

