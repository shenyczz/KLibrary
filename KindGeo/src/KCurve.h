/******************************************************************************
 ** KCurve.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KCurve
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

#include "KPolygon.h"

namespace Geometries
{

	class KIND_EXT_CLASS KCurve : public KPolyline
	{
	public:
		// 等值线和矩形边界构成的多边形分4种情况
		// 1. 等值线的起止点在对边上
		// 2. 等值线的起止点在相邻边上
		// 3. 等值线的起止点在同一条边上
		// 4. 等值线是闭合的等值线
		enum CurveTopology : int
		{
			// 未知
			Unkonw = 0,
			// 1. 等值线的起止点在对边上
			OppositeSide = 1,
			// 2. 等值线的起止点在相邻边上
			AdjacentSide = 2,
			// 3. 等值线的起止点在同一条边上
			SameSide = 3,
			// 4. 等值线是闭合的等值线
			Close = 4,
		};

	public:
		KCurve(void);
		KCurve(const KCurve& curve);
		virtual ~KCurve(void);

	public:
		// 克隆
		KCurve* Clone();

	public:
		// 对象值
		double& Value() { return m_dValue; }
		void SetValue(double dValue) { m_dValue = dValue; }
		double GetValue() { return m_dValue; }

		// 设置-获取曲线的张力系数
		double& Tension() { return m_dTension; }
		void SetTension(double dTension) { m_dTension = dTension; }
		double GetTension() { return m_dTension; }

		// 计算自己的绑定矩形
		void ComputeBoundingBox();

		// 为构造闭合曲线增加点
		void AddBetweenPoint(double x1,double y1,double x2,double y2);

		// 转换为闭合曲线
		CurveTopology KCurve::BuildCloseCurve(KExtents extents);
		CurveTopology BuildCloseCurve(double xmin, double ymin, double xmax, double ymax)
		{ return BuildCloseCurve(KExtents(xmin, ymin, xmax, ymax)); }

		// 计算面积
		double ComputrArea() { m_dArea = Area(); return m_dArea; }
		double GetArea() { return m_dArea; }

	protected:
		// 对象值
		double m_dValue;
		// 对象面积
		double m_dArea;
		// 张力系数
		double m_dTension;
	};

	typedef TPtrCollection<KCurve*> KCurvePtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
