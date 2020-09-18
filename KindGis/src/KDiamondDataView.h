#pragma once

#include "KBlackboardView.h"

namespace Framework
{

	// KDiamondDataView ��ͼ

	class KIND_EXT_CLASS KDiamondDataView : public KBlackboardView
	{
		DECLARE_DYNCREATE(KDiamondDataView)

	protected:
		KDiamondDataView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KDiamondDataView();

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

