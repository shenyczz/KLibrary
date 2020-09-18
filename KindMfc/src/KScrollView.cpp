// src\KScrollView.cpp : ʵ���ļ�
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


	// KScrollView ��ͼ

	void KScrollView::OnInitialUpdate()
	{
		CScrollView::OnInitialUpdate();

		CSize sizeTotal;
		// TODO: �������ͼ�ĺϼƴ�С
		sizeTotal.cx = sizeTotal.cy = 100;
		SetScrollSizes(MM_TEXT, sizeTotal);
	}

	void KScrollView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KScrollView ���

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


	// KScrollView ��Ϣ�������

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
