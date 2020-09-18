#ifndef __INCLUDED_KindMfc_H__
#define __INCLUDED_KindMfc_H__

//-----------------------------------------------------------------------------
#include "KindMfc.inl"

// Framework
//
#include "KWinAppEx.h"			// Application

#include "KFrameWndEx.h"		// Framework
#include "KMDIFrameWndEx.h"

#include "KDocument.h"			// Document

#include "KView.h"				// View
#include "KPrintView.h"
#include "KScrollView.h"

// DockablePanes
//
#include "KDockablePane.h"
#include "KDockablePaneToolBar.h"
#include "KEditCtrlDockablePane.h"

// Gui
//
#include "KHistogramWnd.h"
#include "KSplashScreen.h"
#include "KSplitterWnd.h"
#include "KTreeCtrl.h"
#include "KEditCtrl.h"
#include "KRichEditCtrl.h"

// Ado
//
#include "KAdoDatabase.h"
#include "KAdoCommand.h"
#include "KAdoParameter.h"
#include "KAdoRecordset.h"
#include "KAdoException.h"
#include "KJetEngine.h"
#include "KOleDbDataLink.h"
#include "KOleDbException.h"


//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindMfc"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindMfc_H__
