// KDiamondDataView.cpp : ʵ���ļ�
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


	// KDiamondDataView ��ͼ

	void KDiamondDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KDiamondDataView ���

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


	// KDiamondDataView ��Ϣ�������

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
