// KAgmrsDataDocument.cpp : ʵ���ļ�
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


	// KAgmrsDataDocument ���

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
	// KAgmrsDataDocument ���л�

	void KAgmrsDataDocument::Serialize(CArchive& ar)
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


	// KAgmrsDataDocument ����

	// 2.��� Agmrs ����ͼ��
	BOOL KAgmrsDataDocument::AddAgmrsLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty)
	{
		// ���Ȳ���
		ProgressParam oProgressParam;
		oProgressParam.indexPaneProgress = 1;
		oProgressParam.indexPaneText = 2;
		oProgressParam.SetInfo(_T("����ң������..."));

		// 1. �����ṩ��
		KProvider* pProvider = NULL;
		try
		{
			pProvider = new KAgmrsDataProvider();
			// ע�������ʾ
			pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,&oProgressParam);
			// ������
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

		// 2.ͼ������(���������ṩ��)
		KLayerProperty* pLayerProperty_new = new KLayerProperty();
		if(pLayerProperty)
		{
			*pLayerProperty_new = *(KLayerProperty*)pLayerProperty;
		}
		// ����ͼ������
		if((KString(pLayerProperty_new->GetName())).IsEmpty())
		{
			pLayerProperty_new->SetName(pProvider->GetComment());
		}
		// ����ͼ�������ṩ��
		pLayerProperty_new->SetProvider(pProvider);

		// 3.��Ⱦ��(����ͼ������)
		KRender* pRender = new KAgmrsDataRender();
		pRender->SetLayerProperty(pLayerProperty_new);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(this->GetLayerType(pProvider));

		return AddLayer(pLayer);
	}

	// ���������ṩ��ȡ��ͼ������
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
