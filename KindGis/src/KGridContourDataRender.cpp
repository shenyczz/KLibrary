#include "StdAfx.h"
#include "KGridContourDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KGridContourDataRender::KGridContourDataRender(void)
	{
	}

	KGridContourDataRender::~KGridContourDataRender(void)
	{
	}

	// 渲染
	void KGridContourDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		int i;

		if(NULL == hDC || NULL == lpvData)
			return;

		// 保存参数
		m_hDC = hDC;
		m_lpvData = lpvData;

		// 准备图案
		Graphics graphics(hDC);

		// 地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KGridContourData* pGridContourData = (KGridContourData*)pProvider->GetDataObject();
		if(!pGridContourData)
			return;

		// 是否显示
		if(!pLayerProperty->IsVisible())
			return;

		// 为渲染器注册回调函数
		this->RegiestCallbackFunc(pGridContourData->CallbackFunc(),pGridContourData->CallbackParam());

		// 等值线构造器
		KContourBuilder* pContourBuilder = pGridContourData->GetContourBuilder();
		if(pContourBuilder==NULL)
			return;

		// 等值线集(没有转换为封闭多边形前)
		TPtrCollection<KContour*>* pContourPtrCollection = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::BeforeConvert);
		if(pContourPtrCollection==NULL)
			return;

		// 是否剪切
		if(pLayerProperty->IsClip())
		{
			// 建立剪切区路径
			GraphicsPath* pClipPath = (GraphicsPath*)pMapEngine->BuildClipPath();
			if(pClipPath)
			{
				graphics.ResetClip();
				graphics.SetClip(pClipPath);
			}
		}

		// 是否填充
		if(pLayerProperty->IsFill())
		{
			// 注意 GetContourCollection(1);
			TPtrCollection<KContour*>* pContourPtrCollectionFill = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::AfterConvert);
			int iContourCount = pContourPtrCollectionFill->Count();
			for(i=0;i<iContourCount;i++)
			{
				KContour* pContour = pContourPtrCollectionFill->GetAt(i);
				this->FillContour(&graphics,pContour);
			}
		}

		// 再绘制等值线
		int iContourCount = pContourPtrCollection->Count();
		for(i=0;i<iContourCount;i++)
		{
			KContour* pContour = pContourPtrCollection->GetAt(i);
			this->DrawContour(&graphics,pContour);	// 绘制等值线
			if(pLayerProperty->IsLabel())
			{
				this->LabelContour(&graphics,pContour);	// 绘制等值线
			}
		}

		// 是否显示格点数据
		if(pLayerProperty->IsShowData())
		{
			this->drawGridValue(&graphics);
		}

		return;
	}

	// 显示格点数据
	void KGridContourDataRender::drawGridValue(Graphics* pGraphics)
	{
		int i,j;

		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KGridData* pGridData = (KGridData*)pProvider->GetDataObject();
		KDataInfo* pDataInfo = pGridData->GetDataInfo();

		int nx = pDataInfo->Width();
		int ny = pDataInfo->Height();

		COLORREF clr = m_clrPen;

		for(i=0;i<ny;i++)
		{
			double yy = pGridData->GetYData(i);

			for(j=0;j<nx;j++)
			{
				double xx = pGridData->GetXData(j);

				double vv = pGridData->GetVData(i * nx + j);

				this->LabelValue(pGraphics,xx,yy,vv,clr);
			}
		}

		return;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
