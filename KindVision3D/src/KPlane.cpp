#include "StdAfx.h"
#include "KPlane.h"

namespace Vision3D
{

	KPlane::KPlane(void)
	{
	}


	KPlane::~KPlane(void)
	{
	}

	KPlane& KPlane::Normalize()
	{
		float len = sqrt(a*a + b*b + c*c + d*d);
		a = a / len;
		b = b / len;
		c = c / len;
		d = d / len;
		return *this;
	}

}// namespace Vision3D
