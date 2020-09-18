#include "StdAfx.h"
#include "KPoint.h"


namespace Geometries
{

	KPoint::KPoint(void)
		: KCoordinate()
	{
		SetType(GeometryType_Point);
	}

	KPoint::KPoint(const KPoint& point)
		: KCoordinate(point)
	{
		SetType(GeometryType_Point);
	}

	KPoint::KPoint(double x,double y)
		: KCoordinate(x, y)
	{
		SetType(GeometryType_Point);
	}

	KPoint::~KPoint(void)
	{
	}

	// ¿ËÂ¡
	KPoint* KPoint::Clone()
	{
		return new KPoint(*this);
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
