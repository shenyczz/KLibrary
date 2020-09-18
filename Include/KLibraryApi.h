#ifndef __INCLUDED_KLIBRARY_API_H__
#define __INCLUDED_KLIBRARY_API_H__

//-----------------------------------------------------------------------------
#include "KLibraryApi.inl"

#include "Wsr98dApi.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
#undef DLL_NAME
#define DLL_NAME		"KLibraryApi"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KLIBRARY_API_H__
