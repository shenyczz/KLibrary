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

	// ����ͼ��
	void KRpgDataRender::Rendering(HDC hDC,LPVOID lpvData)
	{
		// ��鴫��Ĳ���
		m_hDC = hDC;
		m_lpvData = lpvData;

		if(NULL == hDC || NULL == lpvData)
			return;

		// ��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;

		// ͼ������
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		if(pLayerProperty==NULL)
			return;

		// �Ƿ���ʾ
		if(!pLayerProperty->IsVisible())
			return;

		//-------------------------------------------------
		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		// ȡ�ò�Ʒ���Ͳ����ݲ�Ʒ���͵��ö�Ӧ����Ⱦ����
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
		// ��鴫��Ĳ���
		ASSERT(hDC);
		ASSERT(lpvData);

		// ��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;

		// ͼ������
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		if(pLayerProperty==NULL)
			return;

		// �Ƿ���ʾ
		if(!pLayerProperty->IsVisible())
			return;

		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		//KDataInfo* pDataInfo = pRpgData->GetDataInfo();

		// ���������
		KRangeBinList* pRangeBinList = pRpgData->GetRangeBinList();
		if(pRangeBinList == NULL)
			return;

		// Gdiplus ����
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

		// ��ֵ
		double dLThreshold = pLayerProperty->ThresholdMin();
		double dUThreshold = pLayerProperty->ThresholdMax();

		// �ֲ�����
		double xx,yy;
		PointF ptf[5];	// ��������꼯

		// ͸���Ⱥ���ɫ
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

			// ��ֵ
			if (pRangeBin->iColorIndex<(int)dLThreshold || pRangeBin->iColorIndex>(int)dUThreshold)
			{
				continue;
			}

			// ��ɫ
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

		// �Ƿ���ʾ������
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
		// ��鴫��Ĳ���
		ASSERT(hDC);
		ASSERT(lpvData);

		// ��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;

		// ͼ������
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		if(pLayerProperty==NULL)
			return;

		// �Ƿ���ʾ
		if(!pLayerProperty->IsVisible())
			return;

		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		// ���ӵ�ֵ������
		KLinkedContourVectorList* pLinkedContourVectorList = pRpgData->GetLinkedContourVectorList();
		if(pLinkedContourVectorList == NULL)
			return;

		// Gdiplus ����
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

		// ��ֵ
		double dLThreshold = pLayerProperty->ThresholdMin();
		double dUThreshold = pLayerProperty->ThresholdMax();

		// ͼ��
		Legend* pLegend = pRpgData->GetLegend();

		// �ֲ�����
		double xx,yy;
		PointF ptf[5];	// ��������꼯

		// ͸���Ⱥ���ɫ
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

			// ��ֵ
			if(pLinkedVector->iColorLevel<(int)dLThreshold || pLinkedVector->iColorLevel>(int)dUThreshold)
			{
				continue;
			}

			// ��ɫ
			COLORREF clr = pLegend[pLinkedVector->iColorLevel].dwColor;
			r = GetRValue(clr);
			g = GetGValue(clr);
			b = GetBValue(clr);

			// ����
			KCurve curve;
			double xmin = 361;
			double xmax = -361;
			double ymin = 181;
			double ymax = -181;

			double dLon0,dLat0;
			pRpgData->GetImageCenter(&dLon0,&dLat0);

			// 1deg = 111.37km	0.0098769866211726676842955912723355
			double dDegreePerKm		= 0.0089790787465206069857232647930322;

			// ���ϽǾ�γ��
			double dLonLft = dLon0 - dDegreePerKm * pRpgData->GetProductRange(pRpgData->GetProductCode());
			double dLatTop = dLat0 + dDegreePerKm * pRpgData->GetProductRange(pRpgData->GetProductCode());

			// �����㼯
			PointVector* pPointVector = &pLinkedVector->oPointVector;
			int count = pPointVector->size();
			for(int k=0;k<count;k++)
			{
				POINT pt = pLinkedVector->oPointVector[k];

				// ����ת��Ϊ��γ��
				// km/4 ת��Ϊ���ص�λ��(�����״�����)
				int xxx = (int)((double)512.0 / 2 + 0.25 * pt.x);
				int yyy = (int)((double)512.0 / 2 - 0.25 * pt.y);

				// ����ת��Ϊ��γ��
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

		// �Ƿ���ʾ������
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

	// ���Ƽ�����
	void KRpgDataRender::DrawPolar(Graphics* pGraphics)
	{
		HDC hDC = this->m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		KLayerProperty* pLayerProperty = m_pLayerProperty;

		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KRpgData* pRpgData = (KRpgData*)pProvider->GetDataObject();
		if(pRpgData==NULL)
			return;

		//��Ʒ����
		int iProductCode = pRpgData->GetProductCode();
		//��Χ
		double dRange = pRpgData->GetProductRange(iProductCode);

		// �״���Ϣ
		RadarInfo* pRadarInfo = pRpgData->GetRadarInfo();

		hDC = pGraphics->GetHDC();

		CDC dc;
		CDC* pDC = &dc;
		pDC->Attach(hDC);

		//����
		CPen pen;
		CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

		// ��ˢ
		CBrush brush;
		CBrush* pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,1,RGB(255,161,123));
		pDC->SelectObject(&pen);

		double x0 = pRadarInfo->dLon;
		double y0 = pRadarInfo->dLat;
		double dDegreePerKm = 0.0098769866211726676842955912723355;	//1.1��=0.008979��γ�� 1/111.37

		double x1,y1,x2,y2;

		int iNum = 5;
		for(int i=0;i<iNum;i++)
		{
			double dDegree = 50.0*(i+1)*dDegreePerKm*dRange/230;

			x1 = (float)(x0-dDegree);
			y1 = (float)(y0+dDegree);

			x2 = (float)(x0+dDegree);
			y2 = (float)(y0-dDegree);

			//���һȦ����30����
			if(i==iNum-1)
			{
				double dTemp = 20.0*dDegreePerKm*dRange/230;	//20���� 50-20 = 30km (��Ϊ230km)

				x1 += (float)dTemp;
				y1 -= (float)dTemp;

				x2 -= (float)dTemp;
				y2 += (float)dTemp;
			}

			pMapEngine->WorldToClient(x1,y1);
			pMapEngine->WorldToClient(x2,y2);

			//������Բ
			pDC->Ellipse((int)x1,(int)y1,(int)x2,(int)y2);

			//�� i=iNum-1 ʱ���Ƽ����꾶����
			if(i==iNum-1)
			{
				double dxy = 50.0*(i+1)*dDegreePerKm*dRange/230;
				//-------------------------------
				//���� X ��
				x1 = (float)(x0-dxy);
				y1 = (float)(y0);

				x2 = (float)(x0+dxy);
				y2 = (float)(y0);

				//����ͶӰ�任
				pMapEngine->WorldToClient(x1,y1);
				pMapEngine->WorldToClient(x2,y2);

				pDC->MoveTo((int)x1,(int)y1);
				pDC->LineTo((int)x2,(int)y2);
				//-------------------------------
				//���� Y ��
				x1 = (float)(x0);
				y1 = (float)(y0-dxy);

				x2 = (float)(x0);
				y2 = (float)(y0+dxy);

				//����ͶӰ�任
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
