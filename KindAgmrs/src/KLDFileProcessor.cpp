#include "StdAfx.h"

#include "KLDFileProcessor.h"

#include "KLDFile.h"
#include "KLDAlgorithm.h"

namespace Agmrs { namespace LDFile
{

	KLDFileProcessor::KLDFileProcessor(void)
	{
	}


	KLDFileProcessor::~KLDFileProcessor(void)
	{
	}

	// 数据转换为 DIB
	BOOL KLDFileProcessor::DataToDIB(int iRChannel, int iGChannel , int iBChannel)
	{
		int i,j;

		// 取得数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		if(!pAgmrsData)
			return FALSE;

		// 数据空
		if(pAgmrsData->IsEmpty())
			return FALSE;

		// 数据信息
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();
		if(!pDataInfo)
			return FALSE;

		// 数据转换为灰度
		this->Data2Gray();

		// 设置融合通道值
		int chr,chg,chb;
		pAgmrsData->GetDataChannelDef(&chr,&chg,&chb);
		if(iRChannel==0 && iGChannel==0 && iBChannel==0)
		{
			pAgmrsData->SetDataChannel(chr,chg,chb);
		}
		else
		{
			pAgmrsData->SetDataChannel(iRChannel,iGChannel,iBChannel);
		}

		// 选择数据通道
		pAgmrsData->GetDataChannel(&chr,&chg,&chb);

		// 通道号转换为通道代码
		int chrCode = pAgmrsData->Channel2ChannelCode(chr);
		int chgCode = pAgmrsData->Channel2ChannelCode(chg);
		int chbCode = pAgmrsData->Channel2ChannelCode(chb);

		// 通道通道代码转换为数据索引
		int rDataIndex = pAgmrsData->GetDataIndex(chrCode);
		int gDataIndex = pAgmrsData->GetDataIndex(chgCode);
		int bDataIndex = pAgmrsData->GetDataIndex(chbCode);

		// 通道缩引 = [0,19]
		if(rDataIndex<0 || gDataIndex<0 || bDataIndex<0)
			return FALSE;

		//数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 24位 DIB 每行的字节
		DWORD dwBytesPerLine = DIBWIDTHBYTES(iWidth * 8 * 3);

		// 数据尺寸(24位)
		DWORD dwSizeImage = dwBytesPerLine * iHeight;

		// 整个DIB 大小(无调色板数据)
		DWORD dwDIBSize = sizeof(BITMAPINFOHEADER) + 0 + dwSizeImage;

		// 一次分配 DIB 内存
		PBYTE pDib = new BYTE[dwDIBSize];
		memset(pDib, 0, sizeof(BYTE) * dwDIBSize);

		// 设置 BITMAPINFOHRADER
		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)pDib;
		pBitmapInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth			= iWidth;
		pBitmapInfo->bmiHeader.biHeight			= iHeight;
		pBitmapInfo->bmiHeader.biPlanes			= 1;
		pBitmapInfo->bmiHeader.biBitCount		= 24;
		pBitmapInfo->bmiHeader.biCompression	= BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	//用BI_RGB方式时设可为0
		pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biClrUsed		= 0;
		pBitmapInfo->bmiHeader.biClrImportant	= 0;

		// DIB 数据指针
		PBYTE pDIBits = pDib + sizeof(BITMAPINFOHEADER) + 0 ;

		// 数据和灰度
		PWORD* ppwData = pAgmrsData->GetData();
		PBYTE* ppucDataGray = pAgmrsData->GetDataGray();

		// 通道数据转换为DIB
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) { pProgressParam->SetInfo(_T("通道数据转换为DIB...")); }
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				WORD wRData = ppwData[rDataIndex][i*iWidth+j];
				WORD wGData = ppwData[gDataIndex][i*iWidth+j];
				WORD wBData = ppwData[bDataIndex][i*iWidth+j];

				BYTE byRGray = ppucDataGray[rDataIndex][wRData];
				BYTE byGGray = ppucDataGray[gDataIndex][wGData];
				BYTE byBGray = ppucDataGray[bDataIndex][wBData];

				//升降轨标记, 1: 升轨, 0: 降轨
				int ii = (iHeight-1-i);

