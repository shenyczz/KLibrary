#ifndef __INCLUDED_KindGis_H__
#define __INCLUDED_KindGis_H__

//-----------------------------------------------------------------------------
#include "KindGis.inl"

// 资源包含文件
// 
#include "GisResource.h"

// static class
//
#include "KCallbackFunc.h"

// Framework
//
#include "KGisWinApp.h"
#include "KGisFrameWnd.h"
#include "KGisMDIFrameWnd.h"

#include "KBlackboardDocument.h"
#include "KBlackboardView.h"

#include "KShapeFileDocument.h"
#include "KShapeFileView.h"

#include "KAxinDataDocument.h"
#include "KAxinDataView.h"

#include "KAgmrsDataDocument.h"
#include "KAgmrsDataView.h"

#include "KRadarDataDocument.h"
#include "KRadarDataView.h"

#include "KDiamondDataDocument.h"
#include "KDiamondDataView.h"

#include "KFinalDocument.h"
#include "KFinalView.h"

#include "KGisDocument.h"
#include "KGisView.h"

// Layers
// 
#include "KLayer.h"
#include "KLayerProperty.h"

// Renders
//
#include "KRender.h"
#include "KAgmrsDataRender.h"
#include "KBlackboardRender.h"
#include "KGridContourDataRender.h"
#include "KGridImageDataRender.h"
#include "KRpgDataRender.h"
#include "KShapeDataRender.h"
#include "KStationDataRender.h"
#include "KWindVectorDataRender.h"

//#include "KImageRender.h"
//#include "KLegendRender.h"
//#include "KGridRender.h"
//#include "KLightningRender.h"
//#include "KModisRender.h"
//#include "KdGridImageRender.h"

#include "KDiamondGridDataRender.h"

// Map
#include "KMapEngine.h"

// Mouse
//
#include "KMouseListener.h"
#include "KMouseListenerCollection.h"

// DockablePanes
// 
#include "KLayerDockablePane.h"
#include "KOutputDockablePane.h"
#include "KPropertyDockablePane.h"
#include "KRollupDockablePane.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindGis"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindGis_H__
