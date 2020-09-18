/******************************************************************************
 ** KCoordinateSystem.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KCoordinateSystem
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/
#pragma once

#include "KGeometry.h"

namespace Geometries
{

	class KIND_EXT_CLASS KCoordinate : public KGeometry
	{
	public:
		KCoordinate(void);
		KCoordinate(const KCoordinate& coordinate);
		KCoordinate(double x,double y);
		virtual ~KCoordinate(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);

	public:
		// 距离
		virtual double Distance(const KCoordinate& other);

	public:
		// 相等 '=='
		bool operator ==(const KCoordinate& rhs)
		{
			return (fabs(X - rhs.X) < 1.0e-12)
				&& (fabs(Y - rhs.Y) < 1.0e-12)
				;
		}
		// 不等 '!='
		bool operator !=(const KCoordinate& rhs)
		{
			return (fabs(X - rhs.X) > 1.0e-12)
				|| (fabs(Y - rhs.Y) > 1.0e-12)
				;
		}

	public:
		double X;
		double Y;
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
