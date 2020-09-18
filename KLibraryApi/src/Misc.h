#pragma once

#include "ApiCommon.h"


//[DllImport("KLibraryApi120ud.dll", CharSet = CharSet.Auto)]
MASC_API HBITMAP _STDCALL CaptureRect(HWND hWnd, LPCRECT lpRect);
