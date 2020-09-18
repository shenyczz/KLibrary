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
		// 删除操作器
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)GetRadarDataOperator();
		_delete(pRpgDataOperator);
	}

	// 清空数据
	void KRpgData::Empty()
	{
		//FMT.01 单色文本数据
		m_MonoTextList.Clear();
		//FMT.02 特殊符号数据
		m_SpecilaSymbolList.Clear();
		//FMT.04 风钩数据
		m_WindBarbList.Clear();
		//FMT.06 单色连接矢量数据
		m_MonoLinkedVectorList.Clear();
		//FMT.07 单色非连接矢量数据(一组线段)
		m_MonoUnlinkedVectorList.Clear();
		//FMT.08 彩色文本数据
		m_ColorTextList.Clear();
		//FMT.09 彩色连接矢量数据
		m_ColorLinkedVectorList.Clear();
		//FMT.10 彩色非连接矢量数据
		m_ColorUnlinkedVectorList.Clear();
		//FMT.15 风暴ID数据
		m_StormIdList.Clear();
		//FMT.19 冰雹概率数据
		m_HdaHailList.Clear();
		//FMT.23 风暴中心路径信息过去数据
		m_ScitPastList;
		//FMT.24 风暴中心路径信息预报数据
		m_ScitForecastList.Clear();
		//FMT.25 风暴路径信息圈数据
		m_StiCircleList.Clear();
		//FMT.x0E03 连接等值线矢量数据
		m_LinkedContourVectorList.Clear();
		//FMT.xAF1F 读取径向数据图像数据
		m_RadialList.Clear();
		//FMT.xBA07 读取栅格数据图像数据
		//FMT.xBA0F 读取栅格数据图像数据
		m_RasterRowList.Clear();

		// 删除操作器
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)GetRadarDataOperator();
		_delete(pRpgDataOperator);

		// 调用父类同名函数
		KWsr98Data::Empty();
	}

	// [1] 装载数据
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
	// [2] 生成图像
	BOOL KRpgData::BuildImage(float fZoom,BOOL bDrawCoordinate,BOOL bDrawInfoArea)
	{
		if(IsEmpty())
			return FALSE;

		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		return pRpgDataOperator->BuildImage(fZoom,bDrawCoordinate,bDrawInfoArea);
	}

	// [3] 保存图像
	BOOL KRpgData::SaveImage(LPCTSTR lpszFile)
	{
		return KRadarData::SaveImage(lpszFile);
	}
	// [4] 生成格点数据
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
	// [5] 生成地理坐标数据
	// 只能使用径向数据和栅格数据
	BOOL KRpgData::ToRangeBin()
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		int iProductCode = this->GetProductCode();
		ProductType eProductType = this->GetProductType(iProductCode);

		BOOL bSuccess = FALSE;

		// 距离库链表
		_delete(m_pRangeBinList);
		m_pRangeBinList = new KRangeBinList();

		// 每条径向射线上的距离库链表
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

	// [6] 保存雷达产品属性数据
	// 保存雷达产品属性数据 m_PSBWriteTextList
	BOOL KRpgData::SavePSBTextData(LPCTSTR lpszFile)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		return pRpgDataOperator->WriteTextData(lpszFile,&m_PSBWriteTextList);
	}
	// 保存雷达产品属性数据 m_GABWriteTextList
	BOOL KRpgData::SaveGABTextData(LPCTSTR lpszFile)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		return pRpgDataOperator->WriteTextData(lpszFile,&m_GABWriteTextList);
	}
	// 保存雷达产品属性数据 m_TABWriteTextList
	BOOL KRpgData::SaveTABTextData(LPCTSTR lpszFile)
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();

		if(pRpgDataOperator == NULL)
			return FALSE;

		return pRpgDataOperator->WriteTextData(lpszFile,&m_TABWriteTextList);
	}

	// [7] 判断数据块是否存在
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

	// [8] 拼图
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

	// [9] 根据数值转换为灰度图
	HBITMAP KRpgData::ToGrayImage()
	{
		KRpgDataOperator* pRpgDataOperator = (KRpgDataOperator*)this->GetRadarDataOperator();
		if (!pRpgDataOperator)
			return FALSE;

		return pRpgDataOperator->ToGrayImage();
	}

	//取得图像的中心经纬度
	void KRpgData::GetImageCenter(double* x,double* y)
	{
		*x = 0.001 * m_oPdb.lLon;
		*y = 0.001 * m_oPdb.lLat;
	}

	/// 静态函数
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
