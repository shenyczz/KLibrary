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
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
 **	   把使用VC++的ApplicationWizard生成的应用程序类(例如：CMyApp)
 **	从 CSfxWinAppExEx 派生, 然后在 CMyApp 的InitInstance() 中调用
 **	KWinAppEx::InitInstance() 后, 就可以得到应用程序所在的绝对路径
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
		//取得应用程序路径
		LPCTSTR GetAppPath();
		//取得状态栏
		CWnd* GetStatusBar();

	private:
		//初始化应用程序路径
		void InitAppPath();

	private:
		TCHAR m_szAppPath[MAX_PATH];	//应用程序全路径


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
