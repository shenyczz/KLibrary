// KDiamondDataDocument.cpp : ʵ���ļ�
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


	// KDiamondDataDocument ���

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
	// KDiamondDataDocument ���л�

	void KDiamondDataDocument::Serialize(CArchive& ar)
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


	// KDiamondDataDocument ����
	// 5.��� Diamond ����ͼ��
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
		// ���Ȳ���
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("����������..."));

		// 1. �����ṩ��
		KProvider* pProvider = new KDiamondDataProvider();
		// ע����Ȼص������ͽ��Ȳ���
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam, TRUE);

		// ������
		if(!pProvider->Open(pszFile))
		{
			_delete(pProvider);
			return FALSE;
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

		// 3.��Ⱦ��(����ͼ������)
		KRender* pRender = new KDiamondGridDataRender();	// �ı�
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Diamond_Grid);			// �ı�

		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
