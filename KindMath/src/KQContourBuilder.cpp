#include "StdAfx.h"
#include "KQContourBuilder.h"

namespace ContourBuilder
{

	KQContourBuilder::KQContourBuilder(void)
	{
	}

	KQContourBuilder::~KQContourBuilder(void)
	{
	}

	//�����ֵ��
	BOOL KQContourBuilder::BuildContours()
	{
		int i;

		//-----------------------------------------------------
		double* pGridData = (double*)this->GetData();
		if(pGridData == NULL)
			return FALSE;

		KDataInfo* pDataInfo = (KDataInfo*)this->GetDataInfo();
		if(pDataInfo==NULL)
			return FALSE;

		// ��ֵ������
		WORD wContourNums = pDataInfo->wContourNums;

		// �� wContourNums = 9999 ʱ��ʾ����������������ʾ
		//    ��ֵ�߼��ֵ ��ֵ����Сֵ ��ֵ�����ֵ
		//if(wContourNums == 8888 || wContourNums == 9999)
		if(wContourNums == AXIN_FMT_CVID_AUTO || wContourNums == AXIN_FMT_CVID_CONTOUR)
		{
			this->SetTraceValue(pDataInfo->dContourInterval,pDataInfo->dContourMin,pDataInfo->dContourMax);
		}

		// �� wContourNums = (0,100) ʱ��ʾ
		// ���������� wContourNums ��Ҫ��׷�ٵĵ�ֵ��ֵ��
		if(wContourNums>0 && wContourNums<100)
		{
			this->SetTraceValue( &pDataInfo->dContourValue[0],wContourNums );
		}

		// ��ֵ��ֵ����
		if(this->GetContourValueCount()<=0)
			return FALSE;

		// ɾ��ȫ����ֵ��
		if(!IsEmpty())
		{
			Empty();
		}
		//-----------------------------------------------------
		// ��λ׷�ٱ��
		m_QContourTracer.ResetTracerFlag();
		// �������� -> ��ֵ��׷����
		m_QContourTracer.SetData(pGridData);
		// ����������Ϣ -> ��ֵ��׷����
		m_QContourTracer.SetDataInfo(pDataInfo);

		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
		if(pProgressParam)
		{
			pProgressParam->SetInfo(_T("׷�ٵ�ֵ��"));
		}

		// Ҫ׷�ٵĵ�ֵ��ֵ����
		int iContourValueCount = this->GetContourValueCount();
		for(i=0;i<iContourValueCount;i++)
		{
			// ����׷��ֵ
			double dTraceValue = this->GetContourValue(i);
			m_QContourTracer.SetTraceValue(dTraceValue);

			// �����������ָ��
			KContour* pContour = new KContour();
			pContour->SetValue(dTraceValue);
			m_QContourTracer.SetContour(pContour);

			// ׷�ٵ�ֵ��
			BOOL bTrace = m_QContourTracer.Tracing();

			// ׷�ٵ�����������
			int iCurveCount = pContour->Count();

			// ׷�ٳ���,�ͷ��ڴ�
			if(!bTrace || iCurveCount==0)
			{
				_delete(pContour);
				continue;
			}

			// ���һ���ֵ��
			m_pContourCollection->Add(pContour);

			// ��ʾ����
			if(this->CallbackFunc())
			{
				if(this->CallbackParam())
				{
					ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
					pProgressParam->lTotalNums = iContourValueCount;
					pProgressParam->lCurrentPos = i;
				}

				this->CallbackFunc()(this->CallbackParam());
			}

		}// ׷�ٵ�ֵ�����

		// ��������
		int iContourCount = m_pContourCollection->Count();
		for(i=0;i<iContourCount;i++)
		{
			KContour* pContour = (KContour*)m_pContourCollection->GetAt(i);
			int iCurveCount = pContour->Count();
			for(int j=0;j<iCurveCount;j++)
			{
				KCurve* pCurve = pContour->GetCurve(j);
				pCurve->ComputeBoundingBox();
#ifdef _DEBUG
				//KBox box = pCurve->GetBoundingBox();
				KExtents& box = (KExtents&)pCurve->GetExtents();
#endif
			}
		}

		// �����е�ֵ��ת��Ϊ�պϵ�����
		// ���ڻ��Ƶ�ֵ��ɫ��ͼ
		this->ConvertToCloseCurve();

		return TRUE;
	}

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
