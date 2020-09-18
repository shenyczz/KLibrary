/******************************************************************************
 ** KHistogramWnd.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KHistogramWnd - 直方图窗口
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
	#define WIDTHBYTES(bits)	((((bits)+31)/32)*4)	// DIB宽度字节数(必须是4的整数倍)
	#endif

	#define SFX_MSG_HISTOGRAM_DRAG WM_USER + 2002	// 直方图拖动用户消息

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
		// 设置/获取数据
		void SetData(LPBYTE lpData) { m_lpData = lpData; m_bValueCounted = FALSE; }
		LPBYTE GetData() { return m_lpData; }

		// 设置/获取数据宽度
		void SetWidth(int iWidth) { m_iWidth = iWidth; }
		int GetWidth() { return m_iWidth; }

		// 设置/获取数据高度
		void SetHeight(int iHeight) { m_iHeight = iHeight; }
		int GetHeight() { return m_iHeight; }

		// 设置/获取数据尺寸
		void SetDataSize(int iWidth, int iHeight) { m_iWidth = iWidth; m_iHeight = iHeight; }
		CSize GetDataSize() { return CSize(m_iWidth,m_iHeight); }

		// 设置/获取最小数据值
		void SetMinValue(int iMinValue) { m_iMinValue = iMinValue; }
		int GetMinValue() { return m_iMinValue; }

		// 设置/获取最大数据值
		void SetMaxValue(int iMaxValue) { m_iMaxValue = iMaxValue; }
		int GetMaxValue() { return m_iMaxValue; }

		// 设置/获取数据范围
		void SetRange(int iMinValue,int iMaxValue) { m_iMinValue = iMinValue; m_iMaxValue = iMaxValue; }
		void GetRange(int& iMinValue,int& iMaxValue) { iMinValue = m_iMinValue; iMaxValue = m_iMaxValue; }

		// 设置/获取数据显示上限值
		void SetUpValue(int iUpValue) { m_iUpValue = iUpValue; }
		int GetUpValue() { return m_iUpValue; }

		// 设置/获取数据显示下限值
		void SetDnValue(int iDnValue) { m_iDnValue = iDnValue; }
		int GetDnValue() { return m_iDnValue; }

		// 设置显示上下阈值
		void SetThreshold(int iDnValue,int iUpValue) { m_iDnValue = iDnValue; m_iUpValue = iUpValue; }
		void GetThreshold(int& iDnValue,int& iUpValue) { iDnValue = m_iDnValue; iUpValue = m_iUpValue; }

		// 设置/获取直方图颜色
		void SetColor(COLORREF clr) { m_clrHist = clr; }
		COLORREF GetColor() { return m_clrHist; }

		// 设置/获取背景颜色
		void SetBkColor(COLORREF clr) { m_clrBack = clr; }
		COLORREF GetBkColor() { return m_clrBack; }

		// 绘制
		void Draw() { Invalidate(TRUE); }

	///--- inline function end----------------------------


	protected:
		// 计算数据各个数据值的计数
		void ComputeDataValueCount();

		// 计算绘制区域矩形
		void ComputeDrawRect();

		// 绘制坐标
		void DrawCoordinate(CDC* pDC);
		// 绘制上下限
		void DrawUpDn(CDC* pDC);
		// 绘制数据
		void DrawData(CDC* pDC);

	private:
		void World2DrawRect(int iValue,int& x,int& y);
		int x2v(int x);
		int v2x(int v);

	protected:
		LPBYTE m_lpData;		// 指向二维数据的指针
		int m_iWidth;			// 数据的宽度
		int m_iHeight;			// 数据的高度

		int m_iMinValue;		// 数据的最小值
		int m_iMaxValue;		// 数据的最大值
		int* m_lpCount;			// 各个数值的计数
		int m_iMaxCount;		// 最大计数

		int m_iUpValue;			// 显示数据区间的上限
		int m_iDnValue;			// 显示数据区间的下限

		int m_iDragState;		// 拖动状态: 0-未拖动  1-拖动下限  2-拖动上限  
		CRect m_rectDraw;		// 绘制区矩形
		CRect m_rectClient;		// 客户区矩形

		BOOL m_bValueCounted;	// 是否计算过数值计数

		HCURSOR m_hCursorDrag;	// 拖动光标

		COLORREF m_clrHist;		// 直方图颜色
		COLORREF m_clrBack;		// 背景颜色
	};

//---------------------------------------------------------
}// namespace Gui - End
//---------------------------------------------------------
