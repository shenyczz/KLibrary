#include "StdAfx.h"
#include "KContourTracer.h"

//#include "KCurveCollection.h"

namespace ContourBuilder
{

	KContourTracer::KContourTracer(void)
		:m_pData(NULL)
		,m_pDataInfo(NULL)
		,m_dTraceValue(0)
		,m_pContour(NULL)
		,m_dwTracerFlag(GetCurrentTime())
	{
	}

	KContourTracer::~KContourTracer(void)
	{
		// ע�ⲻҪ�����ⲿ�����ָ��
	}
	// ����һ����ֵ��
	void KContourTracer::AddCurve(KCurve* pCurve)
	{
		if(pCurve == NULL)
			return;

		if(m_pContour==NULL)
			return;

		m_pContour->Add(pCurve);
	}

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------

