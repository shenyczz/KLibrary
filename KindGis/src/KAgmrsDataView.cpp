// KAgmrsDataView.cpp : ʵ���ļ�
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


	// KAgmrsDataView ��ͼ

	void KAgmrsDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KAgmrsDataView ���

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


	// KAgmrsDataView ��Ϣ�������

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
