#include "StdAfx.h"
#include "KMath.h"

#include <math.h>

//---------------------------------------------------------
double KMath::e			= 2.17828;
double KMath::Pi		= 3.1415926;
double KMath::Epslon	= 1.0e-12;
//---------------------------------------------------------

bool KMath::FloatEqual(double v1, double v2)
{
	return fabs(v2-v1) < 1.0e-12;
}

// 计算两点间的距离
double KMath::Distance(double x1, double y1, double x2, double y2)
{
	double dx = x2 - x1;
	double dy = y2 - y1;

	return sqrt( dx * dx + dy * dy );
}
