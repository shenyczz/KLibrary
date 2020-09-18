/******************************************************************************
 ** KGisView.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGisView
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

#include "KMapEngine.h"
#include "KFinalView.h"

namespace Framework
{
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

// KGisView 视图

namespace Framework
{

	class KIND_EXT_CLASS KGisView : public KFinalView
	{
		DECLARE_DYNCREATE(KGisView)

	protected:
		KGisView();           // 动态创建所使用的受保护的构造函数
		virtual ~KGisView();

	public:
		virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

		// 鼠标左键
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
		// 鼠标右键
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
		// 鼠标移动
		afx_msg void OnMouseMove(UINT nFlags, CPoint point);

		DECLARE_MESSAGE_MAP()

	protected:
		// 追踪矩形
		CRectTracker m_RectTracker;

	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
