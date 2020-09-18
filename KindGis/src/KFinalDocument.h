#pragma once

#include "KShapeFileDocument.h"

namespace Framework
{

	// KFinalDocument 文档

	class KIND_EXT_CLASS KFinalDocument : public KShapeFileDocument
	{
		DECLARE_DYNCREATE(KFinalDocument)

	public:
		KFinalDocument();
		virtual ~KFinalDocument();
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
