#include "StdAfx.h"
#include "KLDFile.h"


namespace Agmrs { namespace LDFile
{

	KLDFile::KLDFile(void)
	{
	}


	KLDFile::~KLDFile(void)
	{
	}

	// װ������
	BOOL KLDFile::LoadData_bin(LPCTSTR lpszFile)
	{
		BOOL bLoad = FALSE;

		BOOL bLd2File = KAgmrsData::IsLd2File(lpszFile);
		BOOL bLd3File = KAgmrsData::IsLd3File(lpszFile);

		if(bLd2File)
		{
			this->SetFlag(2);
			bLoad = LoadData_bin_ld2(lpszFile);
		}
		else if(bLd3File)
		{
			this->SetFlag(3);
			bLoad = LoadData_bin_ld3(lpszFile);
		}

		return bLoad;
	}
	BOOL KLDFile::LoadData_bin_ld2(LPCTSTR lpszFile)
	{
		if(!IsEmpty())
			Empty();

		// ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==0) return FALSE;

		// �����ļ�ͷ�ڴ�
		if(m_pFileHeader==NULL)
		{
			m_pFileHeader = new LD2Header;
		}
		memset(m_pFileHeader,0,sizeof(LD2Header));

		// ��ȡ�ļ�ͷ����
		if(fread(m_pFileHeader,sizeof(LD2Header),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		LD2Header* pFileHeader = (LD2Header*)m_pFileHeader;

		// �ж��ļ���־(�������ľֵ��ļ���ʾ = LA)
		if(!this->IsAgmrsData())
		{
			fclose(fp);
			fp = NULL;
			::MessageBox(NULL, _T("�ж����Ǳ�־����"), _T("��Ϣ"), MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}

		// ͶӰ����
		if(pFileHeader->wProjectType != 1)
		{
			fclose(fp);
			fp = NULL;
			::MessageBox(NULL, _T("Ŀǰֻ֧�ֵȾ�γ��ͶӰ����"), _T("��Ϣ"), MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}

		// ͨ������
		int iChannelNums = pFileHeader->wChannelNums;

		// ͨ�����ݿ�Ⱥ͸߶�
		int iWidth = pFileHeader->wWidth;
		int iHeight = pFileHeader->wHeight;

		long lSkipLength = pFileHeader->dwSkipLength;
		// ����ͷ�������ֽ���,׼����ȡ����
		fseek(fp,lSkipLength,SEEK_CUR);

		// ����ÿͨ�����ݳߴ�
		DWORD dwDataSizePerChannel = iWidth * iHeight;

		// ����ͨ�������ڴ�
		KWord2DMemAllocator::FreeMemory2D(m_ppwData);
		if(!KWord2DMemAllocator::AllocMemory2D(m_ppwData,iChannelNums,dwDataSizePerChannel))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ��ȡͨ������
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) pProgressParam->SetInfo(_T("��ȡͨ������..."));
		for(int i=0;i<iChannelNums;i++)
		{
			if(fread(m_ppwData[i],sizeof(WORD)*dwDataSizePerChannel,1,fp)<1)
			{
				fclose(fp);
				fp = NULL;
				::MessageBox(NULL, _T("��ȡ���ݴ���"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
				return FALSE;
			}

			// ��ʾ����
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iChannelNums;
					pProgressParam->lCurrentPos = i;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}
		}

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		// ���ü���󶨾���
		this->SetExtents(KExtents(pFileHeader->fLonMin,pFileHeader->fLatMin,pFileHeader->fLonMax,pFileHeader->fLatMax));

		// �������ݻҶ��ڴ�
		KByte2DMemAllocator::FreeMemory2D(m_ppucDataGray);
		if(!KByte2DMemAllocator::AllocMemory2D(m_ppucDataGray,iChannelNums,AGMRS_MODIS_DATA_VALUE_MAX+1))
			return FALSE;

		// ���������Ϣ
		this->FillDataInfo(m_pFileHeader);

		return KAgmrsData::LoadData_bin(lpszFile);
	}
	BOOL KLDFile::LoadData_bin_ld3(LPCTSTR lpszFile)
	{
		if(!IsEmpty())
			Empty();

		// ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return FALSE;

		// �����ļ�ͷ�ڴ�
		if(m_pFileHeader==NULL)
		{
			m_pFileHeader = new LD3Header;
		}
		memset(m_pFileHeader,0,sizeof(LD3Header));

		// ��ȡ�ļ�ͷ����
		if(fread(m_pFileHeader,sizeof(LD3Header),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		LD3Header* pFileHeader = (LD3Header*)m_pFileHeader;

		// �ж��ļ���־(�������ľֵ��ļ���ʾ = LA)
		if(!this->IsAgmrsData())
		{
			fclose(fp);
			fp = NULL;
			::MessageBox(NULL, _T("�ж����Ǳ�־����"), _T("��Ϣ"), MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}

		// ͶӰ����
		if(pFileHeader->wProjectType != 1)
		{
			fclose(fp);
			fp = NULL;
			::MessageBox(NULL, _T("Ŀǰֻ֧�ֵȾ�γ��ͶӰ����"), _T("��Ϣ"), MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}

		// ͨ������
		int iChannelNums = pFileHeader->wChannelNums;

		// ͨ�����ݿ�Ⱥ͸߶�
		int iWidth = pFileHeader->wWidth;
		int iHeight = pFileHeader->wHeight;

		long lSkipLength = pFileHeader->dwSkipLength;
		// ����ͷ�������ֽ���,׼����ȡ����
		fseek(fp,lSkipLength,SEEK_CUR);

		// ����ÿͨ�����ݳߴ�
		DWORD dwDataSizePerChannel = iWidth * iHeight;

		// ����ͨ�������ڴ�
		KWord2DMemAllocator::FreeMemory2D(m_ppwData);
		if(!KWord2DMemAllocator::AllocMemory2D(m_ppwData,iChannelNums,dwDataSizePerChannel))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// ��ȡͨ������
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) pProgressParam->SetInfo(_T("��ȡͨ������..."));
		for(int i=0;i<iChannelNums;i++)
		{
			if(fread(m_ppwData[i],sizeof(WORD)*dwDataSizePerChannel,1,fp)<1)
			{
				fclose(fp);
				fp = NULL;
				::MessageBox(NULL, _T("��ȡ���ݴ���"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
				return FALSE;
			}

			// ��ʾ����
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iChannelNums;
					pProgressParam->lCurrentPos = i;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}
		}

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		// ���ü���󶨾���
		this->SetExtents(KExtents(pFileHeader->fLonMin,pFileHeader->fLatMin,pFileHeader->fLonMax,pFileHeader->fLatMax));

		// �������ݻҶ��ڴ�
		KByte2DMemAllocator::FreeMemory2D(m_ppucDataGray);
		if(!KByte2DMemAllocator::AllocMemory2D(m_ppucDataGray,iChannelNums,AGMRS_MODIS_DATA_VALUE_MAX+1))
			return FALSE;

		// ���������Ϣ
		this->FillDataInfo(m_pFileHeader);

		return KAgmrsData::LoadData_bin(lpszFile);
	}

	// ���������Ϣ
	void KLDFile::FillDataInfo(LPVOID lpvData)
	{
		int i;
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)lpvData;

		KDataInfo* pDataInfo = this->GetDataInfo();
		if(!pDataInfo)
			return;

		// �����ļ���ʽ
		pDataInfo->FormatCode()		= this->GetFlag();				// LD2 = 2; LD3 = 3
		//---------------------------------------------------
		pDataInfo->FileID()			= pFileHeader->wFileID;
		pDataInfo->DataSource()		= pFileHeader->wSatelliteID;

		pDataInfo->OrbitNo()		= pFileHeader->wOrbitNo;		// �����
		pDataInfo->UpOrDown()		= pFileHeader->wUpOrDown;		// ��������, 1: ����, 0: ����

		pDataInfo->Year()			= pFileHeader->wYear;
		pDataInfo->Month()			= pFileHeader->wMonth;
		pDataInfo->Day()			= pFileHeader->wDay;
		pDataInfo->Hour()			= pFileHeader->wHour;
		pDataInfo->Minute()			= pFileHeader->wMinute;
		pDataInfo->Second()			= 0;
		pDataInfo->Millisecond()	= 0;

		pDataInfo->DayOrNight()		= pFileHeader->wDayOrNight;	// �����ҹ��ʶ, 0: ����, 1: ��ҹ
		pDataInfo->ChannelNumbers()	= pFileHeader->wChannelNums;
		pDataInfo->ProjectType()	= pFileHeader->wProjectType;

		pDataInfo->Width()			= pFileHeader->wWidth;
		pDataInfo->Height()			= pFileHeader->wHeight;

		pDataInfo->LonInterval()	= pFileHeader->fLonResolution;
		pDataInfo->LatInterval()	= pFileHeader->fLatResolution;

		pDataInfo->StandardLat1()	= pFileHeader->fStandardLat1;
		pDataInfo->StandardLat2()	= pFileHeader->fStandardLat2;

		pDataInfo->RadiusOfEarth()	= pFileHeader->fRadiusOfEarth;

		pDataInfo->MinLon()			= pFileHeader->fLonMin;
		pDataInfo->MaxLon()			= pFileHeader->fLonMax;

		pDataInfo->MinLat()			= pFileHeader->fLatMin;
		pDataInfo->MaxLat()			= pFileHeader->fLatMax;

		pDataInfo->StandardLon()	= pFileHeader->fStandardLon;

		pDataInfo->CenterLon()		= pFileHeader->fCenterLon;
		pDataInfo->CenterLat()		= pFileHeader->fCenterLat;
		//---------------------------------------------------
		LD2Header* pLD2Header = (LD2Header*)FileHeader();
		LD3Header* pLD3Header = (LD3Header*)FileHeader();

		int iChannelNums = pFileHeader->wChannelNums;
		int iFormatCode = pDataInfo->FormatCode();
		if(iFormatCode == 2)
		{
			// ͨ������
			for(i=0;i<iChannelNums;i++)
			{
				pDataInfo->ChannelCode(i) = (int)pLD2Header->byChannelIndex[i];
			}

			// ��Ʒ�汾
			pDataInfo->Version() = pLD2Header->wVersion;	// 2002
		}
		else if(iFormatCode == 3)
		{
			// ͨ������
			for(i=0;i<iChannelNums;i++)
			{
				pDataInfo->ChannelCode(i) = (int)pLD3Header->wChIndex[i];
			}

			// ��Ʒ�汾
			pDataInfo->Version() = pLD3Header->wVersion;	// 2006

			// ����ͨ������
			int iChannelNumsOfAncillary = pLD3Header->wChannelNumsOfAncillary;
			pDataInfo->AncillaryChannelNums() = iChannelNumsOfAncillary;
			// ����ͨ������
			for(i=0;i<iChannelNumsOfAncillary;i++)
			{
				pDataInfo->AncillaryChannelCode(i) = pLD3Header->wAncillaryChIndex[i];
			}
		}
		else
		{
		}

		//WORD	wChannelNumsOfAncillary;	// ����ͨ���� 
		//WORD	wAncillaryChIndex[100];		// ����ͨ������

		//WORD	wComposeDays;				// �Ѻϳ�����
		//WORD	wVersion;					// �汾��
		//UCHAR	NDVIOffset;					// ??
		//bool	bCloudMask;					// WORD	wBytes;				// ���ṹ�ֽ���
		//DWORD	dwSkipLength;				// �ֵ��ļ�ͷ��¼�ĺ�������ֶγ���

		//UCHAR	ucNoaach3Flag;				// 0-3A  1-3B
		//UCHAR	ucFlag3[1807];				// 
		//---------------------------------------------------
		return;
	}

	// ͨ��ӳ��
	// ӳ��ͨ�� [ index->chCode->name ]
	//          [ �ڴ�����ͨ��(Base0) -> ����ͨ��(Base1) + ͨ������
	void KLDFile::MapChannel()
	{
		int index,iChannelCode;

		AgmrsDataChannelMap* pAgmrsDataChannelMap = this->GetAgmrsDataChannelMap();
		pAgmrsDataChannelMap->clear();

		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNumbers = pDataInfo->ChannelNumbers();

		for(index=0;index<iChannelNumbers;index++)
		{
			// ͨ������
			iChannelCode = pDataInfo->ChannelCode(index);

			// û��ͨ������
			if(iChannelCode<=0)
				continue;

			// ����ӳ��
			AgmrsDataChannel oAgmrsDataChannel;
			oAgmrsDataChannel.bExist = TRUE;
			oAgmrsDataChannel.iDataIndex = index;	// ��������
			_stprintf(oAgmrsDataChannel.szName,ChannelCode2String(iChannelCode));

			pAgmrsDataChannelMap->insert(AgmrsDataChannelMap::value_type(index,oAgmrsDataChannel));
		}

		return;
	}

	// ����Ĭ�ϵ���ʾͨ��
	void KLDFile::SetDefaultDisplayChannel()
	{
		// ������Ϣ
		KDataInfo* pDataInfo = this->GetDataInfo();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ����ʱ��ɱ���ʱ
		int flag = this->GetFlag();
		int addHour = flag == 3 ? 8 : 8;	// 3-LD3
		int iHour = pDataInfo->Hour() + addHour;
		if(iHour>=24)
			iHour -= 24;

		// �ж������ϻ��ǰ���
		if(iHour>=6 && iHour<=18)
		{// ����
			pDataInfo->DayOrNight() = 0;	//�����ҹ��ʶ, 0: ����, 1: ��ҹ
			((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight = 0;
		}
		else
		{
			pDataInfo->DayOrNight() = 1;	//�����ҹ��ʶ, 0: ����, 1: ��ҹ
			((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight = 1;
		}

		//-------------------------------------------------
		// �ڲ�ͬ�����������������ö�Ӧ��Ĭ����ʾͨ��
		// ...
		//-------------------------------------------------

		return;
	}

	// ȡ��ͨ�������Ӧ���������� (1-38 ת��Ϊ 0-37)
	// ͨ����ͨ�����롢��������
	// ����MODIS��˵������ͨ����ͨ�����롢����������1-36��1-38��0-37
	// ����ͨ����Channel�����ش�����������ͨ����MODIS������ͨ��Ϊ1-36��ʵ��ͨ��Ϊ1-38���ֱ𱣴���2D�ڴ��0-37ƫ�Ƶ�ַ
	// ͨ�����룺ChannelCode���Ǿֵ��ļ���LA���ļ�ͷ�ṹ�б���Ĵ��룬-1��ʾ��ͨ��������
	// ����������DataIndex����������2D�ڴ��е�����
	// ���ھֵ��ļ���˵����һ������ȫ��ͨ�������ݣ�����MODIS���ݣ�����ֻ����1,2,3,19,21,22,31,32��8��ͨ�������ݡ�
	int KLDFile::GetDataIndex(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		// Ĭ������ͨ��=0
		int iDataIndex = -1;
		int iChannelNumbers = pDataInfo->ChannelNumbers();

		for(int index=0; index<iChannelNumbers; index++)
		{
			if(iChannelCode == pDataInfo->ChannelCode(index))
			{
				iDataIndex = index;
				break;
			}
		}

		return iDataIndex;
	}

	// ͨ������ת��Ϊͨ������(1-38)
	int KLDFile::ChannelName2ChannelCode(LPCTSTR lpszChannelName)
	{
		int iChannelCode = -1;
		int iDataIndex = -1;

		KDataInfo* pDataInfo = this->GetDataInfo();

		// ͨ��ӳ��
		//typedef map<int,AgmrsDataChannel> AgmrsDataChannelMap
		AgmrsDataChannelMap* pAgmrsDataChannelMap = this->GetAgmrsDataChannelMap();

		// �� AgmrsDataChannelMap �õ� ����ָ������
		AgmrsDataChannelMap::iterator itr;
		AgmrsDataChannelMap::iterator ibeg = pAgmrsDataChannelMap->begin();
		AgmrsDataChannelMap::iterator iend = pAgmrsDataChannelMap->end();
		for(itr=ibeg; itr!=iend; itr++)
		{
			if(_tcscmp(lpszChannelName, (*itr).second.szName) == 0)
			{
				//iDataIndex = (*itr).second.iDataIndex;
				iDataIndex = (*itr).first;
				break;
			}
		}

		// ͨ������
		iChannelCode = pDataInfo->ChannelCode(iDataIndex);
		return iChannelCode > 0 ? iChannelCode : -1;
	}

}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
