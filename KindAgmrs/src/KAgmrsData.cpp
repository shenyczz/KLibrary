#include "StdAfx.h"
#include "KAgmrsData.h"

namespace Agmrs
{
	//-----------------------------------------------------
	// static members
	KAvhrrThreshold	KAgmrsData::s_AvhrrThreshold;
	KModisThreshold	KAgmrsData::s_ModisThreshold;
	KMersiThreshold	KAgmrsData::s_MersiThreshold;
	KVirrThreshold	KAgmrsData::s_VirrThreshold;

	KStdPalette		KAgmrsData::s_StdPalette;
	KIdxPalette		KAgmrsData::s_IdxPalette;
	KRbwPalette		KAgmrsData::s_RbwPalette;
	KSegPalette		KAgmrsData::s_SegPalette;
	KLinearPalette	KAgmrsData::s_LinearPalette;
	//-----------------------------------------------------

	KAgmrsData::KAgmrsData(void)
		:m_ppwData(NULL)
		,m_ppucDataGray(NULL)
		,m_pFileHeader(NULL)
		,m_bEmpty(TRUE)
		,m_iRChannel(0)
		,m_iGChannel(0)
		,m_iBChannel(0)
		,m_iRChannelDef(0)
		,m_iGChannelDef(0)
		,m_iBChannelDef(0)
	{
		this->SetType(AgmrsDataSource::Unknown);
		this->SetFileName(_T(""));

		_tcscpy(m_szDayFile,_T(""));
		_tcscpy(m_szNightFile,_T(""));

		_tcscpy(m_szTemp,_T(""));
	}

	KAgmrsData::~KAgmrsData(void)
	{
		Empty();
	}

	// 内存清理
	void KAgmrsData::Empty()
	{
		this->SetType(AgmrsDataSource::Unknown);
		this->SetFileName(_T(""));

		// 内存回收
		KWord2DMemAllocator::FreeMemory2D(m_ppwData);
		KByte2DMemAllocator::FreeMemory2D(m_ppucDataGray);

		_delete(m_pFileHeader);

		m_iRChannelDef = 0;			//红色通道默认值
		m_iGChannelDef = 0;			//绿色通道默认值
		m_iBChannelDef = 0;			//蓝色通道默认值

		m_iRChannel = 0;			//红色通道
		m_iGChannel = 0;			//绿色通道
		m_iBChannel = 0;			//蓝色通道

		m_bEmpty = TRUE;
	}

	// 保存数据
	BOOL KAgmrsData::SaveData(LPCTSTR lpszFile)
	{
		//-------------------------------------------------
		LD2Header* pLD2Header = (LD2Header*)FileHeader();
		if(pLD2Header->wVersion == 2002)
			return SaveData_LD2(lpszFile);
		//-------------------------------------------------
		LD3Header* pLD3Header = (LD3Header*)FileHeader();
		if(pLD3Header->wVersion == 2006)
			return SaveData_LD3(lpszFile);
		//-------------------------------------------------
		return FALSE;
	}

	BOOL KAgmrsData::SaveData_LD2(LPCTSTR lpszFile)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		FILE* fp =_tfopen(lpszFile,_T("wb"));
		if(!fp) return FALSE;

		LD2Header* pFileHeader = (LD2Header*)FileHeader();
		fwrite(pFileHeader,sizeof(LD2Header),1,fp);

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		int iDataSize = iWidth * iHeight * iChannelNums;

		// 数据指针
		PWORD* ppwData = this->GetData();
		fwrite(*ppwData,sizeof(WORD),iDataSize,fp);

		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	BOOL KAgmrsData::SaveData_LD3(LPCTSTR lpszFile)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		FILE* fp =_tfopen(lpszFile,_T("wb"));
		if(!fp) return FALSE;

		LD3Header* pFileHeader = (LD3Header*)FileHeader();
		fwrite(pFileHeader,sizeof(LD3Header),1,fp);

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		int iDataSize = iWidth * iHeight * iChannelNums;

		// 数据指针
		PWORD* ppwData = this->GetData();
		fwrite(*ppwData,sizeof(WORD),iDataSize,fp);

		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	// 装载数据
	BOOL KAgmrsData::LoadData_bin(LPCTSTR lpszFile)
	{
		this->SetFileName(lpszFile);		// 保存文件名
		this->MapChannel();					// 映射通道(通道号和数据索引对应) - pure virtual function
		this->SetDefaultDisplayChannel();	// 设置默认的显示通道 - pure virtual function
		m_bEmpty = FALSE;					// 非空标记置位
		return TRUE;
	}

	// 设置/获取数据通道
	void KAgmrsData::SetDataChannel(int iRChannel,int iGChannel,int iBChannel)
	{
		m_iRChannel = iRChannel;
		m_iGChannel = iGChannel;
		m_iBChannel = iBChannel;
	}
	void KAgmrsData::GetDataChannel(int* iRChannel,int* iGChannel,int* iBChannel)
	{
		*iRChannel = m_iRChannel;
		*iGChannel = m_iGChannel;
		*iBChannel = m_iBChannel;
	}

	// 设置/获取默认数据通道
	void KAgmrsData::SetDataChannelDef(int iRChannel,int iGChannel,int iBChannel)
	{
		m_iRChannelDef = iRChannel;
		m_iGChannelDef = iGChannel;
		m_iBChannelDef = iBChannel;
	}
	void KAgmrsData::GetDataChannelDef(int* iRChannel,int* iGChannel,int* iBChannel)
	{
		*iRChannel = m_iRChannelDef;
		*iGChannel = m_iGChannelDef;
		*iBChannel = m_iBChannelDef;
	}

	// 取得遥感数据源
	int KAgmrsData::GetAgmrsDataSource()
	{
		int iDataType = AgmrsDataSource::Unknown;

		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)this->FileHeader();
		if(!pFileHeader)
			return AgmrsDataSource::Unknown;

		if(AGMRS_LOCAL_DATA_MARKER == pFileHeader->wFileID)
		{
			WORD wSatelliteID = pFileHeader->wSatelliteID;
			iDataType = KAgmrsData::GetAgmrsDataSource(wSatelliteID);
		}

