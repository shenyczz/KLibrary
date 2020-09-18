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
	// 克隆
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

	// 分离
	TCollection<KString>& Split(const TCHAR ch);

public:
	// 操作符重载 +
	KString operator +(const KString& rhs);
	KString operator +(LPCTSTR rhs);

	// 操作符重载 +=
	KString& operator +=(const KString& rhs);
	KString& operator +=(LPCTSTR rhs);

	// 操作符重载 >
	bool operator >(const KString& rhs);
	bool operator >(LPCTSTR rhs);

	// 操作符重载 <
	bool operator <(const KString& rhs);
	bool operator <(LPCTSTR rhs);

	// 操作符重载 ==
	bool operator ==(const KString& rhs);
	bool operator ==(LPCTSTR rhs);

	// 操作符重载 !=
	bool operator !=(const KString& rhs);
	bool operator !=(LPCTSTR rhs);

	// 操作符重载 >=
	bool operator >=(const KString& rhs);
	bool operator >=(LPCTSTR rhs);

	// 操作符重载 <=
	bool operator <=(const KString& rhs);
	bool operator <=(LPCTSTR rhs);

	// 操作符重载 []
	TCHAR operator[](int iChar) const;

	operator LPTSTR();
	operator LPCTSTR();

private:
	_tstring m_String;

public:
	// 保存字符串
	static void SetString(LPTSTR &pszTarget, LPCTSTR pszSource);

	// 字符串大小写转换
	static LPCTSTR UpperString(LPTSTR pszString);
	static LPCTSTR LowerString(LPTSTR pszString);

	// 去除字符串空格
	static LPCTSTR TrimString(LPTSTR pszString);
	static LPCTSTR TrimStringLeft(LPTSTR pszString);
	static LPCTSTR TrimStringRight(LPTSTR pszString);

	// 多字节 <===> 宽字节 
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
