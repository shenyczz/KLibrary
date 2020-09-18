#include "StdAfx.h"
#include "KShapeDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KShapeDataRender::KShapeDataRender(void)
	{
		m_pGraphics = NULL;
	}


	KShapeDataRender::~KShapeDataRender(void)
	{
	}

	void KShapeDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		if(NULL == hDC || NULL == lpvData)
			return;

		m_hDC = hDC;
		m_lpvData = lpvData;

		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// ͼ���Ƿ�ɼ�
		if(!pLayerProperty->IsVisible())
			return;

		// �����ṩ��
		KShapeFileProvider* pShapeFileProvider = (KShapeFileProvider*)pLayerProperty->GetProvider();
		if(!pShapeFileProvider || ProviderType_ShapeFile != pShapeFileProvider->GetType())
			return;

		// ����
		KShapeFile* pShapeFile = (KShapeFile*)pShapeFileProvider->GetDataObject();
		if(!pShapeFile)
			return;

		// ��������
		KDbaseFile* pDbaseFile = pShapeFile->GetDbaseFile();
		//if(!pDbaseFile)
		//	return;

		// ��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		Graphics graphics(hDC);
		m_pGraphics = &graphics;

		// �Ƿ����
		if(pLayerProperty->IsClip())
		{
			// ����������·��
			GraphicsPath* pClipPath = (GraphicsPath*)pMapEngine->BuildClipPath();
			if(pClipPath)
			{
				graphics.ResetClip();
				graphics.SetClip(pClipPath);
			}
		}


		// ����
		ShapeType eShapeType = (ShapeType)pShapeFile->GetType();
		int iShapeCount = pShapeFile->Count();
		for(int i=0; i< iShapeCount; i++)
		{
			KRow* pRow = NULL;
			if(pDbaseFile)
			{
				pRow = pDbaseFile->GetRow(i);
				if(!pRow) continue;
			}

			KGeometry* pGeometry = pShapeFile->GetAt(i);
			DrawGeometry(pGeometry,pRow,eShapeType);
		}

		// FUN_END
	}

	// ����
	void KShapeDataRender::DrawGeometry(KGeometry* pGeometry, KRow* pRow, ShapeType eShapeType)
	{
		int i;

		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;

		// ȡ�ü��о���
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);

		// ������о��ζ�������
		//  p0         p2
		//  +----------+
		//  |          |
		//	|          |
		//	|          |
		//	+----------+
		//	p1         p3

		double xs[4], ys[4];
		xs[0] = x;		ys[0] = y ;
		xs[1] = x;		ys[1] = y + h ;
		xs[2] = x + w;	ys[2] = y ;
		xs[3] = x + w;	ys[3] = y + h ;

		// ת��Ϊ��������
		for ( i = 0; i < 4; i++ )
		{
			pMapEngine->ClientToWorld(xs[i],ys[i]);
		}

		// ���о��εĵ��������������
		// left,top right,bottom
		double clipL, clipT, clipR, clipB;
		clipL = clipR = xs[0];
		clipB = clipT = ys[0];

		for ( i = 1; i < 4; i ++ )
		{
			if ( clipL > xs[i] ) clipL = xs[i];	// left ȡ��Сֵ
			if ( clipT < ys[i] ) clipT = ys[i];	// top ȡ���ֵ(�����Ļ���겻ͬ)

			if ( clipR < xs[i] ) clipR = xs[i];	// rignt ȡ���ֵ
			if ( clipB > ys[i] ) clipB = ys[i];	// bottom ȡ��Сֵ(�����Ļ���겻ͬ)
		}

		// �����ж��������ʾ�ٶ�
		KExtents bbox = pGeometry->GetExtents();
		double intersectL = max(clipL,bbox.Xmin);
		double intersectR = min(clipR,bbox.Xmax);
		double intersectB = max(clipB,bbox.Ymin);
		double intersectT = min(clipT,bbox.Ymax);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			// ������ǵ�����
			if(eShapeType != ShapeType_Point)
			{
				return;
			}
		}

		// ����ͼ�����ʷֱ����
		switch(eShapeType)
		{
		case ShapeType_Point:
			DrawPointCollection(pGeometry,pRow);
			break;

		case ShapeType_Polyline:
			DrawPolylineCollection(pGeometry,pRow);
			break;

		case ShapeType_Polygon:
			DrawPolygonCollection(pGeometry,pRow);
			break;
		}
	}

	// ���Ƶ㼯��
	void KShapeDataRender::DrawPointCollection(KGeometry* pGeometry,KRow* pRow)
	{
		KGeometryPtrCollection* pPointCollection = (KGeometryPtrCollection*)pGeometry;
		int iPointCount = pPointCollection->Count();
		for(int i=0;i<iPointCount;i++)
		{
			KPoint* point = (KPoint*)pPointCollection->GetAt(i);
			DrawPoint(point,pRow);
		}
	}
	// �������߼���
	void KShapeDataRender::DrawPolylineCollection(KGeometry* pGeometry,KRow* pRow)
	{
		KGeometryPtrCollection* pPolylines = (KGeometryPtrCollection*)pGeometry;
		int iPolylineCount = pPolylines->Count();
		for(int i=0;i<iPolylineCount;i++)
		{
			KPolyline* pPolyline = (KPolyline*)pPolylines->GetAt(i);
			DrawPolyline(pPolyline,pRow);
		}
	}
	// ���ƶ���μ���
	void KShapeDataRender::DrawPolygonCollection(KGeometry* pGeometry,KRow* pRow)
	{
		KGeometryPtrCollection* pGeometryPtrCollection = (KGeometryPtrCollection*)pGeometry;
		int iPolygonCount = pGeometryPtrCollection->Count();
		for(int i=0;i<iPolygonCount;i++)
		{
			KPolygon* pPolygon = (KPolygon*)pGeometryPtrCollection->GetAt(i);
			DrawPolygon(pPolygon,pRow);
		}
	}

	void KShapeDataRender::DrawPoint(KGeometry* pGeometry,KRow* pRow)
	{
		HDC hDC = m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		//Graphics graphics(hDC);
		Graphics* pGraphics = m_pGraphics;

		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// ����
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		KPoint* point = (KPoint*)pGeometry;

		double xx = point->X;
		double yy = point->Y;
		pMapEngine->WorldToClient(xx,yy);
		//::Ellipse(hDC,(int)(xx-2),(int)(yy-2),(int)(xx+2),(int)(yy+2));

		// ���Ƶ�����
		if(!pRow)
			return;

		KField* pField = pRow->GetField(_T("NAME"));
		if(!pField)
			return;

		KString strName = pField->GetStringValue();
		if(strName.IsEmpty())
			return;

		// ����
		LOGFONT& lf = pLayerProperty->LogFont();
		FontFamily fontFamily(KString::ToWChar(pLayerProperty->LogFont().lfFaceName));
		REAL fontPoint = (REAL)(-1.0 * pLayerProperty->LogFont().lfHeight / 12 * 9);
		WORD wFontStyle = 0;
		if(lf.lfWeight > 400)	wFontStyle |= 0x01;
		if(lf.lfItalic)			wFontStyle |= 0x02;
		if(lf.lfUnderline)		wFontStyle |= 0x04;
		if(lf.lfStrikeOut)		wFontStyle |= 0x08;

		// ������ɫ
		COLORREF clrFont = pLayerProperty->FontColor();
		r = GetRValue(clrFont);
		g = GetGValue(clrFont);
		b = GetBValue(clrFont);

		// վ������
		Gdiplus::Font fontName(&fontFamily,fontPoint,(FontStyle)wFontStyle,Unit::UnitPoint);
		StringFormat strfmt;
		strfmt.SetAlignment(StringAlignment::StringAlignmentNear);
		strfmt.SetAlignment(StringAlignment::StringAlignmentCenter);
		strfmt.SetLineAlignment(StringAlignment::StringAlignmentCenter);

		// ��ˢ
		SolidBrush brushStationNameBold(Color(a,r,g,b));	// վ��������ɫ

		pGraphics->DrawString(KString::ToWChar((LPCTSTR)strName),-1,&fontName,PointF((REAL)xx,(REAL)yy),&strfmt,&brushStationNameBold);

		return;
	}
	void KShapeDataRender::DrawPolyline(KGeometry* pGeometry,KRow* pRow)
	{
		HDC hDC = m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		//Graphics graphics(hDC);
		Graphics* pGraphics = m_pGraphics;

		KPolyline* pPolyline = (KPolyline*)pGeometry;

		int iPointCount = pPolyline->Count();
		PointF* ptf = new PointF[iPointCount];

		for(int i=0;i<iPointCount;i++)
		{
			KPoint point = *pPolyline->GetAt(i);

			double xx = point.X;
			double yy = point.Y;

			pMapEngine->WorldToClient(xx,yy);

			ptf[i].X = (REAL)xx;
			ptf[i].Y = (REAL)yy;
		}

		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// ͸����
		int iTransparency = pLayerProperty->Transparency();
		// ��ɫ
		COLORREF clr = pLayerProperty->GetColor();
		// �߿�
		int iLineWidth = pLayerProperty->LineWidth();
		// ����
		int iLineStyle = pLayerProperty->LineStyle();

		BYTE a = 255 * (100 - iTransparency) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		Pen pen(Color(a,r,g,b),(REAL)iLineWidth);
		pen.SetDashStyle((DashStyle)iLineStyle);
		pGraphics->DrawLines(&pen,ptf,iPointCount);

		_deletea(ptf);
	}
	void KShapeDataRender::DrawPolygon(KGeometry* pGeometry,KRow* pRow)
	{
		HDC hDC = m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		//Graphics graphics(hDC);
		Graphics* pGraphics = m_pGraphics;

		KPolygon* pPolygon = (KPolygon*)pGeometry;

		int iCLipFlag = 0;
		if(pRow)
		{
			KField* pField = pRow->GetField(_T("CLIP_FLAG"));	// ���б��
			if(pField)
			{
				iCLipFlag = (int)pField->GetDoubleValue();
			}
		}

		int iPointCount = pPolygon->Count();
		PointF* ptf = new PointF[iPointCount];

		for(int i=0;i<iPointCount;i++)
		{
			KPoint point = *pPolygon->GetAt(i);

			double xx = point.X;
			double yy = point.Y;

			pMapEngine->WorldToClient(xx,yy);

			ptf[i].X = (REAL)xx;
			ptf[i].Y = (REAL)yy;
		}

		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// ͸����
		int iTransparency = pLayerProperty->Transparency();
		// ��ɫ
		COLORREF clr = pLayerProperty->GetColor();
		// �߿�
		int iLineWidth = pLayerProperty->LineWidth();
		// ����
		int iLineStyle = pLayerProperty->LineStyle();

		BYTE a = 255 * (100 - iTransparency) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		Pen pen(Color(a,r,g,b),(REAL)iLineWidth);
		pen.SetDashStyle((DashStyle)iLineStyle);
		//graphics.DrawLines(&pen,ptf,iPointCount);

		if(pRow)
		{
			KField* pField = NULL;

			// ����
			pField = pRow->GetField(_T("NAME"));

			// �����ɫ
			pField = pRow->GetField(_T("FILL_COLOR"));
			if(pField)
			{
				COLORREF clrFill = (COLORREF)pField->GetDoubleValue();
				if(clrFill>0)
				{
					Color clrBrush(a,GetRValue(clrFill),GetGValue(clrFill),GetBValue(clrFill));
					pGraphics->FillPolygon(&SolidBrush(clrBrush),ptf,iPointCount);
				}
			}

		}

		if(iCLipFlag>0)
		{
			// ѡ��Ķ���μӴ�
			//pen.SetWidth(2);
		}

		pGraphics->DrawPolygon(&pen,ptf,iPointCount);

		_deletea(ptf);

		//pGraphics->DrawPath(Pen)
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
