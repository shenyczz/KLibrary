#include "stdafx.h"
#include "Misc.h"

static KDib g_Dib;


// ��׽�ͻ�������������ͼ�� - OK
MASC_API HBITMAP _STDCALL CaptureRect(HWND hWnd, LPCRECT lpRect)
{
	HDC hDC = ::GetDC(hWnd);
	g_Dib.Capture(hDC, lpRect);
	::ReleaseDC(hWnd, hDC);

	HBITMAP hBitmap = NULL;
	hBitmap = g_Dib.IsEmpty() ? NULL : g_Dib.Bitmap();
	return hBitmap;
}
