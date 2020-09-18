/******************************************************************************
 ** KPolygon.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPolygon
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KPoint.h"
#include "KPolyline.h"

namespace Geometries
{

	class KIND_EXT_CLASS KPolygon : public KPolyline
	{
	public:
		KPolygon(void);
		KPolygon(const KPolygon& polygon);
		virtual ~KPolygon(void);

	public:
		//计算多边形周长
		double Perimeter();
	};

	//typedef TPtrCollection<KPolygon*> KPolygonPtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
