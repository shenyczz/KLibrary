// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// VisionEarth.h : VisionEarth Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������


// CVisionEarthApp:
// �йش����ʵ�֣������ VisionEarth.cpp
//

#define WM_USER_APPIDLE		(WM_USER + 1000)

class CVisionEarthApp : public CWinAppEx
{
public:
	CVisionEarthApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	virtual BOOL OnIdle(LONG lCount);

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CVisionEarthApp theApp;
