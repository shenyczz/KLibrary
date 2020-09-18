#ifndef __INCLUDED_KindGeo_H__
#define __INCLUDED_KindGeo_H__

//-----------------------------------------------------------------------------
#include "KindGeo.inl"

// Geometries
//
#include "KGeometry.h"				// ����ͼ��
#include "KCoordinate.h"			// ����
#include "KPoint.h"					// ��
#include "KLine.h"					// �߶�
#include "KRect.h"					// ����
#include "KEllipse.h"				// ��Բ
#include "KPolyline.h"				// ����
#include "KPolygon.h"				// �����
#include "KCurve.h"					// ��
#include "KContour.h"				// ��ֵ��(���߼���)
//#include "KSurface.h"				// ��
//#include "KVolume.h"				// ��
#include "KLabel.h"					// ��ע
#include "KLegend.h"				// ͼ��

// ͼ���Ѽ�
#include "KGeometryPtrCollection.h"


// CoordinateSystems
//
#include "KCoordinateSystem.h"		// ����ϵ


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
