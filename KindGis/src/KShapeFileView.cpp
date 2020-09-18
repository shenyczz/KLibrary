// KShapeFileView.cpp : 实现文件
//

#include "stdafx.h"
#include "KShapeFileView.h"

#include "KRadarDataView.h"

namespace Framework
{

	// KShapeFileView

	IMPLEMENT_DYNCREATE(KShapeFileView, KRadarDataView)

	KShapeFileView::KShapeFileView()
	{

	}

	KShapeFileView::~KShapeFileView()
	{
	}

	BEGIN_MESSAGE_MAP(KShapeFileView, KRadarDataView)
	END_MESSAGE_MAP()


	// KShapeFileView 绘图

	void KShapeFileView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KShapeFileView 诊断

	#ifdef _DEBUG
	void KShapeFileView::AssertValid() const
	{
		KRadarDataView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KShapeFileView::Dump(CDumpContext& dc) const
	{
		KRadarDataView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KShapeFileView 消息处理程序
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
