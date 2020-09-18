#include "StdAfx.h"
#include "KMouseListener.h"

#include "KMouseEvent.h"
#include "KMapEngine.h"

namespace Mouse
{
	KMouseListener::KMouseListener(void)
		: m_pMouseEvent(NULL)
		, m_bFinishedGeometry(true)
	{
		m_pointPrv.x = 0;
		m_pointPrv.y = 0;

		m_lastPoint.x = 0;
		m_lastPoint.y = 0;

		m_LButtonPoint.x = 0;	// 鼠标左键首次按下时位置
		m_LButtonPoint.y = 0;	// 鼠标左键首次按下时位置

		m_PointSequence.clear();
	}

	KMouseListener::~KMouseListener(void)
	{
		m_PointSequence.clear();
	}

	// 鼠标左键被按下
	void KMouseListener::OnLButtonDown(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// 记录最近的鼠标位置
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		switch(tool)
		{
		case GisTool_Blackboard_Select_Object:
			pMapEngine->SelectObject(pObject);
			break;
		case GisTool_Blackboard_Delete_Object:
			pMapEngine->DeleteObject(pObject);
			break;

		case GisTool_Blackboard_Draw_Polyline:
		case GisTool_Blackboard_Draw_Curve:
		case GisTool_Blackboard_Draw_Polygon:
		case GisTool_Blackboard_Draw_Close_Curve:
			{
				if(m_bFinishedGeometry)
				{
					m_bFinishedGeometry = FALSE;
					m_PointSequence.clear();
					m_PointSequence.push_back(point);
				}
			}
			break;
		}

		// FUN_END
	}

	// 鼠标左键被抬起。
	void KMouseListener::OnLButtonUp(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// 记录最近的鼠标位置
		m_lastPoint = pMouseEventArgs->GetPoint();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// 位置偏移量
		int dx = point.x - m_LButtonPoint.x ;
		int dy = point.y - m_LButtonPoint.y ;

		// 矩形尺寸
		int x = min(m_LButtonPoint.x,point.x);
		int y = min(m_LButtonPoint.y,point.y);
		int w = abs(m_LButtonPoint.x - point.x);
		int h = abs(m_LButtonPoint.y - point.y);

		switch(tool)
		{
		case GisTool_ZoomIn:	// 放大
			{
				pMapEngine->ZoomIn(x,y,w,h);
				//pMapEngine->Refresh();
			}
			break;
		case GisTool_ZoomOut:	// 缩小
			{
				pMapEngine->ZoomOut(x,y,w,h);
				//pMapEngine->Refresh();
			}
			break;
		case GisTool_ZoomPan:	// 漫游
			{
				POINT* pDrawPos = &pMapEngine->GetDrawingPosition();
				pDrawPos->x = 0;
				pDrawPos->y = 0;
				pMapEngine->ZoomPan(dx,dy);
				pMapEngine->Refresh();
			}
			break;

		case GisTool_Blackboard_Select_Object:			// 选择黑板内含对象
			// 在LButtonDown中实施
			//pMapEngine->SelectObject(pObject);
			break;

		case GisTool_Blackboard_Delete_Object:			// 删除黑板内含对象
			// 在LButtonDown中实施
			//pMapEngine->DeleteObject(pObject);
			break;

		case GisTool_Blackboard_Move_Object:				// 移动黑板内含对象
			{
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->EnableRedraw();
				::InvalidateRect(hWnd,NULL,FALSE);
			}
			break;

		case GisTool_Blackboard_Draw_Point:			// 绘制点
			pMapEngine->AddPoint();
			break;
		case GisTool_Blackboard_Draw_Line:			// 绘制线段
			pMapEngine->AddLine();
			break;
		case GisTool_Blackboard_Draw_Rect:			// 绘制矩形
			pMapEngine->AddRect();
			break;
		case GisTool_Blackboard_Draw_Ellipse:			// 绘制椭圆
			pMapEngine->AddEllipse();
			break;
		case GisTool_Blackboard_Draw_Polyline:		// 绘制折线
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddPolyline();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Polygon:			// 绘制多边形
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddPolygon();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Curve:			// 绘制曲线
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddCurve();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Close_Curve:		// 绘制闭合曲线
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddCloseCurve();
				}
			}
			break;

		case GisTool_Blackboard_Draw_Label:			// 绘制标注
			pMapEngine->AddLabel();
			break;
		case GisTool_Blackboard_Draw_Layer_Legend:	// 绘制图层图例
			pMapEngine->AddLayerLegend();
			break;
		case GisTool_Blackboard_Draw_Custom_Legend:	// 绘制自定义图例
			pMapEngine->AddCustomLegend();
			break;

		case GisTool_Misce_Landmark_Calibrate:	// 地标校准
			{
				m_lastPoint = point ;
				pMapEngine->LandmarkCalibrate(m_LButtonPoint,m_lastPoint,FALSE);
			}
			break;

