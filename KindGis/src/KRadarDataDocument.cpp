// KRadarDataDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "KRadarDataDocument.h"

#include "KRpgDataRender.h"

namespace Framework
{


	// KRadarDataDocument

	IMPLEMENT_DYNCREATE(KRadarDataDocument, KAxinDataDocument)

	KRadarDataDocument::KRadarDataDocument()
	{
	}

	BOOL KRadarDataDocument::OnNewDocument()
	{
		if (!KAxinDataDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	KRadarDataDocument::~KRadarDataDocument()
	{
	}


	BEGIN_MESSAGE_MAP(KRadarDataDocument, KAxinDataDocument)
	END_MESSAGE_MAP()


	// KRadarDataDocument 诊断

	#ifdef _DEBUG
	void KRadarDataDocument::AssertValid() const
	{
		KAxinDataDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KRadarDataDocument::Dump(CDumpContext& dc) const
	{
		KAxinDataDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KRadarDataDocument 序列化

	void KRadarDataDocument::Serialize(CArchive& ar)
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


	// KRadarDataDocument 命令

	// 添加 Radar 数据图层
	BOOL KRadarDataDocument::AddRadarLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		if(KWsr98Data::IsWsr98Data(pszFile))
		{
			return AddWsr98DataLayer(pszFile,pProperty);
		}

		return false;
	}
	BOOL KRadarDataDocument::AddWsr98DataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		if(KRdaData::IsRdaData(pszFile))
		{
			return AddRdaDataLayer(pszFile,pProperty);
		}
		else if(KRpgData::IsRpgData(pszFile))
		{
			return AddRpgDataLayer(pszFile,pProperty);
		}

		return false;
	}
	// 添加新一代雷达基本数据
	BOOL KRadarDataDocument::AddRdaDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		return false;
	}
	// 添加新一代雷达产品数据
	BOOL KRadarDataDocument::AddRpgDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 1. 数据提供者
		KProvider* pProvider = NULL;
		try
		{
			pProvider = new KRadarDataProvider(pszFile);
		}
		catch(...)
		{
			_delete(pProvider);
			return false;
		}

		// 2.图层属性(伴随数据提供者)
		KLayerProperty* pLayerProperty = new KLayerProperty();
		if(pProperty)
		{
			*pLayerProperty = *(KLayerProperty*)pProperty;
		}
		// 设置图层名称
		if((KString(pLayerProperty->GetName())).IsEmpty())
		{
			pLayerProperty->SetName(pProvider->GetComment());
		}
		// 设置图层数据提供者
		pLayerProperty->SetProvider(pProvider);
		// 设置最大阈值
		pLayerProperty->ThresholdMax() = 15;

		// 3.渲染器(伴随图层属性)
		KRender* pRender = new KRpgDataRender();
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Rpg);

		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