				*(pDIBits + ii*dwBytesPerLine+j*3 + 2) = byRGray;	// red
				*(pDIBits + ii*dwBytesPerLine+j*3 + 1) = byGGray;	// green
				*(pDIBits + ii*dwBytesPerLine+j*3 + 0) = byBGray;	// blue

			}//for(j)

			// 显示进度
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iHeight;
					pProgressParam->lCurrentPos = i;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}

		}//for(i)

		// 关联到 KDib 类
		if(!m_pDib_org)
		{
			m_pDib_org = new KDib();
		}
		m_pDib_org->AttachMemory(pDib,TRUE);	// TRUE - 由 KDib 释放内存

		_delete(m_pDib);
		m_pDib = m_pDib_org->Clone();

		this->IsDibChanged() = TRUE;

		//m_pImage = Bitmap::FromBITMAPINFO(pBitmapInfo,pDIBits);
		//if(Status::Ok != m_pImage->GetLastStatus())
		//{
		//	_delete(m_pImage);
		//	return FALSE;
		//}

		return TRUE;
	}

	// 数据转换为灰度
	void KLDFileProcessor::Data2Gray()
	{
		// 计算通道数据转换为灰度数据的参数
		CalcData2GrayParam();

		// 取得数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// 通道数
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 通道数据转换为灰度值
		for(int ch=0;ch<iChannelNums;ch++)
		{
			ChannelData2Gray(&m_pData2GrayParam[ch]);
		}

		return;
	}

	// 计算通道数据转换为灰度数据的参数
	void KLDFileProcessor::CalcData2GrayParam()
	{
		int i,ch,value;

		// 取得数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		int iAvailPoint = 0;		// 有效点数
		WORD wMaxValue = 0;			// 最大值
		WORD wMinValue = 0xFFFF;	// 最小值

		WORD wUpThreshold = AGMRS_MERSI_DATA_VALUE_MAX;	// 通道数据上阈值
		WORD wDnThreshold = 0;							// 通道数据下阈值

		// 通道数
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 单个通道数据字节数
		int iDataSizePerChannel = pDataInfo->Width() * pDataInfo->Height();

		// 通道值密度
		int* iValueIntensity = new int[AGMRS_MERSI_DATA_VALUE_MAX+10];

		// 存储累计概率
		float* fProbability = new float[AGMRS_MERSI_DATA_VALUE_MAX+10];

		// 初始化通道数据转换为灰度数据的参数
		memset(m_pData2GrayParam, 0, sizeof(Data2GrayParam) * AGMRS_MERSI_DATA_CHANNEL_NUMBERS);

		// 通道数据指针
		PWORD* ppwData = pAgmrsData->GetData();

		// 计算各通道参数
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) pProgressParam->SetInfo(_T("通道数据转换为灰度值..."));
		for(ch=0; ch<iChannelNums; ch++)
		{
			memset(iValueIntensity,0,sizeof(int)*(AGMRS_MERSI_DATA_VALUE_MAX+10) );
			memset(fProbability,0,sizeof(float)*(AGMRS_MERSI_DATA_VALUE_MAX+10) );

			wMaxValue = 0;
			wMinValue = 0xFFFF;

			for(i=0;i<iDataSizePerChannel;i++)
			{
				ppwData[ch][i] &= AGMRS_MERSI_DATA_VALUE_MAX;	// 0x0FFF
				WORD wData = ppwData[ch][i];
				if(wData < AGMRS_MERSI_DATA_VALUE_MAX)			//0x0fff = 4095 有效值
				{
					// 记录最大值和最小值
					wMaxValue = wData>wMaxValue ? wData : wMaxValue;
					wMinValue = wData<wMinValue ? wData : wMinValue;

					// 对应通道值计数器 +1
					iValueIntensity[wData]++;	// 通道值密度
				}

			}// next i

			// 有效数据点数量
			iAvailPoint = iDataSizePerChannel - iValueIntensity[0];	// 减去值为0的点

			//计算累计概率
			fProbability[0] = 0.0f;
			fProbability[1] = (float)iValueIntensity[1] / iAvailPoint;	// 次数除以总有效点数
			for(value=2; value<=AGMRS_MERSI_DATA_VALUE_MAX; value++)
			{
				fProbability[value] = fProbability[value-1] + (float)iValueIntensity[value] / iAvailPoint;
			}

			// 过滤掉数量少、非常小的值，得到通道数据下阈值
			for(value=0; value<AGMRS_MERSI_DATA_VALUE_MAX; value++)
			{
				if(fProbability[value] > 0.012)
				{
					wDnThreshold = value; // 通道数据下阈值
					break;
				}
			}

			// 过滤掉数量少、值很大的值，得到通道数据上阈值
			for(value=AGMRS_MERSI_DATA_VALUE_MAX; value>=0; value--)
			{
				if(fProbability[value] < 0.92)
				{
					wUpThreshold = value; // 通道数据上阈值
					break;
				}
			}

			// 通道数据转换为灰度数据的参数
			m_pData2GrayParam[ch].iChannel			= ch;			// 通道号
			m_pData2GrayParam[ch].iEnhanceType		= 2;			// 增强方式(0-直线，1-指数，2-对数 ...)
			m_pData2GrayParam[ch].wMinValue			= wMinValue;	// 最小值
			m_pData2GrayParam[ch].wMaxValue			= wMaxValue;	// 最大值
			m_pData2GrayParam[ch].wThreshold_min	= wDnThreshold;	// 去掉一些小值后的极小值(通道数据下阈值)
			m_pData2GrayParam[ch].wThreshold_max	= wUpThreshold;	// 去掉一些大值后的极大值(通道数据上阈值)
			m_pData2GrayParam[ch].cMinGray			= 50;			// 最小灰度
			m_pData2GrayParam[ch].cMaxGray			= 255;			// 最大灰度
			m_pData2GrayParam[ch].cLessDownGray		= 50;			// 弱最小灰度
			m_pData2GrayParam[ch].cExceedUpGray		= 255;			// 超最大灰度
			m_pData2GrayParam[ch].bAntiPhase		= FALSE;		// 是否反相

			// 是MODIS数据
			if(pAgmrsData->IsModisData())
			{
				if(ch >= 19)
				{// 反相
					m_pData2GrayParam[ch].bAntiPhase = TRUE;
				}

				if(ch == 24)
				{
					m_pData2GrayParam[ch].iEnhanceType = 1;		// 增强方式(0-直线，1-指数，2-对数 ...)
					m_pData2GrayParam[ch].bAntiPhase = FALSE;
				}
			}

			if(m_pData2GrayParam[ch].bAntiPhase)
			{//反相
				m_pData2GrayParam[ch].cLessDownGray = m_pData2GrayParam[ch].cMaxGray;		// 弱最小灰度
				m_pData2GrayParam[ch].cExceedUpGray = m_pData2GrayParam[ch].cMinGray;		// 超最大灰度
			}
			else
			{
				m_pData2GrayParam[ch].cLessDownGray = m_pData2GrayParam[ch].cMinGray;		// 弱最小灰度
				m_pData2GrayParam[ch].cExceedUpGray = m_pData2GrayParam[ch].cMaxGray;		// 超最大灰度
			}

			// 显示进度
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iChannelNums;
					pProgressParam->lCurrentPos = ch;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}

		}// next ch

		// 内存回收
		_deletea(iValueIntensity);
		_deletea(fProbability);

		return;
	}

	// 通道数据转换为灰度
	void KLDFileProcessor::ChannelData2Gray(Data2GrayParam* pData2GrayParam)
	{
		int i,value;

		int ch = pData2GrayParam->iChannel;						// 通道号
		int iEnhanceType = pData2GrayParam->iEnhanceType;		// 增强方式(0-直线，1-指数，2-对数 ...)
		WORD wMinValue = pData2GrayParam->wMinValue;			// 最小值
		WORD wMaxValue = pData2GrayParam->wMaxValue;			// 最大值
		WORD wDnThreshold = pData2GrayParam->wThreshold_min;	// 去掉一些小值后的极小值
		WORD wUpThreshold = pData2GrayParam->wThreshold_max;	// 去掉一些大值后的极大值
		BYTE cMinGray = pData2GrayParam->cMinGray;				// 最小灰度
		BYTE cMaxGray = pData2GrayParam->cMaxGray;				// 最大灰度
		BYTE cLessDownGray = pData2GrayParam->cLessDownGray;	// 弱最小灰度
		BYTE cExceedUpGray = pData2GrayParam->cExceedUpGray;	// 超最大灰度
		BOOL bAntiPhase = pData2GrayParam->bAntiPhase;			// 是否反相

		// 取得数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// 通道数据指针
		PWORD* ppwData = pAgmrsData->GetData();

		// 通道数据灰度指针
		PBYTE* ppucDataGray = pAgmrsData->GetDataGray();
		memset(ppucDataGray[ch], cExceedUpGray, AGMRS_MERSI_DATA_VALUE_MAX);

		// 通道数据尺寸
		int iSizeOfData = pDataInfo->Width() * pDataInfo->Height();
		for(i=0; i<iSizeOfData; i++)
		{
			WORD wData = ppwData[ch][i] & 0x0FFF;
			//通道值为0的灰度为0
			if(wData == 0)
			{
				ppucDataGray[ch][wData] = 0;
			}
			else if(wData > wUpThreshold)
			{
				ppucDataGray[ch][wData] = cExceedUpGray;
			}
			else if(wData < wDnThreshold)
			{
				ppucDataGray[ch][wData] = cLessDownGray;
			}
			else
			{
				// 其余进行增强处理
			}
		}

		WORD wUpValue = wUpThreshold;
		WORD wDownValue = wDnThreshold;

		//Assert(wUpValue-wDownValue>0);
		// 在 DEBUG 环境下有时不报错,但在 Release 时出错
		// 浪费我一天时间

		double a,b;
		switch(iEnhanceType)
		{
		case 0:	// 线性增强
			a = (double)(cMaxGray-cMinGray) / (wUpValue-wDownValue);
			b = (double)cMinGray-a*wDownValue;
			for(value=wDownValue; value<wUpValue+1; value++)
			{
				ppucDataGray[ch][value] = bAntiPhase ? cMaxGray-(BYTE)(a*value+b) : (BYTE)(a*value+b);
			}
			break;

		case 1:	// 指数增强 y=a*exp(b*x)  ,取点(1,min),(255,max)求出a,b
			if(wUpValue-wDownValue!=0)
				b = log((double)cMaxGray/(wUpValue-wDownValue));
			else
				b = 2;

			a = (double)cMaxGray / exp(b*wUpValue);
			for(value=wDownValue; value<wUpValue+1; value++)
			{
				ppucDataGray[ch][value]=bAntiPhase ? cMaxGray-(BYTE)(a*exp(b*value)) : (BYTE)(a*exp(b*value));

				if(ppucDataGray[ch][value]>cMaxGray)
				{
					ppucDataGray[ch][value] = bAntiPhase ? cMinGray : cMaxGray;
				}
			}              
			break;

		case 2:	// 对数增强
			a = (double)(cMaxGray-cMinGray) / (log((double)wUpValue)-log((double)wDownValue));
			b = (double)cMinGray - a*log((double)wDownValue);
			for(value=wDownValue; value<wUpValue+1; value++)
			{
				//if(value>0 || value<255)
				//{
				//	ppucDataGray[ch][value] = bAntiPhase ? (255 - value) : value;
				//}

				ppucDataGray[ch][value] = bAntiPhase ? cMaxGray-(BYTE)(a*log((double)value)+b) : (BYTE)(a*log((double)value)+b);
			}
			break;
		}

		return;
	}

	// 产品生成 - 植被指数
	// 在(0,1)之间
	BOOL KLDFileProcessor::Vix(int iProductID)
	{
		int i,j;

		// 数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		// 数据信息
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// 数据指针
		PWORD* ppwData = pAgmrsData->GetData();
		if(!ppwData)
			return FALSE;

		// 数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 设置算法数据
		WORD wDataAlg[AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX];
		memset(&wDataAlg[0],0,sizeof(WORD)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		KAgmrsAlgorithm* pAgmrsAlgorithm = (KAgmrsAlgorithm*)this->GetDataAlgorithm();
		pAgmrsAlgorithm->SetData(&wDataAlg[0]);

		//-------------------------------------------------
		if(iProductID == AGMRS_PRODUCT_CODE_VIX_LAI)
		{
			// 查看LAI配置数据
			pAgmrsAlgorithm->DateTime = pDataInfo->DateTime();
			if(!pAgmrsAlgorithm->ConfigLai())
			{
				MessageBox(NULL,_T("ConfigLai error"),_T("LAI"),MB_OK);
				return FALSE;
			}
		}
		//-------------------------------------------------

		// 设置数据通道的数据索引
		memset(&m_DataIndexOfChannel[0],-1,sizeof(int)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		for(i=1;i<=36;i++)	// 36是MODIS遗留，如何解决？
		{
			int iChannelCode = pAgmrsData->Channel2ChannelCode(i);
			m_DataIndexOfChannel[i] = pAgmrsData->GetDataIndex(iChannelCode);
		}

		// 设置红光，近红外光、蓝光波段反射率数据通道
		int iRedChannel(-1),iNirChannel(-1),iBluChannel(-1);
		int iDataSource = pAgmrsData->GetAgmrsDataSource();
		switch(iDataSource)
		{
		case AgmrsDataSource::Avhrr:
			{
				iRedChannel = AvhrrChannel::Red;
				iNirChannel = AvhrrChannel::Nir;
				//iBluChannel = AvhrrChannel::Blu;
			}
			break;
		case AgmrsDataSource::Modis:
			{
				iRedChannel = ModisChannel::Red;
				iNirChannel = ModisChannel::Nir;
				iBluChannel = ModisChannel::Blu;
			}
			break;
		case AgmrsDataSource::Mersi:
			{
				iRedChannel = MersiChannel::Red;
				iNirChannel = MersiChannel::Nir;
				iBluChannel = MersiChannel::Blu;
			}
			break;
		case AgmrsDataSource::Virr:
			{
				iRedChannel = VirrChannel::Red;
				iNirChannel = VirrChannel::Nir;
				iBluChannel = VirrChannel::Blu;
			}
			break;
		}

		// 检查需要的通道
		BOOL bChannelOk = FALSE;
		_tstring strInfo = _T("");
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_VIX_NDVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("计算 NDVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_DVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("计算 DVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_RVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("计算 RVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_SAVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("计算 SAVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_EVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0 && (m_DataIndexOfChannel[iBluChannel]>=0));
			strInfo = _T("计算 EVI ...");
			break;

			// 植被指数导出产品
		case AGMRS_PRODUCT_CODE_VIX_LAI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("计算 LAI ...");
			break;
		}

		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("所需通道数据不全! -- Vix"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		//为产品数据分配内存
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float)*iWidth*iHeight);

		// 计算植被指数
		WORD wDataRed(0),wDataNir(0),wDataBlu(0);
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		//=================================================
		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================

		double dBaseValue = 0;
		double dScale = 1.0;

		double dvix_min = 0;
		double dvix_max = 1;

		// 进度条参数
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
		if(pProgressParam) { pProgressParam->SetInfo(strInfo.c_str()); }

		// 计算
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				bValid = FALSE;
				dValue = AGMRS_PRODUCT_DATA_INVALID;

				//升降轨标记, 1: 升轨, 0: 降轨
				//int ii = i;
				int ii = (iHeight - 1) - i;

				int pos = ii*iWidth+j;

				// 取得通道数据
				wDataRed = ppwData[m_DataIndexOfChannel[iRedChannel]][pos] & 0x0FFF;
				wDataRed = wDataRed>=0x0FFE ? 0 : wDataRed;
				wDataNir = ppwData[m_DataIndexOfChannel[iNirChannel]][pos] & 0x0FFF;
				wDataNir = wDataNir>=0x0FFE ? 0 : wDataNir; 
				if(AGMRS_PRODUCT_CODE_VIX_EVI==iProductID)
				{
					wDataBlu = ppwData[m_DataIndexOfChannel[iBluChannel]][pos] & 0x0FFF;
					wDataBlu = wDataBlu>=0x0FFE ? 0 : wDataBlu; 
				}

				// 无效数据
				if(wDataRed==0x0FFF || wDataNir==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
					continue;
				}
				if(wDataRed==0x0FFE || wDataNir==0x0FFE)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
					continue;
				}

				// 为算法提供数据
				wDataAlg[KAgmrsAlgorithm::Red]	= wDataRed;
				wDataAlg[KAgmrsAlgorithm::Nir]	= wDataNir;
				wDataAlg[KAgmrsAlgorithm::Blu]	= wDataBlu;

				// 取得像元点类型
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Land:	// 陆地
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
						switch(iProductID)
						{
						case AGMRS_PRODUCT_CODE_VIX_NDVI:
							dBaseValue = 0.0;
							dScale = 1.0;
							dvix_min = 0;
							dvix_max = 1;
							dValue = pAgmrsAlgorithm->Ndvi();
							break;

						case AGMRS_PRODUCT_CODE_VIX_DVI:
							dBaseValue = 0.0;
							dScale = 1.0;
							dvix_min = 0;
							dvix_max = 1;
							dValue = pAgmrsAlgorithm->Dvi();
							break;

						case AGMRS_PRODUCT_CODE_VIX_RVI:
							dBaseValue = 0.0;
							dScale = 1.0;
							dvix_min = 0;
							dvix_max = 10;
							dValue = pAgmrsAlgorithm->Rvi();
							break;

						case AGMRS_PRODUCT_CODE_VIX_SAVI:
							dBaseValue = 0.0;
							dScale = 1.0;
							dvix_min = 0;
							dvix_max = 1;
							dValue = pAgmrsAlgorithm->Savi();
							break;

						case AGMRS_PRODUCT_CODE_VIX_EVI:
							dBaseValue = 0.0;
							dScale = 1.0;
							dvix_min = 0;
							dvix_max = 1;
							dValue = pAgmrsAlgorithm->Evi();
							break;

						case AGMRS_PRODUCT_CODE_VIX_LAI:	// LAI
							dBaseValue = 0.0;
							dScale = 1.0;
							dvix_min = 0;
							dvix_max = 10;

							//-----------------------------
							// 设置计算LAI所需参数
							// (1)时间
							pAgmrsAlgorithm->DateTime = pDataInfo->DateTime();
							// (2)方程参数(在应用里提供)
							// pAgmrsAlgorithm->SetLaiConfig(KStringArray& lc);
							//-----------------------------

							dValue = pAgmrsAlgorithm->Lai();
							break;
						}

						// 是否有效
						bValid = (dValue>dvix_min && dValue<dvix_max);
						if(bValid)
						{
							//=============================
							dtmin = min(dtmin,dValue);
							dtmax = max(dtmax,dValue);
							//=============================
							// trueValue = value / dScale + dBaseValue;
							m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);
						}
					}
					break;

				case PixelType::Water:		// 水
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					break;

				case PixelType::Cloud:		// 云
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					break;

				case PixelType::Unknown:	// 未知 -9999
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					break;
				}

			}// for(j)

			// 显示进度
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iHeight;
					pProgressParam->lCurrentPos = i;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}


		}// for(i)

		///------------------------------------------------
		// 填充产品数据信息
		KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

		// 1.格式代码
		pProductDataInfo->wFormatCode	= AXIN_FMT_CODE_GRID_IMAGE;	// important

		// 2.日期
		pProductDataInfo->wYear			= pDataInfo->Year();
		pProductDataInfo->wMonth		= pDataInfo->Month();
		pProductDataInfo->wDay			= pDataInfo->Day();
		pProductDataInfo->wHour			= pDataInfo->Hour();
		pProductDataInfo->wMinute		= pDataInfo->Minute();
		pProductDataInfo->wSecond		= 0;
		pProductDataInfo->dwMillisecond	= 0;

		// 3.数据描述
		pProductDataInfo->wElementCode	= 0;
		pProductDataInfo->wProductCode	= iProductID;
		pProductDataInfo->wDataType		= DataType_Float;
		pProductDataInfo->InvalidData()	= AXIN_INVALID_DATA;
		pProductDataInfo->dBaseValue	= dBaseValue;
		pProductDataInfo->dScale		= dScale;

		// 4.格点参数
		pProductDataInfo->xNum			= pDataInfo->Width();
		pProductDataInfo->xMin			= pDataInfo->MinLon();
		pProductDataInfo->xMax			= pDataInfo->MaxLon();
		pProductDataInfo->xInterval		= pDataInfo->LonInterval();

		pProductDataInfo->yNum			= pDataInfo->Height();
		pProductDataInfo->yMin			= pDataInfo->MinLat();
		pProductDataInfo->yMax			= pDataInfo->MaxLat();
		pProductDataInfo->yInterval		= pDataInfo->LatInterval();

		// 注释信息
		TCHAR szTemp[MAX_PATH];
		_stprintf(szTemp,_T("%s"),pAgmrsData->NameProduct(iProductID));
		pProductDataInfo->SetComment(szTemp);


		// 6. 在外部设置调色板和调色板类型
		// ...
		///------------------------------------------------
		return TRUE;
	}


	BOOL KLDFileProcessor::Dix(int iProductID)
	{
		BOOL bValue = FALSE;
		//-------------------------------------------------
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_DIX_ATI:
			bValue = Dix_Ati(iProductID);
			break;

		case AGMRS_PRODUCT_CODE_DIX_TVDI:
			bValue = Dix_Tvdi(iProductID);
			break;

		case AGMRS_PRODUCT_CODE_DIX_PDI:
			bValue = Dix_Pdi(iProductID);
			break;

		case AGMRS_PRODUCT_CODE_DIX_MPDI:
			bValue = Dix_Mpdi(iProductID);
			break;

		default:
			bValue = Dix_Def(iProductID);
			break;
		}

		//-------------------------------------------------
		return bValue;
	}

	// 温度植被干旱指数
	// 需要 NDVI 和 LST
	// TVDI = (Ts-Tsmin) / (Tsmax-Tsmin)
	BOOL KLDFileProcessor::Dix_Tvdi(int iProductID)
	{
		int i,j;

		// 数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		// 数据对象
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// 数据指针
		PWORD* ppwData = pAgmrsData->GetData();
		if(!ppwData)
			return FALSE;

		// 数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 设置数据通道的内存通道
		for(i=1;i<=36;i++)
		{
			int iChannelCode = pAgmrsData->Channel2ChannelCode(i);
			m_DataIndexOfChannel[i] = pAgmrsData->GetDataIndex(iChannelCode);
		}

		// 设置算法数据
		WORD wDataAlg[AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX];
		memset(&wDataAlg[0],0,sizeof(WORD)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		KAgmrsAlgorithm* pAgmrsAlgorithm = (KAgmrsAlgorithm*)this->GetDataAlgorithm();
		pAgmrsAlgorithm->SetData(&wDataAlg[0]);

		// 设置数据通道的数据索引
		memset(&m_DataIndexOfChannel[0],-1,sizeof(int)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		for(i=1;i<=36;i++)	// 36是MODIS遗留，如何解决？
		{
			int iChannelCode = pAgmrsData->Channel2ChannelCode(i);
			m_DataIndexOfChannel[i] = pAgmrsData->GetDataIndex(iChannelCode);
		}

		// 进度条参数
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		PFLOAT* ppfLst = NULL;
		PFLOAT* ppfNdvi = NULL;

		// 1.计算 LST
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("计算 LST ..."));
		if(this->Lst(LstMethod::Sobrino))
		{
			// 分配内存保存 LST 数据
			if(!KFloat2DMemAllocator::AllocMemory2D(ppfLst,iHeight,iWidth))
			{
				::MessageBox(NULL, _T("内存分派错误! -- Dix_Tvdi_01"),_T(""),MB_OK|MB_ICONINFORMATION);
				KFloat2DMemAllocator::FreeMemory2D(ppfLst);
				KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
				return FALSE;
			}
			memcpy(*ppfLst,*m_ppfProduct,sizeof(FLOAT) * iWidth * iHeight);
		}
		if(!ppfLst)
		{
			KFloat2DMemAllocator::FreeMemory2D(ppfLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
			return FALSE;
		}

		// 2.计算 NDVI
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("计算 NDVI ..."));
		if(this->Vix(AGMRS_PRODUCT_CODE_VIX_NDVI))
		{
			// 分配内存保存 NDVI 数据
			if(!KFloat2DMemAllocator::AllocMemory2D(ppfNdvi,iHeight,iWidth))
			{
				::MessageBox(NULL, _T("内存分派错误! -- Dix_Tvdi_02"),_T(""),MB_OK|MB_ICONINFORMATION);
				KFloat2DMemAllocator::FreeMemory2D(ppfLst);
				KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
				return FALSE;
			}
			memcpy(*ppfNdvi,*m_ppfProduct,sizeof(FLOAT) * iWidth * iHeight);
		}
		if(!ppfNdvi)
		{
			KFloat2DMemAllocator::FreeMemory2D(ppfLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
			return FALSE;
		}



		// 3.整理有效植被指数坐标
		//   取得 ndvi [0.01,0.80]*100->[1,80] 范围内对应的 (i,j) 坐标
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("整理有效植被指数坐标..."));
		vector<POINT> ptArray[100];
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				WORD wNdvi = (WORD)(ppfNdvi[i][j] * 100);
				for(int v=1;v<=80;v++)
				{
					if(wNdvi == v)
					{
						POINT pt;	pt.x = j;	pt.y = i;
						ptArray[v].push_back(pt);			// 保存全部 v 值的坐标
					}
				}
			}

			// 显示进度
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iHeight;
					pProgressParam->lCurrentPos = i;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}
		}

		// 4.整理植被指数和对应的地表温度
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("整理植被指数和对应的地表温度..."));
		float fLstMin[100];
		float fLstMax[100];
		for(i=0;i<100;i++)
		{
			fLstMin[i] =  99999;
			fLstMax[i] = -99999;
		}

		for(int v=1;v<=80;v++)
		{
			vector<POINT> pta = ptArray[v];
			int size = pta.size();
			for(int m=0;m<size;m++)
			{
				POINT pt = pta[m];
				i = pt.y;	j = pt.x; 

				float fLst = ppfLst[i][j];				// K
				if( fLst>0+273.15 && fLst<60+273.15 )
				{
					fLstMin[v] = min(fLstMin[v],fLst);
					fLstMax[v] = max(fLstMax[v],fLst);
				}
			}

			// 显示进度
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = 80;
					pProgressParam->lCurrentPos = v;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}
		}

		// 计算有效数据数量
		BOOL bMin[100], bMax[100];
		memset(&bMin[0],FALSE,sizeof(BOOL)*100);
		memset(&bMax[0],FALSE,sizeof(BOOL)*100);
		int iNumberOfMin = 0;
		int iNumberOfMax = 0;

		for(int v=1;v<=80;v++)
		{
			bMin[v] = !(fabs(fLstMin[v] - 99999) < EPS);	// !=  99999
			if(bMin[v]) iNumberOfMin++;

			bMax[v] = !(fabs(fLstMax[v] + 99999) < EPS);	// != -99999
			if(bMax[v]) iNumberOfMax++;
		}

		int imin(0),imax(0);
		double dNdviMin[80],dNdviMax[80];
		double dTsmin[80],dTsmax[80];
		for(i=0;i<100;i++)
		{
			if(bMin[i])
			{
				dNdviMin[imin] = (double)(i)/100;
				dTsmin[imin] = (double)fLstMin[i];	// K
				imin++;
			}

			if(bMax[i])
			{
				dNdviMax[imax] = (double)(i)/100;
				dTsmax[imax] = (double)fLstMax[i];
				imax++;
			}
		}

		double a1,b1,a2,b2;
		double c1[2],c2[2],dt1[6],dt2[6];
		//--------------------------------------------
		//     y = a0 + a1*x
		// TSmin = a1 + b1*NDVI
		// TSmax = a2 + b2*NDVI
		KRegression::Linear1D(&dNdviMin[0],&dTsmin[0],iNumberOfMin,c1,dt1);
		KRegression::Linear1D(&dNdviMax[0],&dTsmax[0],iNumberOfMax,c2,dt2);

		a1 = c1[0];		b1 = c1[1];
		a2 = c2[0];		b2 = c2[1];
		//--------------------------------------------

		//为产品数据分配内存
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			::MessageBox(NULL, _T("内存分派错误! -- Dix_Tvdi_03"),_T(""),MB_OK|MB_ICONINFORMATION);
			KFloat2DMemAllocator::FreeMemory2D(ppfLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float) * iWidth * iHeight);

		// 计算 TVDI
		//
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		double dBaseValue = 0;	// 基本值
		double dScale = 1.0;	// 放大系数

