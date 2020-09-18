#ifndef __INCLUDED_KindAgmrs_H__
#define __INCLUDED_KindAgmrs_H__

//-----------------------------------------------------------------------------
#include "KindAgmrs.inl"

#include "KAgmrsThreshold.h"

#include "KAgmrsData.h"
#include "KAgmrsAlgorithm.h"
#include "KAgmrsDataProcessor.h"

#include "KLDFile.h"
#include "KLDAlgorithm.h"
#include "KLDFileProcessor.h"

#include "KAvhrrData.h"
#include "KAvhrrAlgorithm.h"
#include "KAvhrrDataProcessor.h"

#include "KMersiData.h"
#include "KMersiAlgorithm.h"
#include "KMersiDataProcessor.h"

#include "KModisData.h"
#include "KModisAlgorithm.h"
#include "KModisDataProcessor.h"

#include "KVirrData.h"
#include "KVirrAlgorithm.h"
#include "KVirrDataProcessor.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindAgmrs"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindAgmrs_H__
