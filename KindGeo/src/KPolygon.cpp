#include "StdAfx.h"
#include "KPolygon.h"

namespace Geometries
{

	KPolygon::KPolygon(void)
		: KPolyline()
	{
		SetType(GeometryType_Polygon);
	}

	KPolygon::KPolygon(const KPolygon& polygon)
		: KPolyline(polygon)
	{
	}

	KPolygon::~KPolygon(void)
	{
	}

	//���������ܳ�
	double KPolygon::Perimeter()
	{
		return this->Length();
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
