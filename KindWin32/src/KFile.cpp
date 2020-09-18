#include "StdAfx.h"
#include "KFile.h"

namespace Win32 { namespace IO
{

	KFile::KFile(void)
	{
		_tcscpy(m_szFullPath,_T(""));
		_tcscpy(m_szDrive,_T(""));
		_tcscpy(m_szDir,_T(""));
		_tcscpy(m_szFname,_T(""));
		_tcscpy(m_szExt,_T(""));
	}

	KFile::KFile(LPCTSTR lpszFile)
	{
		_tcscpy(m_szFullPath,_T(""));
		_tcscpy(m_szDrive,_T(""));
		_tcscpy(m_szDir,_T(""));
		_tcscpy(m_szFname,_T(""));
		_tcscpy(m_szExt,_T(""));

		this->splitPath(lpszFile);
	}


	KFile::~KFile(void)
	{
	}

	KString KFile::GetDirectory()
	{
		KString strDirectory;
		strDirectory.Format(_T("%s%s"), m_szDrive, m_szDir);
		return strDirectory;
	}

	KString KFile::GetPath()
	{
		return GetDirectory();
	}

	// 取得文件名
	KString KFile::GetFileName(bool with_ext)
	{
		KString strFileName;
		strFileName.Format(_T("%s%s"), m_szFname, with_ext ? m_szExt : _T(""));
		return strFileName;
	}

	// private
	void KFile::splitPath(LPCTSTR lpszFullPath)
	{
		_tcscpy(m_szFullPath,lpszFullPath);
		_tsplitpath(m_szFullPath,m_szDrive,m_szDir,m_szFname,m_szExt);
	}

	//static
	BOOL KFile::Exists(LPCTSTR lpszFile)
	{
		if(NULL == lpszFile)
			return FALSE;

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(lpszFile, &fd );
		bool bExist = hFind != INVALID_HANDLE_VALUE;
		if(bExist)
		{
			FindClose( hFind );
		}

		return bExist;
	}

	//static
	BOOL KFile::IsBinary(LPCTSTR lpszFile)
	{
		if(!Exists(lpszFile))
			return false;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp)
			return false;

		fseek(fp,0,SEEK_END);
		long len = ftell(fp);
		fseek(fp,0,SEEK_SET);
		if(len==0)
		{
			fclose(fp);
			return false;
		}

		int bytes = len>1024 ? 1024 : len;
		char* pchar = new char[bytes];
		fread(pchar,sizeof(char),bytes,fp);
		fclose(fp);

		bool bBinary = false;
		for(int i=0;i<bytes;i++)
		{
			bBinary = '\0' == *(pchar+i);
			if(bBinary) break;
		}

		delete []pchar;
		pchar = 0;

		return bBinary;
	}

//---------------------------------------------------------
}}// namespace Win32::IO - end
//---------------------------------------------------------
