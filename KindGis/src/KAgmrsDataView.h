#pragma once

#include "KGisViewBase.h"

namespace Framework
{


	// KAgmrsDataView ��ͼ

	class KIND_EXT_CLASS KAgmrsDataView : public KGisViewBase
	{
		DECLARE_DYNCREATE(KAgmrsDataView)

	protected:
		KAgmrsDataView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KAgmrsDataView();

	public:
		virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		DECLARE_MESSAGE_MAP()
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

