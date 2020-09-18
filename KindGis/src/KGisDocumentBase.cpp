// KGisDocumentBase.cpp : 实现文件
//

#include "stdafx.h"
#include "KGisDocumentBase.h"

#include "KGisFrameWnd.h"
#include "KLayerDockablePane.h"

namespace Framework
{

	// KGisDocumentBase

	IMPLEMENT_DYNCREATE(KGisDocumentBase, KDocument)

	KGisDocumentBase::KGisDocumentBase() : m_pMapEngine(NULL)
	{
		m_pMapEngine = new KMapEngine();
	}

	KGisDocumentBase::~KGisDocumentBase()
	{
		_delete(m_pMapEngine);
	}

	BOOL KGisDocumentBase::OnNewDocument()
	{
		if (!KDocument::OnNewDocument())
			return FALSE;

		return TRUE;
	}

	BOOL KGisDocumentBase::OnOpenDocument(LPCTSTR lpszPathName)
	{
		if (!KDocument::OnOpenDocument(lpszPathName))
			return FALSE;

		// 添加图层
		BOOL bOpen = this->AddLayer(lpszPathName,NULL);
		if(!bOpen)
		{
			AfxMessageBox(_T("非预期文件格式!"));
		}

		return bOpen;
	}

	BOOL KGisDocumentBase::OnSaveDocument(LPCTSTR lpszPathName)
	{
		//-----------------------------------------------------
		KGisFrameWnd* pFrame = (KGisFrameWnd*)AfxGetMainWnd();
		KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pFrame->GetLayerWnd();
		KLayer* pLayer = pLayerWnd->GetSelectedLayer();
		if(!pLayer)
		{
			AfxMessageBox(_T("没有选择要保存的数据图层!"));
			return FALSE;
		}

		// 数据提供者
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
		{
			AfxMessageBox(_T("选择要保存的数据图层没有数据提供者!"));
			return FALSE;
		}

		// 数据对象
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
		{
			AfxMessageBox(_T("选择要保存的数据图层没有数据对象!"));
			return FALSE;
		}

		// 保存
		if(!pDataObject->SaveData(lpszPathName))
			return FALSE;
		//-------------------------------------------------
		// 否则调用框架提供的文档保存功能
		//return KDocument::OnSaveDocument(lpszPathName);
		//-------------------------------------------------
		return TRUE;
	}

	BEGIN_MESSAGE_MAP(KGisDocumentBase, KDocument)
	END_MESSAGE_MAP()


	// KGisDocumentBase 诊断

	#ifdef _DEBUG
	void KGisDocumentBase::AssertValid() const
	{
		KDocument::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KGisDocumentBase::Dump(CDumpContext& dc) const
	{
		KDocument::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	#ifndef _WIN32_WCE

	// KGisDocumentBase 序列化
	void KGisDocumentBase::Serialize(CArchive& ar)
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

	// KGisDocumentBase 命令

	// 向 KMapEngine 添加图层
	BOOL KGisDocumentBase::AddLayer(KLayer* pLayer)
	{
		// 图层
		if(!pLayer)
			return FALSE;

		// 地图引擎
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return FALSE;

		// 图层集
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return FALSE;

		// 添加图层
		pLayerPtrCollection->Add(pLayer);

		// 如果是第一个图层,调用复位函数初始化地图控件参数
		BOOL bFirstLayer = (pLayerPtrCollection->Count() == 1);
		if( bFirstLayer)
			pMapEngine->Reset();

		// 拥有地图控件的视图(需要在视图初始化时设置 [ OnInitialUpdate() ])
		HWND hWnd = pMapEngine->GetHwnd();
		// 如果窗口存在向窗口发送添加图层消息
		if(NULL != hWnd)
		{
			CRect clientRect;
			::GetClientRect(hWnd,&clientRect);
			pMapEngine->ReDraw(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height());

			// 向拥有者窗口发送图层消息
			::PostMessage(hWnd, WM_USER_LAYER_ADD, 0, 0);
		}

		return TRUE;
	}
	// 向 KMapEngine 移除图层
	BOOL KGisDocumentBase::RemoveLayer(KLayer* pLayer)
	{
		// 图层
		if(!pLayer)
			return FALSE;

		// 地图引擎
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return FALSE;

		// 图层集
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return FALSE;

		// 删除图层
		pLayerPtrCollection->Remove(pLayer);
		// 解决删除图层后不能马上打开(2011.10.10 - shenyc)
		this->OnNewDocument();

		// 拥有地图控件的视图(需要在视图初始化时设置 [ OnInitialUpdate() ])
		HWND hWnd = pMapEngine->GetHwnd();
		// 如果窗口存在向窗口发送移除图层消息
		if(NULL != hWnd)
		{
			CRect clientRect;
			::GetClientRect(hWnd,&clientRect);
			pMapEngine->ReDraw(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height());

			// 向拥有者窗口发送图层消息
			::PostMessage(hWnd,WM_USER_LAYER_REMOVE,0,0);
		}

		return TRUE;
	}

	// 向 KMapEngine 添加图层
	BOOL KGisDocumentBase::AddLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty)
	{
		// 添加 shp 地图数据
		if(KShapeFile::IsShapeFile(pszFile))
		{
			return AddShapeLayer(pszFile,pLayerProperty);
		}

		// 添加 Agmrs 数据
		else if(KAgmrsData::IsAgmrsData(pszFile))
		{
			return AddAgmrsLayer(pszFile,pLayerProperty);
		}

		// 添加 Axin 数据
		else if(KAxinData::IsAxinData(pszFile))
		{
			return AddAxinLayer(pszFile,pLayerProperty);
		}

		// 添加 Radar 数据
		else if(KRadarData::IsRadarData(pszFile))
		{
			return AddRadarLayer(pszFile,pLayerProperty);
		}

		// 5. 添加 Diamond 数据图层
		if(KDiamondData::IsDiamondData(pszFile))
		{
			return AddDiamondLayer(pszFile,pLayerProperty);
		}

		return false;
	}
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
