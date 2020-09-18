#pragma once

#include "KGisDocumentBase.h"

namespace Framework
{

	// KAgmrsDataDocument 文档

	class KIND_EXT_CLASS KAgmrsDataDocument : public KGisDocumentBase
	{
		DECLARE_DYNCREATE(KAgmrsDataDocument)

	public:
		KAgmrsDataDocument();
		virtual ~KAgmrsDataDocument();
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
		// 添加 Agmrs 数据图层
		virtual BOOL AddAgmrsLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);

	private:
		int GetLayerType(KProvider* pProvider);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
