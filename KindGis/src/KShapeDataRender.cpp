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

		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// 图层是否可见
		if(!pLayerProperty->IsVisible())
			return;

		// 数据提供者
		KShapeFileProvider* pShapeFileProvider = (KShapeFileProvider*)pLayerProperty->GetProvider();
		if(!pShapeFileProvider || ProviderType_ShapeFile != pShapeFileProvider->GetType())
			return;

		// 数据
		KShapeFile* pShapeFile = (KShapeFile*)pShapeFileProvider->GetDataObject();
		if(!pShapeFile)
			return;

		// 属性数据
		KDbaseFile* pDbaseFile = pShapeFile->GetDbaseFile();
		//if(!pDbaseFile)
		//	return;

		// 地图引擎
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		Graphics graphics(hDC);
		m_pGraphics = &graphics;

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


		// 绘制
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

	// 绘制
	void KShapeDataRender::DrawGeometry(KGeometry* pGeometry, KRow* pRow, ShapeType eShapeType)
	{
		int i;

		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;

		// 取得剪切矩形
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);

		// 保存剪切矩形顶点坐标
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

		// 转换为地理坐标
		for ( i = 0; i < 4; i++ )
		{
			pMapEngine->ClientToWorld(xs[i],ys[i]);
		}

		// 剪切矩形的地理坐标矩形属性
		// left,top right,bottom
		double clipL, clipT, clipR, clipB;
		clipL = clipR = xs[0];
		clipB = clipT = ys[0];

		for ( i = 1; i < 4; i ++ )
		{
			if ( clipL > xs[i] ) clipL = xs[i];	// left 取最小值
			if ( clipT < ys[i] ) clipT = ys[i];	// top 取最大值(这和屏幕坐标不同)

			if ( clipR < xs[i] ) clipR = xs[i];	// rignt 取最大值
			if ( clipB > ys[i] ) clipB = ys[i];	// bottom 取最小值(这和屏幕坐标不同)
		}

		// 增加判断以提高显示速度
		KExtents bbox = pGeometry->GetExtents();
		double intersectL = max(clipL,bbox.Xmin);
		double intersectR = min(clipR,bbox.Xmax);
		double intersectB = max(clipB,bbox.Ymin);
		double intersectT = min(clipT,bbox.Ymax);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			// 如果不是点数据
			if(eShapeType != ShapeType_Point)
			{
				return;
			}
		}

		// 根据图形性质分别绘制
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

	// 绘制点集合
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
	// 绘制折线集合
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
	// 绘制多边形集合
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

		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// 绘制
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(0);
		BYTE g = GetGValue(0);
		BYTE b = GetBValue(0);

		KPoint* point = (KPoint*)pGeometry;

		double xx = point->X;
		double yy = point->Y;
		pMapEngine->WorldToClient(xx,yy);
		//::Ellipse(hDC,(int)(xx-2),(int)(yy-2),(int)(xx+2),(int)(yy+2));

		// 绘制点名称
		if(!pRow)
			return;

		KField* pField = pRow->GetField(_T("NAME"));
		if(!pField)
			return;

		KString strName = pField->GetStringValue();
		if(strName.IsEmpty())
			return;

		// 字体
		LOGFONT& lf = pLayerProperty->LogFont();
		FontFamily fontFamily(KString::ToWChar(pLayerProperty->LogFont().lfFaceName));
		REAL fontPoint = (REAL)(-1.0 * pLayerProperty->LogFont().lfHeight / 12 * 9);
		WORD wFontStyle = 0;
		if(lf.lfWeight > 400)	wFontStyle |= 0x01;
		if(lf.lfItalic)			wFontStyle |= 0x02;
		if(lf.lfUnderline)		wFontStyle |= 0x04;
		if(lf.lfStrikeOut)		wFontStyle |= 0x08;

		// 字体颜色
		COLORREF clrFont = pLayerProperty->FontColor();
		r = GetRValue(clrFont);
		g = GetGValue(clrFont);
		b = GetBValue(clrFont);

		// 站点名称
		Gdiplus::Font fontName(&fontFamily,fontPoint,(FontStyle)wFontStyle,Unit::UnitPoint);
		StringFormat strfmt;
		strfmt.SetAlignment(StringAlignment::StringAlignmentNear);
		strfmt.SetAlignment(StringAlignment::StringAlignmentCenter);
		strfmt.SetLineAlignment(StringAlignment::StringAlignmentCenter);

		// 画刷
		SolidBrush brushStationNameBold(Color(a,r,g,b));	// 站点名称颜色

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

		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 透明度
		int iTransparency = pLayerProperty->Transparency();
		// 颜色
		COLORREF clr = pLayerProperty->GetColor();
		// 线宽
		int iLineWidth = pLayerProperty->LineWidth();
		// 线型
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
			KField* pField = pRow->GetField(_T("CLIP_FLAG"));	// 剪切标记
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

		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 透明度
		int iTransparency = pLayerProperty->Transparency();
		// 颜色
		COLORREF clr = pLayerProperty->GetColor();
		// 线宽
		int iLineWidth = pLayerProperty->LineWidth();
		// 线型
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

			// 名称
			pField = pRow->GetField(_T("NAME"));

			// 填充颜色
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
			// 选择的多边形加粗
			//pen.SetWidth(2);
		}

		pGraphics->DrawPolygon(&pen,ptf,iPointCount);

		_deletea(ptf);

		//pGraphics->DrawPath(Pen)
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
