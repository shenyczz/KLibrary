#include "StdAfx.h"
#include "KMapEngine.h"

#include "KLayer.h"
#include "KLayerProperty.h"

// ���캯��
KMapEngine::KMapEngine(void)
{
	// �ͻ����ߴ�
	m_clientWidth = 0;
	m_clientHeight = 0;
	
	// ���ݳߴ�
	m_dataX = 0;
	m_dataY = 0;
	m_dataWidth = 0;
	m_dataHeight = 0;

	// ������������
	m_worldX = 0;
	m_worldY = 0;
	m_worldWidth = 0;
	m_worldHeight = 0;

	// ����������
	m_nClipX = 0;
	m_nClipY = 0;
	m_nClipWidth = 0;
	m_nClipHeight = 0;

	// �Զ�����������
	m_CustomDataMinX = 0;
	m_CustomDataMinY = 0;
	m_CustomDataMaxX = 0;
	m_CustomDataMaxY = 0;

	// ���ھ��
	m_hWnd = NULL;
	// ��ͼ������
	m_hMapBuffer = NULL;
	// ���ƻ�����
	m_hPaintBuffer = NULL;

	// ������ɫ
	m_clrBack = RGB(255,255,255);

	// ���ڵĻ���ԭ��
	m_DrawingPosition.x = 0;
	m_DrawingPosition.y = 0;

	m_sizeDraging.cx = 0;
	m_sizeDraging.cy = 0;

	// ���»���
	m_bRedraw = false;
	// ����ʮ����
	m_bDrawCrossHairs = FALSE;

	// ͼ�㼯
	m_pLayerPtrCollection = new TPtrCollection<KLayer*>();

	//-----------------------------------------------------
	// ����������
	m_pMouseListenerCollection = new KMouseListenerCollection();
	//-------------
	// 1.��� [��ͼ����] ��������
	m_pMouseListenerCollection->AddMouseListener(GisTool_MapController,new KMouseListener);
	// 2.��� [�ڰ����] ��������
	m_pMouseListenerCollection->AddMouseListener(GisTool_BlackboardController,new KMouseListener);
	//-------------
	// ���õ�ǰ�������� (Ĭ��Ϊ����)
	m_pMouseListenerCollection->SetCurrentMouseListenerKey(GisTool_MapController,GisTool_ZoomPan);
	//-----------------------------------------------------

	// ����ȡ·��
	m_pClipPath = NULL; 
}

// ��������
KMapEngine::~KMapEngine(void)
{
	// ��ͼ������
	::DeleteObject(m_hMapBuffer);
	// ���ƻ�����
	::DeleteObject(m_hPaintBuffer);

	// ͼ�㼯
	_delete(m_pLayerPtrCollection);
	// ����������
	_delete(m_pMouseListenerCollection);
	// ����ȡ·��
	_delete(m_pClipPath);
}

// �ͻ��������ת��Ϊ��������
void KMapEngine::ClientToWorld (double& x, double& y)
{
	double x_scale = 1.0 * x / m_clientWidth;	// x �����������
	double y_scale = 1.0 * y / m_clientHeight;	// y �����������

	// �����Ӧ����������
	x = m_worldX + x_scale * m_worldWidth;
	y = m_worldY - y_scale * m_worldHeight;
}

// ���������ת��Ϊ�ͻ�������
void KMapEngine::WorldToClient(double& x, double& y)
{
	double x_scale = 1.0 * (x - m_worldX) / m_worldWidth;
	double y_scale = 1.0 * (y - m_worldY) / m_worldHeight;

	x = x_scale * m_clientWidth;
	y = -y_scale * m_clientHeight;
}

// ��λ
void KMapEngine::Reset()
{
	// FUN_BEG

	// ���û���ж�ͼ�λ����
	if(m_clientHeight<=0)	// 2009.12.02 - shenyc
		return;

	// �������ݰ󶨾���
	this->computeDataBound();

	// ��������󶨾���
	this->computeWorldBound();

	// FUN_END
}

