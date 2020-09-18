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

	// �ڴ�����
	void KAgmrsData::Empty()
	{
		this->SetType(AgmrsDataSource::Unknown);
		this->SetFileName(_T(""));

		// �ڴ����
		KWord2DMemAllocator::FreeMemory2D(m_ppwData);
		KByte2DMemAllocator::FreeMemory2D(m_ppucDataGray);

		_delete(m_pFileHeader);

		m_iRChannelDef = 0;			//��ɫͨ��Ĭ��ֵ
		m_iGChannelDef = 0;			//��ɫͨ��Ĭ��ֵ
		m_iBChannelDef = 0;			//��ɫͨ��Ĭ��ֵ

		m_iRChannel = 0;			//��ɫͨ��
		m_iGChannel = 0;			//��ɫͨ��
		m_iBChannel = 0;			//��ɫͨ��

		m_bEmpty = TRUE;
	}

	// ��������
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

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		int iDataSize = iWidth * iHeight * iChannelNums;

		// ����ָ��
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

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		int iDataSize = iWidth * iHeight * iChannelNums;

		// ����ָ��
		PWORD* ppwData = this->GetData();
		fwrite(*ppwData,sizeof(WORD),iDataSize,fp);

		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	// װ������
	BOOL KAgmrsData::LoadData_bin(LPCTSTR lpszFile)
	{
		this->SetFileName(lpszFile);		// �����ļ���
		this->MapChannel();					// ӳ��ͨ��(ͨ���ź�����������Ӧ) - pure virtual function
		this->SetDefaultDisplayChannel();	// ����Ĭ�ϵ���ʾͨ�� - pure virtual function
		m_bEmpty = FALSE;					// �ǿձ����λ
		return TRUE;
	}

	// ����/��ȡ����ͨ��
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

	// ����/��ȡĬ������ͨ��
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

	// ȡ��ң������Դ
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

	// ���ũ��ң������
	BOOL KAgmrsData::IsAgmrsData()
	{
		BOOL bValue = FALSE;
		//-------------------------------------------------
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)this->FileHeader();
		bValue = AGMRS_LOCAL_DATA_MARKER == pFileHeader->wFileID;
		//-------------------------------------------------
		return bValue;
	}

	// ��γ��ת��Ϊ�������
	// flag = 0 ң������λ��
	// flag = 1 ��Ʒ����λ��
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
	// �������ת��Ϊ��γ������
	// flag = 0 ң������λ��
	// flag = 1 ��Ʒ����λ��
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

	// ȡ�ò�Ʒ���ֵ
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

	// ȡ��ָ�����ȫ��ͨ������
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

	// ��Ʒ���� - ȫ����
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
	// ������ - SIM (Single Item Monitor)
	BOOL KAgmrsData::Sim(int iProductID)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->Sim(iProductID);
	}
	// ȡ�ò�Ʒ����
	PFLOAT* KAgmrsData::GetProductData()
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return NULL;

		return pAgmrsDataProcessor->GetProductData();
	}
	// ����ת��Ϊ DIB
	BOOL KAgmrsData::DataToDIB(int iRChannel,int iGChannel,int iBChannel)
	{ 
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->DataToDIB(iRChannel,iGChannel,iBChannel);
	}
	// �ϲ�����ͨ��
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

	// ȡ����Ԫ����
	int KAgmrsData::GetPixelType(int i,int j)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return PixelType::Unknown;

		return pAgmrsDataProcessor->GetPixelType(i,j);
	}
	// �Ƿ����(����)
	BOOL KAgmrsData::IsClearNight(int i,int j)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		return pAgmrsDataProcessor->IsClearNight(i,j);
	}

	// ����TVDI
	BOOL KAgmrsData::Dix_Tvdi(KDataInfo* pDataInfo,float* pDataNdvi,float* pDataLst,float*& pDataTvdi)
	{
		KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)this->GetDataProcessor();
		if(!pAgmrsDataProcessor)
			return FALSE;

		pAgmrsDataProcessor->RegiestCallbackFunc(m_pCallbackFunc,m_pCallbackParam);
		return pAgmrsDataProcessor->Dix_Tvdi(pDataInfo,pDataNdvi,pDataLst,pDataTvdi);
	}

	// ������Ʒ
	LPCTSTR KAgmrsData::NameProduct(int iProductID)
	{
		// ��Ʒ
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

		// ������Ϣ
		KDataInfo* pDataInfo = this->GetDataInfo();
		KFile file(this->GetFileName());

		// �ļ���(������չ��)
		TCHAR szTemp2[MAX_PATH] = _T("");
		_stprintf(szTemp2,_T("%s"),(LPCTSTR)file.GetFileName(false));


		// ������ɲ�Ʒ����
		_stprintf(m_szTemp,_T("%s_%s"),szTemp1 ,szTemp2);
		// �ļ�����yyyymmdd.HHMMSS.lll.ttt.prdid.prdname
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
	// ȡ����Ԫ�����(km^2)
	// resolution - �ֱ���
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

		// ��ȡ128�ֽ��ļ�ͷ
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
		case AGMRS_SATELLITE_NOAA15_AVHRR:		// �ж��Ƿ� NOAA15_AVHRR ����
		case AGMRS_SATELLITE_NOAA16_AVHRR:		// �ж��Ƿ� NOAA16_AVHRR ����
		case AGMRS_SATELLITE_NOAA18_AVHRR:		// �ж��Ƿ� NOAA18_AVHRR ����
		case AGMRS_SATELLITE_NOAA19_AVHRR:		// �ж��Ƿ� NOAA19_AVHRR ����
		case AGMRS_SATELLITE_FY1D_AVHRR:		// �ж��Ƿ� FY1D_AVHRR ����
			iDataSource = AgmrsDataSource::Avhrr;
			break;

		case AGMRS_SATELLITE_AQUA_MODIS:		// �ж��Ƿ� AQUA_MODIS ����
		case AGMRS_SATELLITE_TERRA_MODIS:		// �ж��Ƿ� TERRA_MODIS ����
			iDataSource = AgmrsDataSource::Modis;
			break;

		case AGMRS_SATELLITE_FY3A_MERSI:		// �ж��Ƿ� FY3A_MERSI ����
		case AGMRS_SATELLITE_FY3B_MERSI:		// �ж��Ƿ� FY3B_MERSI ����
		case AGMRS_SATELLITE_FY3C_MERSI:		// �ж��Ƿ� FY3C_MERSI ����
			iDataSource = AgmrsDataSource::Mersi;
			break;

		case AGMRS_SATELLITE_FY3A_VIRR:			// �ж��Ƿ� FY3A_VIRR ����
		case AGMRS_SATELLITE_FY3B_VIRR:			// �ж��Ƿ� FY3B_VIRR ����
		case AGMRS_SATELLITE_FY3C_VIRR:			// �ж��Ƿ� FY3C_VIRR ����
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
	// ȡ��ũ��Ҫ�ɲ�Ʒ��ɫ��
	KPalette* KAgmrsData::GetAgmrsProductPalette(int iProductCode, PaletteType ePaletteType)
	{
		KPalette* pPalette = NULL;
		switch (ePaletteType)
		{
		case PaletteType_Standard:	// ��׼��ɫ��
			pPalette = &KAgmrsData::s_StdPalette;
			break;

		case PaletteType_Index:		// ������ɫ��
			pPalette = &KAgmrsData::s_IdxPalette;
			break;

		case PaletteType_Rainbow:	// �ʺ��ɫ��
			pPalette = &KAgmrsData::s_RbwPalette;
			break;

		case PaletteType_Segment:	// �ֶε�ɫ��
			pPalette = &KAgmrsData::s_SegPalette;
			break;

		case PaletteType_Linear:	// ���Ե�ɫ��
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;
		case AGMRS_PRODUCT_CODE_SMC_RH:
			pPalette->AddColor( 40.0, 142,  71,   0, _T("�غ�"));
			pPalette->AddColor( 50.0, 242, 121,   0, _T("�к�"));
			pPalette->AddColor( 60.0, 255, 255,   9, _T("�ẵ"));
			pPalette->AddColor( 90.0,  75, 151,   0, _T("����"));
			pPalette->AddColor(100.0, 119, 119, 255, _T("ƫʪ"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("�Ǹ���"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;
		//---------------------------------------------IRG
		case AGMRS_PRODUCT_CODE_IRG_AREA:	// ������
			break;
		case AGMRS_PRODUCT_CODE_IRG_AMOUNT:	// �����
			pPalette->AddColor( 1.0,   0,  96,   0, _T("1.0"));
			pPalette->AddColor( 2.0,  13, 180,   0, _T("2.0"));
			pPalette->AddColor( 3.0,  32, 224,   0, _T("3.0"));
			pPalette->AddColor( 4.0,  96, 255,  16, _T("4.0"));
			pPalette->AddColor( 5.0,  12, 255,  32, _T("5.0"));
			pPalette->AddColor( 6.0, 176, 255,  96, _T("6.0"));
			pPalette->AddColor( 7.0, 224, 255, 128, _T("7.0"));
			pPalette->AddColor( 8.0, 240, 240, 128, _T("8.0"));
			pPalette->AddColor( 9.0, 228, 228,  36, _T("9.0"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;
		//---------------------------------------------LVL
		case AGMRS_PRODUCT_CODE_LVL_MQ_WHEAT:
		case AGMRS_PRODUCT_CODE_LVL_MQ_CORN:
			pPalette->AddColor( 1,   0, 255,   0, _T("һ����"));
			pPalette->AddColor( 2, 178, 151,   0, _T("������"));
			pPalette->AddColor( 3, 255,   0,   0, _T("������"));
			pPalette->AddColor( 4,   0, 122, 133, _T("������"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("�Ǹ���"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;
		case AGMRS_PRODUCT_CODE_LVL_DRY:	// �ɺ��ȼ�
			pPalette->AddColor(1.0, 119, 119, 255, _T("ƫʪ"));
			pPalette->AddColor(2.0,  75, 151,   0, _T("����"));
			pPalette->AddColor(3.0, 255, 255,   9, _T("�ẵ"));
			pPalette->AddColor(4.0, 242, 121,   0, _T("�к�"));
			pPalette->AddColor(5.0, 142,  71,   0, _T("�غ�"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255, 255, 255, _T("�Ǹ���"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;
		case AGMRS_PRODUCT_CODE_LVL_LANDUSE:	// ��������
			pPalette->AddColor(11,   0, 192,   0, _T("11_ˮ��"));
			pPalette->AddColor(12,   0, 192,   0, _T("12_����"));
			pPalette->AddColor(19,   0, 192,   0, _T("19_����"));

			pPalette->AddColor(21,  28, 198, 198, _T("21_���ֵ�"));
			pPalette->AddColor(22,  28, 198, 198, _T("22_��ľ�ֵ�"));
			pPalette->AddColor(23,  28, 198, 198, _T("23_���ֵ�"));
			pPalette->AddColor(24,  28, 198, 198, _T("24_�����ֵ�"));
			pPalette->AddColor(29,  28, 198, 198, _T("29_�ֵ�"));

			pPalette->AddColor(31, 167, 255,   1, _T("31_�߸��ǶȲݵ�"));
			pPalette->AddColor(32, 167, 255,   1, _T("32_�и��ǶȲݵ�"));
			pPalette->AddColor(33, 167, 255,   1, _T("33_�͸��ǶȲݵ�"));
			pPalette->AddColor(39, 167, 255,   1, _T("39_�ݵ�"));

			pPalette->AddColor(41,   0,   0, 255, _T("41_����"));
			pPalette->AddColor(42,   0,   0, 255, _T("42_����"));
			pPalette->AddColor(43,   0,   0, 255, _T("43_ˮ�����"));
			pPalette->AddColor(46, 194,  82, 255, _T("46_̲��"));
			pPalette->AddColor(49,   0,   0, 255, _T("49_ˮ��"));

			pPalette->AddColor(51, 255, 255,   0, _T("51_�����õ�"));
			pPalette->AddColor(52, 255, 255,   0, _T("52_ũ�������õ�"));
			pPalette->AddColor(53, 255, 255,   0, _T("53_���������õ�"));
			pPalette->AddColor(59, 255, 255,   0, _T("59_�ǾӼ�������"));

			pPalette->AddColor(61, 194,  82,  60, _T("61_ɳ��"));
			pPalette->AddColor(63, 194,  82,  60, _T("63_�μ��"));
			pPalette->AddColor(64, 194,  82,  60, _T("64_�����"));
			pPalette->AddColor(65, 194,  82,  60, _T("65_������"));
			pPalette->AddColor(66, 194,  82,  60, _T("66_����"));
			pPalette->AddColor(69, 194,  82,  60, _T("69_����"));
			break;
		//---------------------------------------------SNOW
		case AGMRS_PRODUCT_CODE_SNOW_NDSI:	// ��ѩָ��
			break;
		case AGMRS_PRODUCT_CODE_SNOW_DEPTH:	// ��ѩ���
			break;
		case AGMRS_PRODUCT_CODE_SNOW_COVER:	// ��ѩ����
			break;
		//---------------------------------------------MISC
		case AGMRS_PRODUCT_CODE_MISC_CLOUD:		// �򵥼�� - ��
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255, 255, 255, _T("����"));
			break;
		case AGMRS_PRODUCT_CODE_MISC_FOG:		// �򵥼�� - ��
			break;

		case AGMRS_PRODUCT_CODE_MISC_FIRE:		// �򵥼�� - ���
			//pPalette->AddColor(AXIN_SPECIAL_DATA_FIRING, 255,   0,   0, _T("�����"));
			//pPalette->AddColor(AXIN_SPECIAL_DATA_FIRED,  128,   0,   0, _T("�����"));
			pPalette->AddColor(1, 255,   0,   0, _T("�����"));
			pPalette->AddColor(2, 128,   0,   0, _T("�����"));
			break;

		case AGMRS_PRODUCT_CODE_MISC_SNOW:		// �򵥼�� - ��ѩָ��
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

		case AGMRS_PRODUCT_CODE_MISC_ICE:		// �򵥼�� - ��
			break;

		case AGMRS_PRODUCT_CODE_MISC_LAND:		// �򵥼�� - ½
			break;

		case AGMRS_PRODUCT_CODE_MISC_WATER:		// �򵥼�� - ˮ
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,   0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_MISC_LST:	// �ر��¶�(���϶�)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_MISC_ABE:
			break;

		//---------------------------------------------DIF
		case AGMRS_PRODUCT_CODE_DIF_VIX_NDVI:		// ֲ��ָ���仯 - NDVI (-0.30 ~ +0.30)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_DVI:		// ֲ��ָ���仯 - DVI (-0.10 ~ +0.10)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_RVI:		// ֲ��ָ���仯 - RVI (-0.40 ~ +0.40)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_SAVI:		// ֲ��ָ���仯 - SAVI (-0.25 ~ +0.25)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_EVI:		// ֲ��ָ���仯 - NDVI (-0.30 ~ +0.30)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_VIX_LAI:		// ֲ��ָ���仯 - LAI (-2.5 ~ +2.5)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_DIX_TVDI:		// �ɺ�ָ���仯 - TVDI (-0.80 ~ +0.80)
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
			pPalette->AddColor(AXIN_SPECIAL_DATA_CLOUD, 255,255, 255, _T("����"));
			pPalette->AddColor(AXIN_SPECIAL_DATA_WATER,  0,   0, 255, _T("ˮ��"));
			break;

		case AGMRS_PRODUCT_CODE_DIF_MISC_WATER:		// ˮ��仯
			pPalette->AddColor( 1, 255, 255,   0, _T("����"));
			pPalette->AddColor( 2, 255,   0,   0, _T("����"));
			pPalette->AddColor( 8, 255, 255, 255, _T("����"));
			pPalette->AddColor( 9,  0,   0,  255, _T("ˮ��"));
			break;

		//==================================
		case 0xF000:	// �ؽ��� 1-147 (����)
			pPalette->AddColor( 1, 127, 224,  53, _T("��  ��"));		// 1
			pPalette->AddColor( 2, 121,   5, 230, _T("��  ��"));		// 2
			pPalette->AddColor( 3, 237, 150,  19, _T("�  ��"));		// 3
			pPalette->AddColor( 4, 100, 245, 245, _T("��  ��"));		// 4
			pPalette->AddColor( 5, 245, 110, 195, _T("��  ��"));		// 5
			pPalette->AddColor( 6, 222,   4,   41, _T("����Ͽ"));		// 6
			pPalette->AddColor( 7,  56,  97,  29, _T("��  ��"));		// 7
			pPalette->AddColor( 8,  79,  96, 196, _T("֣  ��"));		// 8
			pPalette->AddColor( 9, 205, 252, 139, _T("��  ��"));		// 9
			pPalette->AddColor(10, 171, 137, 130, _T("��  ��"));		// 10
			pPalette->AddColor(11, 181, 250,  20, _T("ƽ��ɽ"));		// 11
			pPalette->AddColor(12,  82,  14,   4, _T("��  ��"));		// 12
			pPalette->AddColor(13,   6,  13, 115, _T("��  ��"));		// 13
			pPalette->AddColor(14,  50,  94,  94, _T("��  ��"));		// 14
			pPalette->AddColor(15, 219,  94,  92, _T("פ���"));		// 15
			pPalette->AddColor(16, 120,  85,  19, _T("��  ��"));		// 16
			pPalette->AddColor(17, 122,  31, 112, _T("��  ��"));		// 17
			pPalette->AddColor(18,  50, 207, 125, _T("��  Դ"));		// 18
			break;

		case 0xF001:	// ���и�� 1-18 (�ֶ�)
			pPalette->AddColor(30,   0,   128,   0, _T(""));
			break;

		case 0xF002:	// �ؽ��� 1-147 (�ֶ�)
			pPalette->AddColor(200,   0,   0,   255, _T(""));
			break;

		case 0xF003:	// ����� 1-2100 (�ֶ�)
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
