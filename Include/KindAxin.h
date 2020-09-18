#ifndef __INCLUDED_KindAxin_H__
#define __INCLUDED_KindAxin_H__

//-----------------------------------------------------------------------------
#include "KindAxin.inl"

#include "KAxinData.h"
#include "KBlackboardData.h"
#include "KColortableData.h"
#include "KGridData.h"
#include "KGridContourData.h"
#include "KGridImageData.h"
#include "KImageCtrlData.h"
#include "KStationData.h"
#include "KWindVectorData.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindAxin"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindAxin_H__
