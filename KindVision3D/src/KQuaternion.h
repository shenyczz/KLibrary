/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KQuaternion - 四元数
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 - 
 ** Web Site: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KPoint3D.h"

namespace Vision3D
{

	class KIND_EXT_CLASS KQuaternion
	{
	public:
		KQuaternion(void);
		KQuaternion(const KQuaternion& rhs);
		KQuaternion(double x, double y, double z, double w);
		virtual ~KQuaternion(void);

	public:
		double Abs() const;
		double Length() const;
		KQuaternion Conjugate() const;
		void Normalize();

	public:
		KQuaternion operator +(const KQuaternion& rhs) const;
		KQuaternion operator -(const KQuaternion& rhs) const;
		KQuaternion operator *(const double s) const;
		KQuaternion operator *(const KQuaternion& rhs) const;
		KQuaternion operator /(const double s) const;
		KQuaternion operator /(const KQuaternion& rhs) const;

		bool operator ==(const KQuaternion& rhs) const;
		bool operator !=(const KQuaternion& rhs) const;

	public:
		static KQuaternion EulerToQuaternion(double yaw, double pitch, double roll);
		static KPoint3D QuaternionToEuler(KQuaternion q);
		static double Dot(KQuaternion a, KQuaternion b);
		static KQuaternion Slerp(KQuaternion q0, KQuaternion q1, double t);
		static KQuaternion Ln(KQuaternion q);


	public:
		double X,Y,Z,W;
	};

}// namespace Vision3D
