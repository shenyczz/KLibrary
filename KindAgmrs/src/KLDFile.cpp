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

	// 装载数据
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

		// 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==0) return FALSE;

		// 分配文件头内存
		if(m_pFileHeader==NULL)
		{
			m_pFileHeader = new LD2Header;
		}
		memset(m_pFileHeader,0,sizeof(LD2Header));

		// 读取文件头内容
		if(fread(m_pFileHeader,sizeof(LD2Header),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		LD2Header* pFileHeader = (LD2Header*)m_pFileHeader;

		// 判断文件标志(卫星中心局地文件标示 = LA)
		if(!this->IsAgmrsData())
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
		fseek(fp,lSkipLength,SEEK_CUR);

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
		if(!KByte2DMemAllocator::AllocMemory2D(m_ppucDataGray,iChannelNums,AGMRS_MODIS_DATA_VALUE_MAX+1))
			return FALSE;

		// 填充数据信息
		this->FillDataInfo(m_pFileHeader);

		return KAgmrsData::LoadData_bin(lpszFile);
	}
	BOOL KLDFile::LoadData_bin_ld3(LPCTSTR lpszFile)
	{
		if(!IsEmpty())
			Empty();

		// 打开文件
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return FALSE;

		// 分配文件头内存
		if(m_pFileHeader==NULL)
		{
			m_pFileHeader = new LD3Header;
		}
		memset(m_pFileHeader,0,sizeof(LD3Header));

		// 读取文件头内容
		if(fread(m_pFileHeader,sizeof(LD3Header),1,fp)<1)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		LD3Header* pFileHeader = (LD3Header*)m_pFileHeader;

		// 判断文件标志(卫星中心局地文件标示 = LA)
		if(!this->IsAgmrsData())
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
		fseek(fp,lSkipLength,SEEK_CUR);

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
		if(!KByte2DMemAllocator::AllocMemory2D(m_ppucDataGray,iChannelNums,AGMRS_MODIS_DATA_VALUE_MAX+1))
			return FALSE;

		// 填充数据信息
		this->FillDataInfo(m_pFileHeader);

		return KAgmrsData::LoadData_bin(lpszFile);
	}

	// 填充数据信息
	void KLDFile::FillDataInfo(LPVOID lpvData)
	{
		int i;
		LDFHeaderCommon* pFileHeader = (LDFHeaderCommon*)lpvData;

		KDataInfo* pDataInfo = this->GetDataInfo();
		if(!pDataInfo)
			return;

		// 设置文件格式
		pDataInfo->FormatCode()		= this->GetFlag();				// LD2 = 2; LD3 = 3
		//---------------------------------------------------
		pDataInfo->FileID()			= pFileHeader->wFileID;
		pDataInfo->DataSource()		= pFileHeader->wSatelliteID;

		pDataInfo->OrbitNo()		= pFileHeader->wOrbitNo;		// 轨道号
		pDataInfo->UpOrDown()		= pFileHeader->wUpOrDown;		// 升降轨标记, 1: 升轨, 0: 降轨

		pDataInfo->Year()			= pFileHeader->wYear;
		pDataInfo->Month()			= pFileHeader->wMonth;
		pDataInfo->Day()			= pFileHeader->wDay;
		pDataInfo->Hour()			= pFileHeader->wHour;
		pDataInfo->Minute()			= pFileHeader->wMinute;
		pDataInfo->Second()			= 0;
		pDataInfo->Millisecond()	= 0;

		pDataInfo->DayOrNight()		= pFileHeader->wDayOrNight;	// 白天黑夜标识, 0: 白天, 1: 黑夜
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
			// 通道代码
			for(i=0;i<iChannelNums;i++)
			{
				pDataInfo->ChannelCode(i) = (int)pLD2Header->byChannelIndex[i];
			}

			// 产品版本
			pDataInfo->Version() = pLD2Header->wVersion;	// 2002
		}
		else if(iFormatCode == 3)
		{
			// 通道代码
			for(i=0;i<iChannelNums;i++)
			{
				pDataInfo->ChannelCode(i) = (int)pLD3Header->wChIndex[i];
			}

			// 产品版本
			pDataInfo->Version() = pLD3Header->wVersion;	// 2006

			// 辅助通道数量
			int iChannelNumsOfAncillary = pLD3Header->wChannelNumsOfAncillary;
			pDataInfo->AncillaryChannelNums() = iChannelNumsOfAncillary;
			// 辅助通道代码
			for(i=0;i<iChannelNumsOfAncillary;i++)
			{
				pDataInfo->AncillaryChannelCode(i) = pLD3Header->wAncillaryChIndex[i];
			}
		}
		else
		{
		}

		//WORD	wChannelNumsOfAncillary;	// 辅助通道数 
		//WORD	wAncillaryChIndex[100];		// 辅助通道索引

		//WORD	wComposeDays;				// 已合成天数
		//WORD	wVersion;					// 版本号
		//UCHAR	NDVIOffset;					// ??
		//bool	bCloudMask;					// WORD	wBytes;				// 本结构字节数
		//DWORD	dwSkipLength;				// 局地文件头记录的后面填充字段长度

		//UCHAR	ucNoaach3Flag;				// 0-3A  1-3B
		//UCHAR	ucFlag3[1807];				// 
		//---------------------------------------------------
		return;
	}

	// 通道映射
	// 映射通道 [ index->chCode->name ]
	//          [ 内存数据通道(Base0) -> 数据通道(Base1) + 通道名称
	void KLDFile::MapChannel()
	{
		int index,iChannelCode;

		AgmrsDataChannelMap* pAgmrsDataChannelMap = this->GetAgmrsDataChannelMap();
		pAgmrsDataChannelMap->clear();

		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNumbers = pDataInfo->ChannelNumbers();

		for(index=0;index<iChannelNumbers;index++)
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

	// 设置默认的显示通道
	void KLDFile::SetDefaultDisplayChannel()
	{
		// 数据信息
		KDataInfo* pDataInfo = this->GetDataInfo();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 世界时变成北京时
		int flag = this->GetFlag();
		int addHour = flag == 3 ? 8 : 8;	// 3-LD3
		int iHour = pDataInfo->Hour() + addHour;
		if(iHour>=24)
			iHour -= 24;

		// 判断是晚上还是白天
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

		//-------------------------------------------------
		// 在不同传感器数据类中设置对应的默认显示通道
		// ...
		//-------------------------------------------------

		return;
	}

	// 取得通道代码对应的数据索引 (1-38 转换为 0-37)
	// 通道、通道代码、数据索引
	// 对于MODIS来说，数据通道、通道代码、数据索引是1-36、1-38、0-37
	// 数据通道：Channel，星载传感器的数据通道。MODIS的数据通道为1-36，实际通道为1-38，分别保存在2D内存的0-37偏移地址
	// 通道代码：ChannelCode，是局地文件（LA）文件头结构中保存的代码，-1表示该通道不存在
	// 数据索引：DataIndex，是数据在2D内存中的索引
	// 对于局地文件来说，不一定保存全部通道的数据，比如MODIS数据，可以只保存1,2,3,19,21,22,31,32这8个通道的数据。
	int KLDFile::GetDataIndex(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();

		// 默认数据通道=0
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

	// 通道名称转换为通道代码(1-38)
	int KLDFile::ChannelName2ChannelCode(LPCTSTR lpszChannelName)
	{
		int iChannelCode = -1;
		int iDataIndex = -1;

		KDataInfo* pDataInfo = this->GetDataInfo();

		// 通道映射
		//typedef map<int,AgmrsDataChannel> AgmrsDataChannelMap
		AgmrsDataChannelMap* pAgmrsDataChannelMap = this->GetAgmrsDataChannelMap();

		// 从 AgmrsDataChannelMap 得到 数据指针索引
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

		// 通道代码
		iChannelCode = pDataInfo->ChannelCode(iDataIndex);
		return iChannelCode > 0 ? iChannelCode : -1;
	}

}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
