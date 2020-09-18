#ifndef __INCLUDED_KindProvider_H__
#define __INCLUDED_KindProvider_H__

//-----------------------------------------------------------------------------
#include "KindProvider.inl"

// Data Providers
// 
#include "KProvider.h"
#include "KAgmrsDataProvider.h"
#include "KBlackboardProvider.h"
#include "KDiamondDataProvider.h"
#include "KAxinDataProvider.h"
#include "KRadarDataProvider.h"
#include "KShapeFileProvider.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindProvider"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindProvider_H__
