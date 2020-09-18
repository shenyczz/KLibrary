#include "StdAfx.h"
#include "KPolyline.h"

namespace Geometries
{

	KPolyline::KPolyline(void)
		: KGeometry(), TPtrCollection<KPoint*>()
	{
		SetType(GeometryType_Polyline);
	}

	KPolyline::KPolyline(const KPolyline& polyline)
		: KGeometry(polyline)
	{
		int iCount = polyline.Count();
		for(int i=0;i<iCount;i++)
		{
			this->Add(polyline.GetAt(i)->Clone());
		}
	}

	KPolyline::~KPolyline(void)
	{
	}

	// ��¡(��Ҫ�����߻����ڴ�)
	KPolyline* KPolyline::Clone()
	{
		return new KPolyline(*this);
		/*
		KPolyline* pPolyline = new KPolyline();

		int iCount = this->Count();
		for(int i=0;i<iCount;i++)
		{
			pPolyline->Add(this->GetAt(i)->Clone());
		}

		return pPolyline;
		*/
	}

	void KPolyline::Offset(double dx,double dy)
	{
		int iPointCount = this->Count();
		for(int i=0;i<iPointCount;i++)
		{
			this->GetAt(i)->Offset(dx,dy);
		}
	}

	void KPolyline::GetMinCoordinate(double& x, double& y)
	{
		int iPointCount = this->Count();
		for(int i=0;i<iPointCount;i++)
		{
			double xtmp, ytmp;
			this->GetAt(i)->GetMinCoordinate(xtmp, ytmp);
			if(i==0)
			{
				x = xtmp; y = ytmp;
			}
			else
			{
				x = min(x,xtmp); y = min(y,ytmp);
			}
		}
	}

	void KPolyline::GetMaxCoordinate(double& x, double& y)
	{
		int iPointCount = this->Count();
		for(int i=0;i<iPointCount;i++)
		{
			double xtmp, ytmp;
			this->GetAt(i)->GetMinCoordinate(xtmp, ytmp);
			if(i==0)
			{
				x = xtmp; y = ytmp;
			}
			else
			{
				x = max(x,xtmp); y = max(y,ytmp);
			}
		}
	}

	// �Ƿ�պ�
	bool KPolyline::IsClose()
	{
		int iCount = this->Count();
		if(iCount<3)
			return false;

		KPoint pointBeg = *this->GetAt(0);
		KPoint pointEnd = *this->GetAt(iCount-1);

		return (pointBeg == pointEnd);
	}

	// ����
	double KPolyline::Length()
	{
		double length = 0;

		int iCount = this->Count();
		for(int i=0;i<iCount-1;i++)
		{
			KPoint point1 = *this->GetAt(i);
			KPoint point2 = *this->GetAt(i+1);

			length += point1.Distance(point2);
		}

		return length;
	}

	// �����������
	double KPolyline::Area()
	{
		if(!this->IsClose())
			return 0;

		int iPointCount = this->Count();
		if(iPointCount<3)
			return 0;

		//�������
		double dArea(0);
		KPoint point;
		double x1,y1,x2,y2;
		for(int i=0;i<iPointCount-1;i++)
		{
			point = *this->GetAt(i);
			x1 = point.X;
			y1 = point.Y;

			point = *this->GetAt(i+1);
			x2 = point.X;
			y2 = point.Y;

			dArea += (x1*y2 - x2*y1);
		}

		//���һ��͵�һ��
		point = *this->GetAt(iPointCount-1);
		x1 = point.X;
		y1 = point.Y;

		point = *this->GetAt(0);
		x2 = point.X;
		y2 = point.Y;

		dArea += (x1*y2 - x2*y1);

		return fabs(dArea);
	}

	// ���Ƿ����ڲ�
	// ������պϣ�����������յ��γɱպ�����
	bool KPolyline::PtInside(KPoint point)
	{
		KPolyline* pPolyline = this->Clone();

		// ������պ�������ʹ֮��ɱպϵ�
		if(!pPolyline->IsClose())
		{
			pPolyline->Add(pPolyline->GetAt(0)->Clone());
		}

		int iPointCount = this->Count();
		if(iPointCount<3)
		{
			_delete(pPolyline);
			return false;
		}

		// ��������
		// �Ŵ�ϵ��
		int iScale = 10000;
		POINT* lppt = new POINT[iPointCount]; 
		for(int i=0;i<iPointCount;i++)
		{
			KPoint ptmp = *((KPoint*)pPolyline->GetAt(i));
			lppt[i].x = (LONG)(ptmp.X * iScale);
			lppt[i].y = (LONG)(ptmp.Y * iScale);
		}

		HRGN hRgn = ::CreatePolygonRgn(lppt,iPointCount,WINDING);
		_deletea(lppt);
		_delete(pPolyline);

		if(hRgn==NULL)
			return false;

		bool bPtInside = ::PtInRegion(hRgn,(LONG)(point.X*iScale),(LONG)(point.Y*iScale)) ? true : false;

		::DeleteObject(hRgn);
		hRgn = NULL;

		return bPtInside;
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
