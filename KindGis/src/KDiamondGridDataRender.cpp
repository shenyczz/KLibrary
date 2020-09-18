#include "StdAfx.h"
#include "KDiamondGridDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KDiamondGridDataRender::KDiamondGridDataRender(void)
	{
	}


	KDiamondGridDataRender::~KDiamondGridDataRender(void)
	{
	}

	// ��Ⱦ
	void KDiamondGridDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		int i;

		if(!hDC || !lpvData)
			return;

		//-------------------------------------------------
		// 1.�������
		m_hDC = hDC;
		m_lpvData = lpvData;

		// 2.׼��ͼ��
		Graphics graphics(hDC);

		// 3.��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// 4.ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// 5.�����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// 6.���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return;
		//-------------------------------------------------
		KDiamondGridData* pDiamGridData = (KDiamondGridData*)pDataObject;

		// �Ƿ���ʾ
		if(!pLayerProperty->IsVisible())
			return;

		// Ϊ��Ⱦ��ע��ص�����
		this->RegiestCallbackFunc(pDataObject->CallbackFunc(),pDataObject->CallbackParam());

		// ��ֵ�߹�����
		KContourBuilder* pContourBuilder = pDiamGridData->GetContourBuilder();
		if(pContourBuilder==NULL)
			return;

		// ��ֵ�߼�(û��ת��Ϊ��ն����ǰ)
		TPtrCollection<KContour*>* pContourPtrCollection = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::BeforeConvert);
		if(pContourPtrCollection==NULL)
			return;

		// �Ƿ����
		if(pLayerProperty->IsClip())
		{
			// ����������·��
			GraphicsPath* pClipPath = (GraphicsPath*)pMapEngine->BuildClipPath();
			if(pClipPath)
			{
				graphics.ResetClip();
				graphics.SetClip(pClipPath);
			}
		}

		// �Ƿ����
		if(pLayerProperty->IsFill())
		{
			// ע�� GetContourCollection(1);
			TPtrCollection<KContour*>* pContourPtrCollectionFill = pContourBuilder->GetContourCollection(KContourBuilder::CurveConvert::AfterConvert);
			int iContourCount = pContourPtrCollectionFill->Count();
			for(i=0;i<iContourCount;i++)
			{
				KContour* pContour = pContourPtrCollectionFill->GetAt(i);
				this->FillContour(&graphics,pContour);
			}
		}

		// �ٻ��Ƶ�ֵ��
		int iContourCount = pContourPtrCollection->Count();
		for(i=0;i<iContourCount;i++)
		{
			KContour* pContour = pContourPtrCollection->GetAt(i);
			this->DrawContour(&graphics,pContour);	// ���Ƶ�ֵ��
			if(pLayerProperty->IsLabel())
			{
				this->LabelContour(&graphics,pContour);	// ���Ƶ�ֵ��
			}
		}

		// �Ƿ���ʾ�������
		if(pLayerProperty->IsShowData())
		{
			this->drawGridValue(&graphics);
		}

		return;
	}

	// ��ʾ�������
	void KDiamondGridDataRender::drawGridValue(Graphics* pGraphics)
	{
		int i,j;

		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KDiamondGridData* pGridData = (KDiamondGridData*)pProvider->GetDataObject();
		KDataInfo* pDataInfo = pGridData->GetDataInfo();

		int nx = pDataInfo->Width();
		int ny = pDataInfo->Height();

		COLORREF clr = m_clrPen;
		//COLORREF clr = pLayerProperty->Color();

		for(i=0;i<ny;i++)
		{
			double yy = pGridData->GetYData(i);

			for(j=0;j<nx;j++)
			{
				double xx = pGridData->GetXData(j);

				double vv = pGridData->GetVData(i * nx + j);

				this->LabelValue(pGraphics,xx,yy,vv,clr);
			}
		}

		return;
	}

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
