// KFinalView.cpp : ʵ���ļ�
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


	// KFinalView ��ͼ

	void KFinalView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KFinalView ���

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


	// KFinalView ��Ϣ�������

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
