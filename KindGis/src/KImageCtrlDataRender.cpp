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
		// 1.�������
		m_hDC = hDC;
		m_lpvData = lpvData;

		// 2.׼��ͼ��
		Graphics graphics(hDC);

		// 3.��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// 4.ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// 5.�����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 6.���ݶ���
		KImageCtrlData* pDataObject = (KImageCtrlData*)pProvider->GetDataObject();
		if(!pDataObject)
			return;
		//-------------------------------------------------
		// �Ƿ�ɼ�
		if(!pLayerProperty->IsVisible())
			return;

		// Ϊ��Ⱦ��ע��ص�
		this->RegiestCallbackFunc(pDataObject->CallbackFunc(),pDataObject->CallbackParam());

		// ͼ��
		Image* pImage = pDataObject->GetImage();
		if(!pImage)
			return;

		// �󶨾���
		KExtents box = pLayerProperty->GetExtents();
		double xmin	= box.MinX();
		double ymin = box.MinY();
		double xmax	= box.MaxX();
		double ymax	= box.MaxY();
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);

		// ͼ������
		ImageAttributes imgAttributes;

		// ͸��ɫ
		ColorMap clrMap[1];
		clrMap[0].oldColor = Color(255,0,0,0);
		clrMap[0].newColor = Color(0,255,255,255);
		imgAttributes.SetRemapTable(1,clrMap,ColorAdjustType::ColorAdjustTypeBitmap);

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
