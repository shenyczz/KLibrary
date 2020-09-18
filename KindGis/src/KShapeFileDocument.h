#pragma once

#include "KRadarDataDocument.h"

namespace Framework
{

	// KShapeFileDocument �ĵ�

	class KIND_EXT_CLASS KShapeFileDocument : public KRadarDataDocument
	{
		DECLARE_DYNCREATE(KShapeFileDocument)

	public:
		KShapeFileDocument();
		virtual ~KShapeFileDocument();
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
		// ��� Shape ����ͼ��
		virtual BOOL AddShapeLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
