#include "StdAfx.h"
#include "KViewport3D.h"

namespace Vision3D
{

	KViewport3D::KViewport3D(void)
	{
		X = Y = 0;
		MinZ = MaxZ = 0;
		Width = Height = 0;
	}


	KViewport3D::~KViewport3D(void)
	{
	}

}// namespace Vision3D
