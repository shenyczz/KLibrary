#pragma once

#include "KGisDocumentBase.h"

namespace Framework
{

	// KAgmrsDataDocument �ĵ�

	class KIND_EXT_CLASS KAgmrsDataDocument : public KGisDocumentBase
	{
		DECLARE_DYNCREATE(KAgmrsDataDocument)

	public:
		KAgmrsDataDocument();
		virtual ~KAgmrsDataDocument();
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

	protected:
		// ��� Agmrs ����ͼ��
		virtual BOOL AddAgmrsLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);

	private:
		int GetLayerType(KProvider* pProvider);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
