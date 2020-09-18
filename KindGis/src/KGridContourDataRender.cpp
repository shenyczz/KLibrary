#include "StdAfx.h"
#include "KGridContourDataRender.h"

#include "KMapEngine.h"

namespace Renders
{

	KGridContourDataRender::KGridContourDataRender(void)
	{
	}

	KGridContourDataRender::~KGridContourDataRender(void)
	{
	}

	// ��Ⱦ
	void KGridContourDataRender::Rendering(HDC hDC, LPVOID lpvData)
	{
		int i;

		if(NULL == hDC || NULL == lpvData)
			return;

		// �������
		m_hDC = hDC;
		m_lpvData = lpvData;

		// ׼��ͼ��
		Graphics graphics(hDC);

		// ��ͼ����
		KMapEngine *pMapEngine = (KMapEngine*)lpvData;
		if(!pMapEngine)
			return;

		// ͼ������
		KLayerProperty* pLayerProperty = this->GetLayerProperty();
		if(pLayerProperty==NULL)
			return;

		// �����ṩ��
		KProvider* pProvider = pLayerProperty->GetProvider();
		if(!pProvider)
			return;

		// ���ݶ���
		KGridContourData* pGridContourData = (KGridContourData*)pProvider->GetDataObject();
		if(!pGridContourData)
			return;

		// �Ƿ���ʾ
		if(!pLayerProperty->IsVisible())
			return;

		// Ϊ��Ⱦ��ע��ص�����
		this->RegiestCallbackFunc(pGridContourData->CallbackFunc(),pGridContourData->CallbackParam());

		// ��ֵ�߹�����
		KContourBuilder* pContourBuilder = pGridContourData->GetContourBuilder();
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
	void KGridContourDataRender::drawGridValue(Graphics* pGraphics)
	{
		int i,j;

		KMapEngine *pMapEngine = (KMapEngine*)m_lpvData;

		KLayerProperty* pLayerProperty = m_pLayerProperty;
		KProvider* pProvider = pLayerProperty->GetProvider();
		KGridData* pGridData = (KGridData*)pProvider->GetDataObject();
		KDataInfo* pDataInfo = pGridData->GetDataInfo();

		int nx = pDataInfo->Width();
		int ny = pDataInfo->Height();

		COLORREF clr = m_clrPen;

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
