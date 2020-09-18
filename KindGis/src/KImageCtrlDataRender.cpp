#include "StdAfx.h"
#include "KImageCtrlDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KImageCtrlDataRender::KImageCtrlDataRender(void)
	{
	}

	KImageCtrlDataRender::~KImageCtrlDataRender(void)
	{
	}

	void KImageCtrlDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		if(!hDC || !lpvData)
			return;

		//-------------------------------------------------
		// 1.保存参数
		m_hDC = hDC;
		m_lpvData = lpvData;

		// 2.准备图案
		Graphics graphics(hDC);

		// 3.地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// 4.图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// 5.数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 6.数据对象
		KImageCtrlData* pDataObject = (KImageCtrlData*)pProvider->GetDataObject();
		if(!pDataObject)
			return;
		//-------------------------------------------------
		// 是否可见
		if(!pLayerProperty->IsVisible())
			return;

		// 为渲染器注册回调
		this->RegiestCallbackFunc(pDataObject->CallbackFunc(),pDataObject->CallbackParam());

		// 图像
		Image* pImage = pDataObject->GetImage();
		if(!pImage)
			return;

		// 绑定矩形
		KExtents box = pLayerProperty->GetExtents();
		double xmin	= box.MinX();
		double ymin = box.MinY();
		double xmax	= box.MaxX();
		double ymax	= box.MaxY();
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);

		// 图像属性
		ImageAttributes imgAttributes;

		// 透明色
		ColorMap clrMap[1];
		clrMap[0].oldColor = Color(255,0,0,0);
		clrMap[0].newColor = Color(0,255,255,255);
		imgAttributes.SetRemapTable(1,clrMap,ColorAdjustType::ColorAdjustTypeBitmap);

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
