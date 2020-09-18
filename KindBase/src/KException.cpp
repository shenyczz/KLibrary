#include "StdAfx.h"
#include "KException.h"

KException::KException(void)
	: m_strMessage(_T(""))
{
}

KException::KException(const KException& e)
	: m_strMessage(e.m_strMessage)
{
}

KException::KException(LPCTSTR lpszMessage)
	: m_strMessage(lpszMessage)
{
}

KException::~KException(void)
{
}

LPCTSTR KException::Message()
{
	return m_strMessage.c_str();
}
