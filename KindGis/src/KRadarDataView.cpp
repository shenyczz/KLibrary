// KRadarDataView.cpp : ʵ���ļ�
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


	// KRadarDataView ��ͼ

	void KRadarDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KRadarDataView ���

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


	// KRadarDataView ��Ϣ�������

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
