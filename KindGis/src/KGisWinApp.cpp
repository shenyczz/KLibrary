// KGisWinApp.cpp : 实现文件
//

#include "stdafx.h"
#include "KGisWinApp.h"


namespace Framework
{

	// KGisWinApp

	IMPLEMENT_DYNCREATE(KGisWinApp, KWinAppEx)

	KGisWinApp::KGisWinApp()
	{
	}

	KGisWinApp::~KGisWinApp()
	{
	}

	BOOL KGisWinApp::InitInstance()
	{
		return KWinAppEx::InitInstance();
	}

	int KGisWinApp::ExitInstance()
	{
		// TODO: 在此执行任意逐线程清理
		return KWinAppEx::ExitInstance();
	}

	BEGIN_MESSAGE_MAP(KGisWinApp, KWinAppEx)
	END_MESSAGE_MAP()


	// KGisWinApp 消息处理程序

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
