#include "StdAfx.h"
#include "KPoint3D.h"

namespace Vision3D
{

	KPoint3D::KPoint3D(void)
	{
		X = 0;
		Y = 0;
		Z = 0;
	}


	KPoint3D::KPoint3D(const KPoint3D& rhs)
	{
		X = rhs.X;
		Y = rhs.Y;
		Z = rhs.Z;
	}

	KPoint3D::KPoint3D(double x, double y, double z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	KPoint3D::~KPoint3D(void)
	{
	}

}// namespace Vision3D
