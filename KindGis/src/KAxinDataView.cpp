// KAxinDataView.cpp : 实现文件
//

#include "stdafx.h"
#include "KAxinDataView.h"


namespace Framework
{
	// KAxinDataView

	IMPLEMENT_DYNCREATE(KAxinDataView, KDiamondDataView)

	KAxinDataView::KAxinDataView()
	{

	}

	KAxinDataView::~KAxinDataView()
	{
	}

	BEGIN_MESSAGE_MAP(KAxinDataView, KDiamondDataView)
	END_MESSAGE_MAP()


	// KAxinDataView 绘图

	void KAxinDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KAxinDataView 诊断

	#ifdef _DEBUG
	void KAxinDataView::AssertValid() const
	{
		KDiamondDataView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KAxinDataView::Dump(CDumpContext& dc) const
	{
		KDiamondDataView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KAxinDataView 消息处理程序

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
