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

		// �������
		m_hDC = hDC;
		m_lpvData = lpvData;

		// ׼��ͼ��
		Graphics graphics(hDC);

		// ��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KGridImageData* pDataObject = (KGridImageData*)pProvider->GetDataObject();
		if(!pDataObject)
			return;

		// �Ƿ���ʾ
		if(!pLayerProperty->IsVisible())
			return;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return;

		// ͼ��
		Image* pImage = pDataObject->GetImage();
		if(!pImage)
			return;

		// �󶨾���
		KExtents extents = pDataObject->GetExtents();
		double xmin = extents.MinX();
		double ymin = extents.MinY();
		double xmax = extents.MaxX();
		double ymax = extents.MaxY();
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);


		// ͼ������
		ImageAttributes imgAttributes;

		// ͸��ɫ
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

		// ͸����
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
		
		// �Ƿ����
		if(pLayerProperty->IsClip())
		{
			GraphicsPath* pClipPath = (GraphicsPath*)pMapEngine->BuildClipPath();
			if(pClipPath)
			{
				graphics.ResetClip();
				graphics.SetClip(pClipPath);
			}
		}

		// ���ò�ֵģʽ
		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeInvalid);
		// ��������ƽ��ģʽ
		graphics.SetSmoothingMode(SmoothingMode::SmoothingModeNone);

		// ���ƾ���
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
