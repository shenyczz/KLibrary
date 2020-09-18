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

	// 构造等值线(在派生类中定义)
	BOOL KContourBuilder::BuildContours()
	{
		//ComputeBoundingBox();
		return FALSE; 
	}

	// 计算包围盒
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

	// 把所有等值线转化为闭合的曲线
	// 用于绘制等值线色斑图
	// 等值线和边界构成的多边形分4种情况
	// 1. 等值线的起止点在对边上
	// 2. 等值线的起止点在相邻边上
	// 3. 等值线的起止点在同一条边上
	// 4. 等值线是闭合的等值线
	void KContourBuilder::ConvertToCloseCurve()
	{
		int i,j,k;

		// 原始等值线集
		KContourPtrCollection* pContourCollection = m_pContourCollection;
		int iContourCount = pContourCollection->Count();
		if(iContourCount==0)
			return;

		// 新的等值线集(闭合等值线集)
		KContourPtrCollection* pContourCollectionNew = m_pContourCollection_close;
		pContourCollectionNew->Clear();

		// 生成5组等值线
		// 0. 最大的等值线组(剪切矩形)
		// 1. 等值线的起止点在对边上
		// 2. 等值线的起止点在相邻边上
		// 3. 等值线的起止点在同一条边上
		// 4. 等值线是闭合的等值线
		for(i=0;i<5;i++)
		{
			pContourCollectionNew->Add(new KContour());
		}

		/// 新的等值线集中的等值线由原始等值线转换而来
		/// 全部是闭合的曲线
		/// 以闭合曲线的面积大小排序

		KDataInfo* pDataInfo = (KDataInfo*)this->GetDataInfo();
		double xmin = pDataInfo->xMin;
		double ymin = pDataInfo->yMin;
		//double xmax = pDataInfo->xMin + pDataInfo->xInterval * (pDataInfo->xNum -1);
		//double ymax = pDataInfo->yMin + pDataInfo->yInterval * (pDataInfo->yNum -1);
		double xmax = pDataInfo->xMax;
		double ymax = pDataInfo->yMax;

		//-------------------------------------------------
		// 构造一个最大的闭合曲线 - Beg
		//
		//A------------->
		//|             |
		//|             |
		//|             |
		//|             |
		//O<------------V
		KCurve* pCurveMax = new KCurve();
		pCurveMax->Add(new KPoint(xmin,ymin));				// 左下角
		pCurveMax->AddBetweenPoint(xmin,ymin,xmin,ymax);	// 左下角 -> 左上角
		pCurveMax->AddBetweenPoint(xmin,ymax,xmax,ymax);	// 左上角 -> 右上角
		pCurveMax->AddBetweenPoint(xmax,ymax,xmax,ymin);	// 右上角 -> 右下角
		pCurveMax->AddBetweenPoint(xmax,ymin,xmin,ymin);	// 右下角 -> 左下角

		pCurveMax->ComputrArea();	// 多边形面积
		pCurveMax->SetValue(pDataInfo->vAvg);	// 设置为平均值
		pCurveMax->SetExtents(KExtents(xmin,ymin,xmax,ymax));
		//pContourCollectionNew->GetAt(0)->Add(pCurveMax);
		((KContour*)(pContourCollectionNew->GetAt(0)))->Add(pCurveMax);
		//
		// 构造一个最大的闭合曲线 - End
		//-------------------------------------------------

		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
		if(pProgressParam)
		{
			pProgressParam->SetInfo(_T("转换等值线"));
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
				// 新的曲线
				pCurveNew = new KCurve();
				// 原来曲线
				KCurve* pCurve = pContour->GetCurve(j);

				// 设置标记
				pCurveNew->SetFlag(pCurve->GetFlag());
				// 设置曲线值
				pCurveNew->SetValue(pCurve->GetValue());
				// 设置曲线绑定矩形
				pCurveNew->SetExtents(pCurve->GetExtents());

				int iPointCount = pCurve->Count();
				for(k=0;k<iPointCount;k++)
				{
					KPoint point = *pCurve->GetAt(k);
					pCurveNew->Add(new KPoint(point.X,point.Y));
				}

				// 曲线和包围盒的拓扑关系
				KCurve::CurveTopology eTopology = pCurveNew->BuildCloseCurve(xmin,ymin,xmax,ymax);
				if(eTopology == KCurve::CurveTopology::Unkonw)
				{
					_delete(pCurveNew);
					continue;
				}

				// 计算多边形面积
				pCurveNew->ComputrArea();

				// 根据曲线面积以及曲线与包围盒的拓扑关系为等值线添加闭合曲线(降序)
				pContourNew = (KContour*)pContourCollectionNew->GetAt(eTopology);	// 1~4组
				pContourNew->AddCurveByArea(pCurveNew);

			}// for(j)

			// 显示进度
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
		// 重新设置最大的闭合曲线的值
		double dValue1 = -998;
		double dValue2 = -999;
		KCurve* pCurveTemp = NULL;

		KContour contour;
		// 把所有等值线按面积降序排列
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

		// 最大面积曲线值
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
		// 重新设置最大的闭合曲线的值
		//double dValue1 = -998;
		//double dValue2 = -999;
		int iCurveCount = 0;

		// 1.先从等值线的起止点在对边上的等值线中找到面积最大的
		KContour* KContourOppositeSide = (KContour*)pContourCollectionNew->GetAt(KCurve::CurveTopology::OppositeSide);
		iCurveCount = KContourOppositeSide->Count();
		if(iCurveCount>0)
		{
			// 因为添加闭合曲线是根据面积降序的
			pCurveTemp = KContourOppositeSide->GetCurve(0);
			dValue1 = pCurveTemp->GetValue();
		}

		// 0. 最大的等值线组(包含一条边界曲线)
		// 1. 等值线的起止点在对边上
		// 2. 等值线的起止点在相邻边上
		// 3. 等值线的起止点在同一条边上
		// 4. 等值线是闭合的等值线

		// 2.从等值线的起止点在相邻边上的等值线中找到面积最大的
		//   这个
		double dAreaMax(0);
		// 查找最大面积
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
		// 查找最大面积的曲线
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

		// 等值线间隔
		double dContourInterval = pDataInfo->dContourInterval;
		if(KMath::FloatEqual(dValue1,-998.0) && KMath::FloatEqual(dValue2,-999.0))
		{
			pCurveMax->SetValue(pDataInfo->dContourMin);
		}
		else if(!KMath::FloatEqual(dValue1,-998.0) && !KMath::FloatEqual(dValue2,-999.0))
		{
			pCurveMax->SetValue( (dValue1+dValue2) / 2 );			// 设置为平均值
		}
		else if(!KMath::FloatEqual(dValue1,-998.0))
		{
			pCurveMax->SetValue( dValue1+dContourInterval / 2 );	// 设置为平均值
		}
		else if(!KMath::FloatEqual(dValue2,-999.0))
		{
			pCurveMax->SetValue( dValue2-dContourInterval / 2 );	// 设置为平均值
		}
		//-------------------------------------------------

		return;
	}

	// 取得指定标记的曲线
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
	///等值线追踪值管理 - beg
	///-----------------------
	// 设置要追踪的等值线值
	// 名    称: SetTraceValue
	// 功    能: 设置要追踪的等值线值
	// 访    问: public
	// 参    数:
	//			[i] va - 等值线值数组
	//			[i] n - 等值线值数组大小
	//
	// 返    回: 
	// 注    释: 
	BOOL KContourBuilder::SetTraceValue(double* va,int n)
	{
		// 检测等值线值数组
		if(va==NULL)
			return FALSE;

		RemoveAllContourValue();

		for(int i=0;i<n;i++)
		{
			AddContourValue( *(va+i) );
		}

		return TRUE;
	}
	// 设置要追踪的等值线值
	// 名    称: SetTraceValue
	// 功    能: 设置要追踪的等值线值
	// 访    问: public
	// 参    数:
	//			[i] dInterval - 等值线值间隔
	//			[i] dValueBeg - 起始等值线值
	//			[i] dValueEnd - 终止等值线值
	//
	// 返    回: 
	// 注    释: 
	BOOL KContourBuilder::SetTraceValue(double dInterval, double dValueBeg, double dValueEnd)
	{
		return GenerateContourValue(dInterval,dValueBeg,dValueEnd);
	}
	// 增加一个等值线值
	void KContourBuilder::AddContourValue(double dValue)
	{
		m_dValueVector.push_back(dValue);
	}
	// 移除一个等值线值
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
	// 移除全部等值线值
	void KContourBuilder::RemoveAllContourValue()
	{
		m_dValueVector.clear();
	}
	// 取得等值线值
	double KContourBuilder::GetContourValue(int index)
	{
		if( index<0 || index>=(int)m_dValueVector.size() )
			return KIND_INVALID_VALUE;
		else
			return m_dValueVector.at(index);
	}
	// 取得等值线值数量
	int KContourBuilder::GetContourValueCount()
	{
		return m_dValueVector.size();
	}
	// 生成要追踪的等值线值
	BOOL KContourBuilder::GenerateContourValue(double dInterval, double dValueBeg, double dValueEnd)
	{
		// 等值线间隔不得等于零
		if(fabs(dInterval)<1.0e-12)
		{
			//AfxMessageBox("等值线间隔为零 -- CSfxContourManager::GenerateValue(...)");
			return FALSE;
		}
		// 等值线间隔大于零
		// 则起始等值线值必须小于终止等值线值 --> ( (dInterval > 0) && (dValueMin < dValueMax) )
		else if(dInterval > 0)
		{
			if(dValueBeg > dValueEnd)
			{
				double dTemp = dValueBeg;
				dValueBeg = dValueEnd;
				dValueEnd = dValueBeg;
			}
		}
		// 等值线间隔小于零
		// 则起始等值线值必须大于终止等值线值 --> ( (dInterval < 0) && (dValueMin > dValueMax) )
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
			//AfxMessageBox("等值线间隔,起始等值线值,终止等值线值设置错误 -- CSfxContourManager::GenerateValue(...)\n");
			return FALSE;
		}

		// 移除全部等值线值
		RemoveAllContourValue();

		// 如果间隔小于0,变换之
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
	///等值线追踪值管理 - end
	///========================================================

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