// ������ͼ�󶨾���
void KMapEngine::SetViewBound( int x, int y, int w, int h )
{
	// FUN_BEG

	// ���ھ����������
	if(NULL == m_hWnd)
		return;

	// �Ƿ����»���
	if(!this->IsRedraw())
	{
		// �鿴��ͼ��С�Ƿ�ı�(�����ʾ�ٶ�)
		if(m_clientWidth == w && m_clientHeight == h)
		{
			return;
		}
	}

	//-----------------------------------------------------
	// ����worldBound - Beg
	//----------------------
	double originx, originy;	// Դ  (x,y)
	double destx, desty;		// Ŀ��(x,y)

	// ȷ���ͻ�����С�ı���ͼ������
	double offset_width = 0;
	double offset_height = 0;

	if(m_clientWidth != 0 && m_clientHeight != 0)
	{
		// �ͻ������� => ��������
		originx = 0;
		originy = 0;
		this->ClientToWorld(originx,originy);

		destx = w - m_clientWidth;
		desty = h - m_clientHeight;
		this->ClientToWorld(destx,desty);

		offset_width = destx - originx;
		offset_height = desty - originy;
	}

	m_worldWidth += offset_width;
	m_worldHeight -= offset_height;
	//----------------------
	// ����worldBound - End
	//-----------------------------------------------------

	m_clientWidth = w;
	m_clientHeight = h;

	// ������������
	HDC hDC = ::GetDC(m_hWnd);

	::DeleteObject(m_hMapBuffer);
	m_hMapBuffer = ::CreateCompatibleBitmap(hDC,m_clientWidth,m_clientHeight);

	::DeleteObject(m_hPaintBuffer);
	m_hPaintBuffer = ::CreateCompatibleBitmap(hDC,m_clientWidth,m_clientHeight);

	::ReleaseDC(m_hWnd,hDC);

	// ���»���
	this->ReDraw(x,y,w,h);
	this->EnableRedraw(FALSE);

	// FUN_END
}

// ���»���
void KMapEngine::ReDraw(int x, int y, int w, int h)
{
	// FUN_BEG

	if(w==0 || h == 0)
		return;

	// ��ͼ���ӷ�Χʾ��ͼ
    // (x,y)            (0,0)
    //   +-----+          +-----+
    //   |     |h   ==>   |     |h
    //   +--w--+(w,h)     +--w--+(w,h)
    //   ~view~           ~Image~

	// ���ü��о���(��Ⱦ��ʹ�ø�����)
	SetClipRect(x,y,w,h);

	// ��ȡ DC
	HDC hDC = ::GetDC(m_hWnd);

	// ׼����ͼ������(�ڵ�ͼ�������л�ͼ)
	HDC hMapBufferDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldMapBitmap = (HBITMAP)::SelectObject(hMapBufferDC,m_hMapBuffer);
	clearBuffer(hMapBufferDC,x,y,w,h);	// ���������

	// ��ȡ��������С��Χ��
	RECT clip_rect ;
	::GetClipBox(hMapBufferDC,&clip_rect);	// Retrieves the dimensions of the tightest bounding rectangle around the current clipping boundary

	// �����·�Χ�ڵ�ԭ��ͼ��
	HRGN hNewRgn = ::CreateRectRgn(x,y,x+w,y+h);
	::SelectClipRgn(hMapBufferDC,hNewRgn);
	::DeleteObject(hNewRgn);

	// ���Ƹ�ͼ�����

	#pragma region --�������1--

	int iLayerCount = m_pLayerPtrCollection->Count();
	if(1)
	{
		// 1.�Ȼ��ƿ��ƶ����ҿ�ɾ����ͼ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(pLayer->GetType() == LayerType_Blackboard)
				continue;

			KLayerProperty* pLayerProperty = pLayer->GetProperty();
			if(NULL == pLayerProperty)
				continue;

			if(pLayerProperty->IsAllowMove() && pLayerProperty->IsAllowDelete())
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
		// 2.Ȼ����ƿ��ƶ����Ҳ��ɿ�ɾ����ͼ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(pLayer->GetType() == LayerType_Blackboard)
				continue;

			KLayerProperty* pLayerProperty = pLayer->GetProperty();
			if(NULL == pLayerProperty)
				continue;

			if(pLayerProperty->IsAllowMove() && !pLayerProperty->IsAllowDelete())
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
		// 3.�ٻ��Ʋ����ƶ����Ҳ��ɿ�ɾ����ͼ�㲢�Ҳ���ͼ�α༭ͼ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(pLayer->GetType() == LayerType_Blackboard)
				continue;

			KLayerProperty* pLayerProperty = pLayer->GetProperty();
			if(NULL == pLayerProperty)
				continue;

			if(!pLayerProperty->IsAllowMove() && !pLayerProperty->IsAllowDelete())
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
		// 4.������ͼ�α༭ͼ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(pLayer->GetType() == LayerType_Blackboard)
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
	}
	#pragma endregion

	#pragma region --�������2--
	if(0)
	{
		// 1. �Ȼ��Ƴ� Shape, Station, Blackboard ͼ�����������ͼ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(true
				&& pLayer->GetType() != LayerType_Blackboard
				&& pLayer->GetType() != LayerType_Station
				&& pLayer->GetType() != LayerType_Shape
				)
			{
				pLayer->Rendering(hMapBufferDC,this);
			}

			// �����ǻ��Ƶ�ֵ�ߵ���ɢ��
			if(pLayer->GetType() == LayerType_Station
				&& ((KStationData*)pLayer->GetProvider()->GetDataObject())->IsTarceContour())
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
		// 2.��λ���Shapeͼ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(pLayer->GetType() == LayerType_Shape)
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
		// 3.Ȼ�����Stationͼ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(pLayer->GetType() == LayerType_Station
				&& !((KStationData*)pLayer->GetProvider()->GetDataObject())->IsTarceContour())
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
		// 4.������Ϳѻ��
		for(int i=0;i<iLayerCount;i++)
		{
			KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
			if(NULL == pLayer)
				continue;

			if(pLayer->GetType() == LayerType_Blackboard)
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}

	}
	#pragma endregion

	// ����2����ʱû����
	HRGN hPreRgn = ::CreateRectRgnIndirect(&clip_rect);
	::DeleteObject(hPreRgn);

	// Map ----> Paint
	// ��ͼ���������Ƶ����ƻ�����,ˢ��ʱ�ٸ��Ƶ� DC ��
	HDC hPaintBufferDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldPaintBitmap = (HBITMAP)::SelectObject(hPaintBufferDC,m_hPaintBuffer);
	::BitBlt(hPaintBufferDC,x,y,w,h,hMapBufferDC,x,y,SRCCOPY);

	// �ͷ���Դ
	::SelectObject(hPaintBufferDC,hOldPaintBitmap);
	::DeleteDC(hPaintBufferDC);

	::SelectObject(hMapBufferDC,hOldMapBitmap);
	::DeleteDC(hMapBufferDC);

	// �ͷ� DC
	::ReleaseDC(m_hWnd,hDC);

	// FUN_END
}

