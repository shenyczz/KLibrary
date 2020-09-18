#include "StdAfx.h"
#include "KRpgDataRender.h"

#include "KMapEngine.h"
#include "KLayerProperty.h"
#include "KRpgData.h"

namespace Renders
{

	KRpgDataRender::KRpgDataRender(void)
	{
	}

	KRpgDataRender::~KRpgDataRender(void)
	{
	}

	// 绘制图层
	void KRpgDataRender::Rendering(HDC hDC,LPVOID lpvData)
	{
		// 检查传入的参数
		m_hDC = hDC;
		m_lpvData = lpvData;

		if(NULL == hDC || NULL == lpvData)
			return;

		// 地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;

		// 图层属性
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		if(pLayerProperty==NULL)
			return;

		// 是否显示
		if(!pLayerProperty->IsVisible())
			return;

		//-------------------------------------------------
		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		// 取得产品类型并根据产品类型调用对应的渲染函数
		ProductType eProductType = pRpgData->GetProductType(pRpgData->GetProductCode());
		switch(eProductType)
		{
		case ProductType_RadialImage:				// 1
			render_RadialImage(hDC,lpvData);
			break;

		case ProductType_RasterImage:				// 2
			render_RasterImage(hDC,lpvData);
			break;

		case ProductType_LinkedContourVectorImage:	// 3
			render_LinkedContourVectorImage(hDC,lpvData);
			break;

		case ProductType_GeographicAlphaImage:		// 4
			render_GeographicAlphaImage(hDC,lpvData);
			break;

		case ProductType_NonGeographicAlphaImage:	// 5
			render_NonGeographicAlphaImage(hDC,lpvData);
			break;

		case ProductType_AlphanumericImage:			// 6
			render_AlphanumericImage(hDC,lpvData);
			break;

		case ProductType_RasterImageSection:		// 7
			render_RasterImageSection(hDC,lpvData);
			break;

		case ProductType_RasterImageSlice:			// 8
			render_RasterImageSlice(hDC,lpvData);
			break;
		}

		return;
	}

