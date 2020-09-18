#include "StdAfx.h"
#include "KAvhrrData.h"
#include "KAvhrrDataProcessor.h"

namespace Agmrs { namespace Avhrr
{

	KAvhrrData::KAvhrrData(void)
	{
		this->SetType(AgmrsDataSource::Avhrr);

		m_pDataProcessor = new KAvhrrDataProcessor();
		m_pDataProcessor->SetOwner(this);
	}

	KAvhrrData::~KAvhrrData(void)
	{
	}

	// װ������
	BOOL KAvhrrData::LoadData_bin(LPCTSTR lpszFile)
	{
		BOOL bLoad = FALSE;

		BOOL bLd2File = KAvhrrData::IsLd2File(lpszFile);
		BOOL bLd3File = KAvhrrData::IsLd3File(lpszFile);

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
	BOOL KAvhrrData::LoadData_bin_ld2(LPCTSTR lpszFile)
	{
		return FALSE;
	}
	BOOL KAvhrrData::LoadData_bin_ld3(LPCTSTR lpszFile)
	{
		if(!IsEmpty())
			Empty();

		// ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==0) return FALSE;

		// �����ļ�ͷ�ڴ�
		if(m_pFileHeader==NULL)
		{
			m_pFileHeader = new LD3Header;
		}
		memset(m_pFileHeader,0,sizeof(LD3Header));	// ����3a,����3b

		// ��ȡ�ļ�ͷ����
		if(fread(m_pFileHeader,sizeof(LD3Header),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		LD3Header* pFileHeader = (LD3Header*)m_pFileHeader;

		// FY1D ��
		if(pFileHeader->wSatelliteID == AGMRS_SATELLITE_FY1D_AVHRR && pFileHeader->wYear<1900)
		{
			pFileHeader->wYear += 2000;
		}

		// �ж��ļ���־(�������ľֵ��ļ���ʾ = LA)
		if(!this->IsAvhrrData())
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
		//fseek(fp,lSkipLength,SEEK_CUR);

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
		if(!KByte2DMemAllocator::AllocMemory2D(m_ppucDataGray,iChannelNums,AGMRS_AVHRR_DATA_VALUE_MAX + 1))
			return FALSE;

		this->FillDataInfo(m_pFileHeader);

		return KAgmrsData::LoadData_bin(lpszFile);
	}

	/*

	// ӳ��ͨ��(ͨ���ź�����������Ӧ)
	void KAvhrrData::MapChannel()
	{
		int index,iChannelCode;

		AgmrsDataChannelMap* pAgmrsDataChannelMap = this->GetAgmrsDataChannelMap();
		pAgmrsDataChannelMap->clear();

		KDataInfo* pDataInfo = this->GetDataInfo();
		for(index=0;index<AGMRS_AVHRR_DATA_CHANNEL_NUM;index++)
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
	*/

	/*
	// ����ͨ����������ͨ����������
	int KAvhrrData::GetDataIndex(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		// Ĭ������ͨ�� = -1
		int iDataIndex = -1;

		for(int index=0; index<AGMRS_VIRR_DATA_CHANNEL_NUMBERS; index++)
		{
			if(iChannelCode == pDataInfo->ChannelCode(index))
			{
				iDataIndex = index;
				break;
			}
		}

		return iDataIndex;
	}
	*/

	// ����Ĭ�ϵ���ʾͨ��
	void KAvhrrData::SetDefaultDisplayChannel()
	{
		// ������Ϣ
		KDataInfo* pDataInfo = GetDataInfo();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// �ж������ϻ��ǰ���
		int flag = this->GetFlag();
		int addHour = flag == 3 ? 8 : 8;
		int iHour = pDataInfo->Hour() + addHour;
		if(iHour>=24)
			iHour -= 24;

		// ��6������6��Ϊ����
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

		if(IsDay())
		{// ����
			if(iChannelNums==1)
			{
				int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
				SetDataChannelDef(ch,ch,ch);
			}
			else if(iChannelNums==2)
			{
				int ch1 = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
				int ch2 = this->ChannelCode2Channel(pDataInfo->ChannelCode(1));
				SetDataChannelDef(ch1,ch2,ch1);
			}
			else
			{
				SetDataChannelDef(1,2,1);
			}
		}
		else
		{// ����
			if(iChannelNums==1)
			{
				// ��ͨ��
				int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
				SetDataChannelDef(ch,ch,ch);
			}
			else if(iChannelNums==2)
			{
				int ch1 = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
				int ch2 = this->ChannelCode2Channel(pDataInfo->ChannelCode(1));
				SetDataChannelDef(ch1,ch2,ch1);
			}
			else if(iChannelNums==5)
			{
				int chr = this->ChannelCode2Channel(pDataInfo->ChannelCode(2));
				int chg = this->ChannelCode2Channel(pDataInfo->ChannelCode(3));
				int chb = this->ChannelCode2Channel(pDataInfo->ChannelCode(4));
				SetDataChannelDef(chr,chg,chb);
			}
			else
			{
				// ��ͨ��
				int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
				SetDataChannelDef(ch,ch,ch);
			}
		}

		return;
	}

	// ͨ����ת��Ϊͨ������
	int KAvhrrData::Channel2ChannelCode(int iChannel)
	{
		int temp = 1000;
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)m_pFileHeader;
		if(pFileHeader->wSatelliteID == AGMRS_SATELLITE_FY1D_AVHRR)
			temp = 2000;

		return iChannel + temp;
	}

	// ͨ������ת��Ϊͨ����
	int KAvhrrData::ChannelCode2Channel(int iChannelCode)
	{
		int temp = 1000;
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)m_pFileHeader;
		if(pFileHeader->wSatelliteID == AGMRS_SATELLITE_FY1D_AVHRR)
			temp = 2000;

		return iChannelCode - temp;
	}

	// ͨ��ת��Ϊ�ַ���
	LPCTSTR KAvhrrData::ChannelCode2String(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNums = pDataInfo->ChannelNumbers();
		_stprintf(m_szChannel,_T("%2d ͨ��"),this->ChannelCode2Channel(iChannelCode));
		return m_szChannel;
	}

}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------
