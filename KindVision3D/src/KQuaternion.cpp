#include "StdAfx.h"
#include "KQuaternion.h"

#include <math.h>

namespace Vision3D
{

	KQuaternion::KQuaternion(void)
	{
		X = 0;
		Y = 0;
		Z = 0;
		W = 0;
	}

	KQuaternion::KQuaternion(const KQuaternion& rhs)
	{
		X = rhs.X;
		Y = rhs.Y;
		Z = rhs.Z;
		W = rhs.W;
	}

	KQuaternion::KQuaternion(double x, double y, double z, double w)
	{
		X = x;
		Y = y;
		Z = z;
		W = w;
	}

	KQuaternion::~KQuaternion(void)
	{
	}

	double KQuaternion::Abs() const
	{
		return sqrt(X*X + Y*Y + Z*Z + W*W);
	}

	double KQuaternion::Length() const
	{
		return sqrt(X*X + Y*Y + Z*Z + W*W);
	}

	// Conjugate(¹²éî)
	KQuaternion KQuaternion::Conjugate() const
	{
		return KQuaternion(-X, -Y, -Z, W);
	}

	void KQuaternion::Normalize()
	{
		double L = Length();

		X = X / L;
		Y = Y / L;
		Z = Z / L;
		W = W / L;
	}

	KQuaternion KQuaternion::operator +(const KQuaternion& rhs) const
	{
		KQuaternion quaternion;
		quaternion.X += rhs.X;
		quaternion.Y += rhs.Y;
		quaternion.Z += rhs.Z;
		quaternion.W += rhs.W;
		return quaternion;
	}

	KQuaternion KQuaternion::operator -(const KQuaternion& rhs) const
	{
		KQuaternion quaternion;
		quaternion.X -= rhs.X;
		quaternion.Y -= rhs.Y;
		quaternion.Z -= rhs.Z;
		quaternion.W -= rhs.W;
		return quaternion;
	}

	KQuaternion KQuaternion::operator *(const double s) const
	{
		KQuaternion quaternion;
		quaternion.X *= s;
		quaternion.Y *= s;
		quaternion.Z *= s;
		quaternion.W *= s;
		return quaternion;
	}

	KQuaternion KQuaternion::operator *(const KQuaternion& rhs) const
	{
		KQuaternion quaternion
		(
			W * rhs.X + X * rhs.W + Y * rhs.Z - Z * rhs.Y,
			W * rhs.Y + Y * rhs.W + Z * rhs.X - X * rhs.Z,
			W * rhs.Z + Z * rhs.W + X * rhs.Y - Y * rhs.X,
			W * rhs.W - X * rhs.X - Y * rhs.Y - Z * rhs.Z
		);
		return quaternion;
	}

	KQuaternion KQuaternion::operator /(const double s) const
	{
		KQuaternion quaternion;
		quaternion.X /= s;
		quaternion.Y /= s;
		quaternion.Z /= s;
		quaternion.W /= s;
		return quaternion;
	}

	KQuaternion KQuaternion::operator /(const KQuaternion& rhs) const
	{
		KQuaternion quaternion = *this;
		quaternion * (rhs.Conjugate() / rhs.Abs());
		return quaternion;
	}

	bool KQuaternion::operator ==(const KQuaternion& rhs) const
	{
		return X == rhs.X && Y == rhs.Y && Z == rhs.Z && W == rhs.W;
	}

	bool KQuaternion::operator !=(const KQuaternion& rhs) const
	{
		return X != rhs.X || Y != rhs.Y || Z != rhs.Z || W != rhs.W;
	}

	//static 
	KQuaternion KQuaternion::EulerToQuaternion(double yaw, double pitch, double roll)
	{
		double cy = cos(yaw * 0.5);
		double cp = cos(pitch * 0.5);
		double cr = cos(roll * 0.5);
		double sy = sin(yaw * 0.5);
		double sp = sin(pitch * 0.5);
		double sr = sin(roll * 0.5);

		double qw = cy * cp * cr + sy * sp * sr;
		double qx = sy * cp * cr - cy * sp * sr;
		double qy = cy * sp * cr + sy * cp * sr;
		double qz = cy * cp * sr - sy * sp * cr;

		return KQuaternion(qx, qy, qz, qw);
	}

	// static
	KPoint3D KQuaternion::QuaternionToEuler(KQuaternion q)
	{
		double q0 = q.W;
		double q1 = q.X;
		double q2 = q.Y;
		double q3 = q.Z;

		double x = atan2(2 * (q2 * q3 + q0 * q1), (q0 * q0 - q1 * q1 - q2 * q2 + q3 * q3));
		double y = asin(-2 * (q1 * q3 - q0 * q2));
		double z = atan2(2 * (q1 * q2 + q0 * q3), (q0 * q0 + q1 * q1 - q2 * q2 - q3 * q3));

		return KPoint3D(x, y, z);
	}

	// static
	double KQuaternion::Dot(KQuaternion a, KQuaternion b)
	{
		return a.X * b.X + a.Y * b.Y + a.Z * b.Z + a.W * b.W;
	}

	// static
	KQuaternion KQuaternion::Slerp(KQuaternion q0, KQuaternion q1, double t)
	{
		double cosom = q0.X * q1.X + q0.Y * q1.Y + q0.Z * q1.Z + q0.W * q1.W;
		double tmp0, tmp1, tmp2, tmp3;
		if (cosom < 0.0)
		{
			cosom = -cosom;
			tmp0 = -q1.X;
			tmp1 = -q1.Y;
			tmp2 = -q1.Z;
			tmp3 = -q1.W;
		}
		else
		{
			tmp0 = q1.X;
			tmp1 = q1.Y;
			tmp2 = q1.Z;
			tmp3 = q1.W;
		}

		/* calc coeffs */
		double scale0, scale1;

		if ((1.0 - cosom) > 1.0e-12)
		{
			// standard case (slerp)
			double omega = acos(cosom);
			double sinom = sin(omega);
			scale0 = sin((1.0 - t) * omega) / sinom;
			scale1 = sin(t * omega) / sinom;
		}
		else
		{
			/* just lerp */
			scale0 = 1.0 - t;
			scale1 = t;
		}

		KQuaternion q;

		q.X = scale0 * q0.X + scale1 * tmp0;
		q.Y = scale0 * q0.Y + scale1 * tmp1;
		q.Z = scale0 * q0.Z + scale1 * tmp2;
		q.W = scale0 * q0.W + scale1 * tmp3;

		return q;
	}

	//static
	KQuaternion KQuaternion::Ln(KQuaternion q)
	{
		double t = 0;

		double s = sqrt(q.X * q.X + q.Y * q.Y + q.Z * q.Z);
		double om = atan2(s, q.W);

		if (abs(s) < 1.0e-12)
			t = 0.0f;
		else
			t = om / s;

		q.X = q.X * t;
		q.Y = q.Y * t;
		q.Z = q.Z * t;
		q.W = 0.0f;

		return q;
	}

}// namespace Vision3D