	void KRpgDataRender::render_RadialImage(HDC hDC,LPVOID lpvData)
	{
		// 检查传入的参数
		ASSERT(hDC);
		ASSERT(lpvData);

		// 地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;

		// 图层属性
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		if(pLayerProperty==NULL)
			return;

		// 是否显示
		if(!pLayerProperty->IsVisible())
			return;

		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		//KDataInfo* pDataInfo = pRpgData->GetDataInfo();

		// 距离库链表
		KRangeBinList* pRangeBinList = pRpgData->GetRangeBinList();
		if(pRangeBinList == NULL)
			return;

		// Gdiplus 画布
		Graphics graphics(hDC);

		// 是否剪切
		if(pLayerProperty->IsClip())
		{
			GraphicsPath* pClipPath = (GraphicsPath*)pMapEngine->BuildClipPath();
			if(pClipPath)
			{
				graphics.ResetClip();
				graphics.SetClip(pClipPath);
			}
		}

		// 阈值
		double dLThreshold = pLayerProperty->ThresholdMin();
		double dUThreshold = pLayerProperty->ThresholdMax();

		// 局部变量
		double xx,yy;
		PointF ptf[5];	// 距离库坐标集

		// 透明度和颜色
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		KRangeBinList::iterator itr;
		KRangeBinList::iterator itrBeg = pRangeBinList->begin();
		KRangeBinList::iterator itrEnd = pRangeBinList->end();
		for(itr=itrBeg; itr!=itrEnd; itr++)
		{
			RangeBin* pRangeBin = *itr;

			// 阈值
			if (pRangeBin->iColorIndex<(int)dLThreshold || pRangeBin->iColorIndex>(int)dUThreshold)
			{
				continue;
			}

			// 颜色
			r = GetRValue(pRangeBin->clrBin);
			g = GetGValue(pRangeBin->clrBin);
			b = GetBValue(pRangeBin->clrBin);

			for (int k = 0; k < 4; k++)
			{
				xx = pRangeBin->x[k];
				yy = pRangeBin->y[k];

				pMapEngine->WorldToClient(xx, yy);

				ptf[k].X = (REAL)xx;
				ptf[k].Y = (REAL)yy;
			}

			graphics.FillPolygon(&SolidBrush(Color(a, r, g, b)), &ptf[0], 4, FillMode::FillModeWinding);
		}

		// 是否显示极坐标
		if(pLayerProperty->IsShowCoordinate())
		{
			this->DrawPolar(&graphics);
		}

		return;
	}
	void KRpgDataRender::render_RasterImage(HDC hDC,LPVOID lpvData)
	{
		render_RadialImage(hDC,lpvData);
	}
	void KRpgDataRender::render_LinkedContourVectorImage(HDC hDC,LPVOID lpvData)
	{
		// 检查传入的参数
		ASSERT(hDC);
		ASSERT(lpvData);

		// 地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;

		// 图层属性
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		if(pLayerProperty==NULL)
			return;

		// 是否显示
		if(!pLayerProperty->IsVisible())
			return;

		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		// 连接等值线链表
		KLinkedContourVectorList* pLinkedContourVectorList = pRpgData->GetLinkedContourVectorList();
		if(pLinkedContourVectorList == NULL)
			return;

		// Gdiplus 画布
		Graphics graphics(hDC);

		// 是否剪切
		if(pLayerProperty->IsClip())
		{
			GraphicsPath* pClipPath = (GraphicsPath*)pMapEngine->BuildClipPath();
			if(pClipPath)
			{
				graphics.ResetClip();
				graphics.SetClip(pClipPath);
			}
		}

		// 阈值
		double dLThreshold = pLayerProperty->ThresholdMin();
		double dUThreshold = pLayerProperty->ThresholdMax();

		// 图例
		Legend* pLegend = pRpgData->GetLegend();

		// 局部变量
		double xx,yy;
		PointF ptf[5];	// 距离库坐标集

		// 透明度和颜色
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		KLinkedContourVectorList::iterator itr;
		KLinkedContourVectorList::iterator itrBeg = pLinkedContourVectorList->begin();
		KLinkedContourVectorList::iterator itrEnd = pLinkedContourVectorList->end();
		for(itr=itrBeg; itr!=itrEnd; itr++)
		{
			LinkedVector* pLinkedVector = *itr;

			// 阈值
			if(pLinkedVector->iColorLevel<(int)dLThreshold || pLinkedVector->iColorLevel>(int)dUThreshold)
			{
				continue;
			}

			// 颜色
			COLORREF clr = pLegend[pLinkedVector->iColorLevel].dwColor;
			r = GetRValue(clr);
			g = GetGValue(clr);
			b = GetBValue(clr);

			// 曲线
			KCurve curve;
			double xmin = 361;
			double xmax = -361;
			double ymin = 181;
			double ymax = -181;

			double dLon0,dLat0;
			pRpgData->GetImageCenter(&dLon0,&dLat0);

			// 1deg = 111.37km	0.0098769866211726676842955912723355
			double dDegreePerKm		= 0.0089790787465206069857232647930322;

			// 左上角经纬度
			double dLonLft = dLon0 - dDegreePerKm * pRpgData->GetProductRange(pRpgData->GetProductCode());
			double dLatTop = dLat0 + dDegreePerKm * pRpgData->GetProductRange(pRpgData->GetProductCode());

			// 线条点集
			PointVector* pPointVector = &pLinkedVector->oPointVector;
			int count = pPointVector->size();
			for(int k=0;k<count;k++)
			{
				POINT pt = pLinkedVector->oPointVector[k];

				// 坐标转换为经纬度
				// km/4 转换为像素点位置(距离雷达中心)
				int xxx = (int)((double)512.0 / 2 + 0.25 * pt.x);
				int yyy = (int)((double)512.0 / 2 - 0.25 * pt.y);

				// 坐标转换为经纬度
				xx = dLonLft + xxx * dDegreePerKm;
				yy = dLatTop - yyy * dDegreePerKm;

				curve.Add(new KPoint(xx,yy));

				xmin = min(xmin,xx);
				ymin = min(ymin,yy);
				xmax = max(xmax,xx);
				ymax = max(ymax,yy);
			}
			curve.SetExtents(KExtents(xmin,ymin,xmax,ymax));
			this->DrawCurve(&graphics,&curve,clr,FALSE);
		}

		// 是否显示极坐标
		if(pLayerProperty->IsShowCoordinate())
		{
			this->DrawPolar(&graphics);
		}

		return;
	}
	void KRpgDataRender::render_GeographicAlphaImage(HDC hDC,LPVOID lpvData)
	{
		render_RadialImage(hDC,lpvData);
	}
	void KRpgDataRender::render_NonGeographicAlphaImage(HDC hDC,LPVOID lpvData)
	{
		render_RadialImage(hDC,lpvData);
	}
	void KRpgDataRender::render_AlphanumericImage(HDC hDC,LPVOID lpvData)
	{
		render_RadialImage(hDC,lpvData);
	}
	void KRpgDataRender::render_RasterImageSection(HDC hDC,LPVOID lpvData)
	{
		render_RadialImage(hDC,lpvData);
	}
	void KRpgDataRender::render_RasterImageSlice(HDC hDC,LPVOID lpvData)
	{
		render_RadialImage(hDC,lpvData);
	}

