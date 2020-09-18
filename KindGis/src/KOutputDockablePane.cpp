// KOutputDockablePane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KOutputDockablePane.h"

namespace DockablePanes
{

	// KOutputDockablePane

	IMPLEMENT_DYNAMIC(KOutputDockablePane, KDockablePane)

	KOutputDockablePane::KOutputDockablePane()
	{
		m_pEditCtrl = new KEditCtrl();
	}

	KOutputDockablePane::~KOutputDockablePane()
	{
		_delete(m_pEditCtrl);
	}


	BEGIN_MESSAGE_MAP(KOutputDockablePane, KDockablePane)
	END_MESSAGE_MAP()

	// KOutputDockablePane ��Ϣ������� KEditCtrlDockablePane

	// ����ͣ�������ڲ��ؼ�(��Ҫʱ������������)
	BOOL KOutputDockablePane::CreateCtrl()
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty();

		DWORD dwStyleDefault = 0
			| WS_VISIBLE
			| WS_CHILD
			| WS_HSCROLL
			| WS_VSCROLL
			| ES_AUTOHSCROLL
			| ES_AUTOVSCROLL
			| ES_MULTILINE
			| ES_NOHIDESEL
			| ES_READONLY
			//| ES_WANTRETURN
			;

		if(m_pEditCtrl==NULL)
			m_pEditCtrl = new KEditCtrl();

		if(!m_pEditCtrl->Create(dwStyleDefault, rectDummy, this, 1001))
			return FALSE;

		m_pEditCtrl->SetMargins(10,10);

		return TRUE;
	}
	// ���ÿؼ�����(��Ҫʱ������������)
	void KOutputDockablePane::SetCtrlFont()
	{
		KDockablePane::SetCtrlFont();
		if(m_pEditCtrl && m_pEditCtrl->GetSafeHwnd())
			m_pEditCtrl->SetFont(&m_Font);
	}
	// ����ͣ�����ڲ���(��Ҫʱ������������)
	void KOutputDockablePane::AdjustLayout()
	{
		KDockablePane::AdjustLayout();

		if (this->GetSafeHwnd() == NULL)
			return;

		if(m_pEditCtrl == NULL || m_pEditCtrl->GetSafeHwnd() == NULL)
			return;

		CRect rectClient;
		this->GetClientRect(rectClient);

		m_pEditCtrl->SetWindowPos(
			NULL,
			rectClient.left, 
			rectClient.top,
			rectClient.Width(),
			rectClient.Height(), 
			SWP_NOACTIVATE | SWP_NOZORDER);

		return;
	}

	// ˢ��ͣ������(��Ҫʱ������������)
	void KOutputDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		LPCTSTR lpszString = (LPCTSTR)lpvData;
		if(lpszString==NULL)
			return;

		if(m_pEditCtrl==NULL || m_pEditCtrl->GetSafeHwnd() == NULL)
			return;

		if(bForceRefresh)
			m_pEditCtrl->SetWindowText(lpszString);
		else
			m_pEditCtrl->AppendText(lpszString, TRUE);

		return;
	}

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------
