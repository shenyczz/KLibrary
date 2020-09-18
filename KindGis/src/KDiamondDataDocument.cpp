// KDiamondDataDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "KDiamondDataDocument.h"

#include "KDiamondGridDataRender.h"

namespace Framework
{

	// KDiamondDataDocument

	IMPLEMENT_DYNCREATE(KDiamondDataDocument, KBlackboardDocument)

	KDiamondDataDocument::KDiamondDataDocument()
	{
	}

	BOOL KDiamondDataDocument::OnNewDocument()
	{
		if (!KBlackboardDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	KDiamondDataDocument::~KDiamondDataDocument()
	{
	}


	BEGIN_MESSAGE_MAP(KDiamondDataDocument, KBlackboardDocument)
	END_MESSAGE_MAP()


	// KDiamondDataDocument 诊断

	#ifdef _DEBUG
	void KDiamondDataDocument::AssertValid() const
	{
		KBlackboardDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KDiamondDataDocument::Dump(CDumpContext& dc) const
	{
		KBlackboardDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KDiamondDataDocument 序列化

	void KDiamondDataDocument::Serialize(CArchive& ar)
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


	// KDiamondDataDocument 命令
	// 5.添加 Diamond 数据图层
	BOOL KDiamondDataDocument::AddDiamondLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		int iFormatCode = KDiamondData::GetFormatCode(pszFile);
		switch(iFormatCode)
		{
		case DIAM_FMT_CODE_TIN:					// 3
			//return addStationDataLayer(pszFile, pProperty);
			break;

		case DIAM_FMT_CODE_GRID:		// 4
			return addGridDataLayer_Contour(pszFile, pProperty);
			break;
		}

		return FALSE;
	}

	BOOL KDiamondDataDocument::addGridDataLayer_Contour(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 进度参数
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("处理格点数据..."));

		// 1. 数据提供者
		KProvider* pProvider = new KDiamondDataProvider();
		// 注册进度回调函数和进度参数
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam, TRUE);

		// 打开链接
		if(!pProvider->Open(pszFile))
		{
			_delete(pProvider);
			return FALSE;
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

		// 3.渲染器(伴随图层属性)
		KRender* pRender = new KDiamondGridDataRender();	// 改变
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Diamond_Grid);			// 改变

		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
