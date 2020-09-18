#include "StdAfx.h"
#include "KAgmrsDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KAgmrsDataRender::KAgmrsDataRender(void)
	{
	}


	KAgmrsDataRender::~KAgmrsDataRender(void)
	{
	}

	void KAgmrsDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
		if(!pAgmrsData)
			return;

		// 数据处理器
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return;

		// 是否可见
		if(!pLayerProperty->IsVisible())
			return;

		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// 图像
		Image* pImage = pAgmrsDataProcessor->GetImage();
		if(!pImage)
			return;
		
		// 绑定矩形
		KExtents box = pLayerProperty->GetExtents();
		double xmin = box.MinX();
		double ymin = box.MinY();
		double xmax = box.MaxX();
		double ymax = box.MaxY();

		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);

		// 图像属性
		ImageAttributes imgAttributes;

		// 透明色
		ColorMap clrMap[1];
		clrMap[0].oldColor = Color(255,0,0,0);
		clrMap[0].newColor = Color(255,255,255,255);
		//imgAttributes.SetRemapTable(1,clrMap,ColorAdjustType::ColorAdjustTypeBitmap);

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

		// 图案
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

		RectF rcf((REAL)xmin,(REAL)(ymin-fabs(ymax-ymin)),(REAL)fabs(xmax-xmin),(REAL)fabs(ymax-ymin));
		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeDefault);
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

