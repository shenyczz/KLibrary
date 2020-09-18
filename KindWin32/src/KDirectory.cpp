#include "StdAfx.h"
#include "KDirectory.h"

namespace Win32 { namespace IO
{

	KDirectory::KDirectory(void)
	{
	}

	KDirectory::~KDirectory(void)
	{
	}

	//static
	bool KDirectory::Create(LPCTSTR lpszPath)
	{
		if(!lpszPath)
			return false;

		TCHAR szPath[1030] = _T("");
		int len = _tcslen(lpszPath);
		if(len>1024)
			return false;

		// 首先路径末尾加上反斜杠 '\'
		_stprintf(szPath,_T("%s"),lpszPath);
		if(szPath[len-1] != _T('\\') && szPath[len-1] != _T('/'))
		{
			szPath[len] = _T('\\');
			szPath[len+1] = _T('\0');
		}

		TCHAR szDrive[_MAX_DRIVE]		= _T("");	// contains the drive letter and colon. for example - "c:"
		TCHAR szDir[_MAX_DIR]			= _T("");	// contains the directory string. for example - "\system\temp\"
		TCHAR szFilename[_MAX_FNAME]	= _T("");	// contains the file name minus path and ext. for example - "test"
		TCHAR szExt[_MAX_EXT]			= _T("");	// contains the file extension. for example - ".txt"

		// 分解路径
		// 1.如果文件夹为"c:\Temp\Folder\",
		//		szDrive="c:"
		//		szDir="\Temp\Folder\"
		//		szFname="" m_szExt=""
		// 2.如果文件夹为"c:\Temp\Folder",
		//		szDrive="c:"
		//		szDir="\Temp\"
		//		szFname="Folder"
		//		szExt=""
		_tsplitpath(szPath,szDrive,szDir,szFilename,szExt);


		TCHAR szFolder[MAX_PATH] = _T("");
		_stprintf(szFolder,_T("%s"),szDrive);

		TCHAR szDirTemp[MAX_PATH] = _T("");
		_stprintf(szDirTemp,_T("%s"),szDir);

		TCHAR szTemp[MAX_PATH] = _T("");

		while(1)
		{
			TCHAR* pSource = &szDirTemp[1];
			TCHAR* pDest = _tcschr(pSource,_T('\\'));
			if(!pDest)
				break;

			int pos = (int)(pDest-pSource);
			if(pos<=0)
				break;

			_stprintf(szTemp,_T("%s"),szDirTemp);
			memset(szDirTemp,0,MAX_PATH);
			_stprintf(szDirTemp,_T("%s"),szTemp);

			memset(szTemp,0,MAX_PATH);
			_tcsncpy(szTemp,pSource,pos);
			szTemp[pos] = _T('\0');

			_stprintf(szDirTemp,_T("%s"),pSource+pos);

			_stprintf(szFolder,_T("%s\\%s"),szFolder,szTemp);

			if(Exist(szFolder))
				continue;

			if(!::CreateDirectory(szFolder,NULL))
				return false;
		}

		return true;
	}

	//static
	bool KDirectory::Delete(LPCTSTR lpszPath)
	{
		return false;
	}

	//static
	bool KDirectory::Exist(LPCTSTR lpszPath)
	{

		TCHAR szTemp[MAX_PATH] = _T("");
		_stprintf(szTemp,lpszPath);

		int len = _tcslen(lpszPath);
		TCHAR ch = *(lpszPath+len-1);
		if(ch==_T('\\'))
		{
			*(szTemp+len-1) = _T('\0');
		}

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(szTemp, &fd );
		bool bExist = hFind != INVALID_HANDLE_VALUE;

		if(bExist)
		{
			::FindClose( hFind );
		}

		return bExist;
	}

//---------------------------------------------------------
}}// namespace Win32::IO - end
//---------------------------------------------------------
