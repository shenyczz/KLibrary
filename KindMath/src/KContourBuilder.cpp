#include "StdAfx.h"
#include "KContourBuilder.h"

#include "KMath.h"
#include "KPoint.h"
#include "KCurve.h"
//#include "KContourCollection.h"

namespace ContourBuilder
{

	KContourBuilder::KContourBuilder(void)
		:m_pData(0)
		,m_pDataInfo(0)
		,m_pBoundingBox(0)
		,m_pContourCollection(0)
		,m_pContourCollection_close(0)
	{
		m_pContourCollection = new KContourPtrCollection();
		m_pContourCollection_close = new TPtrCollection<KContour*>();
	}

	KContourBuilder::~KContourBuilder(void)
	{
		_delete(m_pBoundingBox);
		_delete(m_pContourCollection);
		_delete(m_pContourCollection_close);
	}

	void KContourBuilder::Empty()
	{
		if(m_pContourCollection)
			m_pContourCollection->Clear();

		if(m_pContourCollection_close)
			m_pContourCollection_close->Clear();
	}

	BOOL KContourBuilder::IsEmpty()
	{
		if(m_pContourCollection==NULL)
			return TRUE;
		else
			return m_pContourCollection->IsEmpty();
	}

	// �����ֵ��(���������ж���)
	BOOL KContourBuilder::BuildContours()
	{
		//ComputeBoundingBox();
		return FALSE; 
	}

	// �����Χ��
	void KContourBuilder::ComputeBoundingBox()
	{
		if(m_pBoundingBox==NULL)
			m_pBoundingBox = new KExtents();

		KDataInfo* pDataInfo = (KDataInfo*)this->GetDataInfo();

		m_pBoundingBox->Xmin = pDataInfo->xMin;
		m_pBoundingBox->Ymin = pDataInfo->yMin;

		m_pBoundingBox->Xmax = pDataInfo->xMax;
		m_pBoundingBox->Ymax = pDataInfo->yMax;
	}

