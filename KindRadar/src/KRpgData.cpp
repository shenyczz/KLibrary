#include "StdAfx.h"
#include "KRpgData.h"

#include "KRpgDataOperator.h"

namespace Wsr98d { namespace Rpg
{

	KRpgData::KRpgData(void)
	{
	}

	KRpgData::~KRpgData(void)
	{
		// ɾ��������
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)GetRadarDataOperator();
		_delete(pRpgDataOperator);
	}

	// �������
	void KRpgData::Empty()
	{
		//FMT.01 ��ɫ�ı�����
		m_MonoTextList.Clear();
		//FMT.02 �����������
		m_SpecilaSymbolList.Clear();
		//FMT.04 �繳����
		m_WindBarbList.Clear();
		//FMT.06 ��ɫ����ʸ������
		m_MonoLinkedVectorList.Clear();
		//FMT.07 ��ɫ������ʸ������(һ���߶�)
		m_MonoUnlinkedVectorList.Clear();
		//FMT.08 ��ɫ�ı�����
		m_ColorTextList.Clear();
		//FMT.09 ��ɫ����ʸ������
		m_ColorLinkedVectorList.Clear();
		//FMT.10 ��ɫ������ʸ������
		m_ColorUnlinkedVectorList.Clear();
		//FMT.15 �籩ID����
		m_StormIdList.Clear();
		//FMT.19 ������������
		m_HdaHailList.Clear();
		//FMT.23 �籩����·����Ϣ��ȥ����
		m_ScitPastList;
		//FMT.24 �籩����·����ϢԤ������
		m_ScitForecastList.Clear();
		//FMT.25 �籩·����ϢȦ����
		m_StiCircleList.Clear();
		//FMT.x0E03 ���ӵ�ֵ��ʸ������
		m_LinkedContourVectorList.Clear();
		//FMT.xAF1F ��ȡ��������ͼ������
		m_RadialList.Clear();
		//FMT.xBA07 ��ȡդ������ͼ������
		//FMT.xBA0F ��ȡդ������ͼ������
		m_RasterRowList.Clear();

		// ɾ��������
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)GetRadarDataOperator();
		_delete(pRpgDataOperator);

