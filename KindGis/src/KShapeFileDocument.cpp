// KShapeFileDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "KShapeFileDocument.h"

#include "KShapeDataRender.h"

namespace Framework
{

	// KShapeFileDocument

	IMPLEMENT_DYNCREATE(KShapeFileDocument, KRadarDataDocument)

	KShapeFileDocument::KShapeFileDocument()
	{
	}

	BOOL KShapeFileDocument::OnNewDocument()
	{
		if (!KRadarDataDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	KShapeFileDocument::~KShapeFileDocument()
	{
	}


	BEGIN_MESSAGE_MAP(KShapeFileDocument, KRadarDataDocument)
	END_MESSAGE_MAP()


	// KShapeFileDocument 诊断

	#ifdef _DEBUG
	void KShapeFileDocument::AssertValid() const
	{
		KRadarDataDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KShapeFileDocument::Dump(CDumpContext& dc) const
	{
		KRadarDataDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KShapeFileDocument 序列化

	void KShapeFileDocument::Serialize(CArchive& ar)
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

	//====================================================
	// Layer[Render[LayerProperty[Provider[DataObject]]]]
	//====================================================

	// KShapeFileDocument 命令

	// 添加 Shape 数据图层
	BOOL KShapeFileDocument::AddShapeLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty)
	{
		// 1. 数据提供者(伴随数据对象)
		KProvider* pProvider = NULL;
		try
		{
			pProvider = new KShapeFileProvider(pszFile);
		}
		catch (...)
		{
			_delete(pProvider);
			return false;
		}

		// 2.图层属性(伴随数据提供者)
		KLayerProperty* pLayerProperty_new = new KLayerProperty();
		if(pLayerProperty)
		{
			*pLayerProperty_new = *(KLayerProperty*)pLayerProperty;
		}
		// 设置图层名称
		if((KString(pLayerProperty_new->GetName())).IsEmpty())
		{
			pLayerProperty_new->SetName(pProvider->GetComment());
		}
		// 设置图层数据提供者
		pLayerProperty_new->SetProvider(pProvider);

		// 3.渲染器(伴随图层属性)
		KRender* pRender = new KShapeDataRender();
		pRender->SetLayerProperty(pLayerProperty_new);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Shape);

		// 添加图层对象
		//return KGisDocumentBase::AddLayer(pLayer);
		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
