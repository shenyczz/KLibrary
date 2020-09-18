#pragma once

namespace Vision3D
{

	class KIND_EXT_CLASS KVector3// : public D3DXVECTOR3
	{
	public:
		KVector3(void);
		KVector3(const KVector3& rhs);
		KVector3(double x, double y, double z);
		virtual ~KVector3(void);

	public:
		KVector3 Clone();

		// ²æ»ý
		KVector3& Cross(const KVector3 other);
		KVector3 Cross(const KVector3 other) const;

		// µã»ý
		double Dot(const KVector3 other) const;

	public:
		// casting
		//operator FLOAT* ();
		//operator CONST FLOAT* () const;

		// unary operators
		KVector3 operator + () const;
		KVector3 operator - () const;

		// assignment operators
		KVector3& operator +=(const KVector3& rhs);
		KVector3& operator -=(const KVector3& rhs);
		KVector3& operator *=(const double s);
		KVector3& operator *=(const KVector3& rhs);
		KVector3& operator /=(const double s);
		KVector3& operator /=(const KVector3& rhs);

		// binary operators
		KVector3 operator +(const KVector3& rhs) const;
		KVector3 operator -(const KVector3& rhs) const;
		KVector3 operator *(const double s) const;
		KVector3 operator *(const KVector3& rhs) const;
		KVector3 operator /(const double s) const;
		KVector3 operator /(const KVector3& rhs) const;

		bool operator ==(const KVector3& rhs) const;
		bool operator !=(const KVector3& rhs) const;

	public:
		double X,Y,Z;

	public:
		static KVector3 Scale(KVector3 source, double scalingFactor);
	};

}// namespace Vision3D