// ���Ƶ�ͼ(�ѻ�������ͼ���Ƶ�ͼ���豸��)
void KMapEngine::DrawMap()
{
	// FUN_BEG

	if(NULL == m_hPaintBuffer)
		return ;

	HDC hDC = ::GetDC(m_hWnd);
	HDC hTempDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hTempDC,m_hPaintBuffer);

	::BitBlt(hDC,
		m_DrawingPosition.x,
		m_DrawingPosition.y,
		m_clientWidth,
		m_clientHeight,
		hTempDC,
		0,
		0,
		SRCCOPY
		);

	::SelectObject(hTempDC,hOldBitmap);
	::DeleteDC(hTempDC);
	::ReleaseDC(m_hWnd, hDC);

	// FUN_END
}

// ˢ��(�ѻ��ƻ�������λͼ���Ƶ� HDC ��)
void KMapEngine::Refresh()
{
	// FUN_BEG

	if(NULL == m_hPaintBuffer)
		return;

	HDC hDC = ::GetDC(m_hWnd);
	HDC hTempDC = ::CreateCompatibleDC(hDC);

	HBITMAP hPreBitmap = (HBITMAP)::SelectObject(hTempDC,m_hPaintBuffer);

	::BitBlt(hDC,
		0,
		0,
		m_clientWidth,
		m_clientHeight,
		hTempDC,
		0,
		0,
		SRCCOPY
		);

	::SelectObject(hTempDC,hPreBitmap);
	::DeleteDC(hTempDC);
	::ReleaseDC(m_hWnd,hDC);

	// FUN_END
}

// ˢ��
void KMapEngine::Refresh(BOOL bRedraw)
{
	Refresh();
	if(bRedraw)
	{
		m_bRedraw =  true;
		::InvalidateRect(m_hWnd,NULL,FALSE);
	}
}

