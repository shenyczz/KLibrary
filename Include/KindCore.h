#ifndef __INCLUDED_KindCore_H__
#define __INCLUDED_KindCore_H__

//-----------------------------------------------------------------------------
#include "KindCore.inl"

// Objects
//
#include "KProperty.h"
#include "KProgressObject.h"

// Images
//
#include "KDib.h"
#include "KImage.h"

// XBase
//
#include "KFieldValue.h"
#include "KField.h"
#include "KFields.h"
#include "KRow.h"
#include "KRows.h"

// Events
//
#include "KEventArgs.h"
#include "KMouseEventArgs.h"

#include "KMouseEvent.h"

// Misce
//
#include "KPaletteItem.h"
#include "KPalette.h"
#include "KStdPalette.h"
#include "KIdxPalette.h"
#include "KRbwPalette.h"
#include "KSegPalette.h"
#include "KLinearPalette.h"

// XML
//
#include "TinyXml.h"
#include "KMarkup.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		KIND_CORE_DLL_NAME
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindCore_H__
