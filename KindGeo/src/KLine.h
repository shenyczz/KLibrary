/******************************************************************************
 ** KLine.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLine - 线段
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

	class KIND_EXT_CLASS KLine : public KGeometry
	{
	public:
		KLine(void);
		KLine(const KLine& line);
		KLine(KPoint headPoint,KPoint tailPoint);
		KLine(double x1,double y1,double x2,double y2);
		virtual ~KLine(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);


	public:
		// 起点
		const KPoint& GetHeadPoint() { return m_pointHead; }
		void SetHeadPoint(KPoint point) { m_pointHead = point; }
		// 终点
		const KPoint& GetTailPoint() { return m_pointTail; }
		void SetTailPoint(KPoint point) { m_pointTail = point; }

		// 是否闭合
		bool IsClose();
		// 长度
		double Length();

	protected:
		KPoint m_pointHead;
		KPoint m_pointTail;
	};

	typedef TPtrCollection<KLine*> KLinePtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
