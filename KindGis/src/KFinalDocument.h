#pragma once

#include "KShapeFileDocument.h"

namespace Framework
{

	// KFinalDocument �ĵ�

	class KIND_EXT_CLASS KFinalDocument : public KShapeFileDocument
	{
		DECLARE_DYNCREATE(KFinalDocument)

	public:
		KFinalDocument();
		virtual ~KFinalDocument();
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
