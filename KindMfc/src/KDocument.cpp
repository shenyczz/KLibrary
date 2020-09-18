// KDocument.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KDocument.h"

// KDocument
namespace Framework
{

	IMPLEMENT_DYNCREATE(KDocument, CDocument)

	KDocument::KDocument()
	{
		m_pDib = new KDib();
	}

	KDocument::~KDocument()
	{
		_delete(m_pDib);
	}

	BOOL KDocument::OnNewDocument()
	{
		if (!CDocument::OnNewDocument())
			return FALSE;

		return TRUE;
	}


	BEGIN_MESSAGE_MAP(KDocument, CDocument)
	END_MESSAGE_MAP()


	// KDocument ���

	#ifdef _DEBUG
	void KDocument::AssertValid() const
	{
		CDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KDocument::Dump(CDumpContext& dc) const
	{
		CDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KDocument ���л�

	void KDocument::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			// TODO: �ڴ���Ӵ洢����
		}
		else
		{
			// TODO: �ڴ���Ӽ��ش���
		}
	}
	#endif


	// KDocument ����

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
