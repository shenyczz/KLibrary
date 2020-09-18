#pragma once

#include "KLayer.h"

#include "KFinalDocument.h"

// KGDocument 文档

namespace Framework
{
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

namespace Framework
{

	class KIND_EXT_CLASS KGisDocument : public KFinalDocument
	{
		DECLARE_DYNCREATE(KGisDocument)

	public:
		KGisDocument();
		virtual ~KGisDocument();

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
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
