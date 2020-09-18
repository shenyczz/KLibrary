// KGFrameWndEx.cpp : 实现文件
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
		m_bToolbarBarRoam = FALSE;			// 漫游工具栏
		m_bToolbarBarBlackboard = FALSE;	// 编辑工具栏
	}

	KGisFrameWnd::~KGisFrameWnd()
	{
	}


	BEGIN_MESSAGE_MAP(KGisFrameWnd, KFrameWndEx)
	END_MESSAGE_MAP()


	// KGFrameWnd 消息处理程序

	// 创建用户工具栏(需要时在派生类重载)
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

		// 1.漫游工具栏
		//-------------------------------------------------
		// KIDR_TOOLBAR_ROAM
		// 使漫游工具栏的6个按钮ID = 40101~40106
		// 在 GisResource.h 修改
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
			m_wndToolBarRoam.SetWindowText(_T("漫游"));

			// "添加或删除按钮"
			//bNameValid = strCustomize.LoadString(KIDS_TOOLBAR_CUSTOMIZE);
			//ASSERT(bNameValid);
			//m_wndToolBarRoam.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

			m_wndToolBarRoam.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockPane(&m_wndToolBarRoam);
		}

		// 2.黑板工具栏
		if(m_bToolbarBarBlackboard)
		{
			uiUserToolbar = iFirstUserToolBarId + index++;
			if(uiUserToolbar > iLastUserToolBarId
				|| !m_wndToolBarBlackboard.CreateEx(this,TBSTYLE_FLAT,WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC,CRect(1,1,1,1),uiUserToolbar)
				|| !m_wndToolBarBlackboard.LoadToolBar(KIDR_TOOLBAR_BLACK_BOARD))
			{
				return FALSE;
			}
			m_wndToolBarBlackboard.SetWindowText(_T("图形编辑"));

			// "添加或删除按钮"
			//bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
			//ASSERT(bNameValid);
			//m_wndToolBarBlackboard.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

			m_wndToolBarBlackboard.EnableDocking(CBRS_ALIGN_ANY);
			EnableDocking(CBRS_ALIGN_ANY);
			DockPane(&m_wndToolBarBlackboard);
		}

		// 3. 其他工具栏

		return TRUE;
	}

	// 创建停靠窗口(需要时在派生类重载)
	BOOL KGisFrameWnd::CreateDockingWindows()
	{
		UINT uDockingWindowsID = 1080;
		EnableDocking(CBRS_ALIGN_ANY);
		//-------------------------------------------------
		// 1. 图层窗口
		m_pLayerWnd = new KLayerDockablePane();
		CDockablePane* pLayerWnd = (CDockablePane*)this->GetLayerWnd();
		if(pLayerWnd)
		{
			if (!pLayerWnd->Create(_T("图层"), this, CRect(0, 0, 200, 200), TRUE, uDockingWindowsID++, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
			{
				TRACE0("未能创建“属性”窗口\n");
				return FALSE; // 未能创建
			}
		}

		// 设置图层窗口图标
		HICON hLayerBarIcon = KResource::LoadIcon(KIDI_ICON_LAYER_HC);
		pLayerWnd->SetIcon(hLayerBarIcon, FALSE);

		pLayerWnd->EnableDocking(CBRS_ALIGN_ANY);
		this->DockPane(pLayerWnd);
		//-------------------------------------------------
		// 2. 属性窗口
		m_pPropertyWnd = new KPropertyDockablePane();
		CDockablePane* pPropertyWnd = (CDockablePane*)this->GetPropertyWnd();
		if (!pPropertyWnd->Create(_T("属性"),this,CRect(0,0,200,200),TRUE,uDockingWindowsID++,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“属性”窗口\n");
			return FALSE; // 未能创建
		}

		// 设置属性窗口图标
		HICON hPropertiesBarIcon = KResource::LoadIcon(KIDI_ICON_PROPERTIES_HC);
		pPropertyWnd->SetIcon(hPropertiesBarIcon, FALSE);

		pPropertyWnd->EnableDocking(CBRS_ALIGN_ANY);
		pPropertyWnd->DockToWindow(pLayerWnd,CBRS_ALIGN_BOTTOM);
		//-------------------------------------------------
		// 3.输出窗口
		m_pOutputWnd = new KOutputDockablePane();
		CDockablePane* pOutputWnd = (CDockablePane*)this->GetOutputWnd();
		if (!pOutputWnd->Create(_T("输出"),this,CRect(0,0,200,200),TRUE,uDockingWindowsID++,WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_LEFT | CBRS_FLOAT_MULTI))
		{
			TRACE0("未能创建“输出”窗口\n");
			return FALSE; // 未能创建
		}

		// 设置输出窗口
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
