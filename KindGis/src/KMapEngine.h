/******************************************************************************
 ** KMapEngine.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KMapEngine - 地图引擎
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

///============================================================================
///自定义消息 - Beg
///----------------
// 向黑板图层添加点
#define WM_USER_ADD_POINT					(WM_USER+1050)
// 向黑板图层添加线段
#define WM_USER_ADD_LINE					(WM_USER+1051)
// 向黑板图层添加矩形
#define WM_USER_ADD_RECT					(WM_USER+1052)
// 向黑板图层添加椭圆
#define WM_USER_ADD_ELLIPSE					(WM_USER+1053)
// 向黑板图层添加折线
#define WM_USER_ADD_POLYLINE				(WM_USER+1054)
// 向黑板图层添加多边形
#define WM_USER_ADD_POLYGON					(WM_USER+1055)
// 向黑板图层添加曲线
#define WM_USER_ADD_CURVE					(WM_USER+1056)
// 向黑板图层添加闭合曲线
#define WM_USER_ADD_CLOSE_CURVE				(WM_USER+1057)

// 添加标注到黑板图层
#define WM_USER_ADD_LABEL					(WM_USER+1070)
// 添加图层图例到黑板图层
#define WM_USER_ADD_LAYER_LEGEND			(WM_USER+1071)
// 添加自定义图例到黑板图层
#define WM_USER_ADD_CUSTOM_LEGEND			(WM_USER+1072)

// 选中对象
#define WM_USER_SELECT_OBJECT				(WM_USER+1080)

// 地标校准
#define WM_USER_LANDMARK_CALIBRATE			(WM_USER+1090)
#define WM_USER_LANDMARK_CALIBRATE_STEP		(WM_USER+1091)
///----------------
///自定义消息 - End
///============================================================================

#include "KLayer.h"
#include "KMouseListener.h"
#include "KMouseListenerCollection.h"

class KIND_EXT_CLASS KMapEngine : public KObject
{
public:
	KMapEngine(void);
	virtual ~KMapEngine(void);

public:
	// 窗口句柄
	virtual void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	virtual HWND GetHwnd() { return m_hWnd; }

	// 重新绘制
	virtual void EnableRedraw(BOOL bRedraw = TRUE) { m_bRedraw = bRedraw; }
	virtual BOOL IsRedraw() { return m_bRedraw; }

	// 绘制十字线
	virtual void EnableDrawCrossHairs(bool bDrawCrossHairs = TRUE) { m_bDrawCrossHairs = bDrawCrossHairs; }
	virtual BOOL IsDrawCrossHairs() { return m_bDrawCrossHairs; }

	// 背景颜色
	virtual void SetBackColor(COLORREF clr) { m_clrBack = clr; }
	virtual COLORREF GetBackColor() { return m_clrBack; }

	// 取得位图句柄
	virtual HBITMAP GetHBitmap() { return m_hPaintBuffer;}

	// 拥有者窗口绘制位置
	virtual POINT& GetDrawingPosition() { return m_DrawingPosition;}

	// 获取图层集
	TPtrCollection<KLayer*>* GetLayerPtrCollection() { return m_pLayerPtrCollection; }

	// 鼠标监听器集
	KMouseListenerCollection* GetMouseListenerCollection() { return m_pMouseListenerCollection; }

	// 取得当前鼠标监听器
	KMouseListener* GetCurrentMouseListener() { return m_pMouseListenerCollection->GetCurrentMouseListener(); }


public:
	// 客户区坐标转换为世界坐标
	virtual void ClientToWorld(double& x, double& y);
	// 世界坐标转换为客户区坐标
	virtual void WorldToClient(double& x, double& y);

public:
	// 复位
	virtual void Reset();
	// 设置视图绑定矩形
	virtual void SetViewBound( int x, int y, int w, int h );
	// 重新绘制
	virtual void ReDraw(int x, int y, int w, int h);
	// 绘制地图
	virtual void DrawMap();
	// 刷新
	virtual void Refresh();
	// 刷新
	virtual void Refresh(BOOL bRedraw);
	// 移动
	virtual void Scroll(int dx, int dy);

	// 设置/获取剪切矩形
	virtual void SetClipRect (int x, int y, int w, int h);
	virtual void GetClipRect (int& x, int& y, int& w, int& h);

	// 有数据绑定矩形
	virtual BOOL HasDataBound();

	// 有自定义数据绑定矩形
	virtual BOOL HasCustomDataBound();

	// 设置自定义数据绑定矩形
	virtual void SetCustomDataBound(double xmin, double ymin, double xmax, double ymax);

public:
	// 放大
	virtual void ZoomIn(int x, int y, int w, int h);
	// 缩小
	virtual void ZoomOut(int x, int y, int w, int h);
	// 漫游
	virtual void ZoomPan(int dx, int dy);

public:
	// 地标校准
	virtual void LandmarkCalibrate(int dx,int dy);
	// 地标校准
	virtual void LandmarkCalibrate(POINT ptBeg,POINT ptEnd,BOOL bDraging);

public:
	// 构造剪切区路径(可重载)
	virtual GraphicsPath* BuildClipPath();

public:
	// 向黑板图层添加点
	virtual void AddPoint();
	// 向黑板图层添加线段
	virtual void AddLine();
	// 向黑板图层添加矩形
	virtual void AddRect();
	// 向黑板图层添加椭圆
	virtual void AddEllipse();
	// 向黑板图层添加折线
	virtual void AddPolyline();
	// 向黑板图层添加多边形
	virtual void AddPolygon();
	// 向黑板图层添加曲线
	virtual void AddCurve();
	// 向黑板图层添加闭合曲线
	virtual void AddCloseCurve();

	// 向黑板图层添加标注
	virtual void AddLabel();
	// 向黑板图层添加图层的图例
	virtual void AddLayerLegend();
	// 向黑板图层添加自定义的图例
	virtual void AddCustomLegend();

public:
	// 盘旋几何图形对象
	virtual void HoverObject(KObject* pObject);
	// 选择几何图形对象
	virtual void SelectObject(KObject* pObject);
	// 删除几何图形对象
	virtual void DeleteObject(KObject* pObject);

	// 取得黑板图层
	virtual KLayer* GetBlackboardLayer(int index);

private:
	// 计算数据绑定矩形
	void computeDataBound();
	// 计算世界绑定矩形
	void computeWorldBound();
	// 清除缓冲区
	void clearBuffer( HDC hDC, int x, int y, int w, int h );

protected:
	// 客户区尺寸
	int m_clientWidth;
	int m_clientHeight;

	// 数据属性
	double m_dataX;
	double m_dataY;
	double m_dataWidth;
	double m_dataHeight;

	// 世界坐标属性
	double m_worldX;
	double m_worldY;
	double m_worldWidth;
	double m_worldHeight;

	// 剪切区属性
	int m_nClipX;
	int m_nClipY;
	int m_nClipWidth;
	int m_nClipHeight;

	// 自定义数据属性
	double m_CustomDataMinX;
	double m_CustomDataMinY;
	double m_CustomDataMaxX;
	double m_CustomDataMaxY;

protected:
	// 窗口句柄
	HWND m_hWnd;

	// 缓冲区
	HBITMAP m_hMapBuffer;	// 地图缓冲区
	HBITMAP m_hPaintBuffer;	// 绘制缓冲区

	// 背景颜色
	COLORREF m_clrBack;

	// 窗口绘制原点
	POINT m_DrawingPosition;

	POINT m_pointDragingBeg;	// 鼠标拖动开始位置
	POINT m_pointDragingEnd;	// 鼠标拖动结束位置

	SIZE m_sizeDraging;			// 鼠标拖动尺寸

	// 重新绘制
	BOOL m_bRedraw;

	// 绘制十字线
	BOOL m_bDrawCrossHairs;	

	// 图层集
	TPtrCollection<KLayer*>* m_pLayerPtrCollection;

	// 鼠标监听器集
	KMouseListenerCollection* m_pMouseListenerCollection;

	// 剪切取路径
	GraphicsPath* m_pClipPath;
};

