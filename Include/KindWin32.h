#ifndef __INCLUDED_KindWin32_H__
#define __INCLUDED_KindWin32_H__

//-----------------------------------------------------------------------------
#include "KindWin32.inl"

#include "KDirectory.h"
#include "KFile.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG	// Debug

	#if defined(_UNICODE)
		#define _KindWin32_Comment_ "KindWin32ud.lib"
		#define _KindWin32_Message_ "Automatically linking with KindWin32ud.dll"
	#else
		#define _KindWin32_Comment_ "KindWin32d.lib" 
		#define _KindWin32_Message_ "Automatically linking with KindWin32d.dll"
	#endif

#else			// Release

	#if defined(_UNICODE)
		#define _KindWin32_Comment_ "KindWin32ur.lib"
		#define _KindWin32_Message_ "Automatically linking with KindWin32ur.dll"
	#else
		#define _KindWin32_Comment_ "KindWin32r.lib" 
		#define _KindWin32_Message_ "Automatically linking with KindWin32r.dll"
	#endif

#endif
//-----------------------------------------------------------------------------
#pragma comment(lib, _KindWin32_Comment_) 
#pragma message(_KindWin32_Message_)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindWin32_H__
