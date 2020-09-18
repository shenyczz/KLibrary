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

	// 装载数据
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

		// 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==0) return FALSE;

		// 分配文件头内存
		if(m_pFileHeader==NULL)
		{
			m_pFileHeader = new LD3Header;
		}
		memset(m_pFileHeader,0,sizeof(LD3Header));	// 晚上3a,白天3b

		// 读取文件头内容
		if(fread(m_pFileHeader,sizeof(LD3Header),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		LD3Header* pFileHeader = (LD3Header*)m_pFileHeader;

		// FY1D 年
		if(pFileHeader->wSatelliteID == AGMRS_SATELLITE_FY1D_AVHRR && pFileHeader->wYear<1900)
		{
			pFileHeader->wYear += 2000;
		}

		// 判断文件标志(卫星中心局地文件标示 = LA)
		if(!this->IsAvhrrData())
		{
			fclose(fp);
			fp = NULL;
			::MessageBox(NULL, _T("判断卫星标志错误"), _T("信息"), MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}

		// 投影类型
		if(pFileHeader->wProjectType != 1)
		{
			fclose(fp);
			fp = NULL;
			::MessageBox(NULL, _T("目前只支持等经纬度投影类型"), _T("信息"), MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}

		// 通道数量
		int iChannelNums = pFileHeader->wChannelNums;

		// 通道数据宽度和高度
		int iWidth = pFileHeader->wWidth;
		int iHeight = pFileHeader->wHeight;

		long lSkipLength = pFileHeader->dwSkipLength;
		// 跳过头后填充的字节数,准备读取数据
		//fseek(fp,lSkipLength,SEEK_CUR);

		// 计算每通道数据尺寸
		DWORD dwDataSizePerChannel = iWidth * iHeight;

		// 分配通道数据内存
		KWord2DMemAllocator::FreeMemory2D(m_ppwData);
		if(!KWord2DMemAllocator::AllocMemory2D(m_ppwData,iChannelNums,dwDataSizePerChannel))
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 读取通道数据
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) pProgressParam->SetInfo(_T("读取通道数据..."));
		for(int i=0;i<iChannelNums;i++)
		{
			if(fread(m_ppwData[i],sizeof(WORD)*dwDataSizePerChannel,1,fp)<1)
			{
				fclose(fp);
				fp = NULL;
				::MessageBox(NULL, _T("读取数据错误"), _T("信息"), MB_OK | MB_ICONWARNING);
				return FALSE;
			}

			// 显示进度
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

		// 关闭文件
		fclose(fp);
		fp = NULL;

		// 设置计算绑定矩形
		this->SetExtents(KExtents(pFileHeader->fLonMin,pFileHeader->fLatMin,pFileHeader->fLonMax,pFileHeader->fLatMax));

		// 分配数据灰度内存
		KByte2DMemAllocator::FreeMemory2D(m_ppucDataGray);
		if(!KByte2DMemAllocator::AllocMemory2D(m_ppucDataGray,iChannelNums,AGMRS_AVHRR_DATA_VALUE_MAX + 1))
			return FALSE;

		this->FillDataInfo(m_pFileHeader);

		return KAgmrsData::LoadData_bin(lpszFile);
	}

	/*

	// 映射通道(通道号和数据索引对应)
	void KAvhrrData::MapChannel()
	{
		int index,iChannelCode;

		AgmrsDataChannelMap* pAgmrsDataChannelMap = this->GetAgmrsDataChannelMap();
		pAgmrsDataChannelMap->clear();

		KDataInfo* pDataInfo = this->GetDataInfo();
		for(index=0;index<AGMRS_AVHRR_DATA_CHANNEL_NUM;index++)
		{
			// 通道代码
			iChannelCode = pDataInfo->ChannelCode(index);

			// 没有通道数据
			if(iChannelCode<=0)
				continue;

			// 增加映射
			AgmrsDataChannel oAgmrsDataChannel;
			oAgmrsDataChannel.bExist = TRUE;
			oAgmrsDataChannel.iDataIndex = index;	// 数据索引
			_stprintf(oAgmrsDataChannel.szName,ChannelCode2String(iChannelCode));
			pAgmrsDataChannelMap->insert(AgmrsDataChannelMap::value_type(index,oAgmrsDataChannel));
		}

		return;
	}
	*/

	/*
	// 根据通道代码区的通道数据索引
	int KAvhrrData::GetDataIndex(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		// 默认数据通道 = -1
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

	// 设置默认的显示通道
	void KAvhrrData::SetDefaultDisplayChannel()
	{
		// 数据信息
		KDataInfo* pDataInfo = GetDataInfo();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 判断是晚上还是白天
		int flag = this->GetFlag();
		int addHour = flag == 3 ? 8 : 8;
		int iHour = pDataInfo->Hour() + addHour;
		if(iHour>=24)
			iHour -= 24;

		// 早6点至晚6点为白天
		if(iHour>=6 && iHour<=18)
		{// 白天
			pDataInfo->DayOrNight() = 0;	//白天黑夜标识, 0: 白天, 1: 黑夜
			((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight = 0;
		}
		else
		{
			pDataInfo->DayOrNight() = 1;	//白天黑夜标识, 0: 白天, 1: 黑夜
			((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight = 1;
		}

		if(IsDay())
		{// 白天
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
		{// 晚上
			if(iChannelNums==1)
			{
				// 单通道
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
				// 单通道
				int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
				SetDataChannelDef(ch,ch,ch);
			}
		}

		return;
	}

	// 通道号转换为通道代码
	int KAvhrrData::Channel2ChannelCode(int iChannel)
	{
		int temp = 1000;
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)m_pFileHeader;
		if(pFileHeader->wSatelliteID == AGMRS_SATELLITE_FY1D_AVHRR)
			temp = 2000;

		return iChannel + temp;
	}

	// 通道代码转换为通道号
	int KAvhrrData::ChannelCode2Channel(int iChannelCode)
	{
		int temp = 1000;
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)m_pFileHeader;
		if(pFileHeader->wSatelliteID == AGMRS_SATELLITE_FY1D_AVHRR)
			temp = 2000;

		return iChannelCode - temp;
	}

	// 通道转换为字符串
	LPCTSTR KAvhrrData::ChannelCode2String(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNums = pDataInfo->ChannelNumbers();
		_stprintf(m_szChannel,_T("%2d 通道"),this->ChannelCode2Channel(iChannelCode));
		return m_szChannel;
	}

}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------
