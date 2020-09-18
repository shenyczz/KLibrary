// KRadarDataDocument.cpp : ʵ���ļ�
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


	// KRadarDataDocument ���

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
	// KRadarDataDocument ���л�

	void KRadarDataDocument::Serialize(CArchive& ar)
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


	// KRadarDataDocument ����

	// ��� Radar ����ͼ��
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
	// �����һ���״��������
	BOOL KRadarDataDocument::AddRdaDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		return false;
	}
	// �����һ���״��Ʒ����
	BOOL KRadarDataDocument::AddRpgDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// 1. �����ṩ��
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

		// 2.ͼ������(���������ṩ��)
		KLayerProperty* pLayerProperty = new KLayerProperty();
		if(pProperty)
		{
			*pLayerProperty = *(KLayerProperty*)pProperty;
		}
		// ����ͼ������
		if((KString(pLayerProperty->GetName())).IsEmpty())
		{
			pLayerProperty->SetName(pProvider->GetComment());
		}
		// ����ͼ�������ṩ��
		pLayerProperty->SetProvider(pProvider);
		// ���������ֵ
		pLayerProperty->ThresholdMax() = 15;

		// 3.��Ⱦ��(����ͼ������)
		KRender* pRender = new KRpgDataRender();
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Rpg);

		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
