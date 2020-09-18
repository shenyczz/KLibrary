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

	//构造等值线
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

		// 等值线数量
		WORD wContourNums = pDataInfo->wContourNums;

		// 当 wContourNums = 9999 时表示紧随其后的三个数表示
		//    等值线间隔值 等值线最小值 等值线最大值
		//if(wContourNums == 8888 || wContourNums == 9999)
		if(wContourNums == AXIN_FMT_CVID_AUTO || wContourNums == AXIN_FMT_CVID_CONTOUR)
		{
			this->SetTraceValue(pDataInfo->dContourInterval,pDataInfo->dContourMin,pDataInfo->dContourMax);
		}

		// 当 wContourNums = (0,100) 时表示
		// 紧随其后的是 wContourNums 个要求追踪的等值线值数
		if(wContourNums>0 && wContourNums<100)
		{
			this->SetTraceValue( &pDataInfo->dContourValue[0],wContourNums );
		}

		// 等值线值数量
		if(this->GetContourValueCount()<=0)
			return FALSE;

		// 删除全部等值线
		if(!IsEmpty())
		{
			Empty();
		}
		//-----------------------------------------------------
		// 复位追踪标记
		m_QContourTracer.ResetTracerFlag();
		// 设置数据 -> 等值线追踪器
		m_QContourTracer.SetData(pGridData);
		// 设置数据信息 -> 等值线追踪器
		m_QContourTracer.SetDataInfo(pDataInfo);

		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
		if(pProgressParam)
		{
			pProgressParam->SetInfo(_T("追踪等值线"));
		}

		// 要追踪的等值线值数量
		int iContourValueCount = this->GetContourValueCount();
		for(i=0;i<iContourValueCount;i++)
		{
			// 设置追踪值
			double dTraceValue = this->GetContourValue(i);
			m_QContourTracer.SetTraceValue(dTraceValue);

			// 设置输出数据指针
			KContour* pContour = new KContour();
			pContour->SetValue(dTraceValue);
			m_QContourTracer.SetContour(pContour);

			// 追踪等值线
			BOOL bTrace = m_QContourTracer.Tracing();

			// 追踪到的曲线数量
			int iCurveCount = pContour->Count();

			// 追踪出错,释放内存
			if(!bTrace || iCurveCount==0)
			{
				_delete(pContour);
				continue;
			}

			// 添加一组等值线
			m_pContourCollection->Add(pContour);

			// 显示进度
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

		}// 追踪等值线完成

		// 计算邦定矩形
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

		// 把所有等值线转化为闭合的曲线
		// 用于绘制等值线色斑图
		this->ConvertToCloseCurve();

		return TRUE;
	}

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
