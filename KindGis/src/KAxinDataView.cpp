// KAxinDataView.cpp : ʵ���ļ�
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


	// KAxinDataView ��ͼ

	void KAxinDataView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KAxinDataView ���

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


	// KAxinDataView ��Ϣ�������

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
