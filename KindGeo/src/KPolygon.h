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
 **   Author: ������.֣�� (shenyczz@163.com)
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
		//���������ܳ�
		double Perimeter();
	};

	//typedef TPtrCollection<KPolygon*> KPolygonPtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
