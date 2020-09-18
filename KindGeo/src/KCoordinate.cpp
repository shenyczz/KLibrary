#include "StdAfx.h"
#include "KCoordinate.h"

namespace Geometries
{

	KCoordinate::KCoordinate(void)
		: KGeometry(),X(0),Y(0)
	{
	}

	KCoordinate::KCoordinate(const KCoordinate& coordinate)
		: KGeometry(coordinate), X(coordinate.X),Y(coordinate.Y)
	{
	}

	KCoordinate::KCoordinate(double x, double y)
		: KGeometry(),X(x),Y(y)
	{
	}

	KCoordinate::~KCoordinate(void)
	{
	}

	void KCoordinate::Offset(double dx,double dy)
	{
		X += dx;
		Y += dy;
	}

	void KCoordinate::GetMinCoordinate(double& x, double& y)
	{
		x = X; y = Y;
	}

	void KCoordinate::GetMaxCoordinate(double& x, double& y)
	{
		x = X; y = Y;
	}

	double KCoordinate::Distance(const KCoordinate& other)
	{
		double dx = this->X-other.X;
		double dy = this->X-other.X;
		return sqrt(dx*dx + dy*dy);
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------