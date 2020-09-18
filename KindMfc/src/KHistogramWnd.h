/******************************************************************************
 ** KHistogramWnd.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KHistogramWnd - ֱ��ͼ����
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
 ** 		KHistogramWnd m_wndHistogram;
 ** 		m_wndHistogram.SetData(LPBYTE lpData);
 ** 		m_wndHistogram.SetDataSize(int iWidth, int iHeight);
 ** 		m_wndHistogram.SetRange(int iMinValue,int iMaxValue);
 ** 		m_wndHistogram.SetThreshold(int iDnValue,int iUpValue);
 ** 		m_wndHistogram.Draw();
 **
******************************************************************************/

#pragma once

namespace Gui
{

	#ifndef WIDTHBYTES
	#define WIDTHBYTES(bits)	((((bits)+31)/32)*4)	// DIB����ֽ���(������4��������)
	#endif

	#define SFX_MSG_HISTOGRAM_DRAG WM_USER + 2002	// ֱ��ͼ�϶��û���Ϣ

	// KHistogramWnd

	class KIND_EXT_CLASS KHistogramWnd : public CWnd
	{
		DECLARE_DYNAMIC(KHistogramWnd)

	public:
		enum EnumDragState
		{
			eDragStateNone = 0,
			eDragStateDown,
			eDragStateUp,
		};

	public:
		KHistogramWnd();
		virtual ~KHistogramWnd();

	protected:
		void Init();
		void Cleanup();

	protected:
		virtual void PreSubclassWindow();
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

		afx_msg void OnPaint();
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

		DECLARE_MESSAGE_MAP()

	///--- inline function beg----------------------------
	public:
		// ����/��ȡ����
		void SetData(LPBYTE lpData) { m_lpData = lpData; m_bValueCounted = FALSE; }
		LPBYTE GetData() { return m_lpData; }

		// ����/��ȡ���ݿ��
		void SetWidth(int iWidth) { m_iWidth = iWidth; }
		int GetWidth() { return m_iWidth; }

		// ����/��ȡ���ݸ߶�
		void SetHeight(int iHeight) { m_iHeight = iHeight; }
		int GetHeight() { return m_iHeight; }

		// ����/��ȡ���ݳߴ�
		void SetDataSize(int iWidth, int iHeight) { m_iWidth = iWidth; m_iHeight = iHeight; }
		CSize GetDataSize() { return CSize(m_iWidth,m_iHeight); }

		// ����/��ȡ��С����ֵ
		void SetMinValue(int iMinValue) { m_iMinValue = iMinValue; }
		int GetMinValue() { return m_iMinValue; }

		// ����/��ȡ�������ֵ
		void SetMaxValue(int iMaxValue) { m_iMaxValue = iMaxValue; }
		int GetMaxValue() { return m_iMaxValue; }

		// ����/��ȡ���ݷ�Χ
		void SetRange(int iMinValue,int iMaxValue) { m_iMinValue = iMinValue; m_iMaxValue = iMaxValue; }
		void GetRange(int& iMinValue,int& iMaxValue) { iMinValue = m_iMinValue; iMaxValue = m_iMaxValue; }

		// ����/��ȡ������ʾ����ֵ
		void SetUpValue(int iUpValue) { m_iUpValue = iUpValue; }
		int GetUpValue() { return m_iUpValue; }

		// ����/��ȡ������ʾ����ֵ
		void SetDnValue(int iDnValue) { m_iDnValue = iDnValue; }
		int GetDnValue() { return m_iDnValue; }

		// ������ʾ������ֵ
		void SetThreshold(int iDnValue,int iUpValue) { m_iDnValue = iDnValue; m_iUpValue = iUpValue; }
		void GetThreshold(int& iDnValue,int& iUpValue) { iDnValue = m_iDnValue; iUpValue = m_iUpValue; }

		// ����/��ȡֱ��ͼ��ɫ
		void SetColor(COLORREF clr) { m_clrHist = clr; }
		COLORREF GetColor() { return m_clrHist; }

		// ����/��ȡ������ɫ
		void SetBkColor(COLORREF clr) { m_clrBack = clr; }
		COLORREF GetBkColor() { return m_clrBack; }

		// ����
		void Draw() { Invalidate(TRUE); }

	///--- inline function end----------------------------


	protected:
		// �������ݸ�������ֵ�ļ���
		void ComputeDataValueCount();

		// ��������������
		void ComputeDrawRect();

		// ��������
		void DrawCoordinate(CDC* pDC);
		// ����������
		void DrawUpDn(CDC* pDC);
		// ��������
		void DrawData(CDC* pDC);

	private:
		void World2DrawRect(int iValue,int& x,int& y);
		int x2v(int x);
		int v2x(int v);

	protected:
		LPBYTE m_lpData;		// ָ���ά���ݵ�ָ��
		int m_iWidth;			// ���ݵĿ��
		int m_iHeight;			// ���ݵĸ߶�

		int m_iMinValue;		// ���ݵ���Сֵ
		int m_iMaxValue;		// ���ݵ����ֵ
		int* m_lpCount;			// ������ֵ�ļ���
		int m_iMaxCount;		// ������

		int m_iUpValue;			// ��ʾ�������������
		int m_iDnValue;			// ��ʾ�������������

		int m_iDragState;		// �϶�״̬: 0-δ�϶�  1-�϶�����  2-�϶�����  
		CRect m_rectDraw;		// ����������
		CRect m_rectClient;		// �ͻ�������

		BOOL m_bValueCounted;	// �Ƿ�������ֵ����

		HCURSOR m_hCursorDrag;	// �϶����

		COLORREF m_clrHist;		// ֱ��ͼ��ɫ
		COLORREF m_clrBack;		// ������ɫ
	};

//---------------------------------------------------------
}// namespace Gui - End
//---------------------------------------------------------
