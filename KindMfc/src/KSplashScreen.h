/******************************************************************************
 ** Header File: SfxSplashScreen.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KSplashScreen - ÉÁÆÁ
 ** Version:
 ** Function:
 ** Explain:

 ** Author: by John O'Byrne 01/10/2002
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender: ShenYongchen(shenyczz@163.com) [ÉêÓÀ³½]
 ** DateTime:
 ** Explain:

 ** Usage:

	// Demo 1, the simplest use of the class
	KSplashScreen *pSplash=new KSplashScreen();
	pSplash->Create(this,NULL,2000,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	pSplash->SetBitmap(IDB_SPLASH,255,0,255);
	pSplash->Show();

	// Demo 2
	KSplashScreen *pSplash=new KSplashScreen();
	pSplash->Create(this,"KSplashScreen dynamic text:",0,CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	pSplash->SetBitmap(IDB_SPLASH,255,0,255);
	pSplash->SetTextFont("Impact",100,CSS_TEXT_NORMAL);
	pSplash->SetTextRect(CRect(125,60,291,104));
	pSplash->SetTextColor(RGB(255,255,255));
	pSplash->SetTextFormat(DT_SINGLELINE | DT_CENTER | DT_VCENTER);
	pSplash->Show();

	Sleep(1000);
	pSplash->SetText("You can display infos");

	Sleep(1000);
	pSplash->SetText("While your app is loading");

	Sleep(1000);
	pSplash->SetText("Just call Hide() when loading");
	
	Sleep(1000);
	pSplash->SetText("is finished");
	Sleep(1500);

	pSplash->Hide();

******************************************************************************/

#pragma once

namespace Gui
{

#define CSS_FADEIN		0x0001
#define CSS_FADEOUT		0x0002
#define CSS_FADE		CSS_FADEIN | CSS_FADEOUT
#define CSS_SHADOW		0x0004
#define CSS_CENTERSCREEN	0x0008
#define CSS_CENTERAPP		0x0010
#define CSS_HIDEONCLICK		0x0020

#define CSS_TEXT_NORMAL		0x0000
#define CSS_TEXT_BOLD		0x0001
#define CSS_TEXT_ITALIC		0x0002
#define CSS_TEXT_UNDERLINE	0x0004

typedef BOOL (WINAPI* FN_ANIMATE_WINDOW)(HWND,DWORD,DWORD);

// KSplashScreen

class KIND_EXT_CLASS KSplashScreen : public CWnd
{
	DECLARE_DYNAMIC(KSplashScreen)

public:
	KSplashScreen();
	virtual ~KSplashScreen();

	//BOOL Create(CWnd *pWndParent,LPCTSTR szText=NULL,DWORD dwTimeout=2000,DWORD dwStyle=CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	BOOL Create(HWND hWndParent=NULL,LPCTSTR szText=NULL,DWORD dwTimeout=2000,DWORD dwStyle=CSS_FADE | CSS_CENTERSCREEN | CSS_SHADOW);
	BOOL SetBitmap(UINT nBitmapID,short red=-1,short green=-1,short blue=-1);
	BOOL SetBitmap(LPCTSTR szFileName,short red=-1,short green=-1,short blue=-1);

	void Show();
	void Hide();

	void SetText(LPCTSTR szText);
	void SetTextFont(LPCTSTR szFont,int nSize,int nStyle);
	void SetTextDefaultFont();
	void SetTextColor(COLORREF crTextColor);
	void SetTextRect(CRect& rcText);
	void SetTextFormat(UINT uTextFormat);
	
protected:	
	FN_ANIMATE_WINDOW m_fnAnimateWindow;
	//CWnd *m_pWndParent;
	HWND m_hWndParent;
	CBitmap m_bitmap;
	CFont m_myFont;
	HRGN m_hRegion;
	
	DWORD m_dwStyle;
	DWORD m_dwTimeout;
	CString m_strText;
	CRect m_rcText;
	UINT m_uTextFormat;
	COLORREF m_crTextColor;

	int m_nBitmapWidth;
	int m_nBitmapHeight;
	int m_nxPos;
	int m_nyPos;
		
	HRGN CreateRgnFromBitmap(HBITMAP hBmp, COLORREF color);
	void DrawWindow(CDC *pDC);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	LRESULT OnPrintClient(WPARAM wParam, LPARAM lParam);
protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();
};


//---------------------------------------------------------
}// namespace Gui - End
//---------------------------------------------------------


