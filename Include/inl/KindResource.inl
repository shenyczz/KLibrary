#ifndef __INCLUDED_KIND_RESOURCE_INL__
#define __INCLUDED_KIND_RESOURCE_INL__

#include "KLibrary.inl"

#ifdef _DEBUG
	#if defined(_UNICODE)
		#define KindResource_Library	_T("KindResourceud.dll")
	#else
		#define KindResource_Library	_T("KindResourced.dll")
	#endif
#else
	#if defined(_UNICODE)
		#define KindResource_Library	_T("KindResourceur.dll")
	#else
		#define KindResource_Library	_T("KindResourcer.dll" )
	#endif
#endif

#endif	//#ifndef __INCLUDED_KIND_RESOURCE_INL__
