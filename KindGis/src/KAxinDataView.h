#pragma once

#include "KDiamondDataView.h"

namespace Framework
{
	// KAxinDataView ��ͼ

	class KIND_EXT_CLASS KAxinDataView : public KDiamondDataView
	{
		DECLARE_DYNCREATE(KAxinDataView)

	protected:
		KAxinDataView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KAxinDataView();

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

