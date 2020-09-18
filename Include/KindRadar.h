#ifndef __INCLUDED_KindRadar_H__
#define __INCLUDED_KindRadar_H__

//-----------------------------------------------------------------------------
#include "KindRadar.inl"

// Radar Data
//
#include "KRadarData.h"
#include "KWsr98Data.h"
#include "KRdaData.h"
#include "KRpgData.h"

// Radar Data Operator
//
#include "KRadarDataOperator.h"
#include "KWsr98DataOperator.h"
#include "KRdaDataOperator.h"
#include "KRpgDataOperator.h"

// Rpg Helper
//
#include "KGab.h"
#include "KSti58gab.h"
#include "KHi59gab.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindRadar"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindRadar_H__
