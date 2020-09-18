#pragma once

#include "KAxinDataView.h"

namespace Framework
{

	// KRadarDataView 视图

	class KIND_EXT_CLASS KRadarDataView : public KAxinDataView
	{
		DECLARE_DYNCREATE(KRadarDataView)

	protected:
		KRadarDataView();           // 动态创建所使用的受保护的构造函数
		virtual ~KRadarDataView();

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

