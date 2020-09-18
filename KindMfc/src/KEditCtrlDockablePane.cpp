// KEditCtrlDockablePane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KEditCtrlDockablePane.h"

namespace DockablePanes
{

	// KEditCtrlDockablePane

	IMPLEMENT_DYNAMIC(KEditCtrlDockablePane, KDockablePane)

	KEditCtrlDockablePane::KEditCtrlDockablePane()
	{

	}

	KEditCtrlDockablePane::~KEditCtrlDockablePane()
	{
	}

	BEGIN_MESSAGE_MAP(KEditCtrlDockablePane, KDockablePane)
	END_MESSAGE_MAP()

	// KEditCtrlDockablePane ��Ϣ�������

	// ����ͣ�������ڲ��ؼ�
	BOOL KEditCtrlDockablePane::CreateCtrl()
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

		if(!m_EditCtrl.Create(dwStyleDefault, rectDummy, this, 1001))
			return FALSE;

		m_EditCtrl.SetMargins(10,10);

		return TRUE;
	}

	// ���ÿؼ�����
	void KEditCtrlDockablePane::SetCtrlFont()
	{
		KDockablePane::SetCtrlFont();
		if(m_EditCtrl.GetSafeHwnd() != NULL)
			m_EditCtrl.SetFont(&m_Font);
	}

	// ����ͣ�����ڲ���
	void KEditCtrlDockablePane::AdjustLayout()
	{
		KDockablePane::AdjustLayout();

		if (this->GetSafeHwnd() == NULL)
			return;

		if(m_EditCtrl.GetSafeHwnd() == NULL)
			return;

		CRect rectClient;
		this->GetClientRect(rectClient);

		m_EditCtrl.SetWindowPos(
			NULL,
			rectClient.left, 
			rectClient.top,
			rectClient.Width(),
			rectClient.Height(), 
			SWP_NOACTIVATE | SWP_NOZORDER);

		return;
	}

	// ˢ��ͣ������
	void KEditCtrlDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		if(m_EditCtrl.GetSafeHwnd() == NULL)
			return;

		LPCTSTR lpszString = (LPCTSTR)lpvData;
		if(lpszString==NULL)
		{
			m_EditCtrl.SetWindowText(_T(""));
			return;
		}

		if(bForceRefresh)
			m_EditCtrl.SetWindowText(lpszString);
		else
			m_EditCtrl.AppendText(lpszString, TRUE);

		return;
	}

}// namespace DockablePanes - end
