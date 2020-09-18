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
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
		if(!pAgmrsData)
			return;

		// ���ݴ�����
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return;

		// �Ƿ�ɼ�
		if(!pLayerProperty->IsVisible())
			return;

		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// ͼ��
		Image* pImage = pAgmrsDataProcessor->GetImage();
		if(!pImage)
			return;
		
		// �󶨾���
		KExtents box = pLayerProperty->GetExtents();
		double xmin = box.MinX();
		double ymin = box.MinY();
		double xmax = box.MaxX();
		double ymax = box.MaxY();

		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);

		// ͼ������
		ImageAttributes imgAttributes;

		// ͸��ɫ
		ColorMap clrMap[1];
		clrMap[0].oldColor = Color(255,0,0,0);
		clrMap[0].newColor = Color(255,255,255,255);
		//imgAttributes.SetRemapTable(1,clrMap,ColorAdjustType::ColorAdjustTypeBitmap);

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

		// ͼ��
		Graphics graphics(hDC);

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

