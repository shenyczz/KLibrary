// KFinalDocument.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KFinalDocument.h"

namespace Framework
{

	// KFinalDocument

	IMPLEMENT_DYNCREATE(KFinalDocument, KShapeFileDocument)

	KFinalDocument::KFinalDocument()
	{
	}

	BOOL KFinalDocument::OnNewDocument()
	{
		if (!KShapeFileDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	KFinalDocument::~KFinalDocument()
	{
	}


	BEGIN_MESSAGE_MAP(KFinalDocument, KShapeFileDocument)
	END_MESSAGE_MAP()


	// KFinalDocument ���

	#ifdef _DEBUG
	void KFinalDocument::AssertValid() const
	{
		KShapeFileDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KFinalDocument::Dump(CDumpContext& dc) const
	{
		KShapeFileDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KFinalDocument ���л�

	void KFinalDocument::Serialize(CArchive& ar)
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


	// KFinalDocument ����

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
