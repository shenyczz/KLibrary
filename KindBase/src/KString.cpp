#include "StdAfx.h"
#include "KString.h"


char KString::s_szReturn[1025] = "";
wchar_t KString::s_wszReturn[1025] = L"";
TCollection<KString> KString::s_StringCollection;

KString::KString(void)
	:m_String(_T(""))
{
}

KString::KString(const KString& rhs)
	:m_String(_T(""))
{
	m_String = rhs.m_String;
}

KString::KString(LPCTSTR lpszString)
	:m_String(_T(""))
{
	if(lpszString)
		m_String = lpszString;
}

KString::~KString(void)
{
	m_String.clear();
}

BOOL KString::IsEmpty()
{
	return m_String.empty();
}

LPCTSTR KString::GetString()
{
	return m_String.c_str();
}
int KString::GetLength() const
{
	return m_String.length();
}

KString& KString::Append(const KString& rhs)
{
	m_String.append(rhs.m_String.c_str());
	return *this;
}
KString& KString::Append(LPCTSTR lpszAppend)
{
	m_String.append(lpszAppend);
	return *this;
}

KString& KString::Replace(LPCTSTR lpszSource, LPCTSTR lpszDest)
{
	_tstring strSource(lpszSource);
	_tstring strDest(lpszDest);

	_tstring::size_type pos = 0;
	_tstring::size_type lenSource = strSource.size();

	while(1)
	{
		pos = m_String.find(strSource);
		if(_tstring::npos == pos)
			break;

		m_String.replace(pos,lenSource,strDest);
	}

	return *this;
}

int KString::Find(LPCTSTR lpszSearch)
{
	_tstring::size_type pos = m_String.find(lpszSearch);
	return (int)(_tstring::npos == pos ? -1 : pos);
}

int KString::ReverseFind(LPCTSTR lpszSearch)
{
	_tstring::size_type pos = m_String.rfind(lpszSearch);
	return (int)(_tstring::npos == pos ? -1 : pos);
}

KString KString::Mid(int iFirst)
{
	TCHAR szBuffer[1024] = _T("");
	int len = m_String.length();

	if(iFirst>=len)
		return NULL;

	if(iFirst<0)
		iFirst = 0;

	_tcscpy(szBuffer,m_String.substr(iFirst).c_str());
	KString strBuffer(szBuffer);
	return strBuffer;

}
KString KString::Mid(int iFirst,int iCount)
{
	TCHAR szBuffer[1024] = _T("");
	int len = m_String.length();

	if(iFirst>=len)
		return NULL;

	if(iFirst<0)
		iFirst = 0;

	_tcscpy(szBuffer,m_String.substr(iFirst,iCount).c_str());
	KString strBuffer(szBuffer);
	return strBuffer;
}

KString KString::Left(_In_ int nCount) const
{
	if (nCount < 0)
		nCount = 0;

	int nLength = GetLength();
	if( nCount >= nLength )
	{
		return( *this );
	}

	return KString(m_String.substr(0,nCount).c_str());
}

KString KString::Right(_In_ int nCount) const
{
	if (nCount < 0)
		nCount = 0;

	int nLength = GetLength();
	if( nCount >= nLength )
	{
		return( *this );
	}

	return KString(m_String.substr(nLength-nCount,nCount).c_str());
}

KString& KString::Trim()
{
	TrimLeft();
	TrimRight();
	return (*this);
}
KString& KString::TrimLeft()
{
	LPTSTR pszString = (LPTSTR)this->GetString();

	while(*pszString)
	{
		if(*pszString != _T(' '))
			break;

		pszString++;
	}

	(*this) = pszString;
	return (*this);
}
KString& KString::TrimRight()
{
	LPTSTR pszString = (LPTSTR)this->GetString();
	int len = _tcslen(pszString);
	pszString += len-1;

	while(*pszString)
	{
		if(*pszString != _T(' '))
			break;

		*pszString = _T('\0');
		pszString--;
	}

	return (*this);
}
KString& KString::Upper()
{
	UpperString((LPTSTR)this->GetString());
	return (*this);
}
KString& KString::Lower()
{
	LowerString((LPTSTR)this->GetString());
	return (*this);
}

KString& KString::Format(LPCTSTR pszFormat, ...)
{
	va_list argList;
	va_start( argList, pszFormat );
	{
		int len = _vsctprintf(pszFormat,argList)	// _vscprintf doesn't count
										+ 1;	// terminating '\0'

		TCHAR* pszBuffer = (TCHAR*)malloc(len* sizeof(TCHAR));
		_vstprintf_s(pszBuffer, len, pszFormat, argList);
		m_String = pszBuffer;
		free(pszBuffer);
		pszBuffer = NULL;
	}
	va_end( argList );

	return (*this);
}

