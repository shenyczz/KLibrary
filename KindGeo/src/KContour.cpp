#include "StdAfx.h"
#include "KContour.h"

namespace Geometries
{

	KContour::KContour(void)
		:KGeometry(), TPtrCollection<KCurve*>()
		,m_dValue(0)
	{
		SetType(GeometryType_Contour);
	}

	KContour::KContour(const KContour& contour)
		:KGeometry(contour), TPtrCollection<KCurve*>(contour)
		,m_dValue(contour.m_dValue)
	{
	}

	KContour::~KContour(void)
	{
	}

	void KContour::Offset(double dx,double dy)
	{
	}

	void KContour::GetMinCoordinate(double& x, double& y)
	{
	}

	void KContour::GetMaxCoordinate(double& x, double& y)
	{
	}

	// ��ָ������ǰ��������
	void KContour::InsertBefor(int index, KCurve* pCurve)
	{
		int loop = 0;

		iterator itr;
		iterator itrBeg = this->begin();
		iterator itrEnd = this->end();
		for(itr=itrBeg;itr!=itrEnd;itr++)
		{
			if(index == loop)
				break; 

			loop++;
		}

		this->insert(itr,pCurve);
	}

	// �������Ϊ��ֵ����ӱպ�����(����)
	void KContour::AddCurveByArea(KCurve* pCurve)
	{
		int iCurveCount = this->Count();
		if(iCurveCount==0)
		{
			this->Add(pCurve);
			return;
		}

		int index = -1;
		for(int i=0;i<iCurveCount;i++)
		{
			double curArea = this->GetAt(i)->Area();
			if(pCurve->Area() > curArea)
			{
				index = i;
				break;
			}
		}

		if(index>=0)
		{
			this->InsertBefor(index,pCurve);
		}
		else
		{
			this->Add(pCurve);
		}

		return;
	}

}
