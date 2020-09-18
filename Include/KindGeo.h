#ifndef __INCLUDED_KindGeo_H__
#define __INCLUDED_KindGeo_H__

//-----------------------------------------------------------------------------
#include "KindGeo.inl"

// Geometries
//
#include "KGeometry.h"				// 几何图形
#include "KCoordinate.h"			// 坐标
#include "KPoint.h"					// 点
#include "KLine.h"					// 线段
#include "KRect.h"					// 矩形
#include "KEllipse.h"				// 椭圆
#include "KPolyline.h"				// 折线
#include "KPolygon.h"				// 多边形
#include "KCurve.h"					// 线
#include "KContour.h"				// 等值线(曲线集合)
//#include "KSurface.h"				// 面
//#include "KVolume.h"				// 体
#include "KLabel.h"					// 标注
#include "KLegend.h"				// 图例

// 图形搜集
#include "KGeometryPtrCollection.h"


// CoordinateSystems
//
#include "KCoordinateSystem.h"		// 坐标系


//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindGeo"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindGeo_H__
