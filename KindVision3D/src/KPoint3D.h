#pragma once

namespace Vision3D
{

	class KIND_EXT_CLASS KPoint3D
	{
	public:
		KPoint3D(void);
		KPoint3D(const KPoint3D& rhs);
		KPoint3D(double x, double y, double z);
		virtual ~KPoint3D(void);

	public:
		double X,Y,Z;
	};

}// namespace Vision3D
