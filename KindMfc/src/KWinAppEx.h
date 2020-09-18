/******************************************************************************
 ** KWinAppEx.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KWinAppEx
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
 **	   ��ʹ��VC++��ApplicationWizard���ɵ�Ӧ�ó�����(���磺CMyApp)
 **	�� CSfxWinAppExEx ����, Ȼ���� CMyApp ��InitInstance() �е���
 **	KWinAppEx::InitInstance() ��, �Ϳ��Եõ�Ӧ�ó������ڵľ���·��
 **
******************************************************************************/

#pragma once

// KWinAppEx

namespace Framework
{

	class KIND_EXT_CLASS KWinAppEx : public CWinAppEx
	{
		DECLARE_DYNAMIC(KWinAppEx)

	public:
		KWinAppEx();
		virtual ~KWinAppEx();

	public:
		virtual BOOL InitInstance();
		virtual int ExitInstance();

	protected:
		DECLARE_MESSAGE_MAP()

	public:
		//ȡ��Ӧ�ó���·��
		LPCTSTR GetAppPath();
		//ȡ��״̬��
		CWnd* GetStatusBar();

	private:
		//��ʼ��Ӧ�ó���·��
		void InitAppPath();

	private:
		TCHAR m_szAppPath[MAX_PATH];	//Ӧ�ó���ȫ·��


		ULONG_PTR  gdiplusToken;
		GdiplusStartupInput gdiplusStartupInput;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------


namespace Framework
{
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
