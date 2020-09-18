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
 **   Author: ������.֣�� (shenyczz@163.com)
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
		// ��ֵ�ߺ;��α߽繹�ɵĶ���η�4�����
		// 1. ��ֵ�ߵ���ֹ���ڶԱ���
		// 2. ��ֵ�ߵ���ֹ�������ڱ���
		// 3. ��ֵ�ߵ���ֹ����ͬһ������
		// 4. ��ֵ���Ǳպϵĵ�ֵ��
		enum CurveTopology : int
		{
			// δ֪
			Unkonw = 0,
			// 1. ��ֵ�ߵ���ֹ���ڶԱ���
			OppositeSide = 1,
			// 2. ��ֵ�ߵ���ֹ�������ڱ���
			AdjacentSide = 2,
			// 3. ��ֵ�ߵ���ֹ����ͬһ������
			SameSide = 3,
			// 4. ��ֵ���Ǳպϵĵ�ֵ��
			Close = 4,
		};

	public:
		KCurve(void);
		KCurve(const KCurve& curve);
		virtual ~KCurve(void);

	public:
		// ��¡
		KCurve* Clone();

	public:
		// ����ֵ
		double& Value() { return m_dValue; }
		void SetValue(double dValue) { m_dValue = dValue; }
		double GetValue() { return m_dValue; }

		// ����-��ȡ���ߵ�����ϵ��
		double& Tension() { return m_dTension; }
		void SetTension(double dTension) { m_dTension = dTension; }
		double GetTension() { return m_dTension; }

		// �����Լ��İ󶨾���
		void ComputeBoundingBox();

		// Ϊ����պ��������ӵ�
		void AddBetweenPoint(double x1,double y1,double x2,double y2);

		// ת��Ϊ�պ�����
		CurveTopology KCurve::BuildCloseCurve(KExtents extents);
		CurveTopology BuildCloseCurve(double xmin, double ymin, double xmax, double ymax)
		{ return BuildCloseCurve(KExtents(xmin, ymin, xmax, ymax)); }

		// �������
		double ComputrArea() { m_dArea = Area(); return m_dArea; }
		double GetArea() { return m_dArea; }

	protected:
		// ����ֵ
		double m_dValue;
		// �������
		double m_dArea;
		// ����ϵ��
		double m_dTension;
	};

	typedef TPtrCollection<KCurve*> KCurvePtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
