#pragma once

#include "KRadarDataDocument.h"

namespace Framework
{

	// KShapeFileDocument 文档

	class KIND_EXT_CLASS KShapeFileDocument : public KRadarDataDocument
	{
		DECLARE_DYNCREATE(KShapeFileDocument)

	public:
		KShapeFileDocument();
		virtual ~KShapeFileDocument();
	#ifndef _WIN32_WCE
		virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
	#endif
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		virtual BOOL OnNewDocument();

		DECLARE_MESSAGE_MAP()

	protected:
		// 添加 Shape 数据图层
		virtual BOOL AddShapeLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
