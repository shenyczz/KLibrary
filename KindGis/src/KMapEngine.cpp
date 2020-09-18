#include "StdAfx.h"
#include "KMapEngine.h"

#include "KLayer.h"
#include "KLayerProperty.h"

// 构造函数
KMapEngine::KMapEngine(void)
{
	// 客户区尺寸
	m_clientWidth = 0;
	m_clientHeight = 0;
	
	// 数据尺寸
	m_dataX = 0;
	m_dataY = 0;
	m_dataWidth = 0;
	m_dataHeight = 0;

	// 世界坐标属性
	m_worldX = 0;
	m_worldY = 0;
	m_worldWidth = 0;
	m_worldHeight = 0;

	// 剪切区属性
	m_nClipX = 0;
	m_nClipY = 0;
	m_nClipWidth = 0;
	m_nClipHeight = 0;

	// 自定义数据属性
	m_CustomDataMinX = 0;
	m_CustomDataMinY = 0;
	m_CustomDataMaxX = 0;
	m_CustomDataMaxY = 0;

	// 窗口句柄
	m_hWnd = NULL;
	// 地图缓冲区
	m_hMapBuffer = NULL;
	// 绘制缓冲区
	m_hPaintBuffer = NULL;

	// 背景颜色
	m_clrBack = RGB(255,255,255);

	// 窗口的绘制原点
	m_DrawingPosition.x = 0;
	m_DrawingPosition.y = 0;

	m_sizeDraging.cx = 0;
	m_sizeDraging.cy = 0;

	// 重新绘制
	m_bRedraw = false;
	// 绘制十字线
	m_bDrawCrossHairs = FALSE;

	// 图层集
	m_pLayerPtrCollection = new TPtrCollection<KLayer*>();

	//-----------------------------------------------------
	// 鼠标监听器集
	m_pMouseListenerCollection = new KMouseListenerCollection();
	//-------------
	// 1.添加 [地图控制] 鼠标监听器
	m_pMouseListenerCollection->AddMouseListener(GisTool_MapController,new KMouseListener);
	// 2.添加 [黑板控制] 鼠标监听器
	m_pMouseListenerCollection->AddMouseListener(GisTool_BlackboardController,new KMouseListener);
	//-------------
	// 设置当前鼠标监听器 (默认为漫游)
	m_pMouseListenerCollection->SetCurrentMouseListenerKey(GisTool_MapController,GisTool_ZoomPan);
	//-----------------------------------------------------

	// 剪切取路径
	m_pClipPath = NULL; 
}

// 析构函数
KMapEngine::~KMapEngine(void)
{
	// 地图缓冲区
	::DeleteObject(m_hMapBuffer);
	// 绘制缓冲区
	::DeleteObject(m_hPaintBuffer);

	// 图层集
	_delete(m_pLayerPtrCollection);
	// 鼠标监听器集
	_delete(m_pMouseListenerCollection);
	// 剪切取路径
	_delete(m_pClipPath);
}

// 客户区坐标标转换为世界坐标
void KMapEngine::ClientToWorld (double& x, double& y)
{
	double x_scale = 1.0 * x / m_clientWidth;	// x 方向比例因子
	double y_scale = 1.0 * y / m_clientHeight;	// y 方向比例因子

	// 计算对应的世界坐标
	x = m_worldX + x_scale * m_worldWidth;
	y = m_worldY - y_scale * m_worldHeight;
}

// 世界坐标标转换为客户区坐标
void KMapEngine::WorldToClient(double& x, double& y)
{
	double x_scale = 1.0 * (x - m_worldX) / m_worldWidth;
	double y_scale = 1.0 * (y - m_worldY) / m_worldHeight;

	x = x_scale * m_clientWidth;
	y = -y_scale * m_clientHeight;
}

// 复位
void KMapEngine::Reset()
{
	// FUN_BEG

	// 如果没有判断图形会变形
	if(m_clientHeight<=0)	// 2009.12.02 - shenyc
		return;

	// 计算数据绑定矩形
	this->computeDataBound();

	// 计算世界绑定矩形
	this->computeWorldBound();

	// FUN_END
}

