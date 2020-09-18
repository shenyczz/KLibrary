#pragma once

#include "KDiamondDataDocument.h"

namespace Framework
{

	// KAxinDataDocument �ĵ�

	class KIND_EXT_CLASS KAxinDataDocument : public KDiamondDataDocument
	{
		DECLARE_DYNCREATE(KAxinDataDocument)

	public:
		KAxinDataDocument();
		virtual ~KAxinDataDocument();
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
		// 4.��� Axin ����ͼ��
		virtual BOOL AddAxinLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
		private:
			BOOL addStationDataLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			BOOL addGridDataLayer_Contour(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			BOOL addGridDataLayer_Image(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			BOOL addGridDataLayer_Wind_Vector(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			BOOL addImageCtrlDataLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);

	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