// �ƶ�(����)
void KMapEngine::Scroll(int dx, int dy)
{
	// FUN_BEG

	// ��ͼ������
	if(NULL == m_hMapBuffer)
		return;
	// ���ƻ�����
	if(NULL == m_hPaintBuffer)
		return;

	double x0 = 0, y0 = 0;
	double x1 = dx, y1 = dy;
	ClientToWorld(x0, y0);    
	ClientToWorld(x1, y1);

	double offset_x = x1 - x0;
	double offset_y = y1 - y0;

	m_worldX -= offset_x;
	m_worldY -= offset_y;

	//-----------------------------------------------------
    // ��հ� - beg
	//---------------
	int invalidatex1, invalidatey1, invalidatew1, invalidateh1;
	int invalidatex2, invalidatey2, invalidatew2, invalidateh2;

	// ȡ�� DC
	HDC hDC = ::GetDC(m_hWnd);

	// ��ͼ������ DC
	HDC hMapBufferDC = ::CreateCompatibleDC(hDC);
	HBITMAP hPreMapBuffer = (HBITMAP)::SelectObject(hMapBufferDC,m_hMapBuffer);

	// ���ƻ����� DC
	HDC hPaintBufferDC = ::CreateCompatibleDC(hDC);
	HBITMAP hPrePaintBuffer = (HBITMAP)::SelectObject(hPaintBufferDC,m_hPaintBuffer);

    if (dx >= 0)
	{
		if(dy >=0)
		{
			::BitBlt(hPaintBufferDC,dx,dy,m_clientWidth-dx,m_clientHeight-dy,hMapBufferDC,0,0,SRCCOPY);
			invalidatex1 = invalidatey1 = 0 ;
			invalidatew1 = m_clientWidth, invalidateh1 = dy ;
			invalidatex2 = 0, invalidatey2 = dy ;
			invalidatew2 = dx, invalidateh2 = m_clientHeight - dy ;
		}
		else
		{
			::BitBlt(hPaintBufferDC,dx,0,m_clientWidth-dx,m_clientHeight+dy,hMapBufferDC,0,-dy,SRCCOPY);
			invalidatex1 = invalidatey1 = 0 ;
			invalidatew1 = dx, invalidateh1 = m_clientHeight + dy ;
			invalidatex2 = 0, invalidatey2 = m_clientHeight + dy ;
			invalidatew2 = m_clientWidth, invalidateh2 = -dy ;
		}
	}
	else
	{
		if(dy >=0)
		{
			::BitBlt(hPaintBufferDC,0,dy,m_clientWidth+dx,m_clientHeight-dy,hMapBufferDC,-dx,0,SRCCOPY);
			invalidatex1 = invalidatey1 = 0 ;
			invalidatew1 = m_clientWidth, invalidateh1 = dy ;
			invalidatex2 = m_clientWidth + dx, invalidatey2 = dy ;
			invalidatew2 = -dx, invalidateh2 = m_clientHeight - dy ;
		}
		else
		{
			::BitBlt(hPaintBufferDC,0,0,m_clientWidth+dx,m_clientHeight+dy,hMapBufferDC,-dx,-dy,SRCCOPY);
			invalidatex1 = m_clientWidth + dx, invalidatey1 = 0 ;
			invalidatew1 = -dx, invalidateh1 = m_clientHeight + dy ;
			invalidatex2 = 0, invalidatey2 = m_clientHeight + dy ;
			invalidatew2 = m_clientWidth, invalidateh2 = -dy ;
		}
	}
	// Map ---> Paint
	::BitBlt(hMapBufferDC,0,0,m_clientWidth,m_clientHeight,hPaintBufferDC,0,0,SRCCOPY);

	// �ͷ� DC
	::SelectObject(hPaintBufferDC,hPrePaintBuffer);
	::DeleteDC(hPaintBufferDC);

	::SelectObject(hMapBufferDC,hPreMapBuffer);
	::DeleteDC(hMapBufferDC);

	::ReleaseDC(m_hWnd,hDC);

	ReDraw(invalidatex1, invalidatey1, invalidatew1, invalidateh1);
	ReDraw(invalidatex2, invalidatey2, invalidatew2, invalidateh2);
	//---------------
	// ��հ� - end
	//-----------------------------------------------------

	// FUN_END
}

// �����ݰ󶨾���
BOOL KMapEngine::HasDataBound()
{
	// ��������ȫΪ��
	BOOL bNoDataBound = TRUE
		&& (fabs(m_dataX)<1.0e-12)
		&& (fabs(m_dataY)<1.0e-12)
		&& (fabs(m_dataWidth)<1.0e-12)
		&& (fabs(m_dataHeight)<1.0e-12);

	return !bNoDataBound;
}

// ���Զ������ݰ󶨾���
BOOL KMapEngine::HasCustomDataBound()
{
	// ��������ȫΪ��
	BOOL bNoCustomDataBound = TRUE
		&& (fabs(m_CustomDataMinX)<1.0e-12)
		&& (fabs(m_CustomDataMinY)<1.0e-12)
		&& (fabs(m_CustomDataMaxX)<1.0e-12)
		&& (fabs(m_CustomDataMaxY)<1.0e-12);

	return !bNoCustomDataBound;
}

// �����Զ������ݰ󶨾���
void KMapEngine::SetCustomDataBound(double xmin, double ymin, double xmax, double ymax)
{
	m_CustomDataMinX = xmin;
	m_CustomDataMinY = ymin;
	m_CustomDataMaxX = xmax;
	m_CustomDataMaxY = ymax;
}

