// KGisDocumentBase.cpp : ʵ���ļ�
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

		// ���ͼ��
		BOOL bOpen = this->AddLayer(lpszPathName,NULL);
		if(!bOpen)
		{
			AfxMessageBox(_T("��Ԥ���ļ���ʽ!"));
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
			AfxMessageBox(_T("û��ѡ��Ҫ���������ͼ��!"));
			return FALSE;
		}

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
		{
			AfxMessageBox(_T("ѡ��Ҫ���������ͼ��û�������ṩ��!"));
			return FALSE;
		}

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
		{
			AfxMessageBox(_T("ѡ��Ҫ���������ͼ��û�����ݶ���!"));
			return FALSE;
		}

		// ����
		if(!pDataObject->SaveData(lpszPathName))
			return FALSE;
		//-------------------------------------------------
		// ������ÿ���ṩ���ĵ����湦��
		//return KDocument::OnSaveDocument(lpszPathName);
		//-------------------------------------------------
		return TRUE;
	}

	BEGIN_MESSAGE_MAP(KGisDocumentBase, KDocument)
	END_MESSAGE_MAP()


	// KGisDocumentBase ���

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

	// KGisDocumentBase ���л�
	void KGisDocumentBase::Serialize(CArchive& ar)
	{
		if (ar.IsStoring())
		{
		}
		else
		{
			// TODO: �ڴ���Ӽ��ش���
		}
	}

	#endif

	// KGisDocumentBase ����

	// �� KMapEngine ���ͼ��
	BOOL KGisDocumentBase::AddLayer(KLayer* pLayer)
	{
		// ͼ��
		if(!pLayer)
			return FALSE;

		// ��ͼ����
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return FALSE;

		// ͼ�㼯
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return FALSE;

		// ���ͼ��
		pLayerPtrCollection->Add(pLayer);

		// ����ǵ�һ��ͼ��,���ø�λ������ʼ����ͼ�ؼ�����
		BOOL bFirstLayer = (pLayerPtrCollection->Count() == 1);
		if( bFirstLayer)
			pMapEngine->Reset();

		// ӵ�е�ͼ�ؼ�����ͼ(��Ҫ����ͼ��ʼ��ʱ���� [ OnInitialUpdate() ])
		HWND hWnd = pMapEngine->GetHwnd();
		// ������ڴ����򴰿ڷ������ͼ����Ϣ
		if(NULL != hWnd)
		{
			CRect clientRect;
			::GetClientRect(hWnd,&clientRect);
			pMapEngine->ReDraw(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height());

			// ��ӵ���ߴ��ڷ���ͼ����Ϣ
			::PostMessage(hWnd, WM_USER_LAYER_ADD, 0, 0);
		}

		return TRUE;
	}
	// �� KMapEngine �Ƴ�ͼ��
	BOOL KGisDocumentBase::RemoveLayer(KLayer* pLayer)
	{
		// ͼ��
		if(!pLayer)
			return FALSE;

		// ��ͼ����
		KMapEngine* pMapEngine = this->GetMapEngine();
		if(!pMapEngine)
			return FALSE;

		// ͼ�㼯
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return FALSE;

		// ɾ��ͼ��
		pLayerPtrCollection->Remove(pLayer);
		// ���ɾ��ͼ��������ϴ�(2011.10.10 - shenyc)
		this->OnNewDocument();

		// ӵ�е�ͼ�ؼ�����ͼ(��Ҫ����ͼ��ʼ��ʱ���� [ OnInitialUpdate() ])
		HWND hWnd = pMapEngine->GetHwnd();
		// ������ڴ����򴰿ڷ����Ƴ�ͼ����Ϣ
		if(NULL != hWnd)
		{
			CRect clientRect;
			::GetClientRect(hWnd,&clientRect);
			pMapEngine->ReDraw(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height());

			// ��ӵ���ߴ��ڷ���ͼ����Ϣ
			::PostMessage(hWnd,WM_USER_LAYER_REMOVE,0,0);
		}

		return TRUE;
	}

	// �� KMapEngine ���ͼ��
	BOOL KGisDocumentBase::AddLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty)
	{
		// ��� shp ��ͼ����
		if(KShapeFile::IsShapeFile(pszFile))
		{
			return AddShapeLayer(pszFile,pLayerProperty);
		}

		// ��� Agmrs ����
		else if(KAgmrsData::IsAgmrsData(pszFile))
		{
			return AddAgmrsLayer(pszFile,pLayerProperty);
		}

		// ��� Axin ����
		else if(KAxinData::IsAxinData(pszFile))
		{
			return AddAxinLayer(pszFile,pLayerProperty);
		}

		// ��� Radar ����
		else if(KRadarData::IsRadarData(pszFile))
		{
			return AddRadarLayer(pszFile,pLayerProperty);
		}

		// 5. ��� Diamond ����ͼ��
		if(KDiamondData::IsDiamondData(pszFile))
		{
			return AddDiamondLayer(pszFile,pLayerProperty);
		}

		return false;
	}
//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
