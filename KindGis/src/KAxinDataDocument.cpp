// KAxinDataDocument.cpp : ʵ���ļ�
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


	// KAxinDataDocument ���

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
	// KAxinDataDocument ���л�

	void KAxinDataDocument::Serialize(CArchive& ar)
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


	// KAxinDataDocument ����

	// 4.��� Axin ����ͼ��
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
		// ���Ȳ���
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("������ɢ������..."));

		// 1. �����ṩ��
		KProvider* pProvider = new KAxinDataProvider();
		// ע����Ȼص������ͽ��Ȳ���
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KStationDataRender();	// �ı�
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Station);				// �ı�

		//------------------------------------------------------
		// ����Ʒͼ����ɫΪ��ɫ
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
		// ���Ȳ���
		ProgressParam oProgressParam;
		oProgressParam.iFlag = 0;
		oProgressParam.indexPaneProgress = 1;
		oProgressParam.indexPaneText = 2;
		oProgressParam.SetInfo(_T("����������..."));

		// 1. �����ṩ��
		KProvider* pProvider = new KAxinDataProvider();
		// ע����Ȼص������ͽ��Ȳ���
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,&oProgressParam, FALSE);
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
		KRender* pRender = new KGridContourDataRender();	// �ı�
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Grid_Contour);			// �ı�

		return AddLayer(pLayer);
	}
	BOOL KAxinDataDocument::addGridDataLayer_Image(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// ���Ȳ���
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("������ͼ������..."));

		// 1. �����ṩ��
		KProvider* pProvider = new KAxinDataProvider();
		// ע����Ȼص������ͽ��Ȳ���
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KGridImageDataRender();	// �ı�
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Grid_Image);			// �ı�

		return AddLayer(pLayer);
	}
	BOOL KAxinDataDocument::addGridDataLayer_Wind_Vector(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// ���Ȳ���
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("����糡ʸ������..."));

		// 1. �����ṩ��
		KProvider* pProvider = new KAxinDataProvider();
		// ע����Ȼص������ͽ��Ȳ���
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KWindVectorDataRender();	// �ı�
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_Wind_Vector);			// �ı�

		return AddLayer(pLayer);
	}
	BOOL KAxinDataDocument::addImageCtrlDataLayer(LPCTSTR pszFile, KLayerProperty* pProperty)
	{
		// ���Ȳ���
		ProgressParam* pProgressParam = new ProgressParam();
		pProgressParam->iFlag = 0;
		pProgressParam->indexPaneProgress = 1;
		pProgressParam->indexPaneText = 2;
		pProgressParam->SetInfo(_T("������ɢ������..."));

		// 1. �����ṩ��
		KProvider* pProvider = new KAxinDataProvider();
		// ע����Ȼص������ͽ��Ȳ���
		pProvider->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback, pProgressParam, TRUE);
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
		KRender* pRender = new KImageCtrlDataRender();	// �ı�
		pRender->SetLayerProperty(pLayerProperty);

		// 4.ͼ��(������Ⱦ��)
		KLayer* pLayer = new KLayer();
		pLayer->SetRender(pRender);
		pLayer->SetType(LayerType_ImageCtrl);			// �ı�

		return AddLayer(pLayer);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
