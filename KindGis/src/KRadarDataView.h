#pragma once

#include "KAxinDataView.h"

namespace Framework
{

	// KRadarDataView ��ͼ

	class KIND_EXT_CLASS KRadarDataView : public KAxinDataView
	{
		DECLARE_DYNCREATE(KRadarDataView)

	protected:
		KRadarDataView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KRadarDataView();

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

