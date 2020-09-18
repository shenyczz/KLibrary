#include "stdafx.h"
#include "ApiCommon.h"


TCHAR g_Buffer[MAX_PATH] = _T("");

KDib g_Dib;
KRpgData g_RpgData;
ApiCommon g_ApiCommon;

ApiCommon::ApiCommon()
{
	m_lpData = NULL;
}
ApiCommon::~ApiCommon()
{
	_delete(m_lpData);
}

void ApiCommon::putData(LPVOID p)
{
	_delete(m_lpData);
	m_lpData = p;
}

void ApiCommon::putBuffer(LPCTSTR lpsz)
{
	_tcsncpy_s(m_Buffer, _countof(m_Buffer), lpsz, _TRUNCATE);	//л╚╨цак
}
