#pragma once

#include "KShapeFileView.h"

namespace Framework
{

	// KFinalView ��ͼ

	class KIND_EXT_CLASS KFinalView : public KShapeFileView
	{
		DECLARE_DYNCREATE(KFinalView)

	protected:
		KFinalView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KFinalView();

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