		// ���ø���ͬ������
		KWsr98Data::Empty();
	}

	// [1] װ������
	BOOL KRpgData::LoadData(LPCTSTR lpszFile,LPVOID lpvData)
	{
		Empty();

		KRpgDataOperator* pRpgDataOperator = new KRpgDataOperator();
		this->SetRadarDataOperator(pRpgDataOperator);
		pRpgDataOperator->SetOwner(this); 

		if(!pRpgDataOperator->LoadData(lpszFile,lpvData))
			return FALSE;

		m_bEmpty = FALSE;

		return KWsr98Data::LoadData(lpszFile,lpvData);
	}
	// [2] ����ͼ��
	BOOL KRpgData::BuildImage(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		if(IsEmpty())
			return FALSE;

		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		return pRpgDataOperator->BuildImage(fZoom,bDrawCoordinate,bDrawInfoArea);
	}

	// [3] ����ͼ��
	BOOL KRpgData::SaveImage(LPCTSTR lpszFile)
	{
		return KRadarData::SaveImage(lpszFile);
	}
	// [4] ���ɸ������
	BOOL KRpgData::ToGrid(LPCTSTR lpszFile,int iThreshold_min,int iThreshold_max,int skip)
	{
		if(IsEmpty())
			return FALSE;

		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		return pRpgDataOperator->ToGrid(lpszFile, iThreshold_min, iThreshold_max, skip);
	}
	BOOL KRpgData::ToGrid(double*& pData, KDataInfo& dataInfo, int iThreshold_min, int iThreshold_max, int skip)
	{
		if(IsEmpty())
			return FALSE;

		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		return pRpgDataOperator->ToGrid(pData,dataInfo,iThreshold_min, iThreshold_max, skip);
	}
	// [5] ���ɵ�����������
	// ֻ��ʹ�þ������ݺ�դ������
	BOOL KRpgData::ToRangeBin()
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		int iProductCode = this->GetProductCode();
		ProductType eProductType = this->GetProductType(iProductCode);

		BOOL bSuccess = FALSE;

		// ���������
		_delete(m_pRangeBinList);
		m_pRangeBinList = new KRangeBinList();

		// ÿ�����������ϵľ��������
		_delete(m_pRangeBinPerRadialList);
		m_pRangeBinPerRadialList = new KRangeBinPerRadialList();

		switch(eProductType)
		{
		case ProductType_RadialImage:
			bSuccess = pRpgDataOperator->Radial2RangeBin(m_pRangeBinList, m_pRangeBinPerRadialList);
			break;

		case ProductType_RasterImage:
			bSuccess = pRpgDataOperator->Raster2RangeBin(m_pRangeBinList);
			break;

		default:
			bSuccess = FALSE;
			break;
		}

		return bSuccess;
	}

	// [6] �����״��Ʒ��������
	// �����״��Ʒ�������� m_PSBWriteTextList
	BOOL KRpgData::SavePSBTextData(LPCTSTR lpszFile)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		return pRpgDataOperator->WriteTextData(lpszFile,&m_PSBWriteTextList);
	}
	// �����״��Ʒ�������� m_GABWriteTextList
	BOOL KRpgData::SaveGABTextData(LPCTSTR lpszFile)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		return pRpgDataOperator->WriteTextData(lpszFile,&m_GABWriteTextList);
	}
	// �����״��Ʒ�������� m_TABWriteTextList
	BOOL KRpgData::SaveTABTextData(LPCTSTR lpszFile)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		return pRpgDataOperator->WriteTextData(lpszFile,&m_TABWriteTextList);
	}

	// [7] �ж����ݿ��Ƿ����
	BOOL KRpgData::ExistPSB()
	{
		PDB* pdb = this->PDBPtr();
		return (!this->IsEmpty() && pdb->lOffsetSymbology > 0);
	}
	BOOL KRpgData::ExistGAB()
	{
		PDB* pdb = this->PDBPtr();
		return (!this->IsEmpty() && pdb->lOffsetGraphic > 0);
	}
	BOOL KRpgData::ExistTAB()
	{
		PDB* pdb = this->PDBPtr();
		return (!this->IsEmpty() && pdb->lOffsetTabular > 0);
	}

	// [8] ƴͼ
	BOOL KRpgData::Union(double*& pData, KDataInfo* pDataInfo,TCollection<KString>* pFileArray)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();
		if (!pRpgDataOperator)
			return FALSE;

		return pRpgDataOperator->Union(pData, pDataInfo, pFileArray);
	}
	BOOL KRpgData::Union(double*& pData, KDataInfo* pDataInfo,LPCTSTR pszFile[],int nFile)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();
		if (!pRpgDataOperator)
			return FALSE;

		return pRpgDataOperator->Union(pData, pDataInfo, pszFile, nFile);
	}
	BOOL KRpgData::Union(double*& pData, KDataInfo* pDataInfo, double*& pData1, KDataInfo* pDataInfo1, double*& pData2, KDataInfo* pDataInfo2)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();
		if (!pRpgDataOperator)
			return FALSE;

		return pRpgDataOperator->Union(pData, pDataInfo, pData1, pDataInfo1, pData2, pDataInfo2);
	}

	// [9] ������ֵת��Ϊ�Ҷ�ͼ
	HBITMAP KRpgData::ToGrayImage()
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();
		if (!pRpgDataOperator)
			return FALSE;

		return pRpgDataOperator->ToGrayImage();
	}

	//ȡ��ͼ������ľ�γ��
	void KRpgData::GetImageCenter(double* x,double* y)
	{
		*x = 0.001 * m_oPdb.lLon;
		*y = 0.001 * m_oPdb.lLat;
	}

	/// ��̬����
	bool KRpgData::IsRpgData(LPCTSTR lpszFile)
	{
		MHB mhb;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return false;

		fseek(fp,0,SEEK_SET);
		fread(&mhb,sizeof(MHB),1,fp);

		fclose(fp);
		fp = NULL;

		KBytesOrder::ReverseBytes(&mhb.iMsgCode);

		return (mhb.iMsgCode >= RPG_DATA_PRODUCT_CODE_MIN && mhb.iMsgCode <= RPG_DATA_PRODUCT_CODE_MAX);
	}

//---------------------------------------------------------
}}// namespace Wsr98d::Rpg - end
//---------------------------------------------------------
