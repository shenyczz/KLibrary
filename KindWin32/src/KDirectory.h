/******************************************************************************
 ** KDirectory.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDirectory
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
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

	class KIND_EXT_CLASS KDirectory
	{
	public:
		KDirectory(void);
		virtual ~KDirectory(void);

	public:
		static bool Create(LPCTSTR lpszPath);
		static bool Delete(LPCTSTR lpszPath);
		static bool Exist(LPCTSTR lpszPath);
	};

//---------------------------------------------------------
}}// namespace Win32::IO - end
//---------------------------------------------------------