// 克隆
KString* KString::Clone()
{
	KString* pString = new KString(*this);
	return pString;
}

void KString::Clear()
{
	m_String.clear();
}

// 分离字符串
TCollection<KString>& KString::Split(const TCHAR ch)
{
	s_StringCollection.Clear();

	TCHAR szPart[1024] = _T("");
	if(!this->IsEmpty())
	{
		int index = 0;
		LPCTSTR psz = m_String.c_str();
		while(*psz)
		{
			// 不是分隔符
			if(ch != *psz)
			{
				szPart[index] = *psz;
				szPart[index+1] = 0;

				psz++;
				index++;
			}
			// 是分隔符
			else
			{
				// 不是空字符串添加到数组
				if(_tcslen(szPart)>0)
				{
					s_StringCollection.Add(KString(szPart));
				}

				index = 0;
				_tcscpy(szPart,_T(""));

				psz++;
			}
		}

		if(_tcslen(szPart)>0)
		{
			s_StringCollection.Add(KString(szPart));
		}
	}

	return s_StringCollection;
}

// 操作符重载 +
KString KString::operator +(const KString& rhs)
{
	KString strReturn(*this);
	strReturn.Append(rhs);
	return strReturn;
}
KString KString::operator +(LPCTSTR rhs)
{
	KString strReturn(*this);
	strReturn.Append(rhs);
	return strReturn;
}

// 操作符重载 +=
KString& KString::operator +=(const KString& rhs)
{
	this->Append(rhs);
	return *this;
}
KString& KString::operator +=(LPCTSTR rhs)
{
	this->Append(rhs);
	return *this;
}

// 操作符重载 ==
bool KString::operator ==(const KString& rhs)
{
	return m_String == rhs.m_String;
}
bool KString::operator ==(LPCTSTR rhs)
{
	return m_String == rhs;
}

// 操作符重载 !=
bool KString::operator !=(const KString& rhs)
{
	return m_String != rhs.m_String;
}
bool KString::operator !=(LPCTSTR rhs)
{
	return m_String != rhs;
}

// 操作符重载 >
bool KString::operator >(const KString& rhs)
{
	return this->m_String > rhs.m_String;
}
bool KString::operator >(LPCTSTR rhs)
{
	return (*this) > KString(rhs);
}

// 操作符重载 <
bool KString::operator <(const KString& rhs)
{
	return this->m_String < rhs.m_String;
}
bool KString::operator <(LPCTSTR rhs)
{
	return (*this) < KString(rhs);
}

// 操作符重载 >=
bool KString::operator >=(const KString& rhs)
{
	return this->m_String >= rhs.m_String;
}
bool KString::operator >=(LPCTSTR rhs)
{
	return (*this) >= KString(rhs);
}

// 操作符重载 <=
bool KString::operator <=(const KString& rhs)
{
	return this->m_String <= rhs.m_String;
}
bool KString::operator <=(LPCTSTR rhs)
{
	return (*this) <= KString(rhs);
}

// 操作符重载 []
TCHAR KString::operator[](int iChar) const
{
	if( (iChar < 0) || (iChar > GetLength()) )
		return _T('\0');

	return m_String[iChar];
}

KString::operator LPTSTR()
{
	return (LPTSTR)m_String.c_str();
}

KString::operator LPCTSTR()
{
	return m_String.c_str();
}

//---------------------------------------------------------静态函数 - Beg

// 保存字符串
void KString::SetString(LPTSTR &pszTarget, LPCTSTR pszSource)
{
	if( pszSource == NULL )
		return;

	int len = _tcslen(pszSource);
	//if(len==0)
	//	return;

	if(pszTarget)
	{
		delete []pszTarget;
		pszTarget = NULL;
	}
	pszTarget = new TCHAR[len+1];
	_tcscpy(pszTarget,pszSource);
}

// 字符串转换为大写
//#define _toupper(_Char)    ( (_Char)-'a'+'A' )
LPCTSTR KString::UpperString(LPTSTR pszString)
{
	TCHAR *p = pszString;
	while(*p)
	{
		// 是字母则转换
		if( *p >= _T('a') && *p <= _T('z') )
		{
			*p = *p + _T('A') - _T('a');
		}

		p++;
	}

	return pszString;
}
// 字符串转换为小写
//#define _tolower(_Char)    ( (_Char)-'A'+'a' )
LPCTSTR KString::LowerString(LPTSTR pszString)
{
	TCHAR *p = pszString;
	while(*p)
	{
		// 是字母则转换
		if( *p >= _T('A') && *p <= _T('Z') )
		{
			*p  = *p + _T('a') - _T('A');
		}

		p++;
	}

	return pszString;
}

