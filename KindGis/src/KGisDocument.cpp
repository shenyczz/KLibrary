// KGDocument.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KGisDocument.h"

// KGisDocument
namespace Framework
{

	IMPLEMENT_DYNCREATE(KGisDocument, KFinalDocument)

	KGisDocument::KGisDocument()
	{
	}

	KGisDocument::~KGisDocument()
	{
	}

	BOOL KGisDocument::OnNewDocument()
	{
		if (!KFinalDocument::OnNewDocument())
			return FALSE;

		return TRUE;
	}

	BEGIN_MESSAGE_MAP(KGisDocument, KFinalDocument)
	END_MESSAGE_MAP()


	// KGDocument ���

	#ifdef _DEBUG
	void KGisDocument::AssertValid() const
	{
		KFinalDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KGisDocument::Dump(CDumpContext& dc) const
	{
		KFinalDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KGDocument ���л�

	void KGisDocument::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
		}
		else
		{
			// TODO: �ڴ���Ӽ��ش���
		}
	}
	#endif

	 //KGisDocument ����

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
