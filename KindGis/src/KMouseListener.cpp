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

		m_LButtonPoint.x = 0;	// �������״ΰ���ʱλ��
		m_LButtonPoint.y = 0;	// �������״ΰ���ʱλ��

		m_PointSequence.clear();
	}

	KMouseListener::~KMouseListener(void)
	{
		m_PointSequence.clear();
	}

	// ������������
	void KMouseListener::OnLButtonDown(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// ��¼��������λ��
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
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

	// ��������̧��
	void KMouseListener::OnLButtonUp(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// ��¼��������λ��
		m_lastPoint = pMouseEventArgs->GetPoint();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// λ��ƫ����
		int dx = point.x - m_LButtonPoint.x ;
		int dy = point.y - m_LButtonPoint.y ;

		// ���γߴ�
		int x = min(m_LButtonPoint.x,point.x);
		int y = min(m_LButtonPoint.y,point.y);
		int w = abs(m_LButtonPoint.x - point.x);
		int h = abs(m_LButtonPoint.y - point.y);

		switch(tool)
		{
		case GisTool_ZoomIn:	// �Ŵ�
			{
				pMapEngine->ZoomIn(x,y,w,h);
				//pMapEngine->Refresh();
			}
			break;
		case GisTool_ZoomOut:	// ��С
			{
				pMapEngine->ZoomOut(x,y,w,h);
				//pMapEngine->Refresh();
			}
			break;
		case GisTool_ZoomPan:	// ����
			{
				POINT* pDrawPos = &pMapEngine->GetDrawingPosition();
				pDrawPos->x = 0;
				pDrawPos->y = 0;
				pMapEngine->ZoomPan(dx,dy);
				pMapEngine->Refresh();
			}
			break;

		case GisTool_Blackboard_Select_Object:			// ѡ��ڰ��ں�����
			// ��LButtonDown��ʵʩ
			//pMapEngine->SelectObject(pObject);
			break;

		case GisTool_Blackboard_Delete_Object:			// ɾ���ڰ��ں�����
			// ��LButtonDown��ʵʩ
			//pMapEngine->DeleteObject(pObject);
			break;

		case GisTool_Blackboard_Move_Object:				// �ƶ��ڰ��ں�����
			{
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->EnableRedraw();
				::InvalidateRect(hWnd,NULL,FALSE);
			}
			break;

		case GisTool_Blackboard_Draw_Point:			// ���Ƶ�
			pMapEngine->AddPoint();
			break;
		case GisTool_Blackboard_Draw_Line:			// �����߶�
			pMapEngine->AddLine();
			break;
		case GisTool_Blackboard_Draw_Rect:			// ���ƾ���
			pMapEngine->AddRect();
			break;
		case GisTool_Blackboard_Draw_Ellipse:			// ������Բ
			pMapEngine->AddEllipse();
			break;
		case GisTool_Blackboard_Draw_Polyline:		// ��������
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddPolyline();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Polygon:			// ���ƶ����
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddPolygon();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Curve:			// ��������
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddCurve();
				}
			}
			break;
		case GisTool_Blackboard_Draw_Close_Curve:		// ���Ʊպ�����
			{
				if(!m_bFinishedGeometry)
				{
					m_PointSequence.push_back(point);
					pMapEngine->AddCloseCurve();
				}
			}
			break;

		case GisTool_Blackboard_Draw_Label:			// ���Ʊ�ע
			pMapEngine->AddLabel();
			break;
		case GisTool_Blackboard_Draw_Layer_Legend:	// ����ͼ��ͼ��
			pMapEngine->AddLayerLegend();
			break;
		case GisTool_Blackboard_Draw_Custom_Legend:	// �����Զ���ͼ��
			pMapEngine->AddCustomLegend();
			break;

		case GisTool_Misce_Landmark_Calibrate:	// �ر�У׼
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

	// ��������˫����
	void KMouseListener::OnLButtonDBLClick(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// ��¼��������λ��
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// λ��ƫ����
		int dx = point.x - m_LButtonPoint.x ;
		int dy = point.y - m_LButtonPoint.y ;

		// ���γߴ�
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

	// ����Ҽ������¡�
	void KMouseListener::OnRButtonDown(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// ��¼��������λ��
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// ...
		// FUN_END
	}

	// ����Ҽ���̧��
	void KMouseListener::OnRButtonUp(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// ��¼��������λ��
		m_lastPoint = pMouseEventArgs->GetPoint();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
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
		case GisTool_Blackboard_Draw_Polygon:		// ���ƶ�������
			{
				if(!m_bFinishedGeometry)
				{
					m_bFinishedGeometry = true;
					m_PointSequence.push_back(point);
					// ������
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
		case GisTool_Blackboard_Draw_Close_Curve:	// ���Ʊպ��������
			{
				if(!m_bFinishedGeometry)
				{
					m_bFinishedGeometry = true;
					m_PointSequence.push_back(point);
					// ������
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

	// ����Ҽ���˫��(��С)
	void KMouseListener::OnRButtonDBLClick(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// ��¼��������λ��
		m_lastPoint = m_LButtonPoint = pMouseEventArgs->GetPoint();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// λ��ƫ����
		int dx = point.x - m_LButtonPoint.x ;
		int dy = point.y - m_LButtonPoint.y ;

		// ���γߴ�
		int x = min(m_LButtonPoint.x,point.x);
		int y = min(m_LButtonPoint.y,point.y);
		int w = abs(m_LButtonPoint.x - point.x);
		int h = abs(m_LButtonPoint.y - point.y);

		pMapEngine->ZoomOut( x,y,w,h );
		pMapEngine->Refresh();
		// FUN_END
	}

	// ����ƶ�
	void KMouseListener::OnMouseMove(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		// ˢ����Ļ
		// OnMouseMove �� OnMouseHover �Ⱥ����
		// ������ˢ�º��� OnMouseHover �����ٴ�ˢ��
		pMapEngine->Refresh();

		// ����ʮ����
		BOOL bDrawCrossHairs = pMapEngine->IsDrawCrossHairs();
		if(bDrawCrossHairs)
		{
			HWND hWnd = pMapEngine->GetHwnd();

			// ���ƾ���
			HDC hDC = ::GetDC(hWnd);

			HPEN hPen = ::CreatePen(PS_DOT,1,RGB(0,0,121));
			HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

			//::SetROP2(hDC,R2_XORPEN);	//R2_NOT R2_XORPEN R2_NOTXORPEN R2_NOP -->OK
			//::SelectObject(hDC,(HGDIOBJ)NULL_BRUSH);

			// ���λ��
			m_lastPoint = pMouseEventArgs->GetPoint();
			POINT ptCur = pMouseEventArgs->GetPoint();

			if(m_pointPrv.x != m_lastPoint.x && m_pointPrv.y != m_lastPoint.y)
				m_pointPrv = m_lastPoint;

			RECT rect;
			::GetClientRect(hWnd,&rect);

			int w = rect.right;
			int h = rect.bottom;

			// ɾ��ԭ����ʮ����
			//if( (m_pointPrv.x != ptCur.x || m_pointPrv.y != ptCur.y))
			//{
			//	::MoveToEx(hDC,0,m_pointPrv.y,NULL);
			//	::LineTo(hDC,w,m_pointPrv.y);
			//	::MoveToEx(hDC,m_pointPrv.x,0,NULL);
			//	::LineTo(hDC,m_pointPrv.x,h);
			//}

			// ����ʮ����
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
				// �����ƶ����ʱ����Ԥ��
				// ��¼�������λ��
				m_lastPoint = point;
				// ���������ж�
				if(m_bFinishedGeometry)
				{
					break;
				}

				// ������
				int iPointCount = m_PointSequence.size();
				if(iPointCount>0)
				{
					int ptc = 2;	// ��Ĭ������ = 2
					if(tool==GisTool_Blackboard_Draw_Polygon || tool==GisTool_Blackboard_Draw_Close_Curve) ptc = 3;
					POINT apt[3] = { m_PointSequence[iPointCount-1], m_lastPoint, m_PointSequence[0]};

					POINT* apoint = new POINT[iPointCount];
					for(int i=0;i<iPointCount;i++)
						apoint[i] = m_PointSequence[i];

					HWND hWnd = pMapEngine->GetHwnd();
					pMapEngine->Refresh();

					// ���ƾ���
					HDC hDC = ::GetDC(hWnd);

					HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

					::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
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

	// ����϶�
	void KMouseListener::OnMouseDrag(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;

		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		switch(tool)
		{
		case GisTool_ZoomIn:	// �Ŵ�
		case GisTool_ZoomOut:	// ��С
			{
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// ���ƾ���
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
				::Rectangle(hDC,m_LButtonPoint.x, m_LButtonPoint.y, point.x, point.y);

				::SelectObject( hDC,hOldPen );
				::DeleteObject(hPen) ;

				::ReleaseDC(hWnd,hDC);
			}
			break;

		case GisTool_ZoomPan:	// ����
			{
				int dx = point.x - m_lastPoint.x ;
				int dy = point.y - m_lastPoint.y ;
				HWND hWnd = pMapEngine->GetHwnd();
				POINT *pDrawingPos = &pMapEngine->GetDrawingPosition();
				pDrawingPos->x = point.x-m_LButtonPoint.x;
				pDrawingPos->y = point.y-m_LButtonPoint.y;
				::ScrollWindow(hWnd,dx,dy,NULL,NULL);
				m_lastPoint = point ;

				// ����ⲿ��ʾ���� - 2008.5.6 by yongchen.shen
				// ����Ļ��������,�ݲ�ʹ��
				//::InvalidateRect(hWnd,NULL,FALSE);
			}
			break;

		case GisTool_Blackboard_Move_Object:	// �϶��༭ͼԪ
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

					// ���ü��϶���������Χ��
					pObject->SetExtents(KExtents(min(xmin,xmax),min(ymin,ymax),max(xmin,xmax),max(ymin,ymax)));

					// ��¼����λ��
					m_lastPoint = point ;

					HWND hWnd = pMapEngine->GetHwnd();
					pMapEngine->Refresh();

					// ���ƾ���
					HDC hDC = ::GetDC(hWnd);

					HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

					::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
					::Rectangle(hDC,(int)box.Xmin, (int)box.Ymin, (int)box.Xmax, (int)box.Ymax);

					::SelectObject( hDC,hOldPen );
					::DeleteObject(hPen) ;

					::ReleaseDC(hWnd,hDC);
				}
			}
			break;

		case GisTool_Blackboard_Draw_Line:	// ����
			{
				// ��¼�������λ��
				m_lastPoint = point ;

				// ������
				POINT apt[2] = {m_LButtonPoint,point};

				// ӵ���ߴ��ھ��
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// ���ƾ���
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(16,16,16));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
				::Polyline(hDC,(const POINT*)&apt,2);

				::SelectObject( hDC,hOldPen );
				::DeleteObject(hPen) ;

				::ReleaseDC(hWnd,hDC);
			}
			break;

		case GisTool_Blackboard_Draw_Rect:	// ���ƾ���
			{
				// ��¼�������λ��
				m_lastPoint = point ;

				// ӵ���ߴ��ھ��
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// ���ƾ���
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(16,16,16));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
				::Rectangle(hDC,m_LButtonPoint.x, m_LButtonPoint.y, point.x, point.y);

				::SelectObject( hDC,hOldPen );
				::DeleteObject(hPen) ;

				::ReleaseDC(hWnd,hDC);
			}
			break;

		case GisTool_Blackboard_Draw_Ellipse:	// ������Բ
			{
				// ��¼�������λ��
				m_lastPoint = point ;

				// ӵ���ߴ��ھ��
				HWND hWnd = pMapEngine->GetHwnd();
				pMapEngine->Refresh();

				// ���ƾ���
				HDC hDC = ::GetDC(hWnd);

				HPEN hPen = ::CreatePen(PS_SOLID,1,RGB(16,16,16));
				HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

				::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
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
				// ��¼�������λ��
				m_lastPoint = point ;

				// ������
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

					// ���ƾ���
					HDC hDC = ::GetDC(hWnd);

					HPEN hPen = ::CreatePen(PS_DOT,1,RGB(255,0,0));
					HPEN hOldPen = (HPEN)::SelectObject(hDC,hPen);

					::SelectObject(hDC,(HBRUSH)::GetStockObject(NULL_BRUSH));	// ѡ��һ���ջ�ˢ
					::Polyline(hDC,(const POINT*)apoint,iPointCount);

					::Polyline(hDC,(const POINT*)apt,ptc);

					::SelectObject( hDC,hOldPen );
					::DeleteObject(hPen) ;

					::ReleaseDC(hWnd,hDC);

					_deletea(apoint);

				}
			}
			break;

		case GisTool_Misce_Landmark_Calibrate:	// �ر�У׼
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

	// ��������ڶ����Ϸ�
	void KMouseListener::OnMouseHover(KMouseEvent* pMouseEvent)
	{
		//-------------------------------------------------
		// ��������¼�
		m_pMouseEvent = pMouseEvent;
		// ȡ������¼�����
		KMouseEventArgs* pMouseEventArgs = &pMouseEvent->GetMouseEventArgs();
		// ��ͼ����
		KMapEngine* pMapEngine = (KMapEngine*)pMouseEventArgs->GetSource();
		// ��¼��������λ��
		m_lastPoint = pMouseEventArgs->GetPoint();

		// �������
		int flag = pMouseEventArgs->GetFlag();		// ����־
		POINT point = pMouseEventArgs->GetPoint();	// ���λ��
		int tool = pMouseEventArgs->GetTool();		// GIS����
		KObject* pObject = (KObject*)pMouseEventArgs->GetTarget();
		//-------------------------------------------------
		if(!pObject )
		{
			//pMapEngine->Refresh();	// ��OnMouseMove�е��ù���
			return;
		}

		if(tool == GisTool_Blackboard_Select_Object			// ѡ�񹤾�
			|| tool == GisTool_Blackboard_Move_Object		// �ƶ�����
			|| tool == GisTool_Blackboard_Delete_Object)	// ɾ������
		{
			pMapEngine->HoverObject(pObject);
		}

		return;
	}

	// ȡ��������
	void KMouseListener::OnCancel()
	{
		// nobody
	}

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------
