// src\KView.cpp : 实现文件
//

#include "stdafx.h"
#include "KView.h"


// KView
namespace Framework
{

	IMPLEMENT_DYNCREATE(KView, CView)

	KView::KView()
	{
		m_pImage = NULL;
		m_clrBack = RGB(255,255,255);
	}

	KView::~KView()
	{
	}

	BEGIN_MESSAGE_MAP(KView, CView)
		ON_WM_ERASEBKGND()
	END_MESSAGE_MAP()


	// KView 绘图

	void KView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KView 诊断

	#ifdef _DEBUG
	void KView::AssertValid() const
	{
		CView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KView::Dump(CDumpContext& dc) const
	{
		CView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KView 消息处理程序

	BOOL KView::OnEraseBkgnd(CDC* pDC)
	{
		CRect rect;
		GetClientRect(&rect);
		pDC->FillSolidRect(&rect,m_clrBack);
		return TRUE;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