// ���ü��о���
void KMapEngine::SetClipRect (int x, int y, int w, int h)
{
	this ->m_nClipX = x ; 
	this ->m_nClipY = y ;
	this ->m_nClipWidth = w ;
	this ->m_nClipHeight = h ;
}

// ȡ�ü��о���
void KMapEngine::GetClipRect (int& x, int& y, int& w, int& h)
{
	x = this ->m_nClipX; 
	y = this ->m_nClipY;
	w = this ->m_nClipWidth;
	h = this ->m_nClipHeight;
}

// �Ŵ�(������)
// x,y,w,h �������ڵĳߴ�
void KMapEngine::ZoomIn(int x, int y, int w, int h)
{
	// FUN_BEG

	// �����е�(�ͻ���)
    int centrex = x + w / 2;
    int centrey = y + h / 2;

	// ����̫С(���絥�����)
	if ( w < 5 || h < 5 )
	{
		w = m_clientWidth / 2;
		h = m_clientHeight / 2;
		x = centrex - w / 2;
		y = centrey - h / 2;
	}

	// ������
    double scale = 1.0 * m_worldWidth / m_worldHeight;

	// ������������Ͻǵ�
	double wleft = x;
	double wtop = y ;
	ClientToWorld( wleft, wtop );

	// ������������½ǵ�
	double wright = x + w;
	double wbottom = y + h;
	ClientToWorld( wright, wbottom );

	double width = fabs(wright-wleft);
	double height = fabs(wbottom-wtop);

	if ( (1.0 * width / height) > scale )
	{
		height = width / scale;
	}
	else
	{
		width = height * scale;
	}

	// �����е�(��������)
	double wcenterx = centrex;
	double wcentery = centrey;
	ClientToWorld( wcenterx, wcentery );

	m_worldX = wcenterx - width / 2;
	m_worldY = wcentery + height / 2;
	m_worldWidth = width;
	m_worldHeight = height;

	// �Ŵ���
	if( (fabs(m_worldWidth)<1.0e-5) || (fabs(m_worldHeight)<-1.0e-5) ) 
		return;

	ReDraw(0,0,m_clientWidth,m_clientHeight);

	// FUN_END
}

// ��С(������)
// x,y,w,h �������ڵĳߴ�
void KMapEngine::ZoomOut(int x, int y, int w, int h)
{
	// FUN_BEG

	// Ĭ����Сһ��
	double scale = 2.0;

	// �����е�(�ͻ���)
    int centrex = x + w / 2;
    int centrey = y + h / 2;

	// ������������Ͻǵ�
	double wleft = x;
	double wtop = y ;
	ClientToWorld( wleft, wtop );

	//������������½ǵ�
	double wright = x + w;
	double wbottom = y + h;
	ClientToWorld( wright, wbottom );

	double width = fabs(wright - wleft);
	double height = fabs(wbottom - wtop);

	if (w > 5 && h > 5)
	{
		double scaleX = m_worldWidth / width;
		double scaleY = m_worldHeight / height;

		if (scaleX > scaleY)
			scale = scaleX;
		else
			scale = scaleY;
	}

	width = scale * m_worldWidth;
	height = scale * m_worldHeight;

	// ����������е�
	double wcenterx = centrex;
	double wcentery = centrey;
	ClientToWorld( wcenterx, wcentery );

	m_worldX = wcenterx - width / 2;
	m_worldY = wcentery + height / 2;
	m_worldWidth = width;
	m_worldHeight = height;

    ReDraw(0,0,m_clientWidth,m_clientHeight);

	// FUN_END
}

// ȡ�úڰ�ͼ��
KLayer* KMapEngine::GetBlackboardLayer(int index)
{
	KLayer* pBalckboardLayer = NULL;

	int iLayerCount = m_pLayerPtrCollection->Count();
	if(index<0 || index>=iLayerCount)
		return NULL;

	int bbLayerIndex = 0;
	for(int i=0;i<iLayerCount;i++)
	{
		KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
		if(NULL == pLayer)
			continue;

		if(pLayer->GetType() == LayerType_Blackboard)
		{
			if(bbLayerIndex == index)
			{
				pBalckboardLayer = pLayer;
				break;
			}

			bbLayerIndex++;
		}
	}

	return pBalckboardLayer;
}

// ����
void KMapEngine::ZoomPan(int dx, int dy)
{
	// FUN_BEG

	Scroll(dx,dy);

	// FUN_END
}