		return iDataType;
	}

	// 检测农气遥感数据
	BOOL KAgmrsData::IsAgmrsData()
	{
		BOOL bValue = FALSE;
		//-------------------------------------------------
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)this->FileHeader();
		bValue = AGMRS_LOCAL_DATA_MARKER == pFileHeader->wFileID;
		//-------------------------------------------------
		return bValue;
	}

	// 经纬度转换为格点坐标
	// flag = 0 遥感数据位置
	// flag = 1 产品数据位置
	BOOL KAgmrsData::LonLat2xy(double dLon,double dLat,int& x,int& y, int flag)
	{
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)m_pFileHeader;

		int iWidth = pFileHeader->wWidth;
		int iHeight = pFileHeader->wHeight;

		float fLonMin = pFileHeader->fLonMin;
		float fLonMax = pFileHeader->fLonMax;

		float fLatMin = pFileHeader->fLatMin;
		float fLatMax = pFileHeader->fLatMax;

		x = (LONG)((dLon - fLonMin) / pFileHeader->fLonResolution + 0.1);

		if(flag == 0)
		{
			y = iHeight - 1 - (LONG)((dLat - fLatMin) / pFileHeader->fLatResolution + 0.1);
		}
		else
		{
			y = (LONG)((dLat - fLatMin) / pFileHeader->fLatResolution + 0.1);
		}

		return (x>=0 && x<=iWidth-1 && y>=0 && y<=iHeight-1);
	}
	// 格点坐标转换为经纬度坐标
	// flag = 0 遥感数据位置
	// flag = 1 产品数据位置
	BOOL KAgmrsData::xy2LonLat(int x,int y,double& lon,double& lat, int flag)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)this->FileHeader();

		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		float fLonMin = pFileHeader->fLonMin;
		float fLonMax = pFileHeader->fLonMax;

		float fLatMin = pFileHeader->fLatMin;
		float fLatMax = pFileHeader->fLatMax;

		lon = fLonMin + pFileHeader->fLonResolution * x;

		if(flag == 0)
		{
			lat = fLatMin + pFileHeader->fLatResolution * (iHeight - 1 - y);
		}
		else
		{
			lat = fLatMin + pFileHeader->fLatResolution * y;
		}

		return ((lon>fLonMin || fabs(lon-fLonMin)<EPS) && (lat>fLatMin || fabs(lat-fLatMin)<EPS));
	}

	// 取得产品格点值
	float KAgmrsData::GetProductValue(double lon, double lat)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		float fValue = AXIN_INVALID_DATA;

		int x,y;
		this->LonLat2xy(lon,lat,x,y,1);

		int w = pDataInfo->Width();

		float* pProductData = *(this->GetProductData());
		if(pProductData)
		{
			fValue = *(pProductData + y*w+x);
		}

		return fValue;
	}

	// 取得指定点的全部通道数据
	ChannelInfoVector* KAgmrsData::GetChannelInfo(double lon, double lat)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		if(!pDataInfo)
			return NULL;

		PWORD* ppData = this->GetData();
		if(!ppData)
			return NULL;

		int x(0),y(0);
		if(!this->LonLat2xy(lon,lat,x,y))
			return NULL;

		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();
		if(x<0 || x>=iWidth || y<0 || y>=iHeight)
			return NULL;

		int pos = y*iWidth+x;
		int iChannelNums = pDataInfo->ChannelNumbers();

		ChannelInfoVector* pChannelInfoVector = &m_ChannelInfoVector;
		pChannelInfoVector->clear();

		try
		{
			for(int ch=0;ch<iChannelNums;ch++)
			{
				ChannelInfo channelInfo;

				int iChannelCode = pDataInfo->ChannelCode(ch);
				channelInfo.Channel = this->ChannelCode2String(iChannelCode);
				channelInfo.Value = 0.1 * ppData[ch][pos];

				pChannelInfoVector->push_back(channelInfo);
			}
		}
		catch(...)
		{
			// no body
		}

		return pChannelInfoVector;
	}

	// 产品生成 - 全照率
	BOOL KAgmrsData::Abe(int iProductID)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);

		return pAgmrsDataProcessor->Abe(iProductID);
	}

	BOOL KAgmrsData::Lst(int iMethod)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->Lst(iMethod);
	}
	// 
	BOOL KAgmrsData::Vix(int iProductID)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->Vix(iProductID);
	}
	// 
	BOOL KAgmrsData::Dix(int iProductID)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->Dix(iProductID);
	}
	// 单项监测 - SIM (Single Item Monitor)
	BOOL KAgmrsData::Sim(int iProductID)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->Sim(iProductID);
	}
	// 取得产品数据
	PFLOAT* KAgmrsData::GetProductData()
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return NULL;

		return pAgmrsDataProcessor->GetProductData();
	}
	// 数据转换为 DIB
	BOOL KAgmrsData::DataToDIB(int iRChannel,int iGChannel,int iBChannel)
	{ 
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->DataToDIB(iRChannel,iGChannel,iBChannel);
	}
	// 合并数据通道
	BOOL KAgmrsData::ChannelCombine(ChannelCombineInfo* pChannelCombineInfo1,
		ChannelCombineInfo* pChannelCombineInfo2,
		LPCTSTR lpszOutputFile,
		KExtents* pExtents)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->ChannelCombine(pChannelCombineInfo1,pChannelCombineInfo2,lpszOutputFile,pExtents);
	}

	// 取得像元类型
	int KAgmrsData::GetPixelType(int i,int j)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return PixelType::Unknown;

		return pAgmrsDataProcessor->GetPixelType(i,j);
	}
	// 是否晴空(无云)
	BOOL KAgmrsData::IsClearNight(int i,int j)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		return pAgmrsDataProcessor->IsClearNight(i,j);
	}

	// 计算TVDI
	BOOL KAgmrsData::Dix_Tvdi(KDataInfo* pDataInfo,float* pDataNdvi,float* pDataLst,float*& pDataTvdi)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->Dix_Tvdi(pDataInfo,pDataNdvi,pDataLst,pDataTvdi);
	}

	// 命名产品
	LPCTSTR KAgmrsData::NameProduct(int iProductID)
	{
		// 产品
		TCHAR szTemp1[16] = _T("");
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_MISC_LST:
			_stprintf(szTemp1, _T("lst"));
			break;

		case AGMRS_PRODUCT_CODE_VIX_NDVI:
			_stprintf(szTemp1, _T("ndvi"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_DVI:
			_stprintf(szTemp1, _T("dvi"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_RVI:
			_stprintf(szTemp1, _T("rvi"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_SAVI:
			_stprintf(szTemp1, _T("savi"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_EVI:
			_stprintf(szTemp1, _T("evi"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_LAI:
			_stprintf(szTemp1, _T("lai"));
			break;

		case AGMRS_PRODUCT_CODE_DIX_ATI:
			_stprintf(szTemp1, _T("ati"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_TVDI:
			_stprintf(szTemp1, _T("tvdi"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_VSWI:
			_stprintf(szTemp1, _T("vswi"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_SWCI:
			_stprintf(szTemp1, _T("swci"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_PDI:
			_stprintf(szTemp1, _T("pdi"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_MPDI:
			_stprintf(szTemp1, _T("mpdi"));
			break;
		}

		// 数据信息
		KDataInfo* pDataInfo = this->GetDataInfo();
		KFile file(this->GetFileName());

		// 文件名(不带扩展名)
		TCHAR szTemp2[MAX_PATH] = _T("");
		_stprintf(szTemp2,_T("%s"),(LPCTSTR)file.GetFileName(false));


		// 组合生成产品名称
		_stprintf(m_szTemp,_T("%s_%s"),szTemp1 ,szTemp2);
		// 文件名：yyyymmdd.HHMMSS.lll.ttt.prdid.prdname
		_stprintf(m_szTemp,_T("%4d%02d%02d.%02d%02d%02d.000.000.%d")
			,pDataInfo->Year()
			,pDataInfo->Month()
			,pDataInfo->Day()
			,pDataInfo->Hour()
			,pDataInfo->Minute()
			,pDataInfo->Second()
			,iProductID
			);

		return m_szTemp;
	}


	///====================================================
	// static
	// 取得像元点面积(km^2)
	// resolution - 分辨率
	double KAgmrsData::GetPixelArea(double lat, double resolution)
	{
		double dArea = 0;

		double bb	= PI * resolution / 180.0 / 2.0;
		double bs	= 6356.863;
		double bm	= PI * lat / 180;
		double p	= 57.29577951;

		double A	= 1.0033636057;
		double B	= 0.0011240273;
		double C	= 0.000016989;
		double D	= 0.00000027;

		dArea = 2.0*bs*bs*
			(
				resolution / p *
				(
					A * sin(bb)*cos(bm) -
					B * sin(3.0*bb)*cos(3.0*bm) +
					C * sin(5.0*bb)*cos(5.0*bm) -
					D * sin(7.0*bb/2.0)*cos(7.0*bm)
				)
			);

		return dArea;
	}

	//static
	BOOL KAgmrsData::IsLd2File(LPCTSTR lpszFile)
	{
		LD2Header fileHeader;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;
		fread(&fileHeader,sizeof(LD2Header),1,fp);
		fclose(fp);

		return fileHeader.wVersion == 2002;
	}
	//static
	BOOL KAgmrsData::IsLd3File(LPCTSTR lpszFile)
	{
		LD3Header fileHeader;

		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(!fp) return FALSE;
		fread(&fileHeader,sizeof(LD3Header),1,fp);
		fclose(fp);

		return fileHeader.wVersion == 2006;
	}
	//static
	BOOL KAgmrsData::IsAgmrsData(LPCTSTR lpszFile)
	{
		BOOL bValue = FALSE;
		//-------------------------------------------------
		int iAgmrsDataSource = GetAgmrsDataSource(lpszFile);
		switch(iAgmrsDataSource)
		{
		case AgmrsDataSource::Avhrr:
		case AgmrsDataSource::Modis:
		case AgmrsDataSource::Mersi:
		case AgmrsDataSource::Virr:
			bValue = TRUE;
			break;

		default:
			bValue = FALSE;
			break;
		}
		//-------------------------------------------------
		return bValue;
	}
	//static
	int KAgmrsData::GetAgmrsDataSource(LPCTSTR lpszFile)
	{
		int iDataSource = AgmrsDataSource::Unknown;

		char FileHead[128] = "";

		// 读取128字节文件头
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(NULL==fp)
		{
			return AgmrsDataSource::Unknown;
		}
		fread(&FileHead[0],1,sizeof(char)*128,fp);
		fclose(fp);


		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)&FileHead[0];

		if(AGMRS_LOCAL_DATA_MARKER == pFileHeader->wFileID)
		{
			WORD wSatelliteID = pFileHeader->wSatelliteID;
			iDataSource = KAgmrsData::GetAgmrsDataSource(wSatelliteID);
		}

		return iDataSource;
	}
	//static
	int KAgmrsData::GetAgmrsDataSource(WORD wSatelliteID)
	{
		int iDataSource = AgmrsDataSource::Unknown;

		switch(wSatelliteID)
		{
		case AGMRS_SATELLITE_NOAA15_AVHRR:		// 判断是否 NOAA15_AVHRR 数据
		case AGMRS_SATELLITE_NOAA16_AVHRR:		// 判断是否 NOAA16_AVHRR 数据
		case AGMRS_SATELLITE_NOAA18_AVHRR:		// 判断是否 NOAA18_AVHRR 数据
		case AGMRS_SATELLITE_NOAA19_AVHRR:		// 判断是否 NOAA19_AVHRR 数据
		case AGMRS_SATELLITE_FY1D_AVHRR:		// 判断是否 FY1D_AVHRR 数据
			iDataSource = AgmrsDataSource::Avhrr;
			break;

		case AGMRS_SATELLITE_AQUA_MODIS:		// 判断是否 AQUA_MODIS 数据
		case AGMRS_SATELLITE_TERRA_MODIS:		// 判断是否 TERRA_MODIS 数据
			iDataSource = AgmrsDataSource::Modis;
			break;

		case AGMRS_SATELLITE_FY3A_MERSI:		// 判断是否 FY3A_MERSI 数据
		case AGMRS_SATELLITE_FY3B_MERSI:		// 判断是否 FY3B_MERSI 数据
		case AGMRS_SATELLITE_FY3C_MERSI:		// 判断是否 FY3C_MERSI 数据
			iDataSource = AgmrsDataSource::Mersi;
			break;

		case AGMRS_SATELLITE_FY3A_VIRR:			// 判断是否 FY3A_VIRR 数据
		case AGMRS_SATELLITE_FY3B_VIRR:			// 判断是否 FY3B_VIRR 数据
		case AGMRS_SATELLITE_FY3C_VIRR:			// 判断是否 FY3C_VIRR 数据
			iDataSource = AgmrsDataSource::Virr;
			break;

		default:
			iDataSource = AgmrsDataSource::Unknown;
			break;
		}

		return iDataSource;
	}

	// static
	KAvhrrThreshold* KAgmrsData::GetAvhrrThreshold()
	{
		return &s_AvhrrThreshold;
	}
	void KAgmrsData::SetAvhrrThreshold(KAvhrrThreshold* pAvhrrThreshold)
	{
		s_AvhrrThreshold = *pAvhrrThreshold;
	}

	// static
	KModisThreshold* KAgmrsData::GetModisThreshold()
	{
		return &s_ModisThreshold;
	}
	void KAgmrsData::SetModisThreshold(KModisThreshold* pModisThreshold)
	{
		s_ModisThreshold = *pModisThreshold;
	}

	// static
	KMersiThreshold* KAgmrsData::GetMersiThreshold()
	{
		return &s_MersiThreshold;
	}
	void KAgmrsData::SetMersiThreshold(KMersiThreshold* pMersiThreshold)
	{
		s_MersiThreshold = *pMersiThreshold;
	}

	// static
	KVirrThreshold* KAgmrsData::GetVirrThreshold()
	{
		return &s_VirrThreshold;
	}
	void KAgmrsData::SetVirrThreshold(KVirrThreshold* pVirrThreshold)
	{
		s_VirrThreshold = *pVirrThreshold;
	}

	// static 
	// 取得农气要干产品调色板
	KPalette* KAgmrsData::GetAgmrsProductPalette(int iProductCode, PaletteType ePaletteType)
	{
		KPalette* pPalette = NULL;
		switch (ePaletteType)
		{
		case PaletteType_Standard:	// 标准调色板
			pPalette = &KAgmrsData::s_StdPalette;
			break;

		case PaletteType_Index:		// 索引调色板
			pPalette = &KAgmrsData::s_IdxPalette;
			break;

		case PaletteType_Rainbow:	// 彩虹调色板
			pPalette = &KAgmrsData::s_RbwPalette;
			break;

		case PaletteType_Segment:	// 分段调色板
			pPalette = &KAgmrsData::s_SegPalette;
			break;

		case PaletteType_Linear:	// 线性调色板
			pPalette = &KAgmrsData::s_LinearPalette;
			break;
		}
		pPalette->Clear();

		switch(iProductCode)
		{
		//---------------------------------------------VIX
		case AGMRS_PRODUCT_CODE_VIX_NDVI:	// [0,1]
		case AGMRS_PRODUCT_CODE_VIX_SAVI:	// [0,1]
		case AGMRS_PRODUCT_CODE_VIX_EVI:	// [0,1]
			pPalette->AddColor(0.05, 194,  82,  60, _T(""));
			pPalette->AddColor(0.10, 207,  99,  50, _T(""));
			pPalette->AddColor(0.15, 221, 126,  36, _T(""));
			pPalette->AddColor(0.20, 229, 144,  27, _T(""));
			pPalette->AddColor(0.25, 238, 161,  19, _T(""));
			pPalette->AddColor(0.30, 242, 186,  15, _T(""));
			pPalette->AddColor(0.35, 247, 215,   8, _T(""));
			pPalette->AddColor(0.40, 255, 255,   0, _T(""));
			pPalette->AddColor(0.45, 167, 242,   1, _T(""));
			pPalette->AddColor(0.50,  77, 230,   0, _T(""));
			pPalette->AddColor(0.55,  82, 217,  18, _T(""));
			pPalette->AddColor(0.60,   0, 206,   0, _T(""));
			pPalette->AddColor(0.65,  15, 197,  63, _T(""));
			pPalette->AddColor(0.70,  24, 173, 115, _T(""));
			pPalette->AddColor(0.75,  28, 155, 144, _T(""));
			pPalette->AddColor(0.80,  27, 122, 142, _T(""));
			pPalette->AddColor(0.85,  19,  85, 133, _T(""));
			pPalette->AddColor(0.90,  11,  44, 123, _T(""));
			pPalette->AddColor(0.95,  10,  32, 100, _T(""));
			pPalette->AddColor(1.00,   5,  16,  64, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_DVI:	// [0,1]
			pPalette->AddColor(0.03, 100,  80,  60, _T(""));
			pPalette->AddColor(0.05, 125,  90,  70, _T(""));
			pPalette->AddColor(0.07, 180, 120,  70, _T(""));
			pPalette->AddColor(0.10, 255, 200,  30, _T(""));
			pPalette->AddColor(0.13, 237, 230,  33, _T(""));
			pPalette->AddColor(0.16,  34, 240,  48, _T(""));
			pPalette->AddColor(0.20,  42, 209,  54, _T(""));
			pPalette->AddColor(0.23,   0, 156,   0, _T(""));
			pPalette->AddColor(0.26,   0, 128,   0, _T(""));
			pPalette->AddColor(0.30,   0, 100,   0, _T(""));
			pPalette->AddColor(0.33,   0,  80,   0, _T(""));
			pPalette->AddColor(0.35,   0,  60,   0, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_RVI:	// [0,10]
			pPalette->AddColor(0.50, 194,  82,  60, _T(""));
			pPalette->AddColor(1.00, 207,  99,  50, _T(""));
			pPalette->AddColor(1.50, 221, 126,  36, _T(""));
			pPalette->AddColor(2.00, 229, 144,  27, _T(""));
			pPalette->AddColor(2.50, 238, 161,  19, _T(""));
			pPalette->AddColor(3.00, 242, 186,  15, _T(""));
			pPalette->AddColor(3.50, 247, 215,   8, _T(""));
			pPalette->AddColor(4.50, 255, 255,   0, _T(""));
			pPalette->AddColor(5.00, 167, 242,   1, _T(""));
			pPalette->AddColor(5.50,  77, 230,   0, _T(""));
			pPalette->AddColor(6.00,  82, 217,  18, _T(""));
			pPalette->AddColor(6.50,   0, 206,   0, _T(""));
			pPalette->AddColor(7.00,  15, 197,  63, _T(""));
			pPalette->AddColor(7.50,  24, 173, 115, _T(""));
			pPalette->AddColor(8.00,  28, 155, 144, _T(""));
			pPalette->AddColor(8.50,  27, 122, 142, _T(""));
			pPalette->AddColor(9.00,  19,  85, 133, _T(""));
			pPalette->AddColor(9.50,  11,  44, 123, _T(""));
			pPalette->AddColor(9.99,  10,  32, 100, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_VIX_LAI:	// [0,10]
			pPalette->AddColor(0.50, 100,  80,  60, _T(""));
			pPalette->AddColor(1.00, 125,  90,  70, _T(""));
			pPalette->AddColor(1.50, 180, 120,  70, _T(""));
			pPalette->AddColor(2.00, 255, 200,  30, _T(""));
			pPalette->AddColor(2.50, 237, 230,  33, _T(""));
			pPalette->AddColor(3.00, 208, 255, 128, _T(""));
			pPalette->AddColor(3.50, 176, 255,  96, _T(""));
			pPalette->AddColor(4.00, 144, 255,  64, _T(""));
			pPalette->AddColor(4.50, 112, 255,  32, _T(""));
			pPalette->AddColor(5.00,  96, 255,  16, _T(""));
			pPalette->AddColor(5.50,  64, 255,   0, _T(""));
			pPalette->AddColor(6.00,  48, 240,   0, _T(""));
			pPalette->AddColor(6.50,  32, 208,   0, _T(""));
			pPalette->AddColor(7.00,  16, 208,   0, _T(""));
			pPalette->AddColor(7.50,   0, 192,   0, _T(""));
			pPalette->AddColor(8.00,   0, 176,   0, _T(""));
			pPalette->AddColor(8.50,   0, 160,   0, _T(""));
			pPalette->AddColor(9.00,   0, 144,   0, _T(""));
			pPalette->AddColor(9.99,   0, 128,   0, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		//---------------------------------------------DIX
		case AGMRS_PRODUCT_CODE_DIX_ATI:
		case AGMRS_PRODUCT_CODE_DIX_VSWI:
			pPalette->AddColor(0.03, 100,  80,  60, _T(""));
			pPalette->AddColor(0.05, 125,  90,  70, _T(""));
			pPalette->AddColor(0.07, 180, 120,  70, _T(""));
			pPalette->AddColor(0.10, 255, 200,  30, _T(""));
			pPalette->AddColor(0.13, 237, 230,  33, _T(""));
			pPalette->AddColor(0.16,  34, 240,  48, _T(""));
			pPalette->AddColor(0.20,  42, 209,  54, _T(""));
			pPalette->AddColor(0.23,   0, 156,   0, _T(""));
			pPalette->AddColor(0.26,   0, 128,   0, _T(""));
			pPalette->AddColor(0.30,   0, 100,   0, _T(""));
			pPalette->AddColor(0.33,   0,  80,   0, _T(""));
			pPalette->AddColor(0.35,   0,  60,   0, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_SWCI:
			pPalette->AddColor(0.05, 100,  80,  60, _T(""));
			pPalette->AddColor(0.10, 125,  90,  70, _T(""));
			pPalette->AddColor(0.15, 180, 120,  70, _T(""));
			pPalette->AddColor(0.20, 255, 200,  30, _T(""));
			pPalette->AddColor(0.25, 237, 230,  33, _T(""));
			pPalette->AddColor(0.30, 208, 255, 128, _T(""));
			pPalette->AddColor(0.35, 176, 255,  96, _T(""));
			pPalette->AddColor(0.40, 144, 255,  64, _T(""));
			pPalette->AddColor(0.45, 112, 255,  32, _T(""));
			pPalette->AddColor(0.50,  96, 255,  16, _T(""));
			pPalette->AddColor(0.55,  64, 255,   0, _T(""));
			pPalette->AddColor(0.60,  48, 240,   0, _T(""));
			pPalette->AddColor(0.65,  32, 224,   0, _T(""));
			pPalette->AddColor(0.70,  16, 208,   0, _T(""));
			pPalette->AddColor(0.75,   0, 192,   0, _T(""));
			pPalette->AddColor(0.80,   0, 176,   0, _T(""));
			pPalette->AddColor(0.85,   0, 160,   0, _T(""));
			pPalette->AddColor(0.90,   0, 144,   0, _T(""));
			pPalette->AddColor(0.95,   0, 128,   0, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_TVDI:
			pPalette->AddColor(0.05,   0, 128,   0, _T(""));
			pPalette->AddColor(0.10,   0, 144,   0, _T(""));
			pPalette->AddColor(0.15,   0, 160,   0, _T(""));
			pPalette->AddColor(0.20,   0, 176,   0, _T(""));
			pPalette->AddColor(0.25,   0, 192,   0, _T(""));
			pPalette->AddColor(0.30,  16, 208,   0, _T(""));
			pPalette->AddColor(0.35,  32, 208,   0, _T(""));
			pPalette->AddColor(0.40,  48, 240,   0, _T(""));
			pPalette->AddColor(0.45,  64, 255,   0, _T(""));
			pPalette->AddColor(0.50,  96, 255,  16, _T(""));
			pPalette->AddColor(0.55, 112, 255,  32, _T(""));
			pPalette->AddColor(0.60, 144, 255,  64, _T(""));
			pPalette->AddColor(0.65, 176, 255,  96, _T(""));
			pPalette->AddColor(0.70, 208, 255, 128, _T(""));
			pPalette->AddColor(0.75, 237, 230,  33, _T(""));
			pPalette->AddColor(0.80, 255, 200,  30, _T(""));
			pPalette->AddColor(0.85, 180, 120,  70, _T(""));
			pPalette->AddColor(0.90, 125,  90,  70, _T(""));
			pPalette->AddColor(0.95, 100,  80,  60, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_DIX_PDI:
		case AGMRS_PRODUCT_CODE_DIX_MPDI:
			pPalette->AddColor(0.20,   0, 128,   0, _T(""));
			pPalette->AddColor(0.25,   0, 160,   0, _T(""));
			pPalette->AddColor(0.30,   0, 192,   0, _T(""));
			pPalette->AddColor(0.35,  32, 208,   0, _T(""));
			pPalette->AddColor(0.40,  64, 255,   0, _T(""));
			pPalette->AddColor(0.45, 112, 255,  32, _T(""));
			pPalette->AddColor(0.50, 176, 255,  96, _T(""));
			pPalette->AddColor(0.55, 237, 230,  33, _T(""));
			pPalette->AddColor(0.60, 180, 120,  70, _T(""));
			pPalette->AddColor(0.65, 100,  80,  60, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		//---------------------------------------------SMC
		case AGMRS_PRODUCT_CODE_SMC_AH:
			pPalette->AddColor(  5.0, 183,  91,   0, _T(""));
			pPalette->AddColor( 10.0, 242, 121,   0, _T(""));
			pPalette->AddColor( 15.0, 255, 255,   0, _T(""));
			pPalette->AddColor( 20.0, 167, 242,   0, _T(""));
			pPalette->AddColor( 25.0,   0, 206,   0, _T(""));
			pPalette->AddColor( 30.0,  24, 173, 115, _T(""));
			pPalette->AddColor( 35.0,  27, 122, 142, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_SMC_RH:
			pPalette->AddColor( 40.0, 142,  71,   0, _T("重旱"));
			pPalette->AddColor( 50.0, 242, 121,   0, _T("中旱"));
			pPalette->AddColor( 60.0, 255, 255,   9, _T("轻旱"));
			pPalette->AddColor( 90.0,  75, 151,   0, _T("适宜"));
			pPalette->AddColor(100.0, 119, 119, 255, _T("偏湿"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("非耕地"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		//---------------------------------------------IRG
		case AGMRS_PRODUCT_CODE_IRG_AREA:	// 灌溉面积
			break;
		case AGMRS_PRODUCT_CODE_IRG_AMOUNT:	// 灌溉量
			pPalette->AddColor( 1.0,   0,  96,   0, _T("1.0"));
			pPalette->AddColor( 2.0,  13, 180,   0, _T("2.0"));
			pPalette->AddColor( 3.0,  32, 224,   0, _T("3.0"));
			pPalette->AddColor( 4.0,  96, 255,  16, _T("4.0"));
			pPalette->AddColor( 5.0,  12, 255,  32, _T("5.0"));
			pPalette->AddColor( 6.0, 176, 255,  96, _T("6.0"));
			pPalette->AddColor( 7.0, 224, 255, 128, _T("7.0"));
			pPalette->AddColor( 8.0, 240, 240, 128, _T("8.0"));
			pPalette->AddColor( 9.0, 228, 228,  36, _T("9.0"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		//---------------------------------------------LVL
		case AGMRS_PRODUCT_CODE_LVL_MQ_WHEAT:
		case AGMRS_PRODUCT_CODE_LVL_MQ_CORN:
			pPalette->AddColor( 1,   0, 255,   0, _T("一类苗"));
			pPalette->AddColor( 2, 178, 151,   0, _T("二类苗"));
			pPalette->AddColor( 3, 255,   0,   0, _T("三类苗"));
			pPalette->AddColor( 4,   0, 122, 133, _T("旺长苗"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("非耕地"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_LVL_DRY:	// 干旱等级
			pPalette->AddColor(1.0, 119, 119, 255, _T("偏湿"));
			pPalette->AddColor(2.0,  75, 151,   0, _T("适宜"));
			pPalette->AddColor(3.0, 255, 255,   9, _T("轻旱"));
			pPalette->AddColor(4.0, 242, 121,   0, _T("中旱"));
			pPalette->AddColor(5.0, 142,  71,   0, _T("重旱"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255, 255, 255, _T("非耕地"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;
		case AGMRS_PRODUCT_CODE_LVL_LANDUSE:	// 土地利用
			pPalette->AddColor(11,   0, 192,   0, _T("11_水田"));
			pPalette->AddColor(12,   0, 192,   0, _T("12_旱田"));
			pPalette->AddColor(19,   0, 192,   0, _T("19_耕地"));

			pPalette->AddColor(21,  28, 198, 198, _T("21_有林地"));
			pPalette->AddColor(22,  28, 198, 198, _T("22_灌木林地"));
			pPalette->AddColor(23,  28, 198, 198, _T("23_疏林地"));
			pPalette->AddColor(24,  28, 198, 198, _T("24_其他林地"));
			pPalette->AddColor(29,  28, 198, 198, _T("29_林地"));

			pPalette->AddColor(31, 167, 255,   1, _T("31_高覆盖度草地"));
			pPalette->AddColor(32, 167, 255,   1, _T("32_中覆盖度草地"));
			pPalette->AddColor(33, 167, 255,   1, _T("33_低覆盖度草地"));
			pPalette->AddColor(39, 167, 255,   1, _T("39_草地"));

			pPalette->AddColor(41,   0,   0, 255, _T("41_河渠"));
			pPalette->AddColor(42,   0,   0, 255, _T("42_湖泊"));
			pPalette->AddColor(43,   0,   0, 255, _T("43_水库坑塘"));
			pPalette->AddColor(46, 194,  82, 255, _T("46_滩地"));
			pPalette->AddColor(49,   0,   0, 255, _T("49_水体"));

			pPalette->AddColor(51, 255, 255,   0, _T("51_城镇用地"));
			pPalette->AddColor(52, 255, 255,   0, _T("52_农村居民点用地"));
			pPalette->AddColor(53, 255, 255,   0, _T("53_工交建设用地"));
			pPalette->AddColor(59, 255, 255,   0, _T("59_城居及工交地"));

			pPalette->AddColor(61, 194,  82,  60, _T("61_沙地"));
			pPalette->AddColor(63, 194,  82,  60, _T("63_盐碱地"));
			pPalette->AddColor(64, 194,  82,  60, _T("64_沼泽地"));
			pPalette->AddColor(65, 194,  82,  60, _T("65_裸土地"));
			pPalette->AddColor(66, 194,  82,  60, _T("66_其他"));
			pPalette->AddColor(69, 194,  82,  60, _T("69_其它"));
			break;
		//---------------------------------------------SNOW
		case AGMRS_PRODUCT_CODE_SNOW_NDSI:	// 积雪指数
			break;
		case AGMRS_PRODUCT_CODE_SNOW_DEPTH:	// 积雪深度
			break;
		case AGMRS_PRODUCT_CODE_SNOW_COVER:	// 积雪覆盖
			break;
		//---------------------------------------------MISC
		case AGMRS_PRODUCT_CODE_MISC_CLOUD:		// 简单监测 - 云
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255, 255, 255, _T("云雾"));
			break;
		case AGMRS_PRODUCT_CODE_MISC_FOG:		// 简单监测 - 雾
			break;

		case AGMRS_PRODUCT_CODE_MISC_FIRE:		// 简单监测 - 火点
			//pPalette->AddColor(AXIN_SPECIAL_DATA_FIRING, 255,   0,   0, _T("明火点"));
			//pPalette->AddColor(AXIN_SPECIAL_DATA_FIRED,  128,   0,   0, _T("过火点"));
			pPalette->AddColor(1, 255,   0,   0, _T("明火点"));
			pPalette->AddColor(2, 128,   0,   0, _T("过火点"));
			break;

		case AGMRS_PRODUCT_CODE_MISC_SNOW:		// 简单监测 - 积雪指数
			pPalette->AddColor(0.40, 128, 128, 128, _T("0.40"));
			pPalette->AddColor(0.45, 128, 128, 128, _T("0.45"));
			pPalette->AddColor(0.50, 128, 128, 128, _T("0.50"));
			pPalette->AddColor(0.55, 128, 128, 128, _T("0.55"));
			pPalette->AddColor(0.60, 128, 128, 128, _T("0.60"));
			pPalette->AddColor(0.65, 128, 128, 128, _T("0.65"));
			pPalette->AddColor(0.70, 128, 128, 128, _T("0.70"));
			pPalette->AddColor(0.75, 128, 128, 128, _T("0.75"));
			pPalette->AddColor(0.80, 128, 128, 128, _T("0.80"));
			pPalette->AddColor(0.85, 128, 128, 128, _T("0.85"));
			pPalette->AddColor(0.90, 128, 128, 128, _T("0.90"));
			pPalette->AddColor(0.95, 128, 128, 128, _T("0.95"));
			pPalette->AddColor(1.00, 128, 128, 128, _T("1.00"));
			break;

		case AGMRS_PRODUCT_CODE_MISC_ICE:		// 简单监测 - 冰
			break;

		case AGMRS_PRODUCT_CODE_MISC_LAND:		// 简单监测 - 陆
			break;

		case AGMRS_PRODUCT_CODE_MISC_WATER:		// 简单监测 - 水
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,   0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_MISC_LST:	// 地表温度(摄氏度)
			pPalette->AddColor(-10.0,   2, 164, 155, _T(""));
			pPalette->AddColor( -5.0,   2, 196, 186, _T(""));
			pPalette->AddColor(  0.0,   0, 255, 192, _T(""));
			pPalette->AddColor(  5.0, 179, 179, 255, _T(""));
			pPalette->AddColor( 10.0, 140, 140, 255, _T(""));
			pPalette->AddColor( 15.0, 104, 104, 255, _T(""));
			pPalette->AddColor( 20.0,   1, 203, 122, _T(""));
			pPalette->AddColor( 22.0,  78, 255,   0, _T(""));
			pPalette->AddColor( 25.0, 146, 255,   0, _T(""));
			pPalette->AddColor( 28.0, 210, 255,   0, _T(""));
			pPalette->AddColor( 30.0, 255, 255, 160, _T(""));
			pPalette->AddColor( 32.0, 255, 255,   0, _T(""));
			pPalette->AddColor( 34.0, 255, 224,   0, _T(""));
			pPalette->AddColor( 36.0, 240, 182,   0, _T(""));
			pPalette->AddColor( 38.0, 240, 160,   0, _T(""));
			pPalette->AddColor( 40.0, 248, 160,   0, _T(""));
			pPalette->AddColor( 42.0, 248,  96,   0, _T(""));
			pPalette->AddColor( 45.0, 248,  64,   0, _T(""));
			pPalette->AddColor( 50.0, 248,  16,   0, _T(""));
			pPalette->AddColor( 55.0, 240,  48,  32, _T(""));
			pPalette->AddColor( 60.0, 234,  48,  32, _T(""));
			pPalette->AddColor( 70.0, 208,  32,  16, _T(""));
			pPalette->AddColor( 80.0, 160,  32,  16, _T(""));
			pPalette->AddColor( 90.0, 128,   0,   0, _T(""));
			pPalette->AddColor( 99.9,  90,   0,   0, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_MISC_ABE:
			break;

		//---------------------------------------------DIF
		case AGMRS_PRODUCT_CODE_DIF_VIX_NDVI:		// 植被指数变化 - NDVI (-0.30 ~ +0.30)
			pPalette->AddColor(-0.30, 255,   0,   0, _T(""));
			pPalette->AddColor(-0.25, 245,  63,  10, _T(""));
			pPalette->AddColor(-0.20, 233,  90,  22, _T(""));
			pPalette->AddColor(-0.15, 220, 104,  35, _T(""));
			pPalette->AddColor(-0.10, 210, 140,  45, _T(""));
			pPalette->AddColor(-0.05, 218, 177,  37, _T(""));
			pPalette->AddColor(-0.02, 255, 255,   0, _T(""));
			pPalette->AddColor( 0.00, 255, 255, 255, _T(""));	// 0
			pPalette->AddColor( 0.02, 177, 254,  10, _T(""));
			pPalette->AddColor( 0.05, 114, 238,   2, _T(""));
			pPalette->AddColor( 0.10, 103, 196,   0, _T(""));
			pPalette->AddColor( 0.15,  71, 136,   0, _T(""));
			pPalette->AddColor( 0.20,  16, 136,  16, _T(""));
			pPalette->AddColor( 0.25,  11,  91,  11, _T(""));
			pPalette->AddColor( 0.30,  17,  83,  52, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_DVI:		// 植被指数变化 - DVI (-0.10 ~ +0.10)
			pPalette->AddColor(-0.10, 255,   0,   0, _T(""));
			pPalette->AddColor(-0.08, 245,  63,  10, _T(""));
			pPalette->AddColor(-0.06, 233,  90,  22, _T(""));
			pPalette->AddColor(-0.04, 220, 104,  35, _T(""));
			pPalette->AddColor(-0.02, 210, 140,  45, _T(""));
			pPalette->AddColor( 0.00, 255, 255, 255, _T(""));	// 0
			pPalette->AddColor( 0.02, 177, 254,  10, _T(""));
			pPalette->AddColor( 0.04, 114, 238,   2, _T(""));
			pPalette->AddColor( 0.06, 103, 196,   0, _T(""));
			pPalette->AddColor( 0.08,  71, 136,   0, _T(""));
			pPalette->AddColor( 0.10,  16, 136,  16, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_RVI:		// 植被指数变化 - RVI (-0.40 ~ +0.40)
			pPalette->AddColor(-0.40, 255,   0,   0, _T(""));
			pPalette->AddColor(-0.30, 245,  63,  10, _T(""));
			pPalette->AddColor(-0.20, 233,  90,  22, _T(""));
			pPalette->AddColor(-0.15, 220, 104,  35, _T(""));
			pPalette->AddColor(-0.10, 210, 140,  45, _T(""));
			pPalette->AddColor(-0.05, 218, 177,  37, _T(""));
			pPalette->AddColor(-0.02, 255, 255,   0, _T(""));
			pPalette->AddColor( 0.00, 255, 255, 255, _T(""));	// 0
			pPalette->AddColor( 0.02, 177, 254,  10, _T(""));
			pPalette->AddColor( 0.05, 114, 238,   2, _T(""));
			pPalette->AddColor( 0.10, 103, 196,   0, _T(""));
			pPalette->AddColor( 0.15,  71, 136,   0, _T(""));
			pPalette->AddColor( 0.20,  16, 136,  16, _T(""));
			pPalette->AddColor( 0.30,  11,  91,  11, _T(""));
			pPalette->AddColor( 0.40,  17,  83,  52, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_SAVI:		// 植被指数变化 - SAVI (-0.25 ~ +0.25)
			pPalette->AddColor(-0.25, 255,   0,   0, _T(""));
			pPalette->AddColor(-0.20, 245,  63,  10, _T(""));
			pPalette->AddColor(-0.15, 233,  90,  22, _T(""));
			pPalette->AddColor(-0.10, 220, 104,  35, _T(""));
			pPalette->AddColor(-0.05, 210, 140,  45, _T(""));
			pPalette->AddColor(-0.02, 218, 177,  37, _T(""));
			pPalette->AddColor(-0.01, 255, 255,   0, _T(""));
			pPalette->AddColor( 0.00, 255, 255, 255, _T(""));	// 0
			pPalette->AddColor( 0.01, 177, 254,  10, _T(""));
			pPalette->AddColor( 0.02, 114, 238,   2, _T(""));
			pPalette->AddColor( 0.05, 103, 196,   0, _T(""));
			pPalette->AddColor( 0.10,  71, 136,   0, _T(""));
			pPalette->AddColor( 0.15,  16, 136,  16, _T(""));
			pPalette->AddColor( 0.20,  11,  91,  11, _T(""));
			pPalette->AddColor( 0.25,  17,  83,  52, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_EVI:		// 植被指数变化 - NDVI (-0.30 ~ +0.30)
			pPalette->AddColor(-0.30, 255,   0,   0, _T(""));
			pPalette->AddColor(-0.25, 245,  63,  10, _T(""));
			pPalette->AddColor(-0.20, 233,  90,  22, _T(""));
			pPalette->AddColor(-0.15, 220, 104,  35, _T(""));
			pPalette->AddColor(-0.10, 210, 140,  45, _T(""));
			pPalette->AddColor(-0.05, 218, 177,  37, _T(""));
			pPalette->AddColor(-0.02, 255, 255,   0, _T(""));
			pPalette->AddColor( 0.00, 255, 255, 255, _T(""));	// 0
			pPalette->AddColor( 0.02, 177, 254,  10, _T(""));
			pPalette->AddColor( 0.05, 114, 238,   2, _T(""));
			pPalette->AddColor( 0.10, 103, 196,   0, _T(""));
			pPalette->AddColor( 0.15,  71, 136,   0, _T(""));
			pPalette->AddColor( 0.20,  16, 136,  16, _T(""));
			pPalette->AddColor( 0.25,  11,  91,  11, _T(""));
			pPalette->AddColor( 0.30,  17,  83,  52, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_LAI:		// 植被指数变化 - LAI (-2.5 ~ +2.5)
			pPalette->AddColor(-2.50, 255,   0,   0, _T(""));
			pPalette->AddColor(-1.80, 245,  63,  10, _T(""));
			pPalette->AddColor(-1.50, 233,  90,  22, _T(""));
			pPalette->AddColor(-1.20, 220, 104,  35, _T(""));
			pPalette->AddColor(-0.90, 210, 140,  45, _T(""));
			pPalette->AddColor(-0.60, 218, 177,  37, _T(""));
			pPalette->AddColor(-0.30, 255, 255,   0, _T(""));
			pPalette->AddColor( 0.00, 255, 255, 255, _T(""));	// 0
			pPalette->AddColor( 0.30, 177, 254,  10, _T(""));
			pPalette->AddColor( 0.60, 114, 238,   2, _T(""));
			pPalette->AddColor( 0.90, 103, 196,   0, _T(""));
			pPalette->AddColor( 2.20,  71, 136,   0, _T(""));
			pPalette->AddColor( 1.50,  16, 136,  16, _T(""));
			pPalette->AddColor( 1.80,  11,  91,  11, _T(""));
			pPalette->AddColor( 2.50,  17,  83,  52, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_DIX_TVDI:		// 干旱指数变化 - TVDI (-0.80 ~ +0.80)
			pPalette->AddColor(-0.80, 255,   0,   0, _T(""));
			pPalette->AddColor(-0.70, 245,  63,  10, _T(""));
			pPalette->AddColor(-0.60, 233,  90,  22, _T(""));
			pPalette->AddColor(-0.50, 220, 104,  35, _T(""));
			pPalette->AddColor(-0.40, 210, 140,  45, _T(""));
			pPalette->AddColor(-0.30, 218, 177,  37, _T(""));
			pPalette->AddColor(-0.20, 255, 255,   0, _T(""));
			pPalette->AddColor( 0.00, 255, 255, 255, _T(""));	// 0
			pPalette->AddColor( 0.20, 177, 254,  10, _T(""));
			pPalette->AddColor( 0.30, 114, 238,   2, _T(""));
			pPalette->AddColor( 0.40, 103, 196,   0, _T(""));
			pPalette->AddColor( 0.50,  71, 136,   0, _T(""));
			pPalette->AddColor( 0.60,  16, 136,  16, _T(""));
			pPalette->AddColor( 0.70,  11,  91,  11, _T(""));
			pPalette->AddColor( 0.80,  17,  83,  52, _T(""));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("云雾"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("水体"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_MISC_WATER:		// 水情变化
			pPalette->AddColor( 1, 255, 255,   0, _T("减少"));
			pPalette->AddColor( 2, 255,   0,   0, _T("增加"));
			pPalette->AddColor( 8, 255, 255, 255, _T("云雾"));
			pPalette->AddColor( 9,  0,   0,  255, _T("水体"));
			break;

		//==================================
		case 0xF000:	// 县界格点 1-147 (索引)
			pPalette->AddColor( 1, 127, 224,  53, _T("安  阳"));		// 1
			pPalette->AddColor( 2, 121,   5, 230, _T("鹤  壁"));		// 2
			pPalette->AddColor( 3, 237, 150,  19, _T("濮  阳"));		// 3
			pPalette->AddColor( 4, 100, 245, 245, _T("新  乡"));		// 4
			pPalette->AddColor( 5, 245, 110, 195, _T("焦  作"));		// 5
			pPalette->AddColor( 6, 222,   4,   41, _T("三门峡"));		// 6
			pPalette->AddColor( 7,  56,  97,  29, _T("洛  阳"));		// 7
			pPalette->AddColor( 8,  79,  96, 196, _T("郑  州"));		// 8
			pPalette->AddColor( 9, 205, 252, 139, _T("开  封"));		// 9
			pPalette->AddColor(10, 171, 137, 130, _T("许  昌"));		// 10
			pPalette->AddColor(11, 181, 250,  20, _T("平顶山"));		// 11
			pPalette->AddColor(12,  82,  14,   4, _T("漯  河"));		// 12
			pPalette->AddColor(13,   6,  13, 115, _T("商  丘"));		// 13
			pPalette->AddColor(14,  50,  94,  94, _T("周  口"));		// 14
			pPalette->AddColor(15, 219,  94,  92, _T("驻马店"));		// 15
			pPalette->AddColor(16, 120,  85,  19, _T("南  阳"));		// 16
			pPalette->AddColor(17, 122,  31, 112, _T("信  阳"));		// 17
			pPalette->AddColor(18,  50, 207, 125, _T("济  源"));		// 18
			break;

		case 0xF001:	// 地市格点 1-18 (分段)
			pPalette->AddColor(30,   0,   128,   0, _T(""));
			break;

		case 0xF002:	// 县界格点 1-147 (分段)
			pPalette->AddColor(200,   0,   0,   255, _T(""));
			break;

		case 0xF003:	// 乡界格点 1-2100 (分段)
			pPalette->AddColor(2101,   128,   192,    0, _T(""));
			break;

		//==================================
		}

		return pPalette;
	}
	///====================================================

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------
