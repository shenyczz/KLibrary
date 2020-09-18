#include "StdAfx.h"
#include "KCurve.h"

namespace Geometries
{

	KCurve::KCurve(void)
		:KPolyline()
		,m_dValue(0)
		,m_dArea(0)
		,m_dTension(0.3)
	{
		SetType(GeometryType_Curve);
	}
	
	KCurve::KCurve(const KCurve& curve)
		:KPolyline(curve)
		,m_dValue(curve.m_dValue)
		,m_dArea(curve.m_dArea)
		,m_dTension(curve.m_dTension)
	{
		SetType(GeometryType_Curve);
	}

	KCurve::~KCurve(void)
	{
	}

	// 克隆
	KCurve* KCurve::Clone()
	{
		return new KCurve(*this);
	}

	// 计算自己的绑定矩形
	void KCurve::ComputeBoundingBox()
	{
		int iPointCount = this->Count();
		if(iPointCount==0)
		{
			this->SetExtents(KExtents(0,0,0,0));
			return;
		}

		double xmin(1.0e+12);
		double ymin(1.0e+12);

		double xmax(-1.0e+12);
		double ymax(-1.0e+12);

		for(int i=0;i<iPointCount;i++)
		{
			KPoint point = *this->GetAt(i);

			xmin = min(xmin,point.X);
			ymin = min(ymin,point.Y);

			xmax = max(xmax,point.X);
			ymax = max(ymax,point.Y);
		}

		this->SetExtents(KExtents(xmin,ymin,xmax,ymax));
	}

	//为构造闭合多边形增加点(由BuildClosePolygon()函数调用)
	//不增加(x1,y1) 但增加 (x2,y2)
	void KCurve::AddBetweenPoint(double x1,double y1,double x2,double y2)
	{
		int i;
		//-----------------------------
		int k = 50;	//等分比例
		//-----------------------------
		double xInterval = (x2 - x1) / k;
		double yInterval = (y2 - y1) / k;
		//-----------------------------
		for(i=1;i<k-1;i++)
		{
			double x = x1 + xInterval * i;
			double y = y1 + yInterval * i;
			this->Add(new KPoint(x,y));
		}
		//-----------------------------
		this->Add(new KPoint(x2,y2));
	}
	// 转换为闭合曲线
	// 根据给出的包围盒
	// 构造一个闭合曲线
	// 曲线和边界的拓扑关系分4种情况
	// 1. 多边形的起止点在对边上
	// 2. 多边形的起止点在相邻边上
	// 3. 多边形的起止点在同一条边上
	// 4. 多边形是闭合的
	// 转换为闭合曲线
	KCurve::CurveTopology KCurve::BuildCloseCurve(KExtents extents)
	{
		CurveTopology eTopology = CurveTopology::Unkonw;

		double xmin = extents.MinX();
		double ymin = extents.MinY();
		double xmax = extents.MaxX();
		double ymax = extents.MaxY();

		int iPointCount = this->Count();
		if(iPointCount==0)
			return eTopology;

		// 曲线的起点
		KPoint ptBeg = *this->GetAt(0);
		double xb = ptBeg.X;
		double yb = ptBeg.Y;

		// 曲线的终点
		KPoint ptEnd = *this->GetAt(iPointCount-1);
		double xe = ptEnd.X;
		double ye = ptEnd.Y;

		//4.等值线是闭合的
		if(this->IsClose())
		{
			eTopology = CurveTopology::Close;
			return eTopology;
		}

		/// 按顺时针方向增加点 - beg
		///
		// 1.多边形的起止点在对边上
		// 1-1.起点在左边,终点在右边
		//if(xb==xmin && xe==xmax)
		if(fabs(xb-xmin)<1.0e-12 && fabs(xe-xmax)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			// 增加(x1,y1)和(x2,y2)之间的若干点
			// 不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymin);
			AddBetweenPoint(xmax,ymin,xmin,ymin);
			AddBetweenPoint(xmin,ymin,xb,yb);
		}
		// 1-2.起点在右边,终点在左边
		//else if(xb==xmax && xe==xmin)
		else if(fabs(xb-xmax)<1.0e-12 && fabs(xe-xmin)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymax);
			AddBetweenPoint(xmin,ymax,xmax,ymax);
			AddBetweenPoint(xmax,ymax,xb,yb);
		}
		// 1-3. 起点在下边,终点在上边
		//else if(yb==ymin && ye==ymax)
		else if(fabs(yb-ymin)<1.0e-12 && fabs(ye-ymax)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymax);
			AddBetweenPoint(xmax,ymax,xmax,ymin);
			AddBetweenPoint(xmax,ymin,xb,yb);
		}
		// 1-4. 起点在上边,终点在下边
		//else if(yb==ymax && ye==ymin)
		else if(fabs(yb-ymax)<1.0e-12 && fabs(ye-ymin)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymin);
			AddBetweenPoint(xmin,ymin,xmin,ymax);
			AddBetweenPoint(xmin,ymax,xb,yb);
		}
 
		// 2.等值线的起止点在相邻边上
		// 2-1. 左下角
		//else if((xb==xmin && ye==ymin) || (xe==xmin && yb==ymin))
		else if((fabs(xb-xmin)<1.0e-12 && fabs(ye-ymin)<1.0e-12) || (fabs(xe-xmin)<1.0e-12 && fabs(yb-ymin)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymin);
			AddBetweenPoint(xmin,ymin,xb,yb);
		}
		// 2-2. 左上角
		//else if((xb==xmin && ye==ymax) || (xe==xmin && yb==ymax))
		else if((fabs(xb-xmin)<1.0e-12 && fabs(ye-ymax)<1.0e-12) || (fabs(xe-xmin)<1.0e-12 && fabs(yb-ymax)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymax);
			AddBetweenPoint(xmin,ymax,xb,yb);
		}
		// 2-3. 右上角
		//else if((yb==ymax && xe==xmax) || (ye==ymax && xb==xmax))
		else if((fabs(yb-ymax)<1.0e-12 && fabs(xe-xmax)<1.0e-12) || (fabs(ye-ymax)<1.0e-12 && fabs(xb-xmax)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymax);
			AddBetweenPoint(xmax,ymax,xb,yb);
		}
		// 2-4. 右下角
		//else if((yb==ymin && xe==xmax) || (ye==ymin && xb==xmax))
		else if((fabs(yb-ymin)<1.0e-12 && fabs(xe-xmax)<1.0e-12) || (fabs(ye-ymin)<1.0e-12 && fabs(xb-xmax)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymin);
			AddBetweenPoint(xmax,ymin,xb,yb);
		}
		// 3. 等值线的起止点在同一条边上
		//else if(xb==xe || yb==ye)
		else if(fabs(xb-xe)<1.0e-12 || fabs(yb-ye)<1.0e-12)
		{
			eTopology = CurveTopology::SameSide;

			//增加(x1,y1)和(x2,y2)之间的若干点
			//不增加(x1,y1) 但增加 (x2,y2)
			AddBetweenPoint(xe,ye,xb,yb);
		}
		///
		/// 按顺时针方向增加点 - end

		return eTopology;
	}


//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
