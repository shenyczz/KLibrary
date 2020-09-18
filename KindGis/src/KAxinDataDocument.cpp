// KAxinDataDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "KAxinDataDocument.h"

#include "KCallbackFunc.h"
#include "KGridContourDataRender.h"
#include "KGridImageDataRender.h"
#include "KImageCtrlDataRender.h"
#include "KStationDataRender.h"
#include "KWindVectorDataRender.h"

namespace Framework
{
	// KAxinDataDocument

	IMPLEMENT_DYNCREATE(KAxinDataDocument, KDiamondDataDocument)

	KAxinDataDocument::KAxinDataDocument()
	{
	}

	BOOL KAxinDataDocument::OnNewDocument()
	{
		if (!KDiamondDataDocument::OnNewDocument())
			return FALSE;
		return TRUE;
	}

	KAxinDataDocument::~KAxinDataDocument()
	{
	}


	BEGIN_MESSAGE_MAP(KAxinDataDocument, KDiamondDataDocument)
	END_MESSAGE_MAP()


	// KAxinDataDocument 诊断

	#ifdef _DEBUG
	void KAxinDataDocument::AssertValid() const
	{
		KDiamondDataDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KAxinDataDocument::Dump(CDumpContext& dc) const
	{
		KDiamondDataDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE
	// KAxinDataDocument 序列化

	void KAxinDataDocument::Serialize(CArchive& ar)
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


	// KAxinDataDocument 命令

	// 4.添加 Axin 数据图层
	BOOL KAxinDataDocument::AddAxinLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		int iFormatCode = KAxinData::GetFormatCode(pszFile);
		switch(iFormatCode)
		{
		case AXIN_FMT_CODE_TIN:					// 30
			return addStationDataLayer(pszFile, pProperty);
			break;

		case AXIN_FMT_CODE_GRID_CONTOUR:		// 40
			return addGridDataLayer_Contour(pszFile, pProperty);
			break;

		case AXIN_FMT_CODE_GRID_IMAGE:			// 41
			return addGridDataLayer_Image(pszFile, pProperty);
			break;

		case AXIN_FMT_CODE_GRID_WIND_VECTOR:	// 49
			return addGridDataLayer_Wind_Vector(pszFile, pProperty);
			break;

		case AXIN_FMT_CODE_IMAGE_CTRL_INFO:		// 70
			return addImageCtrlDataLayer(pszFile, pProperty);
			break;
		}

		return FALSE;
	}

	BOOL KAxinDataDocument::addStationDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 进度参数
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("处理离散点数据..."));

		// 1. 数据提供者
		KProvider* pProvider = new KAxinDataProvider();
		// 注册进度回调函数和进度参数
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KStationDataRender();	// 改变
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Station);				// 改变

		//------------------------------------------------------
		// 火点产品图层颜色为红色
		KDataObject* pDataObject = pProvider->GetDataObject();
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if (pDataObject->GetDataInfo()->ProductCode() == AGMRS_PRODUCT_CODE_MISC_FIRE)
		{
			pLayer->GetProperty()->Color() = RGB(255, 0, 0);
		}
		//------------------------------------------------------

		return AddLayer(pLayer);
	}
	BOOL KAxinDataDocument::addGridDataLayer_Contour(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 进度参数
		ProgressParam oProgressParam;
		oProgressParam.iFlag = 0;
		oProgressParam.indexPaneProgress = 1;
		oProgressParam.indexPaneText = 2;
		oProgressParam.SetInfo(_T("处理格点数据..."));

		// 1. 数据提供者
		KProvider* pProvider = new KAxinDataProvider();
		// 注册进度回调函数和进度参数
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,&oProgressParam, FALSE);
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
		KRender* pRender = new KGridContourDataRender();	// 改变
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Grid_Contour);			// 改变

		return AddLayer(pLayer);
	}
	BOOL KAxinDataDocument::addGridDataLayer_Image(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 进度参数
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("处理格点图像数据..."));

		// 1. 数据提供者
		KProvider* pProvider = new KAxinDataProvider();
		// 注册进度回调函数和进度参数
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KGridImageDataRender();	// 改变
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Grid_Image);			// 改变

		return AddLayer(pLayer);
	}
	BOOL KAxinDataDocument::addGridDataLayer_Wind_Vector(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 进度参数
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("处理风场矢量数据..."));

		// 1. 数据提供者
		KProvider* pProvider = new KAxinDataProvider();
		// 注册进度回调函数和进度参数
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KWindVectorDataRender();	// 改变
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Wind_Vector);			// 改变

		return AddLayer(pLayer);
	}
	BOOL KAxinDataDocument::addImageCtrlDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 进度参数
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("处理离散点数据..."));

		// 1. 数据提供者
		KProvider* pProvider = new KAxinDataProvider();
		// 注册进度回调函数和进度参数
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KImageCtrlDataRender();	// 改变
		pRender->SetLayerProperty(pLayerProperty);

		// 4.图层(伴随渲染器)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_ImageCtrl);			// 改变

		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