// �ر�У׼
void KMapEngine::LandmarkCalibrate(int dx,int dy)
{
	m_sizeDraging.cx = dx;
	m_sizeDraging.cy = dy;
	if(m_sizeDraging.cx==0 && m_sizeDraging.cy==0)
		return;

	::PostMessage(this->GetHwnd(),WM_USER_LANDMARK_CALIBRATE_STEP,(WPARAM)0,(LPARAM)&m_sizeDraging);
}

// �ر�У׼
void KMapEngine::LandmarkCalibrate(POINT ptBeg,POINT ptEnd,BOOL bDraging)
{
	m_sizeDraging.cx = ptEnd.x - ptBeg.x;
	m_sizeDraging.cy = ptEnd.y - ptBeg.y;
	if(m_sizeDraging.cx==0 && m_sizeDraging.cy==0)
		return;

	::PostMessage(this->GetHwnd(),WM_USER_LANDMARK_CALIBRATE,(WPARAM)bDraging,(LPARAM)&m_sizeDraging);
}

// ���������·��(������)
GraphicsPath* KMapEngine::BuildClipPath()
{
	// FUN_BEG

	int i,j,k;

	// ͼ�㼯
	TPtrCollection<KLayer*>* pLayerPtrCollection = m_pLayerPtrCollection;
	if(pLayerPtrCollection==NULL || pLayerPtrCollection->Count()==0)
		return NULL;

	// ͼ������
	int iLayerCount = pLayerPtrCollection->Count();
	if(iLayerCount == 0)
		return NULL;

	// ���ҿ��Թ������·����ͼ��
	KLayer* pLayer = NULL;
	for(i=0;i<iLayerCount;i++)
	{
		KLayer* pLayerTemp = pLayerPtrCollection->GetAt(i);
		if(!pLayerTemp) continue;
		KLayerProperty* pLayerPropertyTemp = (KLayerProperty*)pLayerTemp->GetProperty();
		if(pLayerPropertyTemp && pLayerPropertyTemp->IsBuildClipPath())
		{
			pLayer = pLayerTemp;
		}
	}
	if(!pLayer)
		return NULL;

	// ��Ⱦ��
	KRender* pRender = pLayer->GetRender();
	if(!pRender)
		return NULL;

	// ͼ������
	KLayerProperty* pLayerProperty = (KLayerProperty*)pRender->GetLayerProperty();
	if(!pLayerProperty)
		return NULL;

	// �����ṩ��
	KProvider* pProvider = pLayerProperty->GetProvider();
	if(pProvider->GetType() != ProviderType_ShapeFile)
		return NULL;

	// ���ݶ���
	KShapeFile* pShapeFile = (KShapeFile*)pProvider->GetDataObject();
	if(!pShapeFile)
		return NULL;

	// ��������(����Ϊ��)
	KDbaseFile* pDbaseFile = pShapeFile->GetDbaseFile();
	KRows* pRows = pDbaseFile;

	// ͼ�μ�����
	ShapeType eShapeType = (ShapeType)pShapeFile->GetType();
	if(eShapeType != ShapeType_Polygon)
		return NULL;

	// ����·��
	if(m_pClipPath == NULL)
	{
		m_pClipPath = new GraphicsPath(FillModeWinding);
	}
	GraphicsPath* pGraphicsPath = (GraphicsPath*)m_pClipPath;
	pGraphicsPath->Reset();
	pGraphicsPath->SetFillMode(FillModeWinding);

	// ���е�����
	int iAllPointCount = 0;
	// ͼ������
	int iShapeCount = pShapeFile->Count();
	for(i=0; i< iShapeCount; i++)
	{
		//-------------------------------------------------
		// �鿴�������ݵļ��б��
		int iClipFlag = 1;
		if(pRows)
		{
			KRow* pRow = pRows->GetRowById(i+1);
			if(pRow)
			{
				KField* pField = pRow->GetField(_T("FLAG"));	// ���б��
				if(pField)
				{
					iClipFlag = (int)pField->GetIntergerValue();
				}//if(pFieldValue)

			}// if(pRow)

		}// if(pRows)

		if(iClipFlag == 0)
			continue;
		//-------------------------------------------------
		KGeometryPtrCollection* pPolygonCollection = (KGeometryPtrCollection*)pShapeFile->GetAt(i);
		int iPolygonCount = pPolygonCollection->Count();
		for(j=0;j<iPolygonCount;j++)
		{
			KPolygon* pPolygon = (KPolygon*)pPolygonCollection->GetAt(j);
			int iPointCount = pPolygon->Count();
			PointF* ptf = new PointF[iPointCount];
			memset(ptf,0,sizeof(PointF)*iPointCount);
			for(k=0;k<iPointCount;k++)
			{
				KPoint pt = *pPolygon->GetAt(k);
				double x = pt.X;
				double y = pt.Y;
				this->WorldToClient(x,y);
				ptf[k].X = (REAL)x;
				ptf[k].Y = (REAL)y;

			}// next k

			pGraphicsPath->AddPolygon(ptf,iPointCount);
			_deletea(ptf);

			// ����
			iAllPointCount += iPointCount;

		}// next j
	}// next i

#ifdef _DEBUG
			RectF rectf(0,0,0,0);
			pGraphicsPath->GetBounds(&rectf);

			PathData pathData;
			pGraphicsPath->GetPathData(&pathData);
#endif
	
	return pGraphicsPath->GetPointCount()==0 ? NULL :pGraphicsPath;
}

