#ifndef __INCLUDED_KindDiamond_H__
#define __INCLUDED_KindDiamond_H__

//-----------------------------------------------------------------------------
#include "KindDiamond.inl"

#include "KDiamondData.h"
#include "KDiamondDataAlgorithm.h"
#include "KDiamondDataProcessor.h"

#include "KDiamondGridData.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindDiamond"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindDiamond_H__
