#pragma once

#include "KBlackboardDocument.h"

namespace Framework
{

	// KDiamondDataDocument �ĵ�

	class KIND_EXT_CLASS KDiamondDataDocument : public KBlackboardDocument
	{
		DECLARE_DYNCREATE(KDiamondDataDocument)

	public:
		KDiamondDataDocument();
		virtual ~KDiamondDataDocument();
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
		// 5.��� Diamond ����ͼ��
		virtual BOOL AddDiamondLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
		private:
			//BOOL addStationDataLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			BOOL addGridDataLayer_Contour(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			//BOOL addGridDataLayer_Image(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			//BOOL addGridDataLayer_Wind_Vector(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
			//BOOL addImageCtrlDataLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
