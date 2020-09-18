#pragma once

#include "KShapeFileView.h"

namespace Framework
{

	// KFinalView 视图

	class KIND_EXT_CLASS KFinalView : public KShapeFileView
	{
		DECLARE_DYNCREATE(KFinalView)

	protected:
		KFinalView();           // 动态创建所使用的受保护的构造函数
		virtual ~KFinalView();

	public:
		virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
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

