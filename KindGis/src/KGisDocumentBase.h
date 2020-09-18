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

	// KGisDocumentBase 文档

	class KIND_EXT_CLASS KGisDocumentBase : public KDocument
	{
		DECLARE_DYNCREATE(KGisDocumentBase)

	public:
		KGisDocumentBase();
		virtual ~KGisDocumentBase();
	#ifndef _WIN32_WCE
		virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
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
		// 取得地图引擎
		KMapEngine* GetMapEngine() { return m_pMapEngine; }

	public:
		// 向 KMapEngine 添加图层
		virtual BOOL AddLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty);
		// 向 KMapEngine 添加图层
		virtual BOOL AddLayer(KLayer* pLayer);
		// 从 KMapEngine 移除图层
		virtual BOOL RemoveLayer(KLayer* pLayer);

	protected:
		// 1.添加 Shape 数据图层
		virtual BOOL AddShapeLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 2.添加 Axin 数据图层
		virtual BOOL AddAxinLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 3.添加 Diamond 数据图层
		virtual BOOL AddDiamondLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 4.添加 Radar 数据图层
		virtual BOOL AddRadarLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }
		// 5.添加 Agmrs 数据图层
		virtual BOOL AddAgmrsLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty) { return false; }

	protected:
		// 地图引擎
		KMapEngine* m_pMapEngine;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
