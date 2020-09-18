#pragma once

#include "KVector3.h"

namespace Vision3D
{

	class KIND_EXT_CLASS KMathEngine
	{
	private:
		KMathEngine(void) {}

	public:
		// 球面坐标转换到笛卡尔坐标
		static KVector3 Spherical2Cartesian(double latitude, double longitude, double radius);
	};

}// namespace Vision3D
