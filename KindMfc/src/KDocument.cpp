// KDocument.cpp : 实现文件
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


	// KDocument 诊断

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
	// KDocument 序列化

	void KDocument::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
			// TODO: 在此添加存储代码
		}
		else
		{
			// TODO: 在此添加加载代码
		}
	}
	#endif


	// KDocument 命令

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
