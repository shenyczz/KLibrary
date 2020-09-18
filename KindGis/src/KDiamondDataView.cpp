// KDiamondDataView.cpp : 实现文件
//

#include "stdafx.h"
#include "KDiamondDataView.h"

namespace Framework
{


	// KDiamondDataView

	IMPLEMENT_DYNCREATE(KDiamondDataView, KBlackboardView)

	KDiamondDataView::KDiamondDataView()
	{

	}

	KDiamondDataView::~KDiamondDataView()
	{
	}

	BEGIN_MESSAGE_MAP(KDiamondDataView, KBlackboardView)
	END_MESSAGE_MAP()


	// KDiamondDataView 绘图

	void KDiamondDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KDiamondDataView 诊断

	#ifdef _DEBUG
	void KDiamondDataView::AssertValid() const
	{
		KBlackboardView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KDiamondDataView::Dump(CDumpContext& dc) const
	{
		KBlackboardView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KDiamondDataView 消息处理程序

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
