// src\KScrollView.cpp : 实现文件
//

#include "stdafx.h"
#include "KScrollView.h"


// KScrollView
namespace Framework
{

	IMPLEMENT_DYNCREATE(KScrollView, CScrollView)

	KScrollView::KScrollView()
	{

	}

	KScrollView::~KScrollView()
	{
	}


	BEGIN_MESSAGE_MAP(KScrollView, CScrollView)
	END_MESSAGE_MAP()


	// KScrollView 绘图

	void KScrollView::OnInitialUpdate()
	{
		CScrollView::OnInitialUpdate();

		CSize sizeTotal;
		// TODO: 计算此视图的合计大小
		sizeTotal.cx = sizeTotal.cy = 100;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

	void KScrollView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KScrollView 诊断

	#ifdef _DEBUG
	void KScrollView::AssertValid() const
	{
		CScrollView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KScrollView::Dump(CDumpContext& dc) const
	{
		CScrollView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KScrollView 消息处理程序

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
