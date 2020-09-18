#include "StdAfx.h"
#include "KVector3.h"

namespace Vision3D
{

	KVector3::KVector3(void)
	{
		X = 0;
		Y = 0;
		Z = 0;
	}

	KVector3::KVector3(double x, double y, double z)
	{
		X = x;
		Y = y;
		Z = z;
	}

	KVector3::KVector3(const KVector3& rhs)
	{
		X = rhs.X;
		Y = rhs.Y;
		Z = rhs.Z;
	}

	KVector3::~KVector3(void)
	{
	}

	KVector3 KVector3::Clone()
	{
		return KVector3(*this);
	}

	KVector3& KVector3::Cross(const KVector3 other)
	{
		return *this;
	}
	KVector3 KVector3::Cross(const KVector3 other) const
	{
		return KVector3();
	}

	double KVector3::Dot(const KVector3 other) const
	{
		return 0;
	}

	KVector3 KVector3::operator +() const
	{
		return *this;
	}
	KVector3 KVector3::operator -() const
	{
		KVector3 vector3(*this);
		vector3 *= -1;
		return vector3;
	}

	KVector3& KVector3::operator +=(const KVector3& rhs)
	{
		X += rhs.X;
		Y += rhs.Y;
		Z += rhs.Z;
		return *this;
	}
	KVector3& KVector3::operator -=(const KVector3& rhs)
	{
		X -= rhs.X;
		Y -= rhs.Y;
		Z -= rhs.Z;
		return *this;
	}
	KVector3& KVector3::operator *=(const double s)
	{
		X *= s;
		Y *= s;
		Z *= s;
		return *this;
	}
	KVector3& KVector3::operator *=(const KVector3& rhs)
	{
		return *this;
	}
	KVector3& KVector3::operator /=(const double s)
	{
		X /= s;
		Y /= s;
		Z /= s;
		return *this;
	}
	KVector3& KVector3::operator /=(const KVector3& rhs)
	{
		return *this;
	}

	KVector3 KVector3::operator +(const KVector3& rhs) const
	{
		KVector3 vector3(*this);
		vector3.X += rhs.X;
		vector3.Y += rhs.Y;
		vector3.Z += rhs.Z;
		return vector3;
	}
	KVector3 KVector3::operator -(const KVector3& rhs) const
	{
		KVector3 vector3 = *this;
		vector3.X -= rhs.X;
		vector3.Y -= rhs.Y;
		vector3.Z -= rhs.Z;
		return vector3;
	}
	KVector3 KVector3::operator *(const double s) const
	{
		KVector3 vector3 = *this;
		vector3.X *= s;
		vector3.Y *= s;
		vector3.Z *= s;
		return vector3;
	}
	KVector3 KVector3::operator *(const KVector3& rhs) const
	{
		KVector3 vector3 = *this;
		return vector3;
	}
	KVector3 KVector3::operator /(const double s) const
	{
		KVector3 vector3 = *this;
		vector3.X /= s;
		vector3.Y /= s;
		vector3.Z /= s;
		return vector3;
	}
	KVector3 KVector3::operator /(const KVector3& rhs) const
	{
		KVector3 vector3 = *this;
		return vector3;
	}
	bool KVector3::operator ==(const KVector3& rhs) const
	{
		return true
			&& X == rhs.X
			&& Y == rhs.Y
			&& Z == rhs.Z
			;
	}
	bool KVector3::operator !=(const KVector3& rhs) const
	{
		return false
			|| X != rhs.X
			|| Y != rhs.Y
			|| Z != rhs.Z
			;
	}

	// static
	KVector3 KVector3::Scale(KVector3 source, double scalingFactor)
	{
		KVector3 vector3 = source;
		vector3.X *= scalingFactor;
		vector3.Y *= scalingFactor;
		vector3.Z *= scalingFactor;
		return vector3;
	}


}// namespace Vision3D
