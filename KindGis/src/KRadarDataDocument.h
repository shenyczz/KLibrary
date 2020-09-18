#pragma once

#include "KAxinDataDocument.h"

namespace Framework
{

	// KRadarDataDocument �ĵ�

	class KIND_EXT_CLASS KRadarDataDocument : public KAxinDataDocument
	{
		DECLARE_DYNCREATE(KRadarDataDocument)

	public:
		KRadarDataDocument();
		virtual ~KRadarDataDocument();
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
		// ��� Radar ����ͼ��
		virtual BOOL AddRadarLayer(LPCTSTR pszFile, KLayerProperty* pProperty);

	protected:
		// 
		BOOL AddWsr98DataLayer(LPCTSTR pszFile, KLayerProperty* pProperty);
		private:
			// �����һ���״��������
			BOOL AddRdaDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty);
			// �����һ���״��Ʒ����
			BOOL AddRpgDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
