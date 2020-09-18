// KAgmrsDataView.cpp : 实现文件
//

#include "stdafx.h"
#include "KAgmrsDataView.h"

namespace Framework
{


	// KAgmrsDataView

	IMPLEMENT_DYNCREATE(KAgmrsDataView, KGisViewBase)

	KAgmrsDataView::KAgmrsDataView()
	{

	}

	KAgmrsDataView::~KAgmrsDataView()
	{
	}

	BEGIN_MESSAGE_MAP(KAgmrsDataView, KGisViewBase)
	END_MESSAGE_MAP()


	// KAgmrsDataView 绘图

	void KAgmrsDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KAgmrsDataView 诊断

	#ifdef _DEBUG
	void KAgmrsDataView::AssertValid() const
	{
		KGisViewBase::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KAgmrsDataView::Dump(CDumpContext& dc) const
	{
		KGisViewBase::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KAgmrsDataView 消息处理程序

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