		default:
			break;
		}

		// FUN_END
	}

	// 鼠标左键被双击。
	void KMouseListener::OnLButtonDBLClick(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// 记录最近的鼠标位置
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// 位置偏移量
		int dx = point.x - m_LButtonPoint.x ;
		int dy = point.y - m_LButtonPoint.y ;

		// 矩形尺寸
		int x = min(m_LButtonPoint.x,point.x);
		int y = min(m_LButtonPoint.y,point.y);
		int w = abs(m_LButtonPoint.x - point.x);
		int h = abs(m_LButtonPoint.y - point.y);

		if(tool & GisTool_MapController)
		{
			pMapEngine->ZoomIn( x,y,w,h );
			pMapEngine->Refresh();
		}
		else if(tool & GisTool_BlackboardController && tool == GisTool_Blackboard_Select_Object && pObject)
		{
			::MessageBeep(1);
		}
		// FUN_END
	}

	// 鼠标右键被按下。
	void KMouseListener::OnRButtonDown(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// 记录最近的鼠标位置
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// ...
		// FUN_END
	}

	// 鼠标右键被抬起。
	void KMouseListener::OnRButtonUp(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// 记录最近的鼠标位置
		m_lastPoint = pMouseEventArgs->GetPoint();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		switch(tool)
		{
		case GisTool_Blackboard_Draw_Polyline:
			{
				if(!m_bFinishedGeometry)
				{
					m_bFinishedGeometry = true;
					m_PointSequence.push_back(point);
					pMapEngine->AddPolyline();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Polygon:		// 绘制多边形完成
			{
				if(!m_bFinishedGeometry)
				{
					m_bFinishedGeometry = true;
					m_PointSequence.push_back(point);
					// 添加起点
					m_PointSequence.push_back(m_PointSequence[0]);
					pMapEngine->AddPolygon();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Curve:
			{
				if(!m_bFinishedGeometry)
				{
					m_bFinishedGeometry = true;
					m_PointSequence.push_back(point);
					pMapEngine->AddCurve();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Close_Curve:	// 绘制闭合曲线完成
			{
				if(!m_bFinishedGeometry)
				{
					m_bFinishedGeometry = true;
					m_PointSequence.push_back(point);
					// 添加起点
					m_PointSequence.push_back(m_PointSequence[0]);
					pMapEngine->AddCurve();
				}
			}
			break;
		}

		// Add by shenyc - 2005.5.6
		//::InvalidateRect(pMapEngine->GetOwnerHwnd(),NULL,FALSE);
		// FUN_END
	}

	// 鼠标右键被双击(缩小)
	void KMouseListener::OnRButtonDBLClick(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// 记录最近的鼠标位置
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// 位置偏移量
		int dx = point.x - m_LButtonPoint.x ;
		int dy = point.y - m_LButtonPoint.y ;

		// 矩形尺寸
		int x = min(m_LButtonPoint.x,point.x);
		int y = min(m_LButtonPoint.y,point.y);
		int w = abs(m_LButtonPoint.x - point.x);
		int h = abs(m_LButtonPoint.y - point.y);

		pMapEngine->ZoomOut( x,y,w,h );
		pMapEngine->Refresh();
		// FUN_END
	}

	// 鼠标移动
	void KMouseListener::OnMouseMove(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// 刷新屏幕
		// OnMouseMove 和 OnMouseHover 先后调用
		// 在这里刷新后在 OnMouseHover 不用再次刷新
		pMapEngine->Refresh();

		// 绘制十字线
		BOOL bDrawCrossHairs = pMapEngine->IsDrawCrossHairs();
		if(bDrawCrossHairs)
		{
			HWND hWnd = pMapEngine->GetHwnd();

			// 绘制矩形
			HDC hDC = ::GetDC(hWnd);

			HPEN hPen = ::CreatePen(PS_DOT,1,RGB(0,0,121));
			HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

			//::SetROP2(hDC,R2_XORPEN);	//R2_NOT R2_XORPEN R2_NOTXORPEN R2_NOP -->OK
			//::SelectObject(hDC,(HGDIOBJ)NULL_BRUSH);

			// 鼠标位置
			m_lastPoint = pMouseEventArgs->GetPoint();
			POINT ptCur = pMouseEventArgs->GetPoint();

			if(m_pointPrv.x != m_lastPoint.x && m_pointPrv.y != m_lastPoint.y)
				m_pointPrv = m_lastPoint;

			RECT rect;
			::GetClientRect(hWnd,&rect);

			int w = rect.right;
			int h = rect.bottom;

			// 删除原来的十字线
			//if( (m_pointPrv.x != ptCur.x || m_pointPrv.y != ptCur.y))
			//{
			//	::MoveToEx(hDC,0,m_pointPrv.y,NULL);
			//	::LineTo(hDC,w,m_pointPrv.y);
			//	::MoveToEx(hDC,m_pointPrv.x,0,NULL);
			//	::LineTo(hDC,m_pointPrv.x,h);
			//}

			// 绘制十字线
			::MoveToEx(hDC,0,ptCur.y,NULL);
			::LineTo(hDC,w,ptCur.y);
			::MoveToEx(hDC,ptCur.x,0,NULL);
			::LineTo(hDC,ptCur.x,h);

			::SelectObject( hDC,hOldPen );
			::DeleteObject(hPen) ;

			::ReleaseDC(hWnd,hDC);
		}
		//-------------------------------------------------
		switch(tool)
		{
		case GisTool_Blackboard_Draw_Polyline:
		case GisTool_Blackboard_Draw_Polygon:
		case GisTool_Blackboard_Draw_Curve:
		case GisTool_Blackboard_Draw_Close_Curve:
			{
				// 当绘制多边形时绘制预览
				// 记录坐标最后位置
				m_lastPoint = point;
				// 如果完成则中断
				if(m_bFinishedGeometry)
				{
					break;
				}

				// 点数组
				int iPointCount = m_PointSequence.size();
				if(iPointCount>0)
				{
					int ptc = 2;	// 点默认数量 = 2
					if(tool==GisTool_Blackboard_Draw_Polygon || tool==GisTool_Blackboard_Draw_Close_Curve) ptc = 3;
					POINT apt[3] = { m_PointSequence[iPointCount-1], m_lastPoint, m_PointSequence[0]};

					POINT* apoint = new POINT[iPointCount];
					for(int i=0;i<iPointCount;i++)
						apoint[i] = m_PointSequence[i];

					HWND hWnd = pMapEngine->GetHwnd();
					pMapEngine->Refresh();

					// 绘制矩形
					HDC hDC = ::GetDC(hWnd);

					HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

					::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
					::Polyline(hDC,(const POINT*)apoint,iPointCount);

					::Polyline(hDC,(const POINT*)apt,ptc);

					::SelectObject( hDC,hOldPen );
					::DeleteObject(hPen) ;

					::ReleaseDC(hWnd,hDC);

					_deletea(apoint);

				}// if(iPointCount>0)
			}
			break;

		}// switch(tool)
		//-------------------------------------------------

		return;
	}

	// 鼠标拖动
	void KMouseListener::OnMouseDrag(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;

		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		switch(tool)
		{
		case GisTool_ZoomIn:	// 放大
		case GisTool_ZoomOut:	// 缩小
			{
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// 绘制矩形
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
				::Rectangle(hDC,m_LButtonPoint.x, m_LButtonPoint.y, point.x, point.y);

				::SelectObject( hDC,hOldPen );
				::DeleteObject(hPen) ;

				::ReleaseDC(hWnd,hDC);
			}
			break;

		case GisTool_ZoomPan:	// 漫游
			{
				int dx = point.x - m_lastPoint.x ;
				int dy = point.y - m_lastPoint.y ;
				HWND hWnd = pMapEngine->GetHwnd();
				POINT *pDrawingPos = &pMapEngine->GetDrawingPosition();
				pDrawingPos->x = point.x-m_LButtonPoint.x;
				pDrawingPos->y = point.y-m_LButtonPoint.y;
				::ScrollWindow(hWnd,dx,dy,NULL,NULL);
				m_lastPoint = point ;

				// 解决外部显示问题 - 2008.5.6 by yongchen.shen
				// 有屏幕闪动现象,暂不使用
				//::InvalidateRect(hWnd,NULL,FALSE);
			}
			break;

		case GisTool_Blackboard_Move_Object:	// 拖动编辑图元
			{
				int dx = point.x - m_lastPoint.x ;
				int dy = point.y - m_lastPoint.y ;
				if(pObject)
				{
					KExtents box = pObject->GetExtentsClient();
					double x = box.Xmin + dx;
					double y = box.Ymin + dy;
					pObject->SetExtentsClient(KExtents(x,y,x+box.Width(),y+box.Height()));

					box = pObject->GetExtentsClient();
					double xmin = box.Xmin;
					double ymin = box.Ymin;
					double xmax = box.Xmax;
					double ymax = box.Ymax;

					pMapEngine->ClientToWorld(xmin,ymin);
					pMapEngine->ClientToWorld(xmax,ymax);

					// 设置集合对象的坐标包围盒
					pObject->SetExtents(KExtents(min(xmin,xmax),min(ymin,ymax),max(xmin,xmax),max(ymin,ymax)));

					// 记录坐标位置
					m_lastPoint = point ;

					HWND hWnd = pMapEngine->GetHwnd();
					pMapEngine->Refresh();

					// 绘制矩形
					HDC hDC = ::GetDC(hWnd);

					HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

					::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
					::Rectangle(hDC,(int)box.Xmin, (int)box.Ymin, (int)box.Xmax, (int)box.Ymax);

					::SelectObject( hDC,hOldPen );
					::DeleteObject(hPen) ;

					::ReleaseDC(hWnd,hDC);
				}
			}
			break;

		case GisTool_Blackboard_Draw_Line:	// 画线
			{
				// 记录坐标最后位置
				m_lastPoint = point ;

				// 点数组
				POINT apt[2] = {m_LButtonPoint,point};

				// 拥有者窗口句柄
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// 绘制矩形
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(16,16,16));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
				::Polyline(hDC,(const POINT*)&apt,2);

				::SelectObject( hDC,hOldPen );
				::DeleteObject(hPen) ;

				::ReleaseDC(hWnd,hDC);
			}
			break;

		case GisTool_Blackboard_Draw_Rect:	// 绘制矩形
			{
				// 记录坐标最后位置
				m_lastPoint = point ;

				// 拥有者窗口句柄
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// 绘制矩形
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(16,16,16));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
				::Rectangle(hDC,m_LButtonPoint.x, m_LButtonPoint.y, point.x, point.y);

				::SelectObject( hDC,hOldPen );
				::DeleteObject(hPen) ;

				::ReleaseDC(hWnd,hDC);
			}
			break;

		case GisTool_Blackboard_Draw_Ellipse:	// 绘制椭圆
			{
				// 记录坐标最后位置
				m_lastPoint = point ;

				// 拥有者窗口句柄
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// 绘制矩形
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(16,16,16));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
				::Ellipse(hDC,m_LButtonPoint.x, m_LButtonPoint.y, point.x, point.y);

				::SelectObject( hDC,hOldPen );
				::DeleteObject(hPen) ;

				::ReleaseDC(hWnd,hDC);
			}
			break;

		case GisTool_Blackboard_Draw_Polyline:
		case GisTool_Blackboard_Draw_Polygon:
		case GisTool_Blackboard_Draw_Curve:
		case GisTool_Blackboard_Draw_Close_Curve:
			{
				// 记录坐标最后位置
				m_lastPoint = point ;

				// 点数组
				int iPointCount = m_PointSequence.size();
				if(iPointCount>0)
				{
					int ptc = 2;
					if(tool==GisTool_Blackboard_Draw_Polygon || tool==GisTool_Blackboard_Draw_Close_Curve) ptc = 3;
					POINT apt[3] = { m_PointSequence[iPointCount-1], m_lastPoint, m_PointSequence[0]};

					POINT* apoint = new POINT[iPointCount];
					for(int i=0;i<iPointCount;i++)
					{
						apoint[i] = m_PointSequence[i];
					}

					HWND hWnd = pMapEngine->GetHwnd();
					pMapEngine->Refresh();

					// 绘制矩形
					HDC hDC = ::GetDC(hWnd);

					HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

					::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// 选择一个空画刷
					::Polyline(hDC,(const POINT*)apoint,iPointCount);

					::Polyline(hDC,(const POINT*)apt,ptc);

					::SelectObject( hDC,hOldPen );
					::DeleteObject(hPen) ;

					::ReleaseDC(hWnd,hDC);

					_deletea(apoint);

				}
			}
			break;

		case GisTool_Misce_Landmark_Calibrate:	// 地标校准
			{
				m_lastPoint = point ;
				pMapEngine->LandmarkCalibrate(m_LButtonPoint,m_lastPoint,TRUE);
			}
			break;

		default:
			break;

		}// switch(tool)
		//-------------------------------------------------
		return;
	}

	// 鼠标盘旋在对象上方
	void KMouseListener::OnMouseHover(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// 设置鼠标事件
		m_pMouseEvent = pMouseEvent;
		// 取得鼠标事件参数
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// 地图引擎
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// 记录最近的鼠标位置
		m_lastPoint = pMouseEventArgs->GetPoint();

		// 鼠标属性
		int flag = pMouseEventArgs->GetFlag();		// 鼠标标志
		POINT point = pMouseEventArgs->GetPoint();	// 鼠标位置
		int tool = pMouseEventArgs->GetTool();		// GIS工具
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		if(!pObject )
		{
			//pMapEngine->Refresh();	// 在OnMouseMove中调用过了
			return;
		}

		if(tool == GisTool_Blackboard_Select_Object			// 选择工具
			|| tool == GisTool_Blackboard_Move_Object		// 移动工具
			|| tool == GisTool_Blackboard_Delete_Object)	// 删除工具
		{
			pMapEngine->HoverObject(pObject);
		}

		return;
	}

	// 取消操作。
	void KMouseListener::OnCancel()
	{
		// nobody
	}

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------
