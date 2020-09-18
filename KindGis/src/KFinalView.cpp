// KFinalView.cpp : 实现文件
//

#include "stdafx.h"
#include "KFinalView.h"


namespace Framework
{

	// KFinalView

	IMPLEMENT_DYNCREATE(KFinalView, KShapeFileView)

	KFinalView::KFinalView()
	{

	}

	KFinalView::~KFinalView()
	{
	}

	BEGIN_MESSAGE_MAP(KFinalView, KShapeFileView)
	END_MESSAGE_MAP()


	// KFinalView 绘图

	void KFinalView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KFinalView 诊断

	#ifdef _DEBUG
	void KFinalView::AssertValid() const
	{
		KShapeFileView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KFinalView::Dump(CDumpContext& dc) const
	{
		KShapeFileView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KFinalView 消息处理程序

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
