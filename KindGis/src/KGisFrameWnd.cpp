// KGFrameWndEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KGisFrameWnd.h"

#include "GisResource.h"

#include "KLayerDockablePane.h"
#include "KOutputDockablePane.h"
#include "KPropertyDockablePane.h"


// KGFrameWndEx

namespace Framework
{

	IMPLEMENT_DYNCREATE(KGisFrameWnd, KFrameWndEx)

	KGisFrameWnd::KGisFrameWnd()
	{
		m_bToolbarBarRoam = FALSE;			// ���ι�����
		m_bToolbarBarBlackboard = FALSE;	// �༭������
	}

	KGisFrameWnd::~KGisFrameWnd()
	{
	}


	BEGIN_MESSAGE_MAP(KGisFrameWnd, KFrameWndEx)
	END_MESSAGE_MAP()


	// KGFrameWnd ��Ϣ�������

	// �����û�������(��Ҫʱ������������)
	BOOL KGisFrameWnd::CreateUserToolbars()
	{
		//-------------------------------------------------
		int  iMaxUserToolbars = 10;
		UINT iFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 100;
		UINT iLastUserToolBarId = iFirstUserToolBarId + iMaxUserToolbars - 1;
		//-------------------------------------------------
		int index = 0;

		BOOL bNameValid = FALSE;
		CString strCustomize = _T("");

		UINT uiUserToolbar;

		// 1.���ι�����
		//-------------------------------------------------
		// KIDR_TOOLBAR_ROAM
		// ʹ���ι�������6����ťID = 40101~40106
		// �� GisResource.h �޸�
		// #define ID_ROAM_CANCEL                  40101
		// #define ID_ROAM_REFRESH                 40102
		// #define ID_ROAM_RESET                   40103
		// #define ID_ROAM_ZOOM_IN                 40104
		// #define ID_ROAM_ZOOM_OUT                40105
		// #define ID_ROAM_ZOOM_PAN                40106
		//-------------------------------------------------
		if(m_bToolbarBarRoam)
		{
			uiUserToolbar = iFirstUserToolBarId + index++;
			if(uiUserToolbar > iFirstUserToolBarId
				|| !m_wndToolBarRoam.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),uiUserToolbar)
				|| !m_wndToolBarRoam.LoadToolBar(KIDR_TOOLBAR_ROAM))
			{
				return FALSE;
			}
			m_wndToolBarRoam.SetWindowText(_T("����"));

			// "��ӻ�ɾ����ť"
			//bNameValid = strCustomize.LoadString(KIDS_TOOLBAR_CUSTOMIZE);
			//ASSERT(bNameValid);
			//m_wndToolBarRoam.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

			m_wndToolBarRoam.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockPane(&m_wndToolBarRoam);
		}

		// 2.�ڰ幤����
		if(m_bToolbarBarBlackboard)
		{
			uiUserToolbar = iFirstUserToolBarId + index++;
			if(uiUserToolbar > iLastUserToolBarId
				|| !m_wndToolBarBlackboard.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),uiUserToolbar)
				|| !m_wndToolBarBlackboard.LoadToolBar(KIDR_TOOLBAR_BLACK_BOARD))
			{
				return FALSE;
			}
			m_wndToolBarBlackboard.SetWindowText(_T("ͼ�α༭"));

			// "��ӻ�ɾ����ť"
			//bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
			//ASSERT(bNameValid);
			//m_wndToolBarBlackboard.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

			m_wndToolBarBlackboard.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockPane(&m_wndToolBarBlackboard);
		}

		// 3. ����������

		return TRUE;
	}

	// ����ͣ������(��Ҫʱ������������)
	BOOL KGisFrameWnd::CreateDockingWindows()
	{
		UINT uDockingWindowsID = 1080;
		EnableDocking(CBRS_ALIGN_ANY);
		//-------------------------------------------------
		// 1. ͼ�㴰��
		m_pLayerWnd = new KLayerDockablePane();
		CDockablePane* pLayerWnd = (CDockablePane*)this->GetLayerWnd();
		if(pLayerWnd)
		{
			if (!pLayerWnd->Create(_T("ͼ��"), this, CRect(0, 0, 200, 200), TRUE, uDockingWindowsID++, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
			{
				TRACE0("δ�ܴ��������ԡ�����\n");
				return FALSE; // δ�ܴ���
			}
		}

		// ����ͼ�㴰��ͼ��
		HICON hLayerBarIcon = KResource::LoadIcon(KIDI_ICON_LAYER_HC);
		pLayerWnd->SetIcon(hLayerBarIcon, FALSE);

		pLayerWnd->EnableDocking(CBRS_ALIGN_ANY);
		this->DockPane(pLayerWnd);
		//-------------------------------------------------
		// 2. ���Դ���
		m_pPropertyWnd = new KPropertyDockablePane();
		CDockablePane* pPropertyWnd = (CDockablePane*)this->GetPropertyWnd();
		if (!pPropertyWnd->Create(_T("����"),this,CRect(0,0,200,200),TRUE,uDockingWindowsID++,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ��������ԡ�����\n");
			return FALSE; // δ�ܴ���
		}

		// �������Դ���ͼ��
		HICON hPropertiesBarIcon = KResource::LoadIcon(KIDI_ICON_PROPERTIES_HC);
		pPropertyWnd->SetIcon(hPropertiesBarIcon, FALSE);

		pPropertyWnd->EnableDocking(CBRS_ALIGN_ANY);
		pPropertyWnd->DockToWindow(pLayerWnd,CBRS_ALIGN_BOTTOM);
		//-------------------------------------------------
		// 3.�������
		m_pOutputWnd = new KOutputDockablePane();
		CDockablePane* pOutputWnd = (CDockablePane*)this->GetOutputWnd();
		if (!pOutputWnd->Create(_T("���"),this,CRect(0,0,200,200),TRUE,uDockingWindowsID++,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("δ�ܴ��������������\n");
			return FALSE; // δ�ܴ���
		}

		// �����������
		HICON hOutputBarIcon = KResource::LoadIcon(KIDI_ICON_OUTPUT_HC);
		pOutputWnd->SetIcon(hOutputBarIcon, FALSE);

		pOutputWnd->EnableDocking(CBRS_ALIGN_ANY);
		pOutputWnd->DockToFrameWindow(CBRS_ALIGN_BOTTOM);
		//-------------------------------------------------
		//CDockablePane* pTabbedBar = NULL;
		//pOutputWnd->AttachToTabWnd(pLayerWnd, DM_SHOW, TRUE, &pTabbedBar);
		//DockPane(pOutputWnd);
		//-------------------------------------------------
		return TRUE;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