// ��������ͼ�ζ���
void KMapEngine::HoverObject(KObject* pObject)
{
	if(!pObject)
		return;

	// ȡ�ü���ͼ���豸�����Χ��
	KExtents box = pObject->GetExtentsClient();

	// ˢ��
	//this->Refresh();

	// ���ƾ���
	HWND hWnd = this->GetHwnd();
	HDC hDC = ::GetDC(hWnd);

	HPEN hPen = ::CreatePen(PS_DOT,1,RGB(252,0,0));
	HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

	::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
	::Rectangle(hDC,(int)box.Xmin, (int)box.Ymin, (int)box.Xmax, (int)box.Ymax);

	::SelectObject( hDC,hOldPen );
	::DeleteObject(hPen) ;

	::ReleaseDC(hWnd,hDC);

	// FUN_END
}
// ѡ�񼸺�ͼ�ζ���
void KMapEngine::SelectObject(KObject* pObject)
{
	if(!pObject)
		return;

	// ȡ�õ�һ���ڰ�ͼ��
	KLayer* pBalckboardLayer = this->GetBlackboardLayer(0);
	if(!pBalckboardLayer)
		return;

	// �����ṩ��
	KProvider* pProvider = pBalckboardLayer->GetProvider();
	if(!pProvider)
		return;

	// ���ݶ���
	KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
	if(!pBlackboardData)
		return;

	bool bOldSelect = pObject->IsSelected();
	pBlackboardData->SelectAll(false);
	pObject->Select(!bOldSelect);

	this->EnableRedraw();
	::InvalidateRect(this->GetHwnd(),NULL,FALSE);

	if(pObject->IsSelected())
	{
		HWND hWnd = this->GetHwnd();
		::PostMessage(hWnd,WM_USER_SELECT_OBJECT,0,(LPARAM)pObject);
	}
	// FUN_END
}
// ɾ������ͼ�ζ���
void KMapEngine::DeleteObject(KObject* pObject)
{
	if(!pObject)
		return;

	// ȡ�õ�һ���ڰ�ͼ��
	KLayer* pBalckboardLayer = this->GetBlackboardLayer(0);
	if(!pBalckboardLayer)
		return;

	// �����ṩ��
	KProvider* pProvider = pBalckboardLayer->GetProvider();
	if(!pProvider)
		return;

	// ���ݶ���
	KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
	if(!pBlackboardData)
		return;

	pBlackboardData->RemoveObject(pObject);

	this->EnableRedraw();
	::InvalidateRect(this->GetHwnd(),NULL,FALSE);
	// FUN_END
}

///========================================================
///�ڰ���� - beg
///---------------

// ��ڰ�ͼ����ӵ�
void KMapEngine::AddPoint()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_POINT,0,0);
}
// ��ڰ�ͼ������߶�
void KMapEngine::AddLine()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_LINE,0,0);
}
// ��ڰ�ͼ����Ӿ���
void KMapEngine::AddRect()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_RECT,0,0);
}
// ��ڰ�ͼ�������Բ
void KMapEngine::AddEllipse()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_ELLIPSE,0,0);
}
// ��ڰ�ͼ���������
void KMapEngine::AddPolyline()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_POLYLINE,0,0);
}
// ��ڰ�ͼ����Ӷ����
void KMapEngine::AddPolygon()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_POLYGON,0,0);
}
// ��ڰ�ͼ���������
void KMapEngine::AddCurve()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_CURVE,0,0);
}
// ��ڰ�ͼ����ӱպ�����
void KMapEngine::AddCloseCurve()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_CLOSE_CURVE,0,0);
}
// ��ڰ�ͼ�����ͼ���ͼ��
void KMapEngine::AddLabel()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_LABEL,0,0);
}
// ��ڰ�ͼ�����ѡ��ͼ���ͼ��
void KMapEngine::AddLayerLegend()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_LAYER_LEGEND,0,0);
}
// ��ڰ�ͼ������Զ����ͼ��
void KMapEngine::AddCustomLegend()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_CUSTOM_LEGEND,0,0);
}

