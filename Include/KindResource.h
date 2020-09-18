#ifndef __INCLUDED_KindResource_H__
#define __INCLUDED_KindResource_H__

//-----------------------------------------------------------------------------
#include "KindResource.inl"
//-----------------------------------------------------------------------------
#include "_KResource.h"
#include "KResource.h"
//-----------------------------------------------------------------------------
#ifdef _DEBUG
	#if defined(_UNICODE)
		#define _KindResource_COMMENT_ "KindResourceud.lib"
		#define _KindResource_MESSAGE_ "Automatically linking with KindResourceud.dll"
	#else
		#define _KindResource_COMMENT_ "KindResourced.lib" 
		#define _KindResource_MESSAGE_ "Automatically linking with KindResourced.dll"
	#endif
#else
	#if defined(_UNICODE)
		#define _KindResource_COMMENT_ "KindResourceur.lib"
		#define _KindResource_MESSAGE_ "Automatically linking with KindResourceur.dll"
	#else
		#define _KindResource_COMMENT_ "KindResourcer.lib" 
		#define _KindResource_MESSAGE_ "Automatically linking with KindResourcer.dll"
	#endif
#endif
//-----------------------------------------------------------------------------
#pragma comment(lib, _KindResource_COMMENT_) 
#pragma message(_KindResource_MESSAGE_)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindResource_H__
