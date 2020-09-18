// SfxWinAppEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KWinAppEx.h"

namespace Framework
{

	// KWinAppEx

	IMPLEMENT_DYNAMIC(KWinAppEx, CWinAppEx)

	KWinAppEx::KWinAppEx()
	{
		// ��ʼ��gdiplus�Ļ���
		GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
	}

	KWinAppEx::~KWinAppEx()
	{
	}


	BEGIN_MESSAGE_MAP(KWinAppEx, CWinAppEx)
	END_MESSAGE_MAP()

	BOOL KWinAppEx::InitInstance()
	{
		// ���� UNICODE ��ʾ����
		_tsetlocale( LC_ALL, _T("CHS") );

		// ��ʼ��Ӧ�ó���·��
		InitAppPath();

		return CWinAppEx::InitInstance();
	}

	int KWinAppEx::ExitInstance()
	{
		// �ر�gdiplus�Ļ���
		GdiplusShutdown(gdiplusToken);
		return CWinAppEx::ExitInstance();
	}

	// ȡ��״̬��
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
			{// otherwise traverse(����) children to try and find the status bar...
				pWnd = pMainWnd->GetDescendantWindow(AFX_IDW_STATUS_BAR);
			}
		}

		return pWnd;
	}

	// ��ʼ��Ӧ�ó���·��
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

	//ȡ��Ӧ�ó���·��
	LPCTSTR KWinAppEx::GetAppPath()
	{
		InitAppPath();
		return m_szAppPath;
	}


//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
