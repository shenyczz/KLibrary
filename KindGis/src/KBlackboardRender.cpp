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

	// ����
	void KBlackboardRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		// ��鴫��Ĳ���
		m_hDC = hDC;
		if(NULL == hDC)
			return;

		m_lpvData = lpvData;
		if(NULL == lpvData)
			return;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;
		if(!pMapEngine)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(!pLayerProperty)
			return;

		// �Ƿ�ɼ�
		if(!pLayerProperty->IsVisible())
			return;

		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
		if(!pBlackboardData)
			return;

		// GDI+ ����
		Graphics graphics(hDC);

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

		// ���ƶ���
		KObjectPtrCollection* pObjectPtrCollection = pBlackboardData->GetObjectPtrCollection();
		if(pObjectPtrCollection)
		{
			this->DrawObjects(&graphics,pObjectPtrCollection);
		}

		return;
	}

	// ���ƶ���
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

	// ����һ����
	void KBlackboardRender::drawPoint(Graphics* pGraphics, KPoint* pPoint)
	{
		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		KExtents box = pPoint->GetExtents();	// ��������
		double x = box.Xmin;
		double y = box.Ymax;	// ȡ�����Ͻ�����

		// �޸ĵ������
		box.GetCenter(pPoint->X,pPoint->Y);

		pMapEngine->WorldToClient(x,y);
		pPoint->SetExtentsClient(KExtents(x-5,y-5,x+6,y+6));	// ����ʱ��¼�豸����λ��

		// ����ѡ������
		if(pPoint->IsSelected())
		{
			Pen pen(Color::Red);
			pen.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->FillRectangle(&SolidBrush(Color(64,0,0,252)),RectF((REAL)(x-5),(REAL)(y-5),(REAL)10,(REAL)10));
		}

		// ��ɫ
		COLORREF clr = pPoint->GetColor();
		BYTE a,r,g,b;
		a = 255 * (100 - pPoint->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrPoint(a,r,g,b);

		// ��ߴ�
		RectF rcf((REAL)(x-3),REAL(y-3),(REAL)6,(REAL)6);
		pGraphics->FillEllipse(&SolidBrush(clrPoint),rcf);
		pGraphics->DrawEllipse(&Pen(clrPoint),rcf);

		return;
	}
	// �����߶�
	void KBlackboardRender::drawLine(Graphics* pGraphics, KLine* pLine)
	{
		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// ȡ�ð�Χ��(�ƶ����Χ�иı��ͼ������û�б�)
		KExtents box = pLine->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// ���޸��߶�����
		double x,y;
		pLine->GetMinCoordinate(x,y);	// �߶ε���С����

		// ���ƫ����
		double dx = xmin - x;
		double dy = ymin - y;

		// �޸��߶�����
		pLine->Offset(dx,dy);

		// �ڴ˰Ѱ�Χ��ת��Ϊ�豸���겢����
		// �豸���������Ͻǣ����������i���½ǵ�xyΪ��С
		// ע��ͼ�ο�Ȼ�߶�Ϊ0�����
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
		pLine->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// ע�⣺ KExtents(xmin,ymax,xmax,ymin)

		// ����
		KPoint pt1 = pLine->GetHeadPoint();
		double x1 = pt1.X;
		double y1 = pt1.Y;
		pMapEngine->WorldToClient(x1,y1);

		KPoint pt2 = pLine->GetTailPoint();
		double x2 = pt2.X;
		double y2 = pt2.Y;
		pMapEngine->WorldToClient(x2,y2);

		// �������
		COLORREF clr = pLine->GetColor();
		BYTE a,r,g,b;
		a = 255 * (100 - pLine->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrLine(a,r,g,b);
		Pen penLine(clrLine,(REAL)pLine->GetLineWidth());
		penLine.SetDashStyle((DashStyle)pLine->GetLineStyle());
		pGraphics->DrawLine(&penLine,(REAL)x1,(REAL)y1,(REAL)x2,(REAL)y2);

		// ����ѡ������
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
	// ����һ������
	void KBlackboardRender::drawRect(Graphics* pGraphics, KRect* pRect)
	{
		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// ȡ�ð�Χ��(�ƶ����Χ�иı��ͼ������û�б�)
		KExtents box = pRect->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// ���ݰ�Χ���޸ľ�������
		double x,y;
		pRect->GetMinCoordinate(x,y);	// ���ε���С����

		// ���ƫ����
		double dx = xmin - x;
		double dy = ymin - y;

		// �޸ľ�������
		pRect->Offset(dx,dy);

		// �ڴ˰Ѱ�Χ��ת��Ϊ�豸���겢����
		// �豸���������Ͻ�(�������������½�)��xyΪ��С
		// ע��ͼ�ο�Ȼ�߶�Ϊ0�����
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
		pRect->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// ע�⣺ KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pRect->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());
		/// �������
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pRect->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///����ͼ��
		// ͼ����ɫ
		clr = pRect->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// ͼ�λ���
		Pen penGeometry(clrGeometry,(REAL)pRect->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pRect->GetLineStyle());

		// �����ɫ
		clr = pRect->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		//// ���ƾ���
		//Color clrRect;
		//clrRect.SetFromCOLORREF(pRect->GetColor());
		//Pen penRect(clrRect,(REAL)pRect->GetLineWidth());
		//penRect.SetDashStyle((DashStyle)pRect->GetLineStyle());

		//Color clrFill;
		//clrFill.SetFromCOLORREF(pRect->GetFillColor());

		Brush* pBrush = NULL;
		//SolidBrush			��ɫ��ˢ
		SolidBrush solidBrush(clrFill);
		//HatchBrush			Ӱ�߻�ˢ
		HatchBrush hatchBrush((HatchStyle)pRect->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			ͼ��ˢ
		//LinearGradientBrush	���͹��Ȼ�ˢ

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

		// ���Ʊ߽�
		pGraphics->DrawRectangle(&penGeometry,rcf);

		// ����ѡ������
		if(pRect->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->DrawRectangle(&penSelect,rcf);
		}

		return;
	}
	// ����һ����Բ
	void KBlackboardRender::drawEllipse(Graphics* pGraphics, KEllipse* pEllipse)
	{
		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// ȡ�ð�Χ��(�ƶ����Χ�иı��ͼ������û�б�)
		KExtents box = pEllipse->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// ���ݰ�Χ���޸ľ�������
		double x,y;
		pEllipse->GetMinCoordinate(x,y);	// ���ε���С����

		// ���ƫ����
		double dx = xmin - x;
		double dy = ymin - y;

		// �޸ľ�������
		pEllipse->Offset(dx,dy);

		// �ڴ˰Ѱ�Χ��ת��Ϊ�豸���겢����
		// �豸���������Ͻ�(�������������½�)��xyΪ��С
		// ע��ͼ�ο�Ȼ�߶�Ϊ0�����
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
		pEllipse->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// ע�⣺ KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pEllipse->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// �������
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pEllipse->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///����ͼ��
		// ͼ����ɫ
		clr = pEllipse->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// ͼ�λ���
		Pen penGeometry(clrGeometry,(REAL)pEllipse->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pEllipse->GetLineStyle());

		// �����ɫ
		clr = pEllipse->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		//// ���ƾ���
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
		//SolidBrush			��ɫ��ˢ
		SolidBrush solidBrush(clrFill);
		//HatchBrush			Ӱ�߻�ˢ
		HatchBrush hatchBrush((HatchStyle)pEllipse->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			ͼ��ˢ
		//LinearGradientBrush	���͹��Ȼ�ˢ

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

		// ����ͼ��
		pGraphics->DrawEllipse(&penGeometry,rcf);

		// ����ѡ������
		if(pEllipse->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			//pGraphics->FillEllipse(&SolidBrush(Color(16,0,0,252)),rcf);
			pGraphics->DrawEllipse(&penSelect,rcf);
		}

		return;
	}
	// ����һ������
	void KBlackboardRender::drawPolyline(Graphics* pGraphics, KPolyline* pPolyline)
	{
		int i;

		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// ȡ�ð�Χ��(�ƶ����Χ�иı��ͼ������û�б�)
		KExtents box = pPolyline->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// ���ݰ�Χ���޸ľ�������
		double x,y;
		pPolyline->GetMinCoordinate(x,y);	// ���ε���С����

		// ���ƫ����
		double dx = xmin - x;
		double dy = ymin - y;

		// �޸ľ�������
		pPolyline->Offset(dx,dy);

		// �ڴ˰Ѱ�Χ��ת��Ϊ�豸���겢����
		// �豸���������Ͻ�(�������������½�)��xyΪ��С
		// ע��ͼ�ο�Ȼ�߶�Ϊ0�����
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
		pPolyline->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// ע�⣺ KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pPolyline->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// �������
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pPolyline->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///����ͼ��
		// ͼ����ɫ
		clr = pPolyline->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// ͼ�λ���
		Pen penGeometry(clrGeometry,(REAL)pPolyline->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pPolyline->GetLineStyle());

		// �����ɫ
		clr = pPolyline->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		// ��仭ˢ
		Brush* pBrush = NULL;
		//SolidBrush			��ɫ��ˢ
		SolidBrush solidBrush(clrFill);
		//HatchBrush			Ӱ�߻�ˢ
		HatchBrush hatchBrush((HatchStyle)pPolyline->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			ͼ��ˢ
		//LinearGradientBrush	���͹��Ȼ�ˢ

		// �������
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

		// �㼯��
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

		// ���
		if(pBrush)
		{
			pGraphics->FillPolygon(pBrush,ptf,iPointCount);
		}

		// ���Ʊ߽�
		pGraphics->DrawLines(&penGeometry,ptf,iPointCount);

		// ����ѡ������
		if(pPolyline->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->DrawLines(&penSelect,ptf,iPointCount);
		}

		_deletea(ptf);

		return;
	}
	// ����һ�������
	void KBlackboardRender::drawPolygon(Graphics* pGraphics, KPolygon* pPolygon)
	{
		int i;

		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// ȡ�ð�Χ��(�ƶ����Χ�иı��ͼ������û�б�)
		KExtents box = pPolygon->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// ���ݰ�Χ���޸ľ�������
		double x,y;
		pPolygon->GetMinCoordinate(x,y);	// ���ε���С����

		// ���ƫ����
		double dx = xmin - x;
		double dy = ymin - y;

		// �޸ľ�������
		pPolygon->Offset(dx,dy);

		// �ڴ˰Ѱ�Χ��ת��Ϊ�豸���겢����
		// �豸���������Ͻ�(�������������½�)��xyΪ��С
		// ע��ͼ�ο�Ȼ�߶�Ϊ0�����
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
		pPolygon->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// ע�⣺ KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pPolygon->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// �������
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pPolygon->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///����ͼ��
		// ͼ����ɫ
		clr = pPolygon->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// ͼ�λ���
		Pen penGeometry(clrGeometry,(REAL)pPolygon->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pPolygon->GetLineStyle());

		// �����ɫ
		clr = pPolygon->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		// ��仭ˢ
		Brush* pBrush = NULL;
		//SolidBrush			��ɫ��ˢ
		SolidBrush solidBrush(clrFill);
		//HatchBrush			Ӱ�߻�ˢ
		HatchBrush hatchBrush((HatchStyle)pPolygon->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			ͼ��ˢ
		//LinearGradientBrush	���͹��Ȼ�ˢ

		// �������
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

		// �㼯��
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

		// ���
		if(pBrush)
		{
			pGraphics->FillPolygon(pBrush,ptf,iPointCount);
		}

		// ���Ʊ߽�
		pGraphics->DrawPolygon(&penGeometry,ptf,iPointCount);

		// ����ѡ������
		if(pPolygon->IsSelected())
		{
			Pen penSelect(Color(128,0,222,0),5);
			penSelect.SetDashStyle(DashStyle::DashStyleDash);
			pGraphics->DrawPolygon(&penSelect,ptf,iPointCount);
		}

		_deletea(ptf);

		return;
	}
	// ��������
	void KBlackboardRender::drawCurve(Graphics* pGraphics, KCurve* pCurve)
	{
		int i;

		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		// ȡ�ð�Χ��(�ƶ����Χ�иı��ͼ������û�б�)
		KExtents box = pCurve->GetExtents();
		double xmin = box.Xmin;
		double ymin = box.Ymin;
		double xmax = box.Xmax;
		double ymax = box.Ymax;

		// ���ݰ�Χ���޸ľ�������
		double x,y;
		pCurve->GetMinCoordinate(x,y);	// ���ε���С����

		// ���ƫ����
		double dx = xmin - x;
		double dy = ymin - y;

		// �޸�ͼ������
		pCurve->Offset(dx,dy);

		// �ڴ˰Ѱ�Χ��ת��Ϊ�豸���겢����
		// �豸���������Ͻ�(�������������½�)��xyΪ��С
		// ע��ͼ�ο�Ȼ�߶�Ϊ0�����
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
		pCurve->SetExtentsClient(KExtents(xmin,ymax,xmax,ymin));	// ע�⣺ KExtents(xmin,ymax,xmax,ymin)

		KExtents bbxClient = pCurve->GetExtentsClient();
		RectF rcf((REAL)bbxClient.Xmin,(REAL)bbxClient.Ymin,(REAL)bbxClient.Width(),(REAL)bbxClient.Height());

		/// �������
		///
		COLORREF clr = 0;
		BYTE a,r,g,b;
		a = 255 * (100 - pCurve->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		///����ͼ��
		// ͼ����ɫ
		clr = pCurve->GetColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrGeometry(a,r,g,b);

		// ͼ�λ���
		Pen penGeometry(clrGeometry,(REAL)pCurve->GetLineWidth());
		penGeometry.SetDashStyle((DashStyle)pCurve->GetLineStyle());

		// �����ɫ
		clr = pCurve->GetFillColor();
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);
		Color clrFill(a,r,g,b);

		// ��仭ˢ
		Brush* pBrush = NULL;
		//SolidBrush			��ɫ��ˢ
		SolidBrush solidBrush(clrFill);
		//HatchBrush			Ӱ�߻�ˢ
		HatchBrush hatchBrush((HatchStyle)pCurve->GetHatchStyle(),clrFill,Color::Transparent);
		//TextureBrush			ͼ��ˢ
		//LinearGradientBrush	���͹��Ȼ�ˢ

		// �������
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

		// �㼯��
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

		// ���ߵ�����ϵ��
		REAL tension = (REAL)pCurve->GetTension();


		// ���
		if(pBrush)
		{
			pGraphics->FillClosedCurve(pBrush, ptf, iPointCount, FillMode::FillModeWinding, tension);
		}

		// ���Ʊ߽�
		if(pCurve->IsClose())
			pGraphics->DrawClosedCurve(&penGeometry,ptf,iPointCount,tension);
		else
			pGraphics->DrawCurve(&penGeometry,ptf,iPointCount,tension);

		// ����ѡ������
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
	// ����һ����ע
	void KBlackboardRender::drawLabel(Graphics* pGraphics, KLabel* pLabel)
	{
		// ��ͼ�ؼ�
		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// ��ע�ı�
		WCHAR wszText[MAX_PATH] = L"";
		wcscpy(wszText,KString::ToWChar(pLabel->GetText()));

		// ��ע�ı���������
		KExtents box = pLabel->GetExtents();
		double x = box.Xmin;
		double y = box.Ymax;	// ȡ�����Ͻ�����
		pMapEngine->WorldToClient(x,y);
		PointF pointLabel((REAL)x, (REAL)y);

		// ����
		FontFamily fontFamily(KString::ToWChar(pLabel->GetFontName()));
		REAL fontPoint = (REAL)pLabel->GetFontPoint();
		FontStyle eFontStyle = (FontStyle)pLabel->GetFontStyle();
		Gdiplus::Font fontLabel(&fontFamily,fontPoint,eFontStyle,Unit::UnitPoint);

		// �ַ�����ʽ
		StringFormat stringFormat;

		// ��Χ��
		RectF rcf;
		pGraphics->MeasureString(wszText,-1,&fontLabel,pointLabel,&stringFormat,&rcf);
		pLabel->SetExtentsClient(KExtents(rcf.X,rcf.Y,rcf.GetRight(),rcf.GetBottom()));

		// ������ɫ
		COLORREF clr = pLabel->GetColor();
		BYTE a,r,g,b;
		a = 255 * (100 - pLabel->GetTransparency()) / 100;
		r = GetRValue(clr);	g = GetGValue(clr);	b = GetBValue(clr);

		Color fontColor(a,r,g,b);
		//fontColor.SetFromCOLORREF(pLabel->GetColor());
		SolidBrush brushLabel(fontColor);

		// ����ѡ������
		if(pLabel->IsSelected())
		{
			Pen pen(Color::Red);
			pen.SetDashStyle(DashStyle::DashStyleDash);
			//pGraphics->DrawRectangle(&pen,rcf);
			pGraphics->FillRectangle(&SolidBrush(Color(64,0,0,252)),rcf);
		}

		// ���Ʊ�ע�ı�
		pGraphics->SetInterpolationMode(InterpolationMode::InterpolationModeDefault);
		pGraphics->DrawString(wszText,-1,
			&fontLabel,
			rcf,
			&stringFormat,
			&brushLabel
			);

		return;
	}
	// ����һ��ͼ��
	void KBlackboardRender::drawLegend(Graphics* pGraphics, KLegend* pLegend)
	{
		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = (KMapEngine*)m_lpvData;
		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();

		// ͼ������
		ImageAttributes imgAttributes;

		// ͸��ɫ
		ColorMap clrMap[1];
		clrMap[0].oldColor = Color(255,0,0,0);
		clrMap[0].newColor = Color(0,255,255,255);
		//imgAttributes.SetRemapTable(1,clrMap,ColorAdjustType::ColorAdjustTypeBitmap);

		// ͸����
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

		// �ڴ˰Ѱ�Χ��ת��Ϊ�豸���겢����
		KExtents box = pLegend->GetExtents();
		double x = box.Xmin;
		double y = box.Ymax;	// ȡ�����Ͻ�����
		pMapEngine->WorldToClient(x,y);
		RectF rcf((REAL)x,(REAL)y,(REAL)pImage->GetWidth(),(REAL)pImage->GetHeight());
		pLegend->SetExtentsClient(KExtents(rcf.X,rcf.Y,rcf.GetRight(),rcf.GetBottom()));

		// ����ͼ��
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

		// ����ѡ������
		if(pLegend->IsSelected())
		{
			Pen pen(Color::Red);
			pen.SetDashStyle(DashStyle::DashStyleDash);
			//pGraphics->DrawRectangle(&pen,rcf);
			pGraphics->FillRectangle(&SolidBrush(Color(64,0,0,252)),rcf);
		}

		return;
	}

	// ����ͼ��ͼ��
	Image* KBlackboardRender::buildLegendImage(KLegend* pLegend)
	{
		KPalette* pPalette = pLegend->GetPalette();
		if(!pPalette)
			return NULL;

		int iCount = pPalette->Count();

		int topshift = 16;						// �ϲ�λ��

		int iWidth = pLegend->Width;			// ͼ��ͼ����
		if(iWidth<0) iWidth = 100;

		int margin = pLegend->Margin;			// ҳ�߿հ�
		if(margin>iWidth/2) margin = iWidth/2 - 10;

		int itemWidth = pLegend->ItemWidth;		// ͼ����Ŀ���
		if(itemWidth>=iWidth) itemWidth = iWidth -10;

		int itemHeight = pLegend->ItemHeight;	// ͼ����Ŀ�߶�
		if(itemHeight>=36) itemHeight = 12;

		int itemGap = pLegend->ItemGap;			// ͼ����Ŀ��϶
		if(itemGap<0 || itemGap>=iWidth) itemGap = 0;

		int iUnitWidth = pLegend->UnitWidth;		// ��λ���

		// ����ͼ��ͼ��߶�
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
			::MessageBox(NULL,_T("����ͼ�����! - KLegendData::BuildLegend()(...)"),_T("��Ϣ"),MB_OK|MB_ICONQUESTION);
			return NULL;
		}

		// GDI+ ����
		Graphics g(m_pImage);

		// �ַ�����ʽ
		StringFormat stringFormat;

		// ���Ʊ߿�
		g.FillRectangle(&SolidBrush(Color(255,255,255,255)),Rect(0,0,iWidth,iHeight));
		g.DrawRectangle(&Pen(Color::Black),Rect(0,0,iWidth-1,iHeight-1));

		// ���Ʊ���
		stringFormat.SetAlignment(StringAlignment::StringAlignmentCenter);
		g.DrawString(L"ͼ��"
			,-1
			,&Gdiplus::Font(L"����",9,FontStyle::FontStyleBold)
			,RectF(1.0f*margin, 1.0f*margin, 1.0f*iWidth-margin, 1.0f*topshift)
			,&stringFormat
			,&SolidBrush(Color::Black)
			);

		TCHAR szLabel[MAX_PATH] = _T("");
		stringFormat.SetAlignment(StringAlignment::StringAlignmentFar);
		for(int i=0;i<iCount;i++)
		{
			KPaletteItem* pPaletteItem = pPalette->GetAt(i);

			// ����ͼ��ɫ��
			g.FillRectangle(&SolidBrush(Color(pPaletteItem->R,pPaletteItem->G,pPaletteItem->B))
				,Rect(margin,topshift+margin+(itemHeight+itemGap)*i,itemWidth,itemHeight));
			g.DrawRectangle(&Pen(Color::Black),
				Rect(margin,topshift+margin+(itemHeight+itemGap)*i,itemWidth,itemHeight));

			// ����ͼ���ı�
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
				,&Gdiplus::Font(L"����",1.0f * iFontSize)
				,RectF(xLabel, yLabel, wLabel, hLabel)
				,&stringFormat
				,&SolidBrush(Color::Black)
				);

			// ��ע��λ
			if(i==0)
			{
				g.DrawString(KString::ToWChar(pLegend->GetUnit()),-1,&Gdiplus::Font(L"����",(REAL)iFontSize)
					,PointF(1.0f * iWidth - iUnitWidth, margin + 1.0f*(itemHeight+itemGap)*i+topshift)
					,&SolidBrush(Color::Black));
			}

		}// for(i)

		return m_pImage;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
