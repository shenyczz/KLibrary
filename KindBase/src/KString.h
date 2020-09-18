/******************************************************************************
 ** KString.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KString
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
******************************************************************************/

#pragma once

#include "TCollection.h"

class KIND_EXT_CLASS KString
{
public:
	KString(void);
	KString(const KString& rhs);
	KString(LPCTSTR lpszString);

	virtual ~KString(void);

public:
	BOOL IsEmpty();

public:
	LPCTSTR GetString();
	int GetLength() const;

public:
	// ��¡
	KString* Clone();
	void Clear();

	KString& Append(LPCTSTR lpszAppend);
	KString& Append(const KString& rhs);
	KString& Replace(LPCTSTR lpszSource, LPCTSTR lpszDest);

	KString& Trim();
	KString& TrimLeft();
	KString& TrimRight();

	KString& Upper();
	KString& Lower();

	KString& Format(LPCTSTR pszFormat, ...);

	KString Mid(int iFirst);
	KString Mid(int iFirst,int iCount);
	KString Left(_In_ int nCount) const;
	KString Right(_In_ int nCount) const;

	int Find(LPCTSTR lpszSearch);
	int ReverseFind(LPCTSTR lpszSearch);

	// ����
	TCollection<KString>& Split(const TCHAR ch);

public:
	// ���������� +
	KString operator +(const KString& rhs);
	KString operator +(LPCTSTR rhs);

	// ���������� +=
	KString& operator +=(const KString& rhs);
	KString& operator +=(LPCTSTR rhs);

	// ���������� >
	bool operator >(const KString& rhs);
	bool operator >(LPCTSTR rhs);

	// ���������� <
	bool operator <(const KString& rhs);
	bool operator <(LPCTSTR rhs);

	// ���������� ==
	bool operator ==(const KString& rhs);
	bool operator ==(LPCTSTR rhs);

	// ���������� !=
	bool operator !=(const KString& rhs);
	bool operator !=(LPCTSTR rhs);

	// ���������� >=
	bool operator >=(const KString& rhs);
	bool operator >=(LPCTSTR rhs);

	// ���������� <=
	bool operator <=(const KString& rhs);
	bool operator <=(LPCTSTR rhs);

	// ���������� []
	TCHAR operator[](int iChar) const;

	operator LPTSTR();
	operator LPCTSTR();

private:
	_tstring m_String;

public:
	// �����ַ���
	static void SetString(LPTSTR &pszTarget, LPCTSTR pszSource);

	// �ַ�����Сдת��
	static LPCTSTR UpperString(LPTSTR pszString);
	static LPCTSTR LowerString(LPTSTR pszString);

	// ȥ���ַ����ո�
	static LPCTSTR TrimString(LPTSTR pszString);
	static LPCTSTR TrimStringLeft(LPTSTR pszString);
	static LPCTSTR TrimStringRight(LPTSTR pszString);

	// ���ֽ� <===> ���ֽ� 
	static BOOL MByte2WChar(LPCSTR lpcszSource, LPWSTR lpwszDest, DWORD dwSize);
	static BOOL WChar2MByte(LPCWSTR lpcwszSource, LPSTR lpszDest, DWORD dwSize);

	static LPCSTR ToMByte(LPCSTR lpszSource);
	static LPCSTR ToMByte(LPCWSTR lpszwSource);

	static LPCWSTR ToWChar(LPCSTR lpszSource);
#ifdef _UNICODE
	static LPCWSTR ToWChar(LPCTSTR lpszSource);
#endif

	static LPCTSTR ToTChar(LPCSTR lpszSource);
	static LPCTSTR ToTChar(LPCWSTR lpszwSource);

private:
	static char s_szReturn[1025];
	static wchar_t s_wszReturn[1025];
	static TCollection<KString> s_StringCollection;
};

typedef TCollection<KString> KStringArray;
