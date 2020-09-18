#include "StdAfx.h"
#include "KMathEngine.h"

namespace Vision3D
{
	// ��������ת�����ѿ�������
	KVector3 KMathEngine::Spherical2Cartesian(double latitude, double longitude, double radius)
	{
		latitude *= PI / 180.0;
		longitude *= PI / 180.0;
		double radCosLat = radius * cos(latitude);

		return KVector3(
			radCosLat * cos(longitude),
			radCosLat * sin(longitude),
			radius * sin(latitude)
			);
	}

}// namespace Vision3D
