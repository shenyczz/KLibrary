// SfxWinAppEx.cpp : 实现文件
//

#include "stdafx.h"
#include "KWinAppEx.h"

namespace Framework
{

	// KWinAppEx

	IMPLEMENT_DYNAMIC(KWinAppEx, CWinAppEx)

	KWinAppEx::KWinAppEx()
	{
		// 初始化gdiplus的环境
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	KWinAppEx::~KWinAppEx()
	{
	}


	BEGIN_MESSAGE_MAP(KWinAppEx, CWinAppEx)
	END_MESSAGE_MAP()

	BOOL KWinAppEx::InitInstance()
	{
		// 设置 UNICODE 显示汉字
		_tsetlocale( LC_ALL, _T("CHS") );

		// 初始化应用程序路径
		InitAppPath();

		return CWinAppEx::InitInstance();
	}

	int KWinAppEx::ExitInstance()
	{
		// 关闭gdiplus的环境
		GdiplusShutdown(gdiplusToken);
		return CWinAppEx::ExitInstance();
	}

	// 取得状态栏
	CWnd* KWinAppEx::GetStatusBar()
	{
		CWnd *pMainWnd = AfxGetMainWnd();
		//ASSERT(pMainWnd);
		if(!pMainWnd)
			return NULL;

		CWnd* pWnd = NULL;

		if (pMainWnd)
		{
			if (pMainWnd->IsKindOf(RUNTIME_CLASS(CFrameWnd)))
			{// If main window is a frame window, use normal methods...
				pWnd = ((CFrameWnd*)pMainWnd)->GetMessageBar();
			}
			else
			{// otherwise traverse(遍历) children to try and find the status bar...
				pWnd = pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
			}
		}

		return pWnd;
	}

	// 初始化应用程序路径
	void KWinAppEx::InitAppPath()
	{
		HINSTANCE hInstance = AfxGetInstanceHandle();
		if(!hInstance)
			return;

		::GetModuleFileName(hInstance,m_szAppPath,MAX_PATH);
		size_t i;

		i = _tcslen(m_szAppPath);
		while(m_szAppPath[i] != _T('\\'))
		{
			i--;
		}
		m_szAppPath[i]='\0';
	}

	//取得应用程序路径
	LPCTSTR KWinAppEx::GetAppPath()
	{
		InitAppPath();
		return m_szAppPath;
	}


//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
