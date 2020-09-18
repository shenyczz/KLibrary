#include "StdAfx.h"
#include "KGab.h"

namespace Wsr98d { namespace Rpg { namespace Helper
{

	KGab::KGab(void)
	{
	}

	KGab::~KGab(void)
	{
	}

	// 
	double KGab::ComputeLongitude(double lon0,double lat0,double azimuth,double range)
	{
		double dDegreePerKm = 1.0 / 111.37;

		double lon = lon0 + range * sin(azimuth * 3.1415926 / 180) * dDegreePerKm;
		double lat = lat0 + range * cos(azimuth * 3.1415926 / 180) * dDegreePerKm;

		return lon;
	}

	// 
	double KGab::ComputeLatitude(double lon0,double lat0,double azimuth,double range)
	{
		double dDegreePerKm = 1.0 / 111.37;

		double lon = lon0 + range * sin(azimuth * 3.1415926 / 180) * dDegreePerKm;
		double lat = lat0 + range * cos(azimuth * 3.1415926 / 180) * dDegreePerKm;

		return lat;
	}

//---------------------------------------------------------
}}}// namespace Wsr98d::Rpg::Helper - end
//---------------------------------------------------------
