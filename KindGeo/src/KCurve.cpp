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

	// ��¡
	KCurve* KCurve::Clone()
	{
		return new KCurve(*this);
	}

	// �����Լ��İ󶨾���
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

	//Ϊ����պ϶�������ӵ�(��BuildClosePolygon()��������)
	//������(x1,y1) ������ (x2,y2)
	void KCurve::AddBetweenPoint(double x1,double y1,double x2,double y2)
	{
		int i;
		//-----------------------------
		int k = 50;	//�ȷֱ���
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
	// ת��Ϊ�պ�����
	// ���ݸ����İ�Χ��
	// ����һ���պ�����
	// ���ߺͱ߽�����˹�ϵ��4�����
	// 1. ����ε���ֹ���ڶԱ���
	// 2. ����ε���ֹ�������ڱ���
	// 3. ����ε���ֹ����ͬһ������
	// 4. ������Ǳպϵ�
	// ת��Ϊ�պ�����
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

		// ���ߵ����
		KPoint ptBeg = *this->GetAt(0);
		double xb = ptBeg.X;
		double yb = ptBeg.Y;

		// ���ߵ��յ�
		KPoint ptEnd = *this->GetAt(iPointCount-1);
		double xe = ptEnd.X;
		double ye = ptEnd.Y;

		//4.��ֵ���Ǳպϵ�
		if(this->IsClose())
		{
			eTopology = CurveTopology::Close;
			return eTopology;
		}

		/// ��˳ʱ�뷽�����ӵ� - beg
		///
		// 1.����ε���ֹ���ڶԱ���
		// 1-1.��������,�յ����ұ�
		//if(xb==xmin && xe==xmax)
		if(fabs(xb-xmin)<1.0e-12 && fabs(xe-xmax)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			// ����(x1,y1)��(x2,y2)֮������ɵ�
			// ������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymin);
			AddBetweenPoint(xmax,ymin,xmin,ymin);
			AddBetweenPoint(xmin,ymin,xb,yb);
		}
		// 1-2.������ұ�,�յ������
		//else if(xb==xmax && xe==xmin)
		else if(fabs(xb-xmax)<1.0e-12 && fabs(xe-xmin)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymax);
			AddBetweenPoint(xmin,ymax,xmax,ymax);
			AddBetweenPoint(xmax,ymax,xb,yb);
		}
		// 1-3. ������±�,�յ����ϱ�
		//else if(yb==ymin && ye==ymax)
		else if(fabs(yb-ymin)<1.0e-12 && fabs(ye-ymax)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymax);
			AddBetweenPoint(xmax,ymax,xmax,ymin);
			AddBetweenPoint(xmax,ymin,xb,yb);
		}
		// 1-4. ������ϱ�,�յ����±�
		//else if(yb==ymax && ye==ymin)
		else if(fabs(yb-ymax)<1.0e-12 && fabs(ye-ymin)<1.0e-12)
		{
			eTopology = CurveTopology::OppositeSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymin);
			AddBetweenPoint(xmin,ymin,xmin,ymax);
			AddBetweenPoint(xmin,ymax,xb,yb);
		}
 
		// 2.��ֵ�ߵ���ֹ�������ڱ���
		// 2-1. ���½�
		//else if((xb==xmin && ye==ymin) || (xe==xmin && yb==ymin))
		else if((fabs(xb-xmin)<1.0e-12 && fabs(ye-ymin)<1.0e-12) || (fabs(xe-xmin)<1.0e-12 && fabs(yb-ymin)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymin);
			AddBetweenPoint(xmin,ymin,xb,yb);
		}
		// 2-2. ���Ͻ�
		//else if((xb==xmin && ye==ymax) || (xe==xmin && yb==ymax))
		else if((fabs(xb-xmin)<1.0e-12 && fabs(ye-ymax)<1.0e-12) || (fabs(xe-xmin)<1.0e-12 && fabs(yb-ymax)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmin,ymax);
			AddBetweenPoint(xmin,ymax,xb,yb);
		}
		// 2-3. ���Ͻ�
		//else if((yb==ymax && xe==xmax) || (ye==ymax && xb==xmax))
		else if((fabs(yb-ymax)<1.0e-12 && fabs(xe-xmax)<1.0e-12) || (fabs(ye-ymax)<1.0e-12 && fabs(xb-xmax)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymax);
			AddBetweenPoint(xmax,ymax,xb,yb);
		}
		// 2-4. ���½�
		//else if((yb==ymin && xe==xmax) || (ye==ymin && xb==xmax))
		else if((fabs(yb-ymin)<1.0e-12 && fabs(xe-xmax)<1.0e-12) || (fabs(ye-ymin)<1.0e-12 && fabs(xb-xmax)<1.0e-12))
		{
			eTopology = CurveTopology::AdjacentSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xmax,ymin);
			AddBetweenPoint(xmax,ymin,xb,yb);
		}
		// 3. ��ֵ�ߵ���ֹ����ͬһ������
		//else if(xb==xe || yb==ye)
		else if(fabs(xb-xe)<1.0e-12 || fabs(yb-ye)<1.0e-12)
		{
			eTopology = CurveTopology::SameSide;

			//����(x1,y1)��(x2,y2)֮������ɵ�
			//������(x1,y1) ������ (x2,y2)
			AddBetweenPoint(xe,ye,xb,yb);
		}
		///
		/// ��˳ʱ�뷽�����ӵ� - end

		return eTopology;
	}


//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
