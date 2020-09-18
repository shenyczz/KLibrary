#pragma once

#include "KLayer.h"

#include "KFinalDocument.h"

// KGDocument �ĵ�

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
		virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
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
