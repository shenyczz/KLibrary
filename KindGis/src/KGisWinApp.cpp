// KGisWinApp.cpp : ʵ���ļ�
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
		// TODO: �ڴ�ִ���������߳�����
		return KWinAppEx::ExitInstance();
	}

	BEGIN_MESSAGE_MAP(KGisWinApp, KWinAppEx)
	END_MESSAGE_MAP()


	// KGisWinApp ��Ϣ�������

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
