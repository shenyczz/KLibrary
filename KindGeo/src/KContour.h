/******************************************************************************
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KContour - 等高线
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

#include "KCurve.h"

namespace Geometries
{

	class KIND_EXT_CLASS KContour : public KGeometry, public TPtrCollection<KCurve*>
	{
	public:
		KContour(void);
		KContour(const KContour& contour);
		virtual ~KContour(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);

	public:
		// 在指定索引前插入曲线
		void InsertBefor(int index, KCurve* pCurve);
		// 根据面积为等值线添加闭合曲线(降序)
		void AddCurveByArea(KCurve* pCurve);

		// 设置-获取对象值
		double& Value() { return m_dValue; }
		void SetValue(double dValue) { m_dValue = dValue; }
		double GetValue() { return m_dValue; }

		KCurve* GetCurve(int index) { return this->GetAt(index); }

	private:
		// 值
		double m_dValue;
	};

	typedef TPtrCollection<KContour*> KContourPtrCollection;

}