// 去除字符串前导和尾部空格
// 不改变原字符串
LPCTSTR KString::TrimString(LPTSTR pszString)
{
	return TrimStringLeft((LPTSTR)TrimStringRight(pszString));
}
// 去除字符串前导空格
LPCTSTR KString::TrimStringLeft(LPTSTR pszString)
{
	//TCHAR *p = pszString;
	while(*pszString)
	{
		if(*pszString != _T(' '))
			break;

		pszString++;
	}

	return pszString;
}
// 去除字符串尾部空格
LPCTSTR KString::TrimStringRight(LPTSTR pszString)
{
	TCHAR *p = pszString;
	int len = _tcslen(pszString);
	p += len-1;

	while(*p)
	{
		if(*p != _T(' '))
			break;

		*p = _T('\0');
		p--;
	}

	return pszString;
}

//---------------------------------------------------------
// Name:	 WCharToMByte
// Function: 多字节字符串转换为宽字节字符串
//
// Parameters:
//      [i] lpcszSource - 源字符串指针
//      [o] lpwszDest - 目的字符串缓冲区指针
//      [i] dwSize - 目的字符串缓冲区尺寸
//
// Return: 
//      TRUE - 成功
//      FALSE - 失败
//
// Comment: 
//---------------------------------------------------------
BOOL KString::MByte2WChar(LPCSTR lpcszSource, LPWSTR lpwszDest, DWORD dwSize)
{
	// 取得需要的最小缓冲区大小
	DWORD dwMinSize = MultiByteToWideChar(CP_ACP,0,lpcszSource,-1,NULL,0);

	if(dwSize<dwMinSize)
	{
		return FALSE;
	}

	MultiByteToWideChar(CP_ACP, 0, lpcszSource, -1, lpwszDest, dwSize);  

	return TRUE;
}

//---------------------------------------------------------
// Name:     WCharToMByte
// Function: 宽字节字符串转换为多字节字符串
//
// Parameters:
//		[i] lpcwszSource - 源字符串指针
//		[o] lpszDest - 目的字符串指针
//		[i] dwSize - 目的字符串缓冲区尺寸
//
// Return: 
//		TRUE - 成功
//		FALSE - 失败
//
// Comment: 
//---------------------------------------------------------
BOOL KString::WChar2MByte(LPCWSTR lpcwszSource, LPSTR lpszDest, DWORD dwSize)
{
	// 取得需要的最小缓冲区大小
	DWORD dwMinSize = WideCharToMultiByte(CP_OEMCP,0,lpcwszSource,-1,NULL,0,NULL,FALSE);

	if(dwSize<dwMinSize)
	{
		return FALSE;
	}

	WideCharToMultiByte(CP_OEMCP,0,lpcwszSource,-1,lpszDest,dwSize,NULL,FALSE);

	return TRUE;
}

//static
LPCSTR KString::ToMByte(LPCSTR lpszSource)
{
	return lpszSource;
}
LPCSTR KString::ToMByte(LPCWSTR lpszwSource)
{
	memset((char*)s_szReturn,0,1025);
	KString::WChar2MByte(lpszwSource,s_szReturn,1024);
	return s_szReturn;
}

//static 
LPCWSTR KString::ToWChar(LPCSTR lpszSource)
{
	wcscpy(s_wszReturn,L"");
	KString::MByte2WChar(lpszSource,s_wszReturn,1024);
	return s_wszReturn;
}

#ifdef _UNICODE
//static 
LPCWSTR KString::ToWChar(LPCTSTR lpszSource)
{
	wcscpy(s_wszReturn,L"");
	wcscpy(s_wszReturn,ToWChar(ToMByte(lpszSource)));
	return s_wszReturn;
}
#endif

//static 
LPCTSTR KString::ToTChar(LPCSTR lpszSource)
{
#ifdef _UNICODE
	return ToWChar(lpszSource);
#else
	return lpszSource;
#endif
}
//static 
LPCTSTR KString::ToTChar(LPCWSTR lpszwSource)
{
#ifdef _UNICODE
	return lpszwSource;
#else
	return ToMByte(lpszwSource);
#endif
}

//---------------------------------------------------------静态函数 - End
