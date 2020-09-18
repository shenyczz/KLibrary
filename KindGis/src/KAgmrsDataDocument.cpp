// KAgmrsDataDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "KAgmrsDataDocument.h"

#include "KCallbackFunc.h"
#include "KAgmrsDataRender.h"
#include "KAgmrsDataView.h"

#include "KGisFrameWnd.h"
#include "KLayerDockablePane.h"



namespace Framework
{

	// KAgmrsDataDocument

	IMPLEMENT_DYNCREATE(KAgmrsDataDocument, KGisDocumentBase)

	KAgmrsDataDocument::KAgmrsDataDocument()
	{
	}

	BOOL KAgmrsDataDocument::OnNewDocument()
	{
		if (!KGisDocumentBase::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	KAgmrsDataDocument::~KAgmrsDataDocument()
	{
	}


	BEGIN_MESSAGE_MAP(KAgmrsDataDocument, KGisDocumentBase)
	END_MESSAGE_MAP()


	// KAgmrsDataDocument 诊断

	#ifdef _DEBUG
	void KAgmrsDataDocument::AssertValid() const
	{
		KGisDocumentBase::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KAgmrsDataDocument::Dump(CDumpContext& dc) const
	{
		KGisDocumentBase::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KAgmrsDataDocument 序列化

	void KAgmrsDataDocument::Serialize(CArchive& ar)
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


	// KAgmrsDataDocument 命令

	// 2.添加 Agmrs 数据图层
	BOOL KAgmrsDataDocument::AddAgmrsLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty)
	{
		// 进度参数
		ProgressParam oProgressParam;
		oProgressParam.indexPaneProgress = 1;
		oProgressParam.indexPaneText = 2;
		oProgressParam.SetInfo(_T("处理遥感数据..."));

		// 1. 数据提供者
		KProvider* pProvider = NULL;
		try
		{
			pProvider = new KAgmrsDataProvider();
			// 注册进度显示
			pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,&oProgressParam);
			// 打开链接
			if(!pProvider->Open(pszFile))
			{
				throw KException(_T("Open connection string error."));
			}
		}
		catch(...)
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
		KRender* pRender = new KAgmrsDataRender();
		pRender->SetLayerProperty(pLayerProperty_new);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(this->GetLayerType(pProvider));

		return AddLayer(pLayer);
	}

	// 根据数据提供者取得图层类型
	int KAgmrsDataDocument::GetLayerType(KProvider* pProvider)
	{
		int type = 0;

		KDataObject* pDataObject = pProvider->GetDataObject();
		int objtype = pDataObject->GetType();
		switch(objtype)
		{
		case AgmrsDataSource::Avhrr:
			type = LayerType_Avhrr;
			break;

		case AgmrsDataSource::Modis:
			type = LayerType_Modis;
			break;

		case AgmrsDataSource::Mersi:
			type = LayerType_Mersi;
			break;

		case AgmrsDataSource::Virr:
			type = LayerType_Virr;
			break;
		}

		return type;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