#ifdef _DEBUG
		//=================================================
		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================
#endif

		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("干旱指数反演..."));
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				bValid = FALSE;
				dValue = AGMRS_PRODUCT_DATA_INVALID;

				//升降轨标记, 1: 升轨, 0: 降轨
				int ii = iHeight - 1 - i;

				float fLst = ppfLst[i][j];
				float fNdvi = ppfNdvi[i][j];
				BOOL bLst = (fLst>0+273.15 && fLst<100+273.15);	// (0,100)
				BOOL bNdvi = fNdvi>0 && fNdvi<1.0;				// (0,1)

				//取得像元点类型
				int iPixelType = this->GetPixelType(ii,j);	// y,x
				switch(iPixelType)
				{
				case PixelType::Land:
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
						dBaseValue = 0;	dScale = 1.0;
						if(bLst && bNdvi)
						{
							double ndvi = fNdvi;			// (0.0, 1.0)
							double Tsmin = a1 + b1*ndvi;	// K
							double Tsmax = a2 + b2*ndvi;	// K
							double Ts = fLst;				// K
							if(fabs(Tsmax-Tsmin)<EPS)
							{// Tsmax-Tsmin == 0
								dValue = 0;
							}
							else
							{// 值越大越干旱
								dValue = (Ts - Tsmin) / (Tsmax - Tsmin);
							}
						}
						else
						{
							dValue = 0;
						}

						// 是否有效
						bValid = (dValue>0 && dValue<1);
						if(bValid)
						{
#ifdef _DEBUG
							//=============================
							dtmin = min(dtmin,dValue);
							dtmax = max(dtmax,dValue);
							//=============================
#endif
							m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);		// trueValue = value / dScale + dBaseValue;
						}
					}
					break;

				case PixelType::Water:		// 水 
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					break;

				case PixelType::Cloud:		// 云 
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					break;

				case PixelType::Unknown:	// 未知 99999
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					break;
				}

			}// for(j)

			// 显示进度
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iHeight;
					pProgressParam->lCurrentPos = i;
				}

				m_pCallbackFunc(m_pCallbackParam);
			}

		}//for(i)

		// 释放内存
		KFloat2DMemAllocator::FreeMemory2D(ppfLst);
		KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
		///------------------------------------------------
		// 填充产品数据信息
		KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

		// 1.格式代码
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.日期
		pProductDataInfo->wYear				= pDataInfo->wYear;
		pProductDataInfo->wMonth			= pDataInfo->wMonth;
		pProductDataInfo->wDay				= pDataInfo->wDay;
		pProductDataInfo->wHour				= pDataInfo->wHour;
		pProductDataInfo->wMinute			= pDataInfo->wMinute;
		pProductDataInfo->wSecond			= 0;
		pProductDataInfo->dwMillisecond		= 0;

		// 3.数据描述
		//pProductDataInfo->wElementCode	= eElementStyle_Index;			// important
		pProductDataInfo->wProductCode		= iProductID;
		pProductDataInfo->wDataType			= DataType_Float;				// important
		pProductDataInfo->InvalidData()		= AXIN_INVALID_DATA;
		pProductDataInfo->dBaseValue		= dBaseValue;
		pProductDataInfo->dScale			= dScale;

		// 4.格点参数
		pProductDataInfo->xNum				= pDataInfo->Width();
		pProductDataInfo->xMin				= pDataInfo->MinLon();
		pProductDataInfo->xMax				= pDataInfo->MaxLon();
		pProductDataInfo->xInterval			= pDataInfo->LonInterval();

		pProductDataInfo->yNum				= pDataInfo->Height();
		pProductDataInfo->yMin				= pDataInfo->MinLat();
		pProductDataInfo->yMax				= pDataInfo->MaxLat();
		pProductDataInfo->yInterval			= pDataInfo->LatInterval();

		// 5.注释信息
		KString strPathName(pAgmrsData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileNameWithoutPath = strPathName.Mid(pos+1);		// 取得文件名(无路径)
		KString strProduct(_T("tvdi"));

		TCHAR szComment[MAX_PATH];
		_stprintf(szComment,_T("DIX_%s(%d)_%s"),strProduct.GetString(), iProductID, strFileNameWithoutPath.GetString());
		pProductDataInfo->SetComment(szComment);

		// 6. 在外部设置调色板和调色板类型
		// ...

		///------------------------------------------------

		return TRUE;
	}


//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
