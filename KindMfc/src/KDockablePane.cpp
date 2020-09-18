// KDockablePane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KDockablePane.h"


// KDockablePane

namespace DockablePanes
{

	IMPLEMENT_DYNAMIC(KDockablePane, CDockablePane)

	KDockablePane::KDockablePane()
	{
		m_lpvData = NULL;
	}

	KDockablePane::~KDockablePane()
	{
		m_Font.DeleteObject();
	}


	BEGIN_MESSAGE_MAP(KDockablePane, CDockablePane)
		ON_WM_CREATE()
		ON_WM_SIZE()
	END_MESSAGE_MAP()



	// KDockablePane ��Ϣ�������


	afx_msg int KDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
	{
		if (CDockablePane::OnCreate(lpCreateStruct) == -1)
			return -1;

		if(this->CreateCtrl())
		{
			this->SetCtrlFont();
			this->Refresh(NULL);
			return 0;
		}

		return -1;
	}

	afx_msg void KDockablePane::OnSize(UINT nType, int cx, int cy)
	{
		CDockablePane::OnSize(nType, cx, cy);
		this->AdjustLayout();
	}

	// ����ͣ�������ڲ��ؼ�
	BOOL KDockablePane::CreateCtrl()
	{
		// û�д���
		// ��������������ʵ�־��幦��
		return FALSE;
	}

	// ���ÿؼ�����
	void KDockablePane::SetCtrlFont()
	{
		//m_Font.DeleteObject();
		//m_Font.CreateStockObject(DEFAULT_GUI_FONT);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);
		afxGlobalData.GetNonClientMetrics(info);

		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;

		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);
	}

	// ����ͣ�����ڲ���
	void KDockablePane::AdjustLayout()
	{
		// û�д���
		// ��������������ʵ�־��幦��
	}

	// ˢ��ͣ������
	void KDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		// û�д���
		// ��������������ʵ�־��幦��
	}

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------


