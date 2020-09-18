#ifndef __INCLUDED_KindData_H__
#define __INCLUDED_KindData_H__

//-----------------------------------------------------------------------------
#include "KindData.inl"

// Data Object
// 
#include "KDataObject.h"
#include "KDataAlgorithm.h"
#include "KDataProcessor.h"

#include "KCustomDataObject.h"

// Shapefile
//
#include "KShapeFile.h"
#include "KShapeFileIndex.h"
#include "KShapeFileHeader.h"
#include "KShapeRecordEntry.h"

// DbaseFile
// 
#include "KDbaseFile.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindData"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindData_H__
