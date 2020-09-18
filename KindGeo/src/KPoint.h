/******************************************************************************
 ** KPoint.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPoint
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
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

#include "KCoordinate.h"

namespace Geometries
{

	class KIND_EXT_CLASS KPoint : public KCoordinate
	{
	public:
		KPoint(void);
		KPoint(const KPoint& point);
		KPoint(double x,double y);
		virtual ~KPoint(void);

	public:
		// øÀ¬°
		KPoint* Clone();
	};

	typedef TPtrCollection<KPoint*> KPointPtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
