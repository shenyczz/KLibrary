#ifndef __INCLUDED_API_INL__
#define __INCLUDED_API_INL__


#ifdef KLIBRARYAPI_EXPORTS
#define KLIBRARYAPI_EXPORTS_API __declspec(dllexport)
#else
#define KLIBRARYAPI_EXPORTS_API __declspec(dllimport)
#endif

#ifdef __cplusplus
	#define _EXTERN_C_ extern "C"
	#define _EXTERN_C_REGIONBEGIN extern "C"	{
	#define _EXTERN_C_REGIONEND					}
#else
	#define _EXTERN_C_
	#define _EXTERN_C_REGIONBEGIN
	#define _EXTERN_C_REGIONEND
#endif

#define MASC_API	_EXTERN_C_	KLIBRARYAPI_EXPORTS_API


#ifdef _STDCALL
#undef _STDCALL
#define _STDCALL __stdcall
	#else
#define _STDCALL __stdcall
#endif

#ifdef _CDECL
#undef _CDECL
#define _CDECL __cdecl
#else
#define _CDECL __cdecl
#endif

#endif	//#ifndef __INCLUDED_API_INL__