	// 绘制极坐标
	void KRpgDataRender::DrawPolar(Graphics* pGraphics)
	{
		HDC hDC = this->m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		KLayerProperty* pLayerProperty = m_pLayerProperty;

		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		//产品代码
		int iProductCode = pRpgData->GetProductCode();
		//范围
		double dRange = pRpgData->GetProductRange(iProductCode);

		// 雷达信息
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		hDC = pGraphics->GetHDC();

		CDC dc;
		CDC* pDC = &dc;
		pDC->Attach(hDC);

		//画笔
		CPen pen;
		CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

		// 画刷
		CBrush brush;
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,1,RGB(255,161,123));
		pDC->SelectObject(&pen);

		double x0 = pRadarInfo->dLon;
		double y0 = pRadarInfo->dLat;
		double dDegreePerKm = 0.0098769866211726676842955912723355;	//1.1度=0.008979经纬度 1/111.37

		double x1,y1,x2,y2;

		int iNum = 5;
		for(int i=0;i<iNum;i++)
		{
			double dDegree = 50.0*(i+1)*dDegreePerKm*dRange/230;

			x1 = (float)(x0-dDegree);
			y1 = (float)(y0+dDegree);

			x2 = (float)(x0+dDegree);
			y2 = (float)(y0-dDegree);

			//最后一圈绘制30公里
			if(i==iNum-1)
			{
				double dTemp = 20.0*dDegreePerKm*dRange/230;	//20公里 50-20 = 30km (因为230km)

				x1 += (float)dTemp;
				y1 -= (float)dTemp;

				x2 -= (float)dTemp;
				y2 += (float)dTemp;
			}

			pMapEngine->WorldToClient(x1,y1);
			pMapEngine->WorldToClient(x2,y2);

			//绘制椭圆
			pDC->Ellipse((int)x1,(int)y1,(int)x2,(int)y2);

			//当 i=iNum-1 时绘制极坐标径向线
			if(i==iNum-1)
			{
				double dxy = 50.0*(i+1)*dDegreePerKm*dRange/230;
				//-------------------------------
				//绘制 X 轴
				x1 = (float)(x0-dxy);
				y1 = (float)(y0);

				x2 = (float)(x0+dxy);
				y2 = (float)(y0);

				//坐标投影变换
				pMapEngine->WorldToClient(x1,y1);
				pMapEngine->WorldToClient(x2,y2);

				pDC->MoveTo((int)x1,(int)y1);
				pDC->LineTo((int)x2,(int)y2);
				//-------------------------------
				//绘制 Y 轴
				x1 = (float)(x0);
				y1 = (float)(y0-dxy);

				x2 = (float)(x0);
				y2 = (float)(y0+dxy);

				//坐标投影变换
				pMapEngine->WorldToClient(x1,y1);
				pMapEngine->WorldToClient(x2,y2);

				pDC->MoveTo((int)x1,(int)y1);
				pDC->LineTo((int)x2,(int)y2);
				//-------------------------------
			}
		}

		pDC->SelectObject(pOldPen);
		pen.DeleteObject();

		pDC->SelectObject(pOldBrush);
		brush.DeleteObject();

		pDC->Detach();

		return;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
