// KShapeFileView.cpp : ʵ���ļ�
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


	// KShapeFileView ��ͼ

	void KShapeFileView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KShapeFileView ���

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


	// KShapeFileView ��Ϣ�������
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
