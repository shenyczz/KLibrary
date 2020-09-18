#pragma once

#include "KAxinDataDocument.h"

namespace Framework
{

	// KRadarDataDocument 文档

	class KIND_EXT_CLASS KRadarDataDocument : public KAxinDataDocument
	{
		DECLARE_DYNCREATE(KRadarDataDocument)

	public:
		KRadarDataDocument();
		virtual ~KRadarDataDocument();
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

		DECLARE_MESSAGE_MAP()

	protected:
		// 添加 Radar 数据图层
		virtual BOOL AddRadarLayer(LPCTSTR pszFile, KLayerProperty* pProperty);

	protected:
		// 
		BOOL AddWsr98DataLayer(LPCTSTR pszFile, KLayerProperty* pProperty);
		private:
			// 添加新一代雷达基本数据
			BOOL AddRdaDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty);
			// 添加新一代雷达产品数据
			BOOL AddRpgDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty);
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
