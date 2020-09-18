#pragma once

#include "KMapEngine.h"
#include "KCallbackFunc.h"

namespace Framework
{
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

namespace Framework
{

	// KGisDocumentBase �ĵ�

	class KIND_EXT_CLASS KGisDocumentBase : public KDocument
	{
		DECLARE_DYNCREATE(KGisDocumentBase)

	public:
		KGisDocumentBase();
		virtual ~KGisDocumentBase();
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
		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
		virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);

		DECLARE_MESSAGE_MAP()

	public:
		// ȡ�õ�ͼ����
		KMapEngine* GetMapEngine() { return m_pMapEngine; }

	public:
		// �� KMapEngine ���ͼ��
		virtual BOOL AddLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
		// �� KMapEngine ���ͼ��
		virtual BOOL AddLayer(KLayer* pLayer);
		// �� KMapEngine �Ƴ�ͼ��
		virtual BOOL RemoveLayer(KLayer* pLayer);

	protected:
		// 1.��� Shape ����ͼ��
		virtual BOOL AddShapeLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 2.��� Axin ����ͼ��
		virtual BOOL AddAxinLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 3.��� Diamond ����ͼ��
		virtual BOOL AddDiamondLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 4.��� Radar ����ͼ��
		virtual BOOL AddRadarLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 5.��� Agmrs ����ͼ��
		virtual BOOL AddAgmrsLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }

	protected:
		// ��ͼ����
		KMapEngine* m_pMapEngine;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