	// �����е�ֵ��ת��Ϊ�պϵ�����
	// ���ڻ��Ƶ�ֵ��ɫ��ͼ
	// ��ֵ�ߺͱ߽繹�ɵĶ���η�4�����
	// 1. ��ֵ�ߵ���ֹ���ڶԱ���
	// 2. ��ֵ�ߵ���ֹ�������ڱ���
	// 3. ��ֵ�ߵ���ֹ����ͬһ������
	// 4. ��ֵ���Ǳպϵĵ�ֵ��
	void KContourBuilder::ConvertToCloseCurve()
	{
		int i,j,k;

		// ԭʼ��ֵ�߼�
		KContourPtrCollection* pContourCollection = m_pContourCollection;
		int iContourCount = pContourCollection->Count();
		if(iContourCount==0)
			return;

		// �µĵ�ֵ�߼�(�պϵ�ֵ�߼�)
		KContourPtrCollection* pContourCollectionNew = m_pContourCollection_close;
		pContourCollectionNew->Clear();

		// ����5���ֵ��
		// 0. ���ĵ�ֵ����(���о���)
		// 1. ��ֵ�ߵ���ֹ���ڶԱ���
		// 2. ��ֵ�ߵ���ֹ�������ڱ���
		// 3. ��ֵ�ߵ���ֹ����ͬһ������
		// 4. ��ֵ���Ǳպϵĵ�ֵ��
		for(i=0;i<5;i++)
		{
			pContourCollectionNew->Add(new KContour());
		}

		/// �µĵ�ֵ�߼��еĵ�ֵ����ԭʼ��ֵ��ת������
		/// ȫ���Ǳպϵ�����
		/// �Ապ����ߵ������С����

		KDataInfo* pDataInfo = (KDataInfo*)this->GetDataInfo();
		double xmin = pDataInfo->xMin;
		double ymin = pDataInfo->yMin;
		//double xmax = pDataInfo->xMin + pDataInfo->xInterval * (pDataInfo->xNum -1);
		//double ymax = pDataInfo->yMin + pDataInfo->yInterval * (pDataInfo->yNum -1);
		double xmax = pDataInfo->xMax;
		double ymax = pDataInfo->yMax;

		//-------------------------------------------------
		// ����һ�����ıպ����� - Beg
		//
		//A------------->
		//|             |
		//|             |
		//|             |
		//|             |
		//O<------------V
		KCurve* pCurveMax = new KCurve();
		pCurveMax->Add(new KPoint(xmin,ymin));				// ���½�
		pCurveMax->AddBetweenPoint(xmin,ymin,xmin,ymax);	// ���½� -> ���Ͻ�
		pCurveMax->AddBetweenPoint(xmin,ymax,xmax,ymax);	// ���Ͻ� -> ���Ͻ�
		pCurveMax->AddBetweenPoint(xmax,ymax,xmax,ymin);	// ���Ͻ� -> ���½�
		pCurveMax->AddBetweenPoint(xmax,ymin,xmin,ymin);	// ���½� -> ���½�

		pCurveMax->ComputrArea();	// ��������
		pCurveMax->SetValue(pDataInfo->vAvg);	// ����Ϊƽ��ֵ
		pCurveMax->SetExtents(KExtents(xmin,ymin,xmax,ymax));
		//pContourCollectionNew->GetAt(0)->Add(pCurveMax);
		((KContour*)(pContourCollectionNew->GetAt(0)))->Add(pCurveMax);
		//
		// ����һ�����ıպ����� - End
		//-------------------------------------------------

		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
		if(pProgressParam)
		{
			pProgressParam->SetInfo(_T("ת����ֵ��"));
		}

		// 
		KCurve* pCurveNew = NULL;
		KContour* pContourNew = NULL;

		for(i=0;i<iContourCount;i++)
		{
			KContour* pContour = (KContour*)pContourCollection->GetAt(i);
			int iCurveCount = pContour->Count();
			for(j=0;j<iCurveCount;j++)
			{
				// �µ�����
				pCurveNew = new KCurve();
				// ԭ������
				KCurve* pCurve = pContour->GetCurve(j);

				// ���ñ��
				pCurveNew->SetFlag(pCurve->GetFlag());
				// ��������ֵ
				pCurveNew->SetValue(pCurve->GetValue());
				// �������߰󶨾���
				pCurveNew->SetExtents(pCurve->GetExtents());

				int iPointCount = pCurve->Count();
				for(k=0;k<iPointCount;k++)
				{
					KPoint point = *pCurve->GetAt(k);
					pCurveNew->Add(new KPoint(point.X,point.Y));
				}

				// ���ߺͰ�Χ�е����˹�ϵ
				KCurve::CurveTopology eTopology = pCurveNew->BuildCloseCurve(xmin,ymin,xmax,ymax);
				if(eTopology == KCurve::CurveTopology::Unkonw)
				{
					_delete(pCurveNew);
					continue;
				}

				// �����������
				pCurveNew->ComputrArea();

				// ������������Լ��������Χ�е����˹�ϵΪ��ֵ����ӱպ�����(����)
				pContourNew = (KContour*)pContourCollectionNew->GetAt(eTopology);	// 1~4��
				pContourNew->AddCurveByArea(pCurveNew);

			}// for(j)

			// ��ʾ����
			if(this->CallbackFunc())
			{
				if(this->CallbackParam())
				{
					ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
					pProgressParam->lTotalNums = iContourCount;
					pProgressParam->lCurrentPos = i;
				}

				this->CallbackFunc()(this->CallbackParam());
			}

		}// for(i)

		//-------------------------------------------------
		// �����������ıպ����ߵ�ֵ
		double dValue1 = -998;
		double dValue2 = -999;
		KCurve* pCurveTemp = NULL;

		KContour contour;
		// �����е�ֵ�߰������������
		for(i=1;i<=4;i++)
		{
			KContour* pContourTemp = (KContour*)pContourCollectionNew->GetAt(i);
			int iCurveCount = pContourTemp->Count();
			for(j=0;j<iCurveCount;j++)
			{
				KCurve* pCurveTemp = pContourTemp->GetCurve(j);
				contour.AddCurveByArea((KCurve*)pCurveTemp->Clone());
			}
		}

		//============================test
		//double v[37];
		//double a[37];
		//for(int iii=0;iii<37;iii++)
		//{
		//	v[iii] = ((KCurve*)contour.GetAt(iii))->GetValue();
		//	a[iii] = ((KCurve*)contour.GetAt(iii))->GetArea();
		//}
		//============================

		// ����������ֵ
		KCurve* pCurveMaxArea = (KCurve*)contour.GetAt(0);
		dValue1 = pCurveMaxArea->GetValue();
		int iCount = contour.Count();
		for(i=1;i<iCount;i++)
		{
			DWORD dwFlag = ((KCurve*)contour.GetAt(i))->GetFlag();
			pCurveTemp = this->GetCurve(m_pContourCollection,dwFlag);
			if(pCurveTemp)
			{
				KPoint* pPoint = NULL;
				if(pCurveTemp->IsClose())
				{
					pPoint = pCurveTemp->GetAt(0);
				}
				else
				{
					int pointCount = pCurveTemp->Count();
					pPoint = pCurveTemp->GetAt((1+pointCount)/2);
				}

				if(!pPoint)
					continue;

				if(!pCurveMaxArea->PtInside(*pPoint))
				{
					dValue2 = pCurveTemp->GetValue();
					break;
				}

			}// if(pCurveTemp)

		}// for(i=1;i<iCount;i++)

		double dValueMax = 0.5 * (dValue1 + dValue2);
		pCurveMax->SetValue(0.5 * (dValue1 + dValue2));

		return;

		//m_pContourCollection
		//-------------------------------------------------
		// �����������ıպ����ߵ�ֵ
		//double dValue1 = -998;
		//double dValue2 = -999;
		int iCurveCount = 0;

		// 1.�ȴӵ�ֵ�ߵ���ֹ���ڶԱ��ϵĵ�ֵ�����ҵ��������
		KContour* KContourOppositeSide = (KContour*)pContourCollectionNew->GetAt(KCurve::CurveTopology::OppositeSide);
		iCurveCount = KContourOppositeSide->Count();
		if(iCurveCount>0)
		{
			// ��Ϊ��ӱպ������Ǹ�����������
			pCurveTemp = KContourOppositeSide->GetCurve(0);
			dValue1 = pCurveTemp->GetValue();
		}

		// 0. ���ĵ�ֵ����(����һ���߽�����)
		// 1. ��ֵ�ߵ���ֹ���ڶԱ���
		// 2. ��ֵ�ߵ���ֹ�������ڱ���
		// 3. ��ֵ�ߵ���ֹ����ͬһ������
		// 4. ��ֵ���Ǳպϵĵ�ֵ��

		// 2.�ӵ�ֵ�ߵ���ֹ�������ڱ��ϵĵ�ֵ�����ҵ��������
		//   ���
		double dAreaMax(0);
		// ����������
		for(i=2;i<=4;i++)
		{
			KContour* pContourTemp = (KContour*)pContourCollectionNew->GetAt(i);
			iCurveCount = pContourTemp->Count();
			for(j=0;j<iCurveCount;j++)
			{
				pCurveTemp = pContourTemp->GetCurve(j);
				double dArea = pCurveTemp->GetArea();
				dAreaMax = max(dAreaMax,dArea);
			}
		}
		// ����������������
		for(i=2;i<=4;i++)
		{
			KContour* KContourTemp = (KContour*)pContourCollectionNew->GetAt(i);
			iCurveCount = KContourTemp->Count();
			for(j=0;j<iCurveCount;j++)
			{
				pCurveTemp = KContourTemp->GetCurve(j);
				double dArea = pCurveTemp->GetArea();
				if(fabs(dArea-dAreaMax)<EPS)
				{
					dValue2 = pCurveTemp->GetValue();
					break;
				}
			}
		}

		// ��ֵ�߼��
		double dContourInterval = pDataInfo->dContourInterval;
		if(KMath::FloatEqual(dValue1,-998.0) && KMath::FloatEqual(dValue2,-999.0))
		{
			pCurveMax->SetValue(pDataInfo->dContourMin);
		}
		else if(!KMath::FloatEqual(dValue1,-998.0) && !KMath::FloatEqual(dValue2,-999.0))
		{
			pCurveMax->SetValue( (dValue1+dValue2) / 2 );			// ����Ϊƽ��ֵ
		}
		else if(!KMath::FloatEqual(dValue1,-998.0))
		{
			pCurveMax->SetValue( dValue1+dContourInterval / 2 );	// ����Ϊƽ��ֵ
		}
		else if(!KMath::FloatEqual(dValue2,-999.0))
		{
			pCurveMax->SetValue( dValue2-dContourInterval / 2 );	// ����Ϊƽ��ֵ
		}
		//-------------------------------------------------

		return;
	}