// 设置视图绑定矩形
void KMapEngine::SetViewBound( int x, int y, int w, int h )
{
	// FUN_BEG

	// 窗口句柄必须设置
	if(NULL == m_hWnd)
		return;

	// 是否重新绘制
	if(!this->IsRedraw())
	{
		// 查看视图大小是否改变(提高显示速度)
		if(m_clientWidth == w && m_clientHeight == h)
		{
			return;
		}
	}

	//-----------------------------------------------------
	// 处理worldBound - Beg
	//----------------------
	double originx, originy;	// 源  (x,y)
	double destx, desty;		// 目的(x,y)

	// 确保客户区大小改变后地图不变形
	double offset_width = 0;
	double offset_height = 0;

	if(m_clientWidth != 0 && m_clientHeight != 0)
	{
		// 客户区坐标 => 世界坐标
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
	// 处理worldBound - End
	//-----------------------------------------------------

	m_clientWidth = w;
	m_clientHeight = h;

	// 创建脱屏对象
	HDC hDC = ::GetDC(m_hWnd);

	::DeleteObject(m_hMapBuffer);
	m_hMapBuffer = ::CreateCompatibleBitmap(hDC,m_clientWidth,m_clientHeight);

	::DeleteObject(m_hPaintBuffer);
	m_hPaintBuffer = ::CreateCompatibleBitmap(hDC,m_clientWidth,m_clientHeight);

	::ReleaseDC(m_hWnd,hDC);

	// 重新绘制
	this->ReDraw(x,y,w,h);
	this->EnableRedraw(FALSE);

	// FUN_END
}

// 重新绘制
void KMapEngine::ReDraw(int x, int y, int w, int h)
{
	// FUN_BEG

	if(w==0 || h == 0)
		return;

	// 地图可视范围示意图
    // (x,y)            (0,0)
    //   +-----+          +-----+
    //   |     |h   ==>   |     |h
    //   +--w--+(w,h)     +--w--+(w,h)
    //   ~view~           ~Image~

	// 设置剪切矩形(渲染器使用该数据)
	SetClipRect(x,y,w,h);

	// 获取 DC
	HDC hDC = ::GetDC(m_hWnd);

	// 准备地图缓冲区(在地图缓冲区中画图)
	HDC hMapBufferDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldMapBitmap = (HBITMAP)::SelectObject(hMapBufferDC,m_hMapBuffer);
	clearBuffer(hMapBufferDC,x,y,w,h);	// 清除缓冲区

	// 获取剪切区最小包围盒
	RECT clip_rect ;
	::GetClipBox(hMapBufferDC,&clip_rect);	// Retrieves the dimensions of the tightest bounding rectangle around the current clipping boundary

	// 保存新范围内的原有图像
	HRGN hNewRgn = ::CreateRectRgn(x,y,x+w,y+h);
	::SelectClipRgn(hMapBufferDC,hNewRgn);
	::DeleteObject(hNewRgn);

	// 绘制各图层对象

	#pragma region --分类绘制1--

	int iLayerCount = m_pLayerPtrCollection->Count();
	if(1)
	{
		// 1.先绘制可移动并且可删除的图层
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
		// 2.然后绘制可移动并且不可可删除的图层
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
		// 3.再绘制不可移动并且不可可删除的图层并且不是图形编辑图层
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
		// 4.最后绘制图形编辑图层
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

	#pragma region --分类绘制2--
	if(0)
	{
		// 1. 先绘制除 Shape, Station, Blackboard 图层以外的其他图层
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

			// 或者是绘制等值线的离散点
			if(pLayer->GetType() == LayerType_Station
				&& ((KStationData*)pLayer->GetProvider()->GetDataObject())->IsTarceContour())
			{
				pLayer->Rendering(hMapBufferDC,this);
			}
		}
		// 2.其次绘制Shape图层
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
		// 3.然后绘制Station图层
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
		// 4.最后绘制涂鸦板
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

	// 下面2行暂时没有用
	HRGN hPreRgn = ::CreateRectRgnIndirect(&clip_rect);
	::DeleteObject(hPreRgn);

	// Map ----> Paint
	// 地图缓冲区复制到绘制缓冲区,刷新时再复制到 DC 中
	HDC hPaintBufferDC = ::CreateCompatibleDC(hDC);
	HBITMAP hOldPaintBitmap = (HBITMAP)::SelectObject(hPaintBufferDC,m_hPaintBuffer);
	::BitBlt(hPaintBufferDC,x,y,w,h,hMapBufferDC,x,y,SRCCOPY);

	// 释放资源
	::SelectObject(hPaintBufferDC,hOldPaintBitmap);
	::DeleteDC(hPaintBufferDC);

	::SelectObject(hMapBufferDC,hOldMapBitmap);
	::DeleteDC(hMapBufferDC);

	// 释放 DC
	::ReleaseDC(m_hWnd,hDC);

	// FUN_END
}

// 绘制地图(把缓冲区的图像复制到图形设备中)
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

// 刷新(把绘制缓冲区的位图复制到 HDC 中)
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

// 刷新
void KMapEngine::Refresh(BOOL bRedraw)
{
	Refresh();
	if(bRedraw)
	{
		m_bRedraw =  true;
		::InvalidateRect(m_hWnd,NULL,FALSE);
	}
}

// 移动(滚屏)
void KMapEngine::Scroll(int dx, int dy)
{
	// FUN_BEG

	// 地图缓冲区
	if(NULL == m_hMapBuffer)
		return;
	// 绘制缓冲区
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
    // 填补空白 - beg
	//---------------
	int invalidatex1, invalidatey1, invalidatew1, invalidateh1;
	int invalidatex2, invalidatey2, invalidatew2, invalidateh2;

	// 取得 DC
	HDC hDC = ::GetDC(m_hWnd);

	// 地图缓冲区 DC
	HDC hMapBufferDC = ::CreateCompatibleDC(hDC);
	HBITMAP hPreMapBuffer = (HBITMAP)::SelectObject(hMapBufferDC,m_hMapBuffer);

	// 绘制缓冲区 DC
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

	// 释放 DC
	::SelectObject(hPaintBufferDC,hPrePaintBuffer);
	::DeleteDC(hPaintBufferDC);

	::SelectObject(hMapBufferDC,hPreMapBuffer);
	::DeleteDC(hMapBufferDC);

	::ReleaseDC(m_hWnd,hDC);

	ReDraw(invalidatex1, invalidatey1, invalidatew1, invalidateh1);
	ReDraw(invalidatex2, invalidatey2, invalidatew2, invalidateh2);
	//---------------
	// 填补空白 - end
	//-----------------------------------------------------

	// FUN_END
}

// 有数据绑定矩形
BOOL KMapEngine::HasDataBound()
{
	// 数据属性全为零
	BOOL bNoDataBound = TRUE
		&& (fabs(m_dataX)<1.0e-12)
		&& (fabs(m_dataY)<1.0e-12)
		&& (fabs(m_dataWidth)<1.0e-12)
		&& (fabs(m_dataHeight)<1.0e-12);

	return !bNoDataBound;
}

// 有自定义数据绑定矩形
BOOL KMapEngine::HasCustomDataBound()
{
	// 数据属性全为零
	BOOL bNoCustomDataBound = TRUE
		&& (fabs(m_CustomDataMinX)<1.0e-12)
		&& (fabs(m_CustomDataMinY)<1.0e-12)
		&& (fabs(m_CustomDataMaxX)<1.0e-12)
		&& (fabs(m_CustomDataMaxY)<1.0e-12);

	return !bNoCustomDataBound;
}

// 设置自定义数据绑定矩形
void KMapEngine::SetCustomDataBound(double xmin, double ymin, double xmax, double ymax)
{
	m_CustomDataMinX = xmin;
	m_CustomDataMinY = ymin;
	m_CustomDataMaxX = xmax;
	m_CustomDataMaxY = ymax;
}

// 设置剪切矩形
void KMapEngine::SetClipRect (int x, int y, int w, int h)
{
	this ->m_nClipX = x ; 
	this ->m_nClipY = y ;
	this ->m_nClipWidth = w ;
	this ->m_nClipHeight = h ;
}

// 取得剪切矩形
void KMapEngine::GetClipRect (int& x, int& y, int& w, int& h)
{
	x = this ->m_nClipX; 
	y = this ->m_nClipY;
	w = this ->m_nClipWidth;
	h = this ->m_nClipHeight;
}

// 放大(拉窗口)
// x,y,w,h 是拉窗口的尺寸
void KMapEngine::ZoomIn(int x, int y, int w, int h)
{
	// FUN_BEG

	// 矩形中点(客户区)
    int centrex = x + w / 2;
    int centrey = y + h / 2;

	// 矩形太小(比如单击鼠标)
	if ( w < 5 || h < 5 )
	{
		w = m_clientWidth / 2;
		h = m_clientHeight / 2;
		x = centrex - w / 2;
		y = centrey - h / 2;
	}

	// 比例尺
    double scale = 1.0 * m_worldWidth / m_worldHeight;

	// 世界坐标的左上角点
	double wleft = x;
	double wtop = y ;
	ClientToWorld( wleft, wtop );

	// 世界坐标的右下角点
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

	// 矩形中点(世界坐标)
	double wcenterx = centrex;
	double wcentery = centrey;
	ClientToWorld( wcenterx, wcentery );

	m_worldX = wcenterx - width / 2;
	m_worldY = wcentery + height / 2;
	m_worldWidth = width;
	m_worldHeight = height;

	// 放大极限
	if( (fabs(m_worldWidth)<1.0e-5) || (fabs(m_worldHeight)<-1.0e-5) ) 
		return;

	ReDraw(0,0,m_clientWidth,m_clientHeight);

	// FUN_END
}

// 缩小(拉窗口)
// x,y,w,h 是拉窗口的尺寸
void KMapEngine::ZoomOut(int x, int y, int w, int h)
{
	// FUN_BEG

	// 默认缩小一半
	double scale = 2.0;

	// 矩形中点(客户区)
    int centrex = x + w / 2;
    int centrey = y + h / 2;

	// 世界坐标的左上角点
	double wleft = x;
	double wtop = y ;
	ClientToWorld( wleft, wtop );

	//世界坐标的右下角点
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

	// 世界坐标的中点
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

// 取得黑板图层
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

// 漫游
void KMapEngine::ZoomPan(int dx, int dy)
{
	// FUN_BEG

	Scroll(dx,dy);

	// FUN_END
}

// 地标校准
void KMapEngine::LandmarkCalibrate(int dx,int dy)
{
	m_sizeDraging.cx = dx;
	m_sizeDraging.cy = dy;
	if(m_sizeDraging.cx==0 && m_sizeDraging.cy==0)
		return;

	::PostMessage(this->GetHwnd(),WM_USER_LANDMARK_CALIBRATE_STEP,(WPARAM)0,(LPARAM)&m_sizeDraging);
}

// 地标校准
void KMapEngine::LandmarkCalibrate(POINT ptBeg,POINT ptEnd,BOOL bDraging)
{
	m_sizeDraging.cx = ptEnd.x - ptBeg.x;
	m_sizeDraging.cy = ptEnd.y - ptBeg.y;
	if(m_sizeDraging.cx==0 && m_sizeDraging.cy==0)
		return;

	::PostMessage(this->GetHwnd(),WM_USER_LANDMARK_CALIBRATE,(WPARAM)bDraging,(LPARAM)&m_sizeDraging);
}

// 构造剪切区路径(可重载)
GraphicsPath* KMapEngine::BuildClipPath()
{
	// FUN_BEG

	int i,j,k;

	// 图层集
	TPtrCollection<KLayer*>* pLayerPtrCollection = m_pLayerPtrCollection;
	if(pLayerPtrCollection==NULL || pLayerPtrCollection->Count()==0)
		return NULL;

	// 图层数量
	int iLayerCount = pLayerPtrCollection->Count();
	if(iLayerCount == 0)
		return NULL;

	// 查找可以构造剪切路径的图层
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

	// 渲染器
	KRender* pRender = pLayer->GetRender();
	if(!pRender)
		return NULL;

	// 图层属性
	KLayerProperty* pLayerProperty = (KLayerProperty*)pRender->GetLayerProperty();
	if(!pLayerProperty)
		return NULL;

	// 数据提供者
	KProvider* pProvider = pLayerProperty->GetProvider();
	if(pProvider->GetType() != ProviderType_ShapeFile)
		return NULL;

	// 数据对象
	KShapeFile* pShapeFile = (KShapeFile*)pProvider->GetDataObject();
	if(!pShapeFile)
		return NULL;

	// 属性数据(允许为空)
	KDbaseFile* pDbaseFile = pShapeFile->GetDbaseFile();
	KRows* pRows = pDbaseFile;

	// 图形集类型
	ShapeType eShapeType = (ShapeType)pShapeFile->GetType();
	if(eShapeType != ShapeType_Polygon)
		return NULL;

	// 剪切路径
	if(m_pClipPath == NULL)
	{
		m_pClipPath = new GraphicsPath(FillModeWinding);
	}
	GraphicsPath* pGraphicsPath = (GraphicsPath*)m_pClipPath;
	pGraphicsPath->Reset();
	pGraphicsPath->SetFillMode(FillModeWinding);

	// 所有点数量
	int iAllPointCount = 0;
	// 图形数量
	int iShapeCount = pShapeFile->Count();
	for(i=0; i< iShapeCount; i++)
	{
		//-------------------------------------------------
		// 查看属性数据的剪切标记
		int iClipFlag = 1;
		if(pRows)
		{
			KRow* pRow = pRows->GetRowById(i+1);
			if(pRow)
			{
				KField* pField = pRow->GetField(_T("FLAG"));	// 剪切标记
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

			// 测试
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

// 盘旋几何图形对象
void KMapEngine::HoverObject(KObject* pObject)
{
	if(!pObject)
		return;

	// 取得几何图形设备坐标包围盒
	KExtents box = pObject->GetExtentsClient();

	// 刷新
	//this->Refresh();

	// 绘制矩形
	HWND hWnd = this->GetHwnd();
	HDC hDC = ::GetDC(hWnd);

	HPEN hPen = ::CreatePen(PS_DOT,1,RGB(252,0,0));
	HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

	::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
	::Rectangle(hDC,(int)box.Xmin, (int)box.Ymin, (int)box.Xmax, (int)box.Ymax);

	::SelectObject( hDC,hOldPen );
	::DeleteObject(hPen) ;

	::ReleaseDC(hWnd,hDC);

	// FUN_END
}
// 选择几何图形对象
void KMapEngine::SelectObject(KObject* pObject)
{
	if(!pObject)
		return;

	// 取得第一个黑板图层
	KLayer* pBalckboardLayer = this->GetBlackboardLayer(0);
	if(!pBalckboardLayer)
		return;

	// 数据提供者
	KProvider* pProvider = pBalckboardLayer->GetProvider();
	if(!pProvider)
		return;

	// 数据对象
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
// 删除几何图形对象
void KMapEngine::DeleteObject(KObject* pObject)
{
	if(!pObject)
		return;

	// 取得第一个黑板图层
	KLayer* pBalckboardLayer = this->GetBlackboardLayer(0);
	if(!pBalckboardLayer)
		return;

	// 数据提供者
	KProvider* pProvider = pBalckboardLayer->GetProvider();
	if(!pProvider)
		return;

	// 数据对象
	KBlackboardData* pBlackboardData = (KBlackboardData*)pProvider->GetDataObject();
	if(!pBlackboardData)
		return;

	pBlackboardData->RemoveObject(pObject);

	this->EnableRedraw();
	::InvalidateRect(this->GetHwnd(),NULL,FALSE);
	// FUN_END
}

///========================================================
///黑板操作 - beg
///---------------

// 向黑板图层添加点
void KMapEngine::AddPoint()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_POINT,0,0);
}
// 向黑板图层添加线段
void KMapEngine::AddLine()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_LINE,0,0);
}
// 向黑板图层添加矩形
void KMapEngine::AddRect()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_RECT,0,0);
}
// 向黑板图层添加椭圆
void KMapEngine::AddEllipse()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_ELLIPSE,0,0);
}
// 向黑板图层添加折线
void KMapEngine::AddPolyline()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_POLYLINE,0,0);
}
// 向黑板图层添加多边形
void KMapEngine::AddPolygon()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_POLYGON,0,0);
}
// 向黑板图层添加曲线
void KMapEngine::AddCurve()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_CURVE,0,0);
}
// 向黑板图层添加闭合曲线
void KMapEngine::AddCloseCurve()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_CLOSE_CURVE,0,0);
}
// 向黑板图层添加图层的图例
void KMapEngine::AddLabel()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_LABEL,0,0);
}
// 向黑板图层添加选中图层的图例
void KMapEngine::AddLayerLegend()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_LAYER_LEGEND,0,0);
}
// 向黑板图层添加自定义的图例
void KMapEngine::AddCustomLegend()
{
	::PostMessage(this->GetHwnd(),WM_USER_ADD_CUSTOM_LEGEND,0,0);
}

