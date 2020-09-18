// KShapeFileDocument.cpp : ʵ���ļ�
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


	// KShapeFileDocument ���

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
	// KShapeFileDocument ���л�

	void KShapeFileDocument::Serialize(CArchive& ar)
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

	//====================================================
	// Layer[Render[LayerProperty[Provider[DataObject]]]]
	//====================================================

	// KShapeFileDocument ����

	// ��� Shape ����ͼ��
	BOOL KShapeFileDocument::AddShapeLayer(LPCTSTR pszFile, KLayerProperty* pLayerProperty)
	{
		// 1. �����ṩ��(�������ݶ���)
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
		KRender* pRender = new KShapeDataRender();
		pRender->SetLayerProperty(pLayerProperty_new);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Shape);

		// ���ͼ�����
		//return KGisDocumentBase::AddLayer(pLayer);
		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