	// ȡ��ָ����ǵ�����
	KCurve* KContourBuilder::GetCurve(KContourPtrCollection* pContourPtrCollection, DWORD dwFlag)
	{
		int i,j;

		int nContour = pContourPtrCollection->Count();
		for(i=0;i<nContour;i++)
		{
			KContour* pContour = pContourPtrCollection->GetAt(i);
			int nCurve = pContour->Count();
			for(j=0;j<nCurve;j++)
			{
				KCurve* pCurve = pContour->GetAt(j);
				if(pCurve->GetFlag() == dwFlag)
				{
					return pCurve;
				}
			}
		}

		return NULL;
	}

	///========================================================
	///��ֵ��׷��ֵ���� - beg
	///-----------------------
	// ����Ҫ׷�ٵĵ�ֵ��ֵ
	// ��    ��: SetTraceValue
	// ��    ��: ����Ҫ׷�ٵĵ�ֵ��ֵ
	// ��    ��: public
	// ��    ��:
	//			[i] va - ��ֵ��ֵ����
	//			[i] n - ��ֵ��ֵ�����С
	//
	// ��    ��: 
	// ע    ��: 
	BOOL KContourBuilder::SetTraceValue(double* va,int n)
	{
		// ����ֵ��ֵ����
		if(va==NULL)
			return FALSE;

		RemoveAllContourValue();

		for(int i=0;i<n;i++)
		{
			AddContourValue( *(va+i) );
		}

		return TRUE;
	}
	// ����Ҫ׷�ٵĵ�ֵ��ֵ
	// ��    ��: SetTraceValue
	// ��    ��: ����Ҫ׷�ٵĵ�ֵ��ֵ
	// ��    ��: public
	// ��    ��:
	//			[i] dInterval - ��ֵ��ֵ���
	//			[i] dValueBeg - ��ʼ��ֵ��ֵ
	//			[i] dValueEnd - ��ֹ��ֵ��ֵ
	//
	// ��    ��: 
	// ע    ��: 
	BOOL KContourBuilder::SetTraceValue(double dInterval, double dValueBeg, double dValueEnd)
	{
		return GenerateContourValue(dInterval,dValueBeg,dValueEnd);
	}
	// ����һ����ֵ��ֵ
	void KContourBuilder::AddContourValue(double dValue)
	{
		m_dValueVector.push_back(dValue);
	}
	// �Ƴ�һ����ֵ��ֵ
	void KContourBuilder::RemoveContourValue(double dValue)
	{
		vector<double>::iterator it;
		vector<double>::iterator itbeg = m_dValueVector.begin();
		vector<double>::iterator itend = m_dValueVector.end();
		for(it=itbeg;it!=itend;it++)
		{
			if( fabs(dValue-(*it))<1.0e-12 )
			{
				m_dValueVector.erase(it);
				break;
			}
		}
	}
	// �Ƴ�ȫ����ֵ��ֵ
	void KContourBuilder::RemoveAllContourValue()
	{
		m_dValueVector.clear();
	}
	// ȡ�õ�ֵ��ֵ
	double KContourBuilder::GetContourValue(int index)
	{
		if( index<0 || index>=(int)m_dValueVector.size() )
			return KIND_INVALID_VALUE;
		else
			return m_dValueVector.at(index);
	}
	// ȡ�õ�ֵ��ֵ����
	int KContourBuilder::GetContourValueCount()
	{
		return m_dValueVector.size();
	}
	// ����Ҫ׷�ٵĵ�ֵ��ֵ
	BOOL KContourBuilder::GenerateContourValue(double dInterval, double dValueBeg, double dValueEnd)
	{
		// ��ֵ�߼�����õ�����
		if(fabs(dInterval)<1.0e-12)
		{
			//AfxMessageBox("��ֵ�߼��Ϊ�� -- CSfxContourManager::GenerateValue(...)");
			return FALSE;
		}
		// ��ֵ�߼��������
		// ����ʼ��ֵ��ֵ����С����ֹ��ֵ��ֵ --> ( (dInterval > 0) && (dValueMin < dValueMax) )
		else if(dInterval > 0)
		{
			if(dValueBeg > dValueEnd)
			{
				double dTemp = dValueBeg;
				dValueBeg = dValueEnd;
				dValueEnd = dValueBeg;
			}
		}
		// ��ֵ�߼��С����
		// ����ʼ��ֵ��ֵ���������ֹ��ֵ��ֵ --> ( (dInterval < 0) && (dValueMin > dValueMax) )
		else if(dInterval < 0)
		{
			if(dValueBeg < dValueEnd)
			{
				double dTemp = dValueBeg;
				dValueBeg = dValueEnd;
				dValueEnd = dValueBeg;
			}
		}
		else
		{
			//AfxMessageBox("��ֵ�߼��,��ʼ��ֵ��ֵ,��ֹ��ֵ��ֵ���ô��� -- CSfxContourManager::GenerateValue(...)\n");
			return FALSE;
		}

		// �Ƴ�ȫ����ֵ��ֵ
		RemoveAllContourValue();

		// ������С��0,�任֮
		if(dInterval<0)
		{
			double dTemp = dValueBeg;
			dValueBeg = dValueEnd;
			dValueEnd = dTemp;
			dInterval = (double)fabs(dInterval);
		}

		double val = dValueBeg;
		for (val=dValueBeg; val <= dValueEnd; val+=dInterval)
		{
			AddContourValue(val);
		}

		return TRUE;
	}
	///-----------------------
	///��ֵ��׷��ֵ���� - end
	///========================================================

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
