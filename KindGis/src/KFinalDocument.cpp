// KFinalDocument.cpp : 实现文件
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


	// KFinalDocument 诊断

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
	// KFinalDocument 序列化

	void KFinalDocument::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
		}
		else
		{
			// TODO: 在此添加加载代码
		}
	}
	#endif


	// KFinalDocument 命令

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
