#include "StdAfx.h"
#include "KGridImageDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KGridImageDataRender::KGridImageDataRender(void)
	{
	}


	KGridImageDataRender::~KGridImageDataRender(void)
	{
	}

	void KGridImageDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
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
		KGridImageData* pDataObject = (KGridImageData*)pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// 是否显示
		if(!pLayerProperty->IsVisible())
			return;

		// 数据信息
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// 图像
		Image* pImage = pDataObject->GetImage();
		if(!pImage)
			return;

		// 绑定矩形
		KExtents extents = pDataObject->GetExtents();
		double xmin = extents.MinX();
		double ymin = extents.MinY();
		double xmax = extents.MaxX();
		double ymax = extents.MaxY();
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);


		// 图像属性
		ImageAttributes imgAttributes;

		// 透明色
		if(pDataInfo->bHasTransparentColor)
		{
			COLORREF clrTransparent = pDataInfo->dwTransparentColor;
			BYTE r = GetRValue(clrTransparent);
			BYTE g = GetGValue(clrTransparent);
			BYTE b = GetBValue(clrTransparent);

			ColorMap clrMap[1];
			clrMap[0].oldColor = Color(255,r,g,b);
			clrMap[0].newColor = Color(0,r,g,b);
			imgAttributes.SetRemapTable(1,clrMap,ColorAdjustType::ColorAdjustTypeBitmap);
		}

		// 透明度
		REAL tra = 0.01f * (100 - pLayerProperty->GetTransparency());
		ColorMatrix clrMatrix = 
		{
			1.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 0.0, tra, 0.0,
			0.0, 0.0, 0.0, 0.0, 1.0,
		};
		imgAttributes.SetColorMatrix(&clrMatrix,ColorMatrixFlags::ColorMatrixFlagsDefault,ColorAdjustType::ColorAdjustTypeBitmap);
		
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

		// 设置插值模式
		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeInvalid);
		// 设置线条平滑模式
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeNone);

		// 绘制矩形
		RectF rcf((REAL)xmin,(REAL)(ymin-fabs(ymax-ymin)),(REAL)fabs(xmax-xmin),(REAL)fabs(ymax-ymin));
		graphics.DrawImage(pImage
			,rcf
			,0
			,0
			,(REAL)pImage->GetWidth()
			,(REAL)pImage->GetHeight()
			,UnitPixel
			,&imgAttributes
			);

		return;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