///---------------
///�ڰ���� - end
///========================================================


//---------------------------------------------------------
// ˽�и������� - beg
//-------------------

// �������ݰ󶨾���
// m_dataX
// m_dataY
// m_dataWidth
// m_dataHeight
void KMapEngine::computeDataBound()
{
	// FUN_BEG


	//-----------------------------------------------------
	// ���Զ������ݰ�
	if (this->HasCustomDataBound())
	{
		m_dataX = m_CustomDataMinX;
		m_dataY = m_CustomDataMinY;
		m_dataWidth = fabs(m_CustomDataMaxX-m_CustomDataMinX); 
		m_dataHeight = fabs(m_CustomDataMaxY-m_CustomDataMinY) ;
		return;
	}
	//-----------------------------------------------------
	// �� Layer �����м���
	// m_dataX
	// m_dataY
	// m_dataWidth
	// m_dataHeight
	double x, y ,w , h ;

	bool bFistData = false;
	int iLayerCount = m_pLayerPtrCollection->Count();
	for(int i=0; i<iLayerCount; i++)
	{
		// ͼ��
		KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
		if(!pLayer)
			continue;

		// ��Ⱦ��
		KRender* pRender = pLayer->GetRender();
		if(!pRender)
			continue;

		KLayerProperty* pLayerProperty = pRender->GetLayerProperty();
		if(!pLayerProperty)
			continue;

		if(!pLayerProperty->IsVisible())
			continue;

		// ���˲�ʹ�õ�ͼ��
		int type = pLayer->GetType();
		// ...

		KExtents extents = pLayerProperty->GetExtents();
		x = extents.MinX(); y = extents.MinY(); w = extents.Width(); h = extents.Height();
		if(w==0 || h==0)
			continue;

		if(!bFistData)
		{
			m_dataX = x;
			m_dataY = y;
			m_dataWidth = w; 
			m_dataHeight = h ;
			bFistData = true;
		}
		else
		{
			if ( x < m_dataX )						m_dataX = x;
			if ( y < m_dataY )						m_dataY = y;
			if ( x + w > m_dataX + m_dataWidth )	m_dataWidth = x + w - m_dataX ;
			if ( y + h > m_dataY + m_dataHeight )	m_dataHeight = y + h - m_dataY ;
		}
	}

	// FUN_END
}

// ��������󶨾���
// m_worldX
// m_worldY
// m_worldWidth
// m_worldHeight
void KMapEngine::computeWorldBound()
{
	// FUN_BEG

	if(m_clientHeight<=0)
		return;

	// ����
	// m_worldX
	// m_worldY
	// m_worldWidth
	// m_worldHeight
	// Notice: ��������ϵ
	//   A y
	//   |
	// --+------->x
	//   |
	m_worldX = m_dataX;
	m_worldY = m_dataY;
	m_worldWidth = m_dataWidth;
	m_worldHeight = m_dataHeight;
	if(fabs(m_worldHeight)<1.0e-12)	// 2009.12.02 - shenyc
		return;

	// Notice: ��Ļ����ϵ
	//   |
	// --+------->x
	//   |
	//   V y
	m_worldY = m_worldY + m_worldHeight;	// Y�����ƶ�������

	// ���������
	double world_scale = 1.0 * m_worldWidth / m_worldHeight;		// �����������
	double client_scale = 1.0 * m_clientWidth / m_clientHeight;		// ��Ļ�������

	// ���ݱ����ߵ���
	// m_worldX
	// m_worldY
	// m_worldWidth
	// m_worldHeight
	if (world_scale > client_scale)
	{
		double height = m_worldWidth / client_scale;
		double y = m_worldY + (height - m_worldHeight) / 2.0;
		m_worldY = y ;
		m_worldHeight = height ;
	}
	else
	{
		double width = m_worldHeight * client_scale;
		double x = m_worldX - (width - m_worldWidth) / 2.0;
		m_worldX = x ;
		m_worldWidth = width ;
	}

	// FUN_END
}

// ���������
void KMapEngine::clearBuffer( HDC hDC, int x, int y, int w, int h )
{
	RECT rect;
	rect.left = x;
	rect.top = y;
	rect.right = x + w;
	rect.bottom = y + h;

	HBRUSH hBrush = ::CreateSolidBrush(m_clrBack);
	::FillRect(hDC,&rect,hBrush);
	::DeleteObject(hBrush);
}

//-------------------
// ˽�и������� - end
//---------------------------------------------------------
