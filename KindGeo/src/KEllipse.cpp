#include "StdAfx.h"
#include "KEllipse.h"

namespace Geometries
{

	KEllipse::KEllipse(void)
		: KRect()
	{
		SetType(GeometryType_Ellipse);
	}

	KEllipse::KEllipse(const KEllipse& ellipse)
		: KRect(ellipse)
	{
		SetType(GeometryType_Ellipse);
	}

	KEllipse::KEllipse(double x,double y,double w,double h)
		: KRect(x,y,w,h)
	{
		SetType(GeometryType_Ellipse);
	}

	KEllipse::~KEllipse(void)
	{
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
