#pragma once

#include "KRadarDataView.h"

namespace Framework
{

	// KShapeFileView ��ͼ

	class KIND_EXT_CLASS KShapeFileView : public KRadarDataView
	{
		DECLARE_DYNCREATE(KShapeFileView)

	protected:
		KShapeFileView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KShapeFileView();

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

