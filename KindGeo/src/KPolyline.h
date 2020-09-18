/******************************************************************************
 ** KPolyline.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPolyline
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

#include "KPoint.h"

namespace Geometries
{

	class KIND_EXT_CLASS KPolyline : public KGeometry, public TPtrCollection<KPoint*>
	{
	public:
		KPolyline(void);
		KPolyline(const KPolyline& polyline);
		virtual ~KPolyline(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);

	public:
		// 是否闭合
		bool IsClose();
		// 长度
		double Length();
		//计算多边形面积
		double Area();

		// 点是否在内部
		bool PtInside(KPoint point);

		// 克隆
		KPolyline* Clone();
	};

	//typedef TPtrCollection<KPolyline*> KPolylinePtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