///---------------
///黑板操作 - end
///========================================================


//---------------------------------------------------------
// 私有辅助函数 - beg
//-------------------

// 计算数据绑定矩形
// m_dataX
// m_dataY
// m_dataWidth
// m_dataHeight
void KMapEngine::computeDataBound()
{
	// FUN_BEG


	//-----------------------------------------------------
	// 有自定义数据绑定
	if (this->HasCustomDataBound())
	{
		m_dataX = m_CustomDataMinX;
		m_dataY = m_CustomDataMinY;
		m_dataWidth = fabs(m_CustomDataMaxX-m_CustomDataMinX); 
		m_dataHeight = fabs(m_CustomDataMaxY-m_CustomDataMinY) ;
		return;
	}
	//-----------------------------------------------------
	// 从 Layer 集合中计算
	// m_dataX
	// m_dataY
	// m_dataWidth
	// m_dataHeight
	double x, y ,w , h ;

	bool bFistData = false;
	int iLayerCount = m_pLayerPtrCollection->Count();
	for(int i=0; i<iLayerCount; i++)
	{
		// 图层
		KLayer* pLayer = m_pLayerPtrCollection->GetAt(i);
		if(!pLayer)
			continue;

		// 渲染器
		KRender* pRender = pLayer->GetRender();
		if(!pRender)
			continue;

		KLayerProperty* pLayerProperty = pRender->GetLayerProperty();
		if(!pLayerProperty)
			continue;

		if(!pLayerProperty->IsVisible())
			continue;

		// 过滤不使用的图层
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

// 计算世界绑定矩形
// m_worldX
// m_worldY
// m_worldWidth
// m_worldHeight
void KMapEngine::computeWorldBound()
{
	// FUN_BEG

	if(m_clientHeight<=0)
		return;

	// 计算
	// m_worldX
	// m_worldY
	// m_worldWidth
	// m_worldHeight
	// Notice: 地理坐标系
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

	// Notice: 屏幕坐标系
	//   |
	// --+------->x
	//   |
	//   V y
	m_worldY = m_worldY + m_worldHeight;	// Y坐标移动到下面

	// 计算比例尺
	double world_scale = 1.0 * m_worldWidth / m_worldHeight;		// 世界坐标比例
	double client_scale = 1.0 * m_clientWidth / m_clientHeight;		// 屏幕坐标比例

	// 根据比例尺调整
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

// 清除缓冲区
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
// 私有辅助函数 - end
//---------------------------------------------------------
