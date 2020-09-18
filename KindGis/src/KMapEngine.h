/******************************************************************************
 ** KMapEngine.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KMapEngine - ��ͼ����
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
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
///�Զ�����Ϣ - Beg
///----------------
// ��ڰ�ͼ����ӵ�
#define WM_USER_ADD_POINT					(WM_USER+1050)
// ��ڰ�ͼ������߶�
#define WM_USER_ADD_LINE					(WM_USER+1051)
// ��ڰ�ͼ����Ӿ���
#define WM_USER_ADD_RECT					(WM_USER+1052)
// ��ڰ�ͼ�������Բ
#define WM_USER_ADD_ELLIPSE					(WM_USER+1053)
// ��ڰ�ͼ���������
#define WM_USER_ADD_POLYLINE				(WM_USER+1054)
// ��ڰ�ͼ����Ӷ����
#define WM_USER_ADD_POLYGON					(WM_USER+1055)
// ��ڰ�ͼ���������
#define WM_USER_ADD_CURVE					(WM_USER+1056)
// ��ڰ�ͼ����ӱպ�����
#define WM_USER_ADD_CLOSE_CURVE				(WM_USER+1057)

// ��ӱ�ע���ڰ�ͼ��
#define WM_USER_ADD_LABEL					(WM_USER+1070)
// ���ͼ��ͼ�����ڰ�ͼ��
#define WM_USER_ADD_LAYER_LEGEND			(WM_USER+1071)
// ����Զ���ͼ�����ڰ�ͼ��
#define WM_USER_ADD_CUSTOM_LEGEND			(WM_USER+1072)

// ѡ�ж���
#define WM_USER_SELECT_OBJECT				(WM_USER+1080)

// �ر�У׼
#define WM_USER_LANDMARK_CALIBRATE			(WM_USER+1090)
#define WM_USER_LANDMARK_CALIBRATE_STEP		(WM_USER+1091)
///----------------
///�Զ�����Ϣ - End
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
	// ���ھ��
	virtual void SetHwnd(HWND hWnd) { m_hWnd = hWnd; }
	virtual HWND GetHwnd() { return m_hWnd; }

	// ���»���
	virtual void EnableRedraw(BOOL bRedraw = TRUE) { m_bRedraw = bRedraw; }
	virtual BOOL IsRedraw() { return m_bRedraw; }

	// ����ʮ����
	virtual void EnableDrawCrossHairs(bool bDrawCrossHairs = TRUE) { m_bDrawCrossHairs = bDrawCrossHairs; }
	virtual BOOL IsDrawCrossHairs() { return m_bDrawCrossHairs; }

	// ������ɫ
	virtual void SetBackColor(COLORREF clr) { m_clrBack = clr; }
	virtual COLORREF GetBackColor() { return m_clrBack; }

	// ȡ��λͼ���
	virtual HBITMAP GetHBitmap() { return m_hPaintBuffer;}

	// ӵ���ߴ��ڻ���λ��
	virtual POINT& GetDrawingPosition() { return m_DrawingPosition;}

	// ��ȡͼ�㼯
	TPtrCollection<KLayer*>* GetLayerPtrCollection() { return m_pLayerPtrCollection; }

	// ����������
	KMouseListenerCollection* GetMouseListenerCollection() { return m_pMouseListenerCollection; }

	// ȡ�õ�ǰ��������
	KMouseListener* GetCurrentMouseListener() { return m_pMouseListenerCollection->GetCurrentMouseListener(); }


public:
	// �ͻ�������ת��Ϊ��������
	virtual void ClientToWorld(double& x, double& y);
	// ��������ת��Ϊ�ͻ�������
	virtual void WorldToClient(double& x, double& y);

public:
	// ��λ
	virtual void Reset();
	// ������ͼ�󶨾���
	virtual void SetViewBound( int x, int y, int w, int h );
	// ���»���
	virtual void ReDraw(int x, int y, int w, int h);
	// ���Ƶ�ͼ
	virtual void DrawMap();
	// ˢ��
	virtual void Refresh();
	// ˢ��
	virtual void Refresh(BOOL bRedraw);
	// �ƶ�
	virtual void Scroll(int dx, int dy);

	// ����/��ȡ���о���
	virtual void SetClipRect (int x, int y, int w, int h);
	virtual void GetClipRect (int& x, int& y, int& w, int& h);

	// �����ݰ󶨾���
	virtual BOOL HasDataBound();

	// ���Զ������ݰ󶨾���
	virtual BOOL HasCustomDataBound();

	// �����Զ������ݰ󶨾���
	virtual void SetCustomDataBound(double xmin, double ymin, double xmax, double ymax);

public:
	// �Ŵ�
	virtual void ZoomIn(int x, int y, int w, int h);
	// ��С
	virtual void ZoomOut(int x, int y, int w, int h);
	// ����
	virtual void ZoomPan(int dx, int dy);

public:
	// �ر�У׼
	virtual void LandmarkCalibrate(int dx,int dy);
	// �ر�У׼
	virtual void LandmarkCalibrate(POINT ptBeg,POINT ptEnd,BOOL bDraging);

public:
	// ���������·��(������)
	virtual GraphicsPath* BuildClipPath();

public:
	// ��ڰ�ͼ����ӵ�
	virtual void AddPoint();
	// ��ڰ�ͼ������߶�
	virtual void AddLine();
	// ��ڰ�ͼ����Ӿ���
	virtual void AddRect();
	// ��ڰ�ͼ�������Բ
	virtual void AddEllipse();
	// ��ڰ�ͼ���������
	virtual void AddPolyline();
	// ��ڰ�ͼ����Ӷ����
	virtual void AddPolygon();
	// ��ڰ�ͼ���������
	virtual void AddCurve();
	// ��ڰ�ͼ����ӱպ�����
	virtual void AddCloseCurve();

	// ��ڰ�ͼ����ӱ�ע
	virtual void AddLabel();
	// ��ڰ�ͼ�����ͼ���ͼ��
	virtual void AddLayerLegend();
	// ��ڰ�ͼ������Զ����ͼ��
	virtual void AddCustomLegend();

public:
	// ��������ͼ�ζ���
	virtual void HoverObject(KObject* pObject);
	// ѡ�񼸺�ͼ�ζ���
	virtual void SelectObject(KObject* pObject);
	// ɾ������ͼ�ζ���
	virtual void DeleteObject(KObject* pObject);

	// ȡ�úڰ�ͼ��
	virtual KLayer* GetBlackboardLayer(int index);

private:
	// �������ݰ󶨾���
	void computeDataBound();
	// ��������󶨾���
	void computeWorldBound();
	// ���������
	void clearBuffer( HDC hDC, int x, int y, int w, int h );

protected:
	// �ͻ����ߴ�
	int m_clientWidth;
	int m_clientHeight;

	// ��������
	double m_dataX;
	double m_dataY;
	double m_dataWidth;
	double m_dataHeight;

	// ������������
	double m_worldX;
	double m_worldY;
	double m_worldWidth;
	double m_worldHeight;

	// ����������
	int m_nClipX;
	int m_nClipY;
	int m_nClipWidth;
	int m_nClipHeight;

	// �Զ�����������
	double m_CustomDataMinX;
	double m_CustomDataMinY;
	double m_CustomDataMaxX;
	double m_CustomDataMaxY;

protected:
	// ���ھ��
	HWND m_hWnd;

	// ������
	HBITMAP m_hMapBuffer;	// ��ͼ������
	HBITMAP m_hPaintBuffer;	// ���ƻ�����

	// ������ɫ
	COLORREF m_clrBack;

	// ���ڻ���ԭ��
	POINT m_DrawingPosition;

	POINT m_pointDragingBeg;	// ����϶���ʼλ��
	POINT m_pointDragingEnd;	// ����϶�����λ��

	SIZE m_sizeDraging;			// ����϶��ߴ�

	// ���»���
	BOOL m_bRedraw;

	// ����ʮ����
	BOOL m_bDrawCrossHairs;	

	// ͼ�㼯
	TPtrCollection<KLayer*>* m_pLayerPtrCollection;

	// ����������
	KMouseListenerCollection* m_pMouseListenerCollection;

	// ����ȡ·��
	GraphicsPath* m_pClipPath;
};

