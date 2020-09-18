// KRadarDataView.cpp : 实现文件
//

#include "stdafx.h"
#include "KRadarDataView.h"

namespace Framework
{


	// KRadarDataView

	IMPLEMENT_DYNCREATE(KRadarDataView, KAxinDataView)

	KRadarDataView::KRadarDataView()
	{

	}

	KRadarDataView::~KRadarDataView()
	{
	}

	BEGIN_MESSAGE_MAP(KRadarDataView, KAxinDataView)
	END_MESSAGE_MAP()


	// KRadarDataView 绘图

	void KRadarDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KRadarDataView 诊断

	#ifdef _DEBUG
	void KRadarDataView::AssertValid() const
	{
		KAxinDataView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KRadarDataView::Dump(CDumpContext& dc) const
	{
		KAxinDataView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KRadarDataView 消息处理程序

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
