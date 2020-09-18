#include "StdAfx.h"
#include "KRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KRender::KRender(void)
	{
		m_pLayerProperty = NULL;
	}

	KRender::~KRender(void)
	{
		_delete(m_pLayerProperty);
	}

	// 绘制等值线
	void KRender::DrawContour(Graphics* pGraphics, KContour* pContour)
	{
		HDC hDC = this->m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KDataObject* pDataObject = pProvider->GetDataObject();
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		// 调色板
		KPalette* pPalette = pDataObject->GetPalette();
		//if(!pPalette)
		//	return;

		// 图层颜色
		COLORREF clr = pLayerProperty->GetColor();

		int iCurveCount = pContour->Count();
		for(int i=0;i<iCurveCount;i++)
		{
			KCurve* pCurve = pContour->GetCurve(i);
			// 取得曲线值
			double dValue = pCurve->GetValue();
			// 取得渲染颜色
			if(pLayerProperty->IsRainbow() && pPalette)
			{
				clr = pPalette->GetColor(dValue);
			}
			// 是否加粗显示
			BOOL bBold = fabs(dValue - pDataInfo->dBoldContourValue) < EPS;
			this->DrawCurve(pGraphics,pCurve,clr,bBold);
		}

		return;
	}
	// 填充等值线
	void KRender::FillContour(Graphics* pGraphics, KContour* pContour)
	{
		HDC hDC = this->m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KDataObject* pDataObject = pProvider->GetDataObject();
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		// 调色板
		KPalette* pPalette = pDataObject->GetPalette();
		if(!pPalette)
			return;

		// 图层颜色
		COLORREF clr = pLayerProperty->GetColor();

		int iCurveCount = pContour->Count();
		for(int i=0;i<iCurveCount;i++)
		{
			KCurve* pCurve = pContour->GetCurve(i);
			if(!pCurve->IsClose())
			{
				continue;
			}

			// 取得曲线值
			double dValue = pCurve->GetValue();

			// 取得渲染颜色
			//COLORREF clr = pPalette->GetSegColor(dValue);
			//COLORREF clr = pPalette->GetDitColor(dValue);
			if(pPalette)
			{
				clr = pPalette->GetColor(dValue);
			}

			this->FillCurve(pGraphics,pCurve,clr);
		}

		return;
	}
	// 标注等值线
	void KRender::LabelContour(Graphics* pGraphics, KContour* pContour)
	{
		HDC hDC = this->m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KDataObject* pDataObject = pProvider->GetDataObject();
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		// 调色板
		KPalette* pPalette = pDataObject->GetPalette();
		// 图层颜色
		COLORREF clr = pLayerProperty->GetColor();

		int iCurveCount = pContour->Count();
		for(int i=0;i<iCurveCount;i++)
		{
			KCurve* pCurve = pContour->GetCurve(i);

			// 取得曲线值
			double dValue = pCurve->GetValue();

			// 取得渲染颜色
			if(pLayerProperty->IsRainbow())
			{
				//clr = this->GetRenderColor(pDataInfo,dValue);
				clr = pPalette->GetColor(dValue);
			}

			this->LabelCurve(pGraphics,pCurve,clr);
		}

		return;
	}

	// 绘制曲线
	void KRender::DrawCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr, BOOL bBold)
	{
		int i;

		HDC hDC = this->m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KDataObject* pDataObject = pProvider->GetDataObject();
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		// 取得剪切矩形
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);
		// 保存剪切矩形顶点坐标
		//
		//  p0         p2
		//  +----------+
		//  |          |
		//	|          |
		//	|          |
		//	+----------+
		//	p1         p3
		//
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

		// 剪切矩形的地理坐标矩形属性(left,top right,bottom)
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
		KExtents bbox = pCurve->GetExtents();
		double intersectL = max(clipL,bbox.Xmin);
		double intersectR = min(clipR,bbox.Xmax);
		double intersectB = max(clipB,bbox.Ymin);
		double intersectT = min(clipT,bbox.Ymax);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			return;
		}

		/// 下面绘制
		///
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		int iPointCount = pCurve->Count();
		PointF* ptf = new PointF[iPointCount];
		for( i=0;i<iPointCount;i++)
		{
			KPoint point = *pCurve->GetAt(i);

			double x = point.X;
			double y = point.Y;
			pMapEngine->WorldToClient(x,y);

			ptf[i].X = (REAL)x;
			ptf[i].Y = (REAL)y;
		}

		REAL fWidth = bBold ? 2.0f : 1.0f;
		//REAL fTension = 0.15f;
		//REAL fTension = (REAL)pCurve->GetTension();
		REAL fTension = (REAL)pLayerProperty->Tension();
		pGraphics->DrawCurve(&Pen(Color(a,r,g,b),fWidth), ptf, iPointCount, fTension);
		_deletea(ptf);
		return;
	}
	// 绘制曲线
	void KRender::FillCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr)
	{
		int i;

		HDC hDC = this->m_hDC;
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KDataObject* pDataObject = pProvider->GetDataObject();
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();

		// 取得剪切矩形
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);
		// 保存剪切矩形顶点坐标
		//
		//  p0         p2
		//  +----------+
		//  |          |
		//	|          |
		//	|          |
		//	+----------+
		//	p1         p3
		//
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

		// 剪切矩形的地理坐标矩形属性(left,top right,bottom)
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
		KExtents bbox = pCurve->GetExtents();
		double intersectL = max(clipL,bbox.Xmin);
		double intersectR = min(clipR,bbox.Xmax);
		double intersectB = max(clipB,bbox.Ymin);
		double intersectT = min(clipT,bbox.Ymax);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			return;
		}

		/// 下面绘制
		///
		BYTE a = 255 * (100 - pLayerProperty->GetTransparency()) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		int iPointCount = pCurve->Count();
		PointF* ptf = new PointF[iPointCount];
		for( i=0;i<iPointCount;i++)
		{
			KPoint point = *pCurve->GetAt(i);

			double x = point.X;
			double y = point.Y;
			pMapEngine->WorldToClient(x,y);

			ptf[i].X = (REAL)x;
			ptf[i].Y = (REAL)y;
		}

		// 填充
		//REAL fTension = 0.15f;
		//REAL fTension = (REAL)pCurve->GetTension();
		REAL fTension = (REAL)pLayerProperty->Tension();
		pGraphics->FillClosedCurve(&SolidBrush(Color(a,r,g,b)), ptf, iPointCount, FillMode::FillModeWinding, fTension);
		_deletea(ptf);

		return;
	}
	// 标注曲线
	void KRender::LabelCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr)
	{
		int i;

		ASSERT(m_hDC);
		HDC hDC = m_hDC;

		//Graphics graphics(hDC);

		ASSERT(m_lpvData);
		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;

		// 取得剪切矩形
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);

		// 保存剪切矩形顶点坐标
		//
		//  p0         p2
		//  +----------+
		//  |          |
		//	|          |
		//	|          |
		//	+----------+
		//	p1         p3
		//
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

		// 剪切矩形的地理坐标矩形属性(left,top right,bottom)
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
		KExtents bbox = pCurve->GetExtents();
		double intersectL = max(clipL,bbox.Xmin);
		double intersectR = min(clipR,bbox.Xmax);
		double intersectB = max(clipB,bbox.Ymin);
		double intersectT = min(clipT,bbox.Ymax);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			return;
		}

		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 透明度
		int iTransparency = pLayerProperty->GetTransparency();

		/// 下面标注
		///
		//BYTE a = 255;
		BYTE a = 255 * (100 - iTransparency) / 100;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		// 形成标注文本
		TCHAR szTemp[MAX_PATH];
		CString strLabel(_T("")),strFormat(_T(""));
		double dValue = pCurve->GetValue();
		double n,dFraction;	
		dFraction = modf(dValue,&n);	//n-整数部分,dFraction-小数部分
		_itot((int)n,szTemp,10);		//把整数变成字符串
		int len = (int)_tcslen(szTemp);	//整数部分长度
		if(fabs(dFraction)<EPS)		//小数部分==0
		{
			strFormat.Format(_T("%%%d.0f"),len);	//只要整数部分
		}
		else
		{
			strFormat.Format(_T("%%%d.1f"),len+2);	//保留一位小数
		}
		strLabel.Format(strFormat,dValue);		//9999.9

		// 绘制文本
		Gdiplus::Font font(L"Arial",8);
		RectF rc(0,0,0,0);
		PointF fltPoint(0,0);
		StringFormat strfmt;
		strfmt.SetAlignment(StringAlignment::StringAlignmentCenter);
		strfmt.SetLineAlignment(StringAlignment::StringAlignmentCenter);

		//HatchBrush hatchBrush(HatchStyle::HatchStyle05Percent,Color(a,r,g,b),Color(a,0,0,0));
		SolidBrush solidBrush(Color(a,r,g,b));

		//if(m_pLayerProperty->IsRainbow())
		//{
		//	solidBrush.SetColor(Color(a,r,g,b));
		//}
		//else
		//{
		//	solidBrush.SetColor(Color(a,128,128,128));
		//}

		Brush* pBrush = &solidBrush;

		// 1.先在起点和标注
		int iPointCount = pCurve->Count();
		if(iPointCount<3)
			return;

		double xx[3],yy[3];
		xx[0] = pCurve->GetAt(0)->X;
		yy[0] = pCurve->GetAt(0)->Y;
		xx[2] = pCurve->GetAt(iPointCount-1)->X;
		yy[2] = pCurve->GetAt(iPointCount-1)->Y;

		// 坐标变换
		pMapEngine->WorldToClient(xx[0],yy[0]);
		pMapEngine->WorldToClient(xx[2],yy[2]);

		pGraphics->DrawString(KString::ToWChar(strLabel),-1,&font,PointF((REAL)xx[0],(REAL)yy[0]),&strfmt,pBrush);
		pGraphics->DrawString(KString::ToWChar(strLabel),-1,&font,PointF((REAL)xx[2],(REAL)yy[2]),&strfmt,pBrush);


		// 2.如果是开曲线则在中间取一点标注
		if(!pCurve->IsClose() && iPointCount >= 6)
		{
			xx[1] = pCurve->GetAt(iPointCount/2)->X;
			yy[1] = pCurve->GetAt(iPointCount/2)->Y;

			// 坐标变换
			pMapEngine->WorldToClient(xx[1],yy[1]);

			pGraphics->DrawString(KString::ToWChar(strLabel),-1,&font,PointF((REAL)xx[1],(REAL)yy[1]),&strfmt,pBrush);

			//if(m_bAlong)	//要沿着曲线的方向
			//{
			//	m_dAngle= angle(pSfxPoint[0]->x,pSfxPoint[0]->y,pSfxPoint[2]->x,pSfxPoint[2]->y)*180.0f/PI;
			//	//BuildFont(pDC);
			//	m_Font.DeleteObject();
			//	m_Font.CreatePointFont(pDC,m_iFontPoint,m_lpszFaceName,m_bBold,m_bItalic);
			//	pDC->SelectObject(&m_Font);

			//	TRACE("%s m_dAngle = %8.3lf\n",lpszLabel,m_dAngle);
			//}
		}

		return;
	}

	// 标注数值
	void KRender::LabelValue(Graphics* pGraphics, double X, double Y, double V, COLORREF clr)
	{
		int i;

		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;

		// 取得剪切矩形
		int x,y,w,h;
		pMapEngine->GetClipRect(x,y,w,h);

		// 保存剪切矩形顶点坐标
		//
		//  p0         p2
		//  +----------+
		//  |          |
		//	|          |
		//	|          |
		//	+----------+
		//	p1         p3
		//
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

		// 剪切矩形的地理坐标矩形属性(left,top right,bottom)
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
		double intersectL = max(clipL,X-0.01);
		double intersectR = min(clipR,X+0.01);
		double intersectB = max(clipB,Y-0.01);
		double intersectT = min(clipT,Y+0.01);
		if ( intersectL > intersectR || intersectB > intersectT )
		{
			return;
		}

		/// 下面标注
		///
		BYTE a = 255;
		BYTE r = GetRValue(clr);
		BYTE g = GetGValue(clr);
		BYTE b = GetBValue(clr);

		// 形成标注文本
		TCHAR szTemp[MAX_PATH];
		CString strLabel(_T("")),strFormat(_T(""));
		double dValue = V;
		double n,dFraction;	
		dFraction = modf(dValue,&n);	//n-整数部分,dFraction-小数部分
		_itot((int)n,szTemp,10);		//把整数变成字符串
		int len = (int)_tcslen(szTemp);	//整数部分长度
		if(fabs(dFraction)<EPS)		//小数部分==0
		{
			strFormat.Format(_T("%%%d.0f"),len);	//只要整数部分
		}
		else
		{
			strFormat.Format(_T("%%%d.1f"),len+2);	//保留一位小数
		}
		strLabel.Format(strFormat,dValue);		//9999.9

		// 绘制文本
		Gdiplus::Font font(L"Arial",8);
		RectF rc(0,0,0,0);
		PointF fltPoint(0,0);
		StringFormat strfmt;
		strfmt.SetAlignment(StringAlignment::StringAlignmentCenter);
		strfmt.SetLineAlignment(StringAlignment::StringAlignmentCenter);

		HatchBrush hatchBrush(HatchStyle::HatchStyle05Percent,Color(a,r,g,b),Color(a,0,0,0));
		SolidBrush solidBrush(Color(a,r,g,b));

		Brush* pBrush = &solidBrush;

		// 坐标变换
		double xx = X;
		double yy = Y;
		pMapEngine->WorldToClient(xx,yy);

		pGraphics->DrawString(KString::ToWChar(strLabel),-1,&font,PointF((REAL)xx,(REAL)yy),&strfmt,pBrush);

		return;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
