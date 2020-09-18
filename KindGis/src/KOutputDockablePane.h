#pragma once

// KOutputDockablePane

namespace DockablePanes
{

	class KIND_EXT_CLASS KOutputDockablePane : public KDockablePane
	{
		DECLARE_DYNAMIC(KOutputDockablePane)

	public:
		KOutputDockablePane();
		virtual ~KOutputDockablePane();

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

	protected:
		KEditCtrl* m_pEditCtrl;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

