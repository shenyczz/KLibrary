#include "StdAfx.h"
#include "KBlackboardRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KBlackboardRender::KBlackboardRender(void)
	{
		m_pImage = NULL;
	}

	KBlackboardRender::~KBlackboardRender(void)
	{
		_delete(m_pImage);
	}

	// 绘制
	void KBlackboardRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		// 检查传入的参数
		m_hDC = hDC;
		if(NULL == hDC)
			return;

		m_lpvData = lpvData;
		if(NULL == lpvData)
			return;

		// 地图控件
		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;
		if(!pMapEngine)
			return;

		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// 是否可见
		if(!pLayerProperty->IsVisible())
			return;

		// 数据提供者
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 数据对象
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return;

		// GDI+ 画布
		Graphics graphics(hDC);

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

		// 绘制对象
		KObjectPtrCollection* pObjectPtrCollection = pBlackboardData->GetObjectPtrCollection();
		if(pObjectPtrCollection)
		{
			this->DrawObjects(&graphics,pObjectPtrCollection);
		}

		return;
	}

	// 绘制对象
	void KBlackboardRender::DrawObjects(Graphics* pGraphics, KObjectPtrCollection* pObjectPtrCollection)
	{
		int iCount = pObjectPtrCollection->Count();
		for(int i=0;i<iCount;i++)
		{
			KObject* pObject = pObjectPtrCollection->GetAt(i);
			if(!pObject)
				continue;

			int type = pObject->GetType();
			switch(type)
			{
			case GeometryType_Point:
				this->drawPoint(pGraphics,(KPoint*)pObject);
				break;
			case GeometryType_Line:
				this->drawLine(pGraphics,(KLine*)pObject);
				break;
			case GeometryType_Rect:
				this->drawRect(pGraphics,(KRect*)pObject);
				break;
			case GeometryType_Ellipse:
				this->drawEllipse(pGraphics,(KEllipse*)pObject);
				break;
			case GeometryType_Polyline:
				this->drawPolyline(pGraphics,(KPolyline*)pObject);
				break;
			case GeometryType_Polygon:
				this->drawPolygon(pGraphics,(KPolygon*)pObject);
				break;
			case GeometryType_Curve:
				this->drawCurve(pGraphics,(KCurve*)pObject);
				break;

			case GeometryType_Label:
				this->drawLabel(pGraphics,(KLabel*)pObject);
				break;
			case GeometryType_Legend:
				this->drawLegend(pGraphics,(KLegend*)pObject);
				break;
			}
		}
		return;
	}

	// 绘制一个点
	void KBlackboardRender::drawPoint(Graphics* pGraphics, KPoint* pPoint)
	{
		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		KExtents box = pPoint->GetExtents();	// 世界坐标
		double x = box.Xmin;
		double y = box.Ymax;	// 取得左上角坐标

		// 修改点的坐标
		box.GetCenter(pPoint->X,pPoint->Y);

		pMapEngine->WorldToClient(x,y);
		pPoint->SetExtentsClient(KExtents(x-5,y-5,x+6,y+6));	// 绘制时记录设备坐标位置

		// 绘制选中区域
		if(pPoint->IsSelected())
		{
			Pen pen(Color::Red);
			pen.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->FillRectangle(&SolidBrush(Color(64,0,0,252)),RectF((REAL)(x-5),(REAL)(y-5),(REAL)10,(REAL)10));
		}

		// 颜色
		COLORREF clr = pPoint->GetColor();
		BYTE a,r,g,b;
		a = 255 * (100 - pPoint->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrPoint(a,r,g,b);

		// 点尺寸
		RectF rcf((REAL)(x-3),REAL(y-3),(REAL)6,(REAL)6);
		pGraphics->FillEllipse(&SolidBrush(clrPoint),rcf);
		pGraphics->DrawEllipse(&Pen(clrPoint),rcf);

		return;
	}
	// 绘制线段
	void KBlackboardRender::drawLine(Graphics* pGraphics, KLine* pLine)
	{
		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 取得包围盒(移动后包围盒改变而图形坐标没有变)
		KExtents box = pLine->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// 先修改线段坐标
		double x,y;
		pLine->GetMinCoordinate(x,y);	// 线段的最小坐标

		// 求出偏移量
		double dx = xmin - x;
		double dy = ymin - y;

		// 修改线段坐标
		pLine->Offset(dx,dy);

		// 在此把包围盒转换为设备坐标并保存
		// 设备坐标是左上角，世界坐标和i坐下角的xy为最小
		// 注意图形宽度或高度为0的情况
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);
		if(fabs(xmax-xmin)<EPS)
		{
			xmin -= 2;	xmax += 3;
		}
		if(fabs(ymax-ymin)<EPS)
		{
			ymin += 2;	ymax -= 2;
		}
		pLine->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// 注意： KExtents(xmin,ymax,xmax,ymin)

		// 绘制
		KPoint pt1 = pLine->GetHeadPoint();
		double x1 = pt1.X;
		double y1 = pt1.Y;
		pMapEngine->WorldToClient(x1,y1);

		KPoint pt2 = pLine->GetTailPoint();
		double x2 = pt2.X;
		double y2 = pt2.Y;
		pMapEngine->WorldToClient(x2,y2);

		// 下面绘制
		COLORREF clr = pLine->GetColor();
		BYTE a,r,g,b;
		a = 255 * (100 - pLine->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrLine(a,r,g,b);
		Pen penLine(clrLine,(REAL)pLine->GetLineWidth());
		penLine.SetDashStyle((DashStyle)pLine->GetLineStyle());
		pGraphics->DrawLine(&penLine,(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);

		// 绘制选择区域
		if(pLine->IsSelected())
		{
			KExtents bbxClient = pLine->GetExtentsClient();
			RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			//pGraphics->FillRectangle(&SolidBrush(Color(64,0,0,252)),rcf);
			pGraphics->DrawLine(&penSelect,(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);
		}

		return;
	}
	// 绘制一个矩形
	void KBlackboardRender::drawRect(Graphics* pGraphics, KRect* pRect)
	{
		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// 取得包围盒(移动后包围盒改变而图形坐标没有变)
		KExtents box = pRect->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// 根据包围盒修改矩形坐标
		double x,y;
		pRect->GetMinCoordinate(x,y);	// 矩形的最小坐标

		// 求出偏移量
		double dx = xmin - x;
		double dy = ymin - y;

		// 修改矩形坐标
		pRect->Offset(dx,dy);

		// 在此把包围盒转换为设备坐标并保存
		// 设备坐标是左上角(世界坐标是左下角)的xy为最小
		// 注意图形宽度或高度为0的情况
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);
		if(fabs(xmax-xmin)<EPS)
		{
			xmin -= 2;	xmax += 3;
		}
		if(fabs(ymax-ymin)<EPS)
		{
			ymin += 2;	ymax -= 2;
		}
		pRect->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// 注意： KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pRect->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());
		/// 下面绘制
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pRect->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///绘制图形
		// 图形颜色
		clr = pRect->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// 图形画笔
		Pen penGeometry(clrGeometry,(REAL)pRect->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pRect->GetLineStyle());

		// 填充颜色
		clr = pRect->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		//// 绘制矩形
		//Color clrRect;
		//clrRect.SetFromCOLORREF(pRect->GetColor());
		//Pen penRect(clrRect,(REAL)pRect->GetLineWidth());
		//penRect.SetDashStyle((DashStyle)pRect->GetLineStyle());

		//Color clrFill;
		//clrFill.SetFromCOLORREF(pRect->GetFillColor());

		Brush* pBrush = NULL;
		//SolidBrush			单色换刷
		SolidBrush solidBrush(clrFill);
		//HatchBrush			影线换刷
		HatchBrush hatchBrush((HatchStyle)pRect->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			图像画刷
		//LinearGradientBrush	线型过度画刷

		int iFillType = pRect->GetFillType();
		switch(iFillType)
		{
		case 0:
			pBrush = &solidBrush;
			break;

		case 1:
			pBrush = &hatchBrush;
			break;

		default:
			pBrush = NULL;
			break;
		}

		if(pBrush)
		{
			pGraphics->FillRectangle(pBrush,rcf);
		}

		// 绘制边界
		pGraphics->DrawRectangle(&penGeometry,rcf);

		// 绘制选择区域
		if(pRect->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->DrawRectangle(&penSelect,rcf);
		}

		return;
	}
	// 绘制一个椭圆
	void KBlackboardRender::drawEllipse(Graphics* pGraphics, KEllipse* pEllipse)
	{
		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// 取得包围盒(移动后包围盒改变而图形坐标没有变)
		KExtents box = pEllipse->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// 根据包围盒修改矩形坐标
		double x,y;
		pEllipse->GetMinCoordinate(x,y);	// 矩形的最小坐标

		// 求出偏移量
		double dx = xmin - x;
		double dy = ymin - y;

		// 修改矩形坐标
		pEllipse->Offset(dx,dy);

		// 在此把包围盒转换为设备坐标并保存
		// 设备坐标是左上角(世界坐标是左下角)的xy为最小
		// 注意图形宽度或高度为0的情况
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);
		if(fabs(xmax-xmin)<EPS)
		{
			xmin -= 2;	xmax += 3;
		}
		if(fabs(ymax-ymin)<EPS)
		{
			ymin += 2;	ymax -= 2;
		}
		pEllipse->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// 注意： KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pEllipse->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// 下面绘制
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pEllipse->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///绘制图形
		// 图形颜色
		clr = pEllipse->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// 图形画笔
		Pen penGeometry(clrGeometry,(REAL)pEllipse->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pEllipse->GetLineStyle());

		// 填充颜色
		clr = pEllipse->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		//// 绘制矩形
		//Color clrEllipse;
		//clrEllipse.SetFromCOLORREF(pEllipse->GetColor());
		//Pen penEllipse(clrEllipse,(REAL)pEllipse->GetLineWidth());
		//penEllipse.SetDashStyle((DashStyle)pEllipse->GetLineStyle());

		////==============================
		////pRect->SetFillType(1);
		////pRect->SetFillColor(255);
		////==============================

		//Color clrFill;
		//clrFill.SetFromCOLORREF(pEllipse->GetFillColor());

		Brush* pBrush = NULL;
		//SolidBrush			单色换刷
		SolidBrush solidBrush(clrFill);
		//HatchBrush			影线换刷
		HatchBrush hatchBrush((HatchStyle)pEllipse->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			图像画刷
		//LinearGradientBrush	线型过度画刷

		int iFillType = pEllipse->GetFillType();
		switch(iFillType)
		{
		case 0:
			pBrush = &solidBrush;
			break;

		case 1:
			pBrush = &hatchBrush;
			break;

		default:
			pBrush = NULL;
			break;
		}

		if(pBrush)
		{
			pGraphics->FillEllipse(pBrush,rcf);
		}

		// 绘制图形
		pGraphics->DrawEllipse(&penGeometry,rcf);

		// 绘制选择区域
		if(pEllipse->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			//pGraphics->FillEllipse(&SolidBrush(Color(16,0,0,252)),rcf);
			pGraphics->DrawEllipse(&penSelect,rcf);
		}

		return;
	}
	// 绘制一个折线
	void KBlackboardRender::drawPolyline(Graphics* pGraphics, KPolyline* pPolyline)
	{
		int i;

		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// 取得包围盒(移动后包围盒改变而图形坐标没有变)
		KExtents box = pPolyline->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// 根据包围盒修改矩形坐标
		double x,y;
		pPolyline->GetMinCoordinate(x,y);	// 矩形的最小坐标

		// 求出偏移量
		double dx = xmin - x;
		double dy = ymin - y;

		// 修改矩形坐标
		pPolyline->Offset(dx,dy);

		// 在此把包围盒转换为设备坐标并保存
		// 设备坐标是左上角(世界坐标是左下角)的xy为最小
		// 注意图形宽度或高度为0的情况
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);
		if(fabs(xmax-xmin)<EPS)
		{
			xmin -= 2;	xmax += 3;
		}
		if(fabs(ymax-ymin)<EPS)
		{
			ymin += 2;	ymax -= 2;
		}
		pPolyline->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// 注意： KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pPolyline->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// 下面绘制
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pPolyline->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///绘制图形
		// 图形颜色
		clr = pPolyline->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// 图形画笔
		Pen penGeometry(clrGeometry,(REAL)pPolyline->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pPolyline->GetLineStyle());

		// 填充颜色
		clr = pPolyline->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		// 填充画刷
		Brush* pBrush = NULL;
		//SolidBrush			单色换刷
		SolidBrush solidBrush(clrFill);
		//HatchBrush			影线换刷
		HatchBrush hatchBrush((HatchStyle)pPolyline->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			图像画刷
		//LinearGradientBrush	线型过度画刷

		// 填充类型
		int iFillType = pPolyline->GetFillType();
		switch(iFillType)
		{
		case 0:
			pBrush = &solidBrush;
			break;

		case 1:
			pBrush = &hatchBrush;
			break;

		default:
			pBrush = NULL;
			break;
		}

		// 点集合
		int iPointCount = pPolyline->Count();
		if(iPointCount==0)
			return;

		PointF* ptf = new PointF[iPointCount];
		for(i=0;i<iPointCount;i++)
		{
			KPoint point = *pPolyline->GetAt(i);

			double xx = point.X;
			double yy = point.Y;

			pMapEngine->WorldToClient(xx,yy);

			ptf[i].X = (REAL)xx;
			ptf[i].Y = (REAL)yy;
		}

		// 填充
		if(pBrush)
		{
			pGraphics->FillPolygon(pBrush,ptf,iPointCount);
		}

		// 绘制边界
		pGraphics->DrawLines(&penGeometry,ptf,iPointCount);

		// 绘制选择区域
		if(pPolyline->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->DrawLines(&penSelect,ptf,iPointCount);
		}

		_deletea(ptf);

		return;
	}
	// 绘制一个多边形
	void KBlackboardRender::drawPolygon(Graphics* pGraphics, KPolygon* pPolygon)
	{
		int i;

		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// 取得包围盒(移动后包围盒改变而图形坐标没有变)
		KExtents box = pPolygon->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// 根据包围盒修改矩形坐标
		double x,y;
		pPolygon->GetMinCoordinate(x,y);	// 矩形的最小坐标

		// 求出偏移量
		double dx = xmin - x;
		double dy = ymin - y;

		// 修改矩形坐标
		pPolygon->Offset(dx,dy);

		// 在此把包围盒转换为设备坐标并保存
		// 设备坐标是左上角(世界坐标是左下角)的xy为最小
		// 注意图形宽度或高度为0的情况
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);
		if(fabs(xmax-xmin)<EPS)
		{
			xmin -= 2;	xmax += 3;
		}
		if(fabs(ymax-ymin)<EPS)
		{
			ymin += 2;	ymax -= 2;
		}
		pPolygon->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// 注意： KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pPolygon->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// 下面绘制
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pPolygon->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///绘制图形
		// 图形颜色
		clr = pPolygon->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// 图形画笔
		Pen penGeometry(clrGeometry,(REAL)pPolygon->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pPolygon->GetLineStyle());

		// 填充颜色
		clr = pPolygon->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		// 填充画刷
		Brush* pBrush = NULL;
		//SolidBrush			单色换刷
		SolidBrush solidBrush(clrFill);
		//HatchBrush			影线换刷
		HatchBrush hatchBrush((HatchStyle)pPolygon->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			图像画刷
		//LinearGradientBrush	线型过度画刷

		// 填充类型
		int iFillType = pPolygon->GetFillType();
		switch(iFillType)
		{
		case 0:
			pBrush = &solidBrush;
			break;

		case 1:
			pBrush = &hatchBrush;
			break;

		default:
			pBrush = NULL;
			break;
		}

		// 点集合
		int iPointCount = pPolygon->Count();
		if(iPointCount==0)
			return;

		PointF* ptf = new PointF[iPointCount];
		for(i=0;i<iPointCount;i++)
		{
			KPoint point = *pPolygon->GetAt(i);

			double xx = point.X;
			double yy = point.Y;

			pMapEngine->WorldToClient(xx,yy);

			ptf[i].X = (REAL)xx;
			ptf[i].Y = (REAL)yy;
		}

		// 填充
		if(pBrush)
		{
			pGraphics->FillPolygon(pBrush,ptf,iPointCount);
		}

		// 绘制边界
		pGraphics->DrawPolygon(&penGeometry,ptf,iPointCount);

		// 绘制选择区域
		if(pPolygon->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->DrawPolygon(&penSelect,ptf,iPointCount);
		}

		_deletea(ptf);

		return;
	}
	// 绘制曲线
	void KBlackboardRender::drawCurve(Graphics* pGraphics, KCurve* pCurve)
	{
		int i;

		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// 取得包围盒(移动后包围盒改变而图形坐标没有变)
		KExtents box = pCurve->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// 根据包围盒修改矩形坐标
		double x,y;
		pCurve->GetMinCoordinate(x,y);	// 矩形的最小坐标

		// 求出偏移量
		double dx = xmin - x;
		double dy = ymin - y;

		// 修改图形坐标
		pCurve->Offset(dx,dy);

		// 在此把包围盒转换为设备坐标并保存
		// 设备坐标是左上角(世界坐标是左下角)的xy为最小
		// 注意图形宽度或高度为0的情况
		pMapEngine->WorldToClient(xmin,ymin);
		pMapEngine->WorldToClient(xmax,ymax);
		if(fabs(xmax-xmin)<EPS)
		{
			xmin -= 2;	xmax += 3;
		}
		if(fabs(ymax-ymin)<EPS)
		{
			ymin += 2;	ymax -= 2;
		}
		pCurve->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// 注意： KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pCurve->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// 下面绘制
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pCurve->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///绘制图形
		// 图形颜色
		clr = pCurve->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// 图形画笔
		Pen penGeometry(clrGeometry,(REAL)pCurve->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pCurve->GetLineStyle());

		// 填充颜色
		clr = pCurve->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		// 填充画刷
		Brush* pBrush = NULL;
		//SolidBrush			单色换刷
		SolidBrush solidBrush(clrFill);
		//HatchBrush			影线换刷
		HatchBrush hatchBrush((HatchStyle)pCurve->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			图像画刷
		//LinearGradientBrush	线型过度画刷

		// 填充类型
		int iFillType = pCurve->GetFillType();
		switch(iFillType)
		{
		case 0:
			pBrush = &solidBrush;
			break;

		case 1:
			pBrush = &hatchBrush;
			break;

		default:
			pBrush = NULL;
			break;
		}

		// 点集合
		int iPointCount = pCurve->Count();
		if(iPointCount==0)
			return;

		PointF* ptf = new PointF[iPointCount];
		for(i=0;i<iPointCount;i++)
		{
			KPoint point = *pCurve->GetAt(i);

			double xx = point.X;
			double yy = point.Y;

			pMapEngine->WorldToClient(xx,yy);

			ptf[i].X = (REAL)xx;
			ptf[i].Y = (REAL)yy;
		}

		// 曲线的张力系数
		REAL tension = (REAL)pCurve->GetTension();


		// 填充
		if(pBrush)
		{
			pGraphics->FillClosedCurve(pBrush, ptf, iPointCount, FillMode::FillModeWinding, tension);
		}

		// 绘制边界
		if(pCurve->IsClose())
			pGraphics->DrawClosedCurve(&penGeometry,ptf,iPointCount,tension);
		else
			pGraphics->DrawCurve(&penGeometry,ptf,iPointCount,tension);

		// 绘制选择区域
		if(pCurve->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			if(pCurve->IsClose())
				pGraphics->DrawClosedCurve(&penSelect,ptf,iPointCount,tension);
			else
				pGraphics->DrawCurve(&penSelect,ptf,iPointCount,tension);
		}

		_deletea(ptf);

		return;
	}
	// 绘制一个标注
	void KBlackboardRender::drawLabel(Graphics* pGraphics, KLabel* pLabel)
	{
		// 地图控件
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 标注文本
		WCHAR wszText[MAX_PATH] = L"";
		wcscpy(wszText,KString::ToWChar(pLabel->GetText()));

		// 标注文本世界坐标
		KExtents box = pLabel->GetExtents();
		double x = box.Xmin;
		double y = box.Ymax;	// 取得左上角坐标
		pMapEngine->WorldToClient(x,y);
		PointF pointLabel((REAL)x, (REAL)y);

		// 字体
		FontFamily fontFamily(KString::ToWChar(pLabel->GetFontName()));
		REAL fontPoint = (REAL)pLabel->GetFontPoint();
		FontStyle eFontStyle = (FontStyle)pLabel->GetFontStyle();
		Gdiplus::Font fontLabel(&fontFamily,fontPoint,eFontStyle,Unit::UnitPoint);

		// 字符串格式
		StringFormat stringFormat;

		// 包围盒
		RectF rcf;
		pGraphics->MeasureString(wszText,-1,&fontLabel,pointLabel,&stringFormat,&rcf);
		pLabel->SetExtentsClient(KExtents(rcf.X,rcf.Y,rcf.GetRight(),rcf.GetBottom()));

		// 字体颜色
		COLORREF clr = pLabel->GetColor();
		BYTE a,r,g,b;
		a = 255 * (100 - pLabel->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		Color fontColor(a,r,g,b);
		//fontColor.SetFromCOLORREF(pLabel->GetColor());
		SolidBrush brushLabel(fontColor);

		// 绘制选中区域
		if(pLabel->IsSelected())
		{
			Pen pen(Color::Red);
			pen.SetDashStyle(DashStyle::DashStyleDash);
			//pGraphics->DrawRectangle(&pen,rcf);
			pGraphics->FillRectangle(&SolidBrush(Color(64,0,0,252)),rcf);
		}

		// 绘制标注文本
		pGraphics->SetInterpolationMode(InterpolationMode::InterpolationModeDefault);
		pGraphics->DrawString(wszText,-1,
			&fontLabel,
			rcf,
			&stringFormat,
			&brushLabel
			);

		return;
	}
	// 绘制一个图例
	void KBlackboardRender::drawLegend(Graphics* pGraphics, KLegend* pLegend)
	{
		// 地图控件
		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;
		// 图层属性
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// 图像属性
		ImageAttributes imgAttributes;

		// 透明色
		ColorMap clrMap[1];
		clrMap[0].oldColor = Color(255,0,0,0);
		clrMap[0].newColor = Color(0,255,255,255);
		//imgAttributes.SetRemapTable(1,clrMap,ColorAdjustType::ColorAdjustTypeBitmap);

		// 透明度
		REAL tra = 0.01f * (100 - pLegend->GetTransparency());
		ColorMatrix clrMatrix = 
		{
			1.0, 0.0, 0.0, 0.0, 0.0,
			0.0, 1.0, 0.0, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0, 0.0,
			0.0, 0.0, 0.0, tra, 0.0,
			0.0, 0.0, 0.0, 0.0, 1.0,
		};
		imgAttributes.SetColorMatrix(&clrMatrix,ColorMatrixFlags::ColorMatrixFlagsDefault,ColorAdjustType::ColorAdjustTypeBitmap);

		Image* pImage = this->buildLegendImage(pLegend);
		if(!pImage)
			return;

		//((KImage*)pImage)->Save(_T("e:\\temp\\1.bmp"));

		// 在此把包围盒转换为设备坐标并保存
		KExtents box = pLegend->GetExtents();
		double x = box.Xmin;
		double y = box.Ymax;	// 取得左上角坐标
		pMapEngine->WorldToClient(x,y);
		RectF rcf((REAL)x,(REAL)y,(REAL)pImage->GetWidth(),(REAL)pImage->GetHeight());
		pLegend->SetExtentsClient(KExtents(rcf.X,rcf.Y,rcf.GetRight(),rcf.GetBottom()));

		// 绘制图像
		pGraphics->SetInterpolationMode(InterpolationMode::InterpolationModeDefault);
		pGraphics->DrawImage(pImage
			,rcf
			,0
			,0
			,(REAL)pImage->GetWidth()
			,(REAL)pImage->GetHeight()
			,UnitPixel
			,&imgAttributes
			);

		// 绘制选中区域
		if(pLegend->IsSelected())
		{
			Pen pen(Color::Red);
			pen.SetDashStyle(DashStyle::DashStyleDash);
			//pGraphics->DrawRectangle(&pen,rcf);
			pGraphics->FillRectangle(&SolidBrush(Color(64,0,0,252)),rcf);
		}

		return;
	}

	// 生成图例图像
	Image* KBlackboardRender::buildLegendImage(KLegend* pLegend)
	{
		KPalette* pPalette = pLegend->GetPalette();
		if(!pPalette)
			return NULL;

		int iCount = pPalette->Count();

		int topshift = 16;						// 上部位移

		int iWidth = pLegend->Width;			// 图例图像宽度
		if(iWidth<0) iWidth = 100;

		int margin = pLegend->Margin;			// 页边空白
		if(margin>iWidth/2) margin = iWidth/2 - 10;

		int itemWidth = pLegend->ItemWidth;		// 图例条目宽度
		if(itemWidth>=iWidth) itemWidth = iWidth -10;

		int itemHeight = pLegend->ItemHeight;	// 图例条目高度
		if(itemHeight>=36) itemHeight = 12;

		int itemGap = pLegend->ItemGap;			// 图例条目间隙
		if(itemGap<0 || itemGap>=iWidth) itemGap = 0;

		int iUnitWidth = pLegend->UnitWidth;		// 单位宽度

		// 计算图例图像高度
		int iHeight = topshift + (itemHeight+itemGap) * iCount + margin * 2;


		int iFontSize = 9;
		int iItemLabelGap = 5;

		if(itemHeight<=9) iFontSize = itemHeight-1;
		if(iFontSize<0) iFontSize = 9;

		_delete(m_pImage);
		m_pImage = new Bitmap(iWidth,iHeight);
		if(m_pImage->GetLastStatus() != Status::Ok)
		{
			_delete(m_pImage);
			::MessageBox(NULL,_T("生成图像错误! - KLegendData::BuildLegend()(...)"),_T("信息"),MB_OK|MB_ICONQUESTION);
			return NULL;
		}

		// GDI+ 画布
		Graphics g(m_pImage);

		// 字符串格式
		StringFormat stringFormat;

		// 绘制边框
		g.FillRectangle(&SolidBrush(Color(255,255,255,255)),Rect(0,0,iWidth,iHeight));
		g.DrawRectangle(&Pen(Color::Black),Rect(0,0,iWidth-1,iHeight-1));

		// 绘制标题
		stringFormat.SetAlignment(StringAlignment::StringAlignmentCenter);
		g.DrawString(L"图例"
			,-1
			,&Gdiplus::Font(L"宋体",9,FontStyle::FontStyleBold)
			,RectF(1.0f*margin, 1.0f*margin, 1.0f*iWidth-margin, 1.0f*topshift)
			,&stringFormat
			,&SolidBrush(Color::Black)
			);

		TCHAR szLabel[MAX_PATH] = _T("");
		stringFormat.SetAlignment(StringAlignment::StringAlignmentFar);
		for(int i=0;i<iCount;i++)
		{
			KPaletteItem* pPaletteItem = pPalette->GetAt(i);

			// 绘制图例色标
			g.FillRectangle(&SolidBrush(Color(pPaletteItem->R,pPaletteItem->G,pPaletteItem->B))
				,Rect(margin,topshift+margin+(itemHeight+itemGap)*i,itemWidth,itemHeight));
			g.DrawRectangle(&Pen(Color::Black),
				Rect(margin,topshift+margin+(itemHeight+itemGap)*i,itemWidth,itemHeight));

			// 绘制图例文本
			if(NULL == pPaletteItem->GetComment() || 0 == _tcslen(pPaletteItem->GetComment()))
			{
				_stprintf(szLabel,_T("%.2f"),pPaletteItem->Value());
			}
			else
			{
				_stprintf(szLabel,_T("%s"),pPaletteItem->GetComment());
			}

			REAL xLabel = 1.0f * itemWidth + iItemLabelGap;
			REAL yLabel = 1.0f * (itemHeight + itemGap) * i + margin + topshift;
			REAL wLabel = 1.0f * iWidth - xLabel - iUnitWidth;
			REAL hLabel = 1.0f * itemHeight;
			g.DrawString(KString::ToWChar(szLabel)
				,-1
				,&Gdiplus::Font(L"宋体",1.0f * iFontSize)
				,RectF(xLabel, yLabel, wLabel, hLabel)
				,&stringFormat
				,&SolidBrush(Color::Black)
				);

			// 标注单位
			if(i==0)
			{
				g.DrawString(KString::ToWChar(pLegend->GetUnit()),-1,&Gdiplus::Font(L"宋体",(REAL)iFontSize)
					,PointF(1.0f * iWidth - iUnitWidth, margin + 1.0f*(itemHeight+itemGap)*i+topshift)
					,&SolidBrush(Color::Black));
			}

		}// for(i)

		return m_pImage;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
