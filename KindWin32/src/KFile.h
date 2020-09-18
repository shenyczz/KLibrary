/******************************************************************************
 ** KFile.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KFile
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

namespace Win32 { namespace IO
{

	class KIND_EXT_CLASS KFile
	{
	public:
		KFile(void);
		KFile(LPCTSTR lpszFile);
		virtual ~KFile(void);

	public:
		LPCTSTR GetDrive()		{ return m_szDrive; }
		LPCTSTR GetDir()		{ return m_szDir; }
		LPCTSTR GetFname()		{ return m_szFname; }
		LPCTSTR GetExt()		{ return m_szExt; }

		LPCTSTR GetFullPath()	{ return m_szFullPath; }

	public:
		// 取得路径
		KString GetPath();
		KString GetDirectory();
		// 取得文件名
		KString GetFileName(bool with_ext = true);

	private:
		void splitPath(LPCTSTR lpszFullPath);

	protected:
		TCHAR m_szDrive[_MAX_DRIVE];	/* c: */
		TCHAR m_szDir[_MAX_DIR];		/* \TempPath\ */
		TCHAR m_szFname[_MAX_FNAME];	/* ExampleFile */
		TCHAR m_szExt[_MAX_EXT];		/* .txt */

		TCHAR m_szFullPath[_MAX_PATH];	/* c:\TempPath\ExampleFile.txt */

	public:
		// 是否存在
		static BOOL Exists(LPCTSTR lpszFile);
		// 是二进制文件
		static BOOL IsBinary(LPCTSTR lpszFile);
	};

//---------------------------------------------------------
}}// namespace Win32::IO - end
//---------------------------------------------------------
