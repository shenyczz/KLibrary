#include "StdAfx.h"
#include "KAvhrrDataProcessor.h"

#include "KAvhrrData.h"
#include "KAvhrrAlgorithm.h"

namespace Agmrs { namespace Avhrr
{

	KAvhrrDataProcessor::KAvhrrDataProcessor(void)
	{
		this->m_pDataAlgorithm = new KAvhrrAlgorithm();
	}

	KAvhrrDataProcessor::~KAvhrrDataProcessor(void)
	{
	}

	// 数据转换为 DIB
	BOOL KAvhrrDataProcessor::DataToDIB(int iRChannel,int iGChannel,int iBChannel)
	{
		int i,j;

		// 取得数据对象
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		if(!pAvhrrData)
			return FALSE;

		// 数据空
		if(pAvhrrData->IsEmpty())
			return FALSE;

		// 数据信息
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();
		if(!pDataInfo)
			return FALSE;

		// 数据转换为灰度
		this->Data2Gray();

		// 设置融合通道值
		int chr,chg,chb;
		pAvhrrData->GetDataChannelDef(&chr,&chg,&chb);
		if(iRChannel==0 && iGChannel==0 && iBChannel==0)
		{
			pAvhrrData->SetDataChannel(chr,chg,chb);
		}
		else
		{
			pAvhrrData->SetDataChannel(iRChannel,iGChannel,iBChannel);
		}

		// 选择数据通道
		pAvhrrData->GetDataChannel(&chr,&chg,&chb);

		// 通道号转换为通道代码
		int chrCode = pAvhrrData->Channel2ChannelCode(chr);
		int chgCode = pAvhrrData->Channel2ChannelCode(chg);
		int chbCode = pAvhrrData->Channel2ChannelCode(chb);

		// 通道通道代码转换为数据索引
		int rDataIndex = pAvhrrData->GetDataIndex(chrCode);
		int gDataIndex = pAvhrrData->GetDataIndex(chgCode);
		int bDataIndex = pAvhrrData->GetDataIndex(chbCode);

		// 通道缩引 = [0,9]
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
		PWORD* ppwData = pAvhrrData->GetData();
		PBYTE* ppucDataGray = pAvhrrData->GetDataGray();

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
	void KAvhrrDataProcessor::Data2Gray()
	{
		// 计算通道数据转换为灰度数据的参数
		CalcData2GrayParam();

		// 取得数据对象
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// 通道数
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 通道数据转换为灰度值
		for(int ch=0;ch<iChannelNums;ch++)	// 0-19
		{
			ChannelData2Gray(&m_pData2GrayParam[ch]);
		}

		return;
	}

	// 计算通道数据转换为灰度数据的参数
	void KAvhrrDataProcessor::CalcData2GrayParam()
	{
		int i,ch,value;

		// 取得数据对象
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		int iAvailPoint = 0;		// 有效点数
		WORD wMaxValue = 0;			// 最大值
		WORD wMinValue = 0xFFFF;	// 最小值

		WORD wUpThreshold = AGMRS_VIRR_DATA_VALUE_MAX;	// 通道数据上阈值
		WORD wDnThreshold = 0;							// 通道数据下阈值

		// 通道数
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 单个通道数据字节数
		int iDataSizePerChannel = pDataInfo->Width() * pDataInfo->Height();

		// 通道值密度
		int* iValueIntensity = new int[AGMRS_VIRR_DATA_VALUE_MAX+10];

		// 存储累计概率
		float* fProbability = new float[AGMRS_VIRR_DATA_VALUE_MAX+10];

		// 初始化通道数据转换为灰度数据的参数
		memset(m_pData2GrayParam, 0, sizeof(Data2GrayParam) * AGMRS_VIRR_DATA_CHANNEL_NUMBERS);

		// 通道数据指针
		PWORD* ppwData = pAvhrrData->GetData();

		// 计算各通道参数
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) pProgressParam->SetInfo(_T("通道数据转换为灰度值..."));
		for(ch=0; ch<iChannelNums; ch++)
		{
			memset(iValueIntensity,0,sizeof(int)*(AGMRS_VIRR_DATA_VALUE_MAX+10) );
			memset(fProbability,0,sizeof(float)*(AGMRS_VIRR_DATA_VALUE_MAX+10) );

			wMaxValue = 0;
			wMinValue = 0xFFFF;

			for(i=0;i<iDataSizePerChannel;i++)
			{
				ppwData[ch][i] &= AGMRS_VIRR_DATA_VALUE_MAX;	// 0x0FFF
				WORD wData = ppwData[ch][i];
				if(wData < AGMRS_VIRR_DATA_VALUE_MAX)			//0x0fff = 4095 有效值
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
			for(value=2; value<=AGMRS_VIRR_DATA_VALUE_MAX; value++)
			{
				fProbability[value] = fProbability[value-1] + (float)iValueIntensity[value] / iAvailPoint;
			}

			// 过滤掉数量少、非常小的值，得到通道数据下阈值
			for(value=0; value<AGMRS_VIRR_DATA_VALUE_MAX; value++)
			{
				if(fProbability[value] > 0.012)
				{
					wDnThreshold = value; // 通道数据下阈值
					break;
				}
			}

			// 过滤掉数量少、值很大的值，得到通道数据上阈值
			for(value=AGMRS_VIRR_DATA_VALUE_MAX; value>=0; value--)
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

			/*
			if(ch >= 19)
			{// 反相
				m_Data2GrayParam[ch].bAntiPhase = TRUE;
			}

			if(ch == 24)
			{
				m_Data2GrayParam[ch].iEnhanceType = 1;		// 增强方式(0-直线，1-指数，2-对数 ...)
				m_Data2GrayParam[ch].bAntiPhase = FALSE;
			}
			*/

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
	void KAvhrrDataProcessor::ChannelData2Gray(Data2GrayParam* pData2GrayParam)
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
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// 通道数据指针
		PWORD* ppwData = pAvhrrData->GetData();

		// 通道数据灰度指针
		PBYTE* ppucDataGray = pAvhrrData->GetDataGray();
		memset(ppucDataGray[ch], cExceedUpGray, AGMRS_VIRR_DATA_VALUE_MAX);

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

	// 像元类型
	// PixelType::Cloud,	// 云
	// PixelType::Fog,		// 雾
	// PixelType::Fire,		// 火
	// PixelType::Snow,		// 雪
	// PixelType::Ice,		// 冰
	// PixelType::Land,		// 陆
	// PixelType::Water,	// 水(1,2)
	int KAvhrrDataProcessor::GetPixelType(int i,int j)
	{
		int iPixelType = PixelType::Unknown;
		//-------------------------------------------------
		// 数据对象
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		// 数据信息
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();
		// 数据指针
		PWORD* ppwData = pAvhrrData->GetData();

		//数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		if(i<0 || j<0 || i>=iHeight || j>=iWidth)
			return PixelType::Unknown;

		// 判别阈值
		KAvhrrThreshold threshold = *((KAvhrrThreshold*)pAvhrrData->GetThreshold());

		int ch1		= m_DataIndexOfChannel[1];		// %
		int ch2		= m_DataIndexOfChannel[2];		// %
		int ch3		= m_DataIndexOfChannel[3];		// %
		int ch4		= m_DataIndexOfChannel[4];		// %
		int ch5		= m_DataIndexOfChannel[5];		// %

		double dDataCh1(0),dDataCh2(0),dDataCh3(0),dDataCh4(0),dDataCh5(0);

		if(ch1>=0)	dDataCh1	= 1.0 * ppwData[ch1][i*iWidth+j] / 10;		// %
		if(ch2>=0)	dDataCh2	= 1.0 * ppwData[ch2][i*iWidth+j] / 10;		// %
		if(ch3>=0)	dDataCh3	= 1.0 * ppwData[ch3][i*iWidth+j] / 10;		// K
		if(ch4>=0)	dDataCh4	= 1.0 * ppwData[ch4][i*iWidth+j] / 10;		// K
		if(ch5>=0)	dDataCh5	= 1.0 * ppwData[ch5][i*iWidth+j] / 10;		// K

		//-------------------------------------------------
		// 1.先判断云和水体
		if(m_DataIndexOfChannel[1]>=0 && m_DataIndexOfChannel[2]>=0 && m_DataIndexOfChannel[4]>=0)	// 存在 4 通道
		{
			if (dDataCh1>threshold.dCloudRed && (dDataCh2 - dDataCh1)>threshold.dCloudNirRed && dDataCh4<threshold.dCloudT11um)
			{//云
				iPixelType = PixelType::Cloud;
			}
			else if (dDataCh1<threshold.dWaterRed && dDataCh2<threshold.dWaterNir && (dDataCh2 - dDataCh1)<threshold.dWaterNirRed)
			{//水体
				iPixelType = PixelType::Water;
			}
			else
			{//陆地
				iPixelType = PixelType::Land;
			}
		}
		// 不存在 4 通道
		else if(m_DataIndexOfChannel[1]>=0 && m_DataIndexOfChannel[2]>=0)
		{
			if (dDataCh1>threshold.dCloudRed && (dDataCh2 - dDataCh1)>threshold.dCloudNirRed)
			{//云
				iPixelType = PixelType::Cloud;
			}
			//{雾			}
			else if (dDataCh1<threshold.dWaterRed && dDataCh2<threshold.dWaterNir && (dDataCh2 - dDataCh1)<threshold.dWaterNirRed)
			{//水体
				iPixelType = PixelType::Water;
			}
			else
			{//陆地
				iPixelType = PixelType::Land;
			}
		}
		else
		{
			iPixelType = PixelType::Unknown;
		}
		//-------------------------------------------------
		// 2. 判断大雾(云雾分离)
		if(iPixelType == PixelType::Cloud)
		{
		}
		//-------------------------------------------------
		// 3. 判断火点
		BOOL bDay = pAvhrrData->IsDay();
		// 火点排除： 所有满足 T 4 <315K （夜间 305K ）或 ΔT 41 ＜ 5K （ 3K ）的像素都不是火点
		BOOL bNotFire = FALSE;	// 一定不是火点
		bNotFire = bDay
			? dDataCh3 < threshold.dFiringT3930nm || (dDataCh3 - dDataCh4) < threshold.dFiringT3930nm_T11um
			: dDataCh3 < threshold.dNightFiringT3930nm || (dDataCh3 - dDataCh4) < threshold.dNightFiringT3930nm;

		int fireMethod = this->GetFlag();

		// 3.1 常规 AVHRR 火灾探测算法如下：
		// A: R1<20%
		// B: R2<25%
		// C: T3>315K(晚上为293K)
		// D: T3-T4>10K(晚上为8K)
		// 其中R1、R2为1、2通道的反射率值，T3、T4为3、4通道辐射亮温值。
		// 如果满足以下条件，就可以将该点确认为火点：
		// Fire = A && B && C && D
		if(fireMethod == 1 && (iPixelType == PixelType::Land || iPixelType == PixelType::Cloud) && !bNotFire )
		{
			// 1.判断明火点
			BOOL bFireNaked	= FALSE;
			double R1 = dDataCh1;
			double R2 = dDataCh2;
			double T3 = dDataCh3;
			double T3_4 = dDataCh3 - dDataCh4;

			if(bDay)
			{
				BOOL bCondition_A = R1 < threshold.dFiringRed;						// CH1<20
				BOOL bCondition_B = R2 < threshold.dFiringNir;						// CH2<25
				BOOL bCondition_C = T3 > threshold.dFiringT3930nm;						// CH3>315
				BOOL bCondition_D = T3_4 > threshold.dFiringT3930nm_T11um;					// CH3-CH4>10
				bFireNaked = bCondition_A && bCondition_B && bCondition_C && bCondition_D;
			}
			else
			{
				BOOL bCondition_C = T3 > threshold.dNightFiringT3930nm;
				BOOL bCondition_D = T3_4 > threshold.dNightFiringT3930nm_T11um;
				bFireNaked = bCondition_C && bCondition_D;
			}

			// 2.再判断暗火点
			BOOL bFireDark	= FALSE;
			if(bFireNaked)
			{
				// waiting...
			}

			// 3.再判断过火点
			BOOL bFireEmber	= FALSE;
			if(bFireNaked && !bFireDark)
			{
				// waiting...
			}

			// 赋值像素类型
			if(bFireNaked)
				iPixelType = PixelType::FireNaked;	// 明火
			else if(bFireEmber)
				iPixelType = PixelType::FireEmber;	// 过火
			else if(bFireDark)
				iPixelType = PixelType::FireDark;	// 暗火
			else
				iPixelType = iPixelType;			// 不变
		}

		// 3.2 复杂 AVHRR 火灾探测算法如下：
		// 设有 5 个逻辑条件(A 、 B 、 a 、 b 、 X)
		// A： T4 > T4b + 4 * δT4b		a: ΔT41 > ΔT41b + 4 * δT41b 
		// B： T4 > 320K (夜间 315K)	b: T41 > 20K (夜间 10K)
		// X： T4 > 360K (夜间 330K)
		// Z:  最后还要考虑太阳耀斑
		//
		// 如果满足以下条件，就可以将该点确认为火点：
		// 1. 非耀斑
		// 2. (A or B) and (a or b) or (X)
		// 
		// 火点(需要AVHRR通道1,2,3,4和3,4背景温度)
		// 条件 B和b 是常规方法的条件, 根据综合条件公式(A or B) and (a or b) or (X)
		// 本方法监测出的火点数量应该不少于常规方法监测出的火点数量
		//-------------------------------------------------
		// [A]: T4 > T4b + 4 * δT4b
		// T4		-> 背景窗口中中心波长为 4um通道温度(K)
		// T4b		-> 背景窗口中中心波长为 4um通道背景温度(K)
		// δT4b	-> 背景窗口中T4b的标准偏差,如果小于2K用2K替代

		// [a]: ΔT41 > ΔT41b + 4 * δT41b 
		// ΔT41	-> T4 - T11
		// ΔT41b	-> T4b - T11b
		// δT41b	-> 背景窗口中T41b的标准偏差

		// [B]: T4 > 320K (夜间 315K)
		// T4		-> 背景窗口中中心波长为 4um通道温度(K)

		// [b]: T41 > 20K (夜间 10K)
		// T41		-> T4 - T11

		// [X]: T4 > 360K (夜间 330K)
		// T4		-> 背景窗口中中心波长为 4um通道温度(K)

		// [Z]: 最后还要考虑太阳耀斑（）
		//      0.64um 和 0.86um 两个通道的反射率都大于 30%
		//      耀斑角小于40度

		// T11		-> 中心波长为 11um通道温度(K)
		// T11b		-> 中心波长为 11um通道背景温度(K)
		//-------------------------------------------------
		if((fireMethod == 2 || fireMethod == 3) && (iPixelType == PixelType::Land || iPixelType == PixelType::Cloud) && !bNotFire )
		{
			//---------------------------------------------
			// 定义足够大数组
			int na = 5000;			// 数组维数(定义足够大数组)
			double t4a[5000];		// T4		背景温度数组
			double t11a[5000];		// T11		背景温度数组
			double t41a[5000];		// T4-T11	背景温度数组
			memset(t4a, 0,sizeof(double)*na);
			memset(t11a,0,sizeof(double)*na);
			memset(t41a,0,sizeof(double)*na);

			// 背景窗口尺寸
			int np = 20;			// 背景行列数 np*np = 400
			int r0 = i-np;			// 开始行
			if(r0<0) r0 = 0;
			int c0 = j-np;			// 开始列
			if(c0<0) c0 = 0;

			// 取得T4和T11的背景温度
			GetBackgroundTemperature(t4a, na,ch3,r0,c0,np,np);	// 取得T4的背景温度
			GetBackgroundTemperature(t11a,na,ch4,r0,c0,np,np);	// 取得T11的背景温度
			//---------------------------------------------

			// 排除火点
			// 提取背景信息时滤除火点的条件为： ΔT 41 = T 4 -T 11 ＞ 20 K （夜间为 10K ）； T 4 ＞ 320K （ 315K ）。
			// ...

			// 计算量个通道亮温差
			for(i=0;i<np*np;i++)
			{
				t41a[i] = t4a[i] - t11a[i];
			}

			// T4、T4b、δT4b
			double T4 = dDataCh3;
			double T4b = KStatistic::RsAvg(t4a,na);		// 背景温度平均值 - T4b
			double detT4b = KStatistic::RsSd(t4a,na);	// 背景温度标准偏差 - δT4b
			if(detT4b<2) detT4b = 2;					// 

			// T11、T11b、δT11b
			double T11 = dDataCh4;
			double T11b = KStatistic::RsAvg(t11a,na);
			double detT11b = KStatistic::RsSd(t11a,na);
			if(detT11b<2) detT11b = 2;

			// T41、T41b、δT41b
			double T41 = T4 - T11;
			double T41b = KStatistic::RsAvg(t41a,na);	// 背景温度平均值 - T41b
			double detT41b = KStatistic::RsSd(t41a,na);	// 背景温度标准偏差 - δT41b
			if(detT41b<2) detT41b = 2;
			//---------------------------------------------
			// 1.先判断明火点
			BOOL bFireNaked	= FALSE;
			if(bDay)	// 白天
			{
				// 通道数据1,2,3,4(1,2通道用于耀斑排除)
				BOOL bHasData = ch1>=0 && ch2>=0 && ch3>=0 && ch4>=0;
				if(bHasData)
				{
					// A. T4 > T4b + 4 * δT4b
					BOOL bCondition_A = T4 > T4b + 4 * detT4b;

					// a. ΔT41 > ΔT41b + 4 * δT41b 
					BOOL bCondition_a = T41 > T41b + 4 * detT41b;

					// B. T4 > 320K (夜间 315K)
					BOOL bCondition_B = T4 > threshold.dFiringT3930nm;
					if(fireMethod == 2) bCondition_B = FALSE;

					// b. T41 > 20K (夜间 10K)
					BOOL bCondition_b = T41 > threshold.dFiringT3930nm_T11um;
					if(fireMethod == 2) bCondition_b = FALSE;

					// X. T4 > 360K (夜间 330K)
					BOOL bCondition_X = T4 > 360;

					// 综合条件
					//（ A or B ） and （ a or b ） or （ X ） 
					bFireNaked = (bCondition_A || bCondition_B) && (bCondition_a || bCondition_b) || bCondition_X;

					// Z. 太阳耀斑滤除
					//    0.64um 和 0.86um 两个通道的反射率都大于 30%
					//    耀斑角小于40度
					//BOOL bSolarFlare = dDataCh1 > 30 && dDataCh2 > 30;
					BOOL bSolarFlare = dDataCh1 > threshold.dFiringRed && dDataCh2 > threshold.dFiringNir;
					// 滤除耀斑
					bFireNaked = bFireNaked && !bSolarFlare;
				}
			}
			else	// 晚上
			{
				// 通道数据3,4
				BOOL bHasData = ch3>=0 && ch4>=0;
				if(bHasData)
				{
					// A. T4 > T4b + 4 * δT4b
					BOOL bCondition_A = T4 > T4b + 4 * detT4b;

					// a. ΔT41 > ΔT41b + 4 * δT41b 
					BOOL bCondition_a = T41 > T41b + 4 * detT41b;

					// B. T4 > 320K (夜间 315K)
					BOOL bCondition_B = T4 > threshold.dNightFiringT3930nm;

					// b. T41 > 20K (夜间 10K)
					BOOL bCondition_b = T41 > threshold.dNightFiringT3930nm_T11um;

					// X. T4 > 360K (夜间 330K)
					BOOL bCondition_X = T4 > 330;

					// 综合条件
					//（ A or B ） and （ a or b ） or （ X ） 
					bFireNaked = (bCondition_A || bCondition_B) && (bCondition_a || bCondition_b) || bCondition_X;
				}
			}

			// 2.再判断暗火点
			BOOL bFireDark	= FALSE;
			if(bFireNaked)
			{
				// waiting...
			}

			// 3.再判断过火点
			BOOL bFireEmber	= FALSE;
			if(bFireNaked && !bFireDark)
			{
				// waiting...
			}

			// 赋值像素类型
			if(bFireNaked)
				iPixelType = PixelType::FireNaked;	// 明火
			else if(bFireEmber)
				iPixelType = PixelType::FireEmber;	// 过火
			else if(bFireDark)
				iPixelType = PixelType::FireDark;	// 暗火
			else
				iPixelType = iPixelType;			// 不变
		}
		//-------------------------------------------------
		// 4. 积雪判断
		//-------------------------------------------------
		return iPixelType;
	}

	// 产品生成 - 植被指数
	// 在(0,1)之间
	BOOL KAvhrrDataProcessor::Vix(int iProductID)
	{
		int i,j;

		// 数据对象
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		// 数据对象
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// 数据指针
		PWORD* ppwData = pAvhrrData->GetData();
		if(!ppwData)
			return FALSE;

		// 数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 通道数据
		WORD wDataCh[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];	// AGMRS_MODIS_DATA_CHANNEL_NUM_MAX=40
		memset(&wDataCh[0],0,sizeof(WORD)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		// 通道号(-1表示不存在)
		int ch[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		// 设置数据通道的内存通道
		for(i=1;i<=36;i++)
		{
			int ch38 = pAvhrrData->Channel2ChannelCode(i);
			ch[i] = pAvhrrData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// 算法对象
		KAvhrrAlgorithm oAvhrrAlgorithm;
		oAvhrrAlgorithm.SetData(&wDataCh[0]);	// 为算法设置计算的数据

		// 进度条参数
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		// 检查需要的通道
		BOOL bChannelOk = FALSE;
		_tstring strInfo = _T("");
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_VIX_NDVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("计算 NDVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_DVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("计算 DVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_RVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("计算 RVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_SAVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("计算 SAVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_EVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[3]>=0);
			strInfo = _T("计算 EVI ...");
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
		WORD wDataCh1(0),wDataCh2(0),wDataCh3(0);
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		//=================================================
		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================

		double dBaseValue = 0;
		double dScale = 1.0;

		if(pProgressParam) { pProgressParam->SetInfo(strInfo.c_str()); }
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				bValid = FALSE;
				dValue = AGMRS_PRODUCT_DATA_INVALID;

				//升降轨标记, 1: 升轨, 0: 降轨
				//int ii = i;
				int ii = iHeight - 1 - i;

				int pos = ii*iWidth+j;

				// 取得通道数据
				wDataCh1	= ppwData[ch[1]][pos] & 0x0FFF;			wDataCh1 = wDataCh1>=0x0FFE ? 0 : wDataCh1;
				wDataCh2	= ppwData[ch[2]][pos] & 0x0FFF;			wDataCh2 = wDataCh2>=0x0FFE ? 0 : wDataCh2; 
				if(AGMRS_PRODUCT_CODE_VIX_EVI==iProductID)
				{
					wDataCh3	= ppwData[ch[3]][pos] & 0x0FFF;		wDataCh3 = wDataCh3>=0x0FFE ? 0 : wDataCh3; 
				}

				// 无效数据
				if(wDataCh1==0x0FFF || wDataCh2==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
					continue;
				}
				if(wDataCh1==0x0FFE || wDataCh2==0x0FFE)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
					continue;
				}

				// 为算法提供数据
				wDataCh[1]	= wDataCh1;
				wDataCh[2]	= wDataCh2;
				wDataCh[3]	= wDataCh3;

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
							dBaseValue = 0.0; dScale = 1.0;
							dValue = oAvhrrAlgorithm.Ndvi();
							break;

						case AGMRS_PRODUCT_CODE_VIX_DVI:
							dBaseValue = 0.0; dScale = 1.0;
							dValue = oAvhrrAlgorithm.Dvi();
							break;

						case AGMRS_PRODUCT_CODE_VIX_RVI:
							dBaseValue = 0.0; dScale = 1.0;
							dValue = oAvhrrAlgorithm.Rvi();
							break;
						}

						// 是否有效
						bValid = (dValue>0 && dValue<1);
						if(bValid)
						{
							//=============================
							dtmin = min(dtmin,dValue);
							dtmax = max(dtmax,dValue);
							//=============================
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

		}// for(i)

		///------------------------------------------------
		// 填充产品数据信息
		KDataInfo* pProductDataInfo = pAvhrrData->GetProductDataInfo();

		// 1.格式代码
		pProductDataInfo->wFormatCode	= AXIN_FMT_CODE_GRID_IMAGE;			// important

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
		_stprintf(szTemp,_T("%s"),pAvhrrData->NameProduct(iProductID));
		pProductDataInfo->SetComment(szTemp);


		// 6. 在外部设置调色板和调色板类型
		// ...
		///------------------------------------------------

		return TRUE;
	}

	// 产品生成 - 单项监测 - SIM (Single Item Monitor)
	BOOL KAvhrrDataProcessor::Sim(int iProductID)
	{
		int i,j;

		// 数据对象
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		// 数据信息
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// 数据指针
		PWORD* ppwData = pAvhrrData->GetData();
		if(!ppwData)
			return FALSE;

		// 数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 通道数量
		int iChannelNums = pDataInfo->ChannelNumbers();

		// 通道数据
		WORD wDataCh[AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX];
		memset(&wDataCh[0],0,sizeof(WORD)*(AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX));

		// 通道号(-1表示不存在)
		int ch[AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX));

		// 设置数据通道的内存通道
		for(i=1;i<=AGMRS_AVHRR_DATA_CHANNEL_NUM;i++)
		{
			int iChannelCode = pAvhrrData->Channel2ChannelCode(i);
			ch[i] = pAvhrrData->GetDataIndex(iChannelCode);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// 算法对象
		KAvhrrAlgorithm oAvhrrAlgorithm;
		oAvhrrAlgorithm.SetData(&wDataCh[0]);	// 为算法设置计算的数据

		// 处理信息
		TCHAR szInfo[MAX_PATH] = _T("");
		// 进度条参数
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();


		// 检查需要的通道
		BOOL bChannelOk = FALSE; 
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_MISC_CLOUD:	// 云
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_FOG:	// 雾
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_SNOW:	// 雪
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_ICE:	// 冰
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_FIRE:	// 火点
			{
				_stprintf(szInfo,_T("火点监测产品 ..."));
				if(pAvhrrData->IsDay())
					bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[3]>=0) && (ch[4]>=0);
				else
					bChannelOk = (ch[3]>=0) && (ch[4]>=0);
			}
			break;

		case AGMRS_PRODUCT_CODE_MISC_WATER:	// 水体
			//bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			break;
		}

		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("所需通道数据不全! -- Sim"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		//为产品数据分配内存
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float)*iWidth*iHeight);

		// 分配火点集合内存
		FirePointVector* pFirePointVector = NULL;
		if(iProductID == AGMRS_PRODUCT_CODE_MISC_FIRE)
		{
			_delete(m_pFirePointVector);
			m_pFirePointVector = new FirePointVector();
			pFirePointVector = m_pFirePointVector;
		}

		// 计算植被指数
		WORD wDataCh1(0),wDataCh2(0),wDataCh3(0),wDataCh4(0),wDataCh5(0);
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		int iFireCount = 0;		// 火点数量
		int iSnowCount = 0;		// 雪点数量

		double lon =0 ;
		double lat = 0;

		double dBaseValue = 0;
		double dScale = 1.0;

		if(pProgressParam) pProgressParam->SetInfo(szInfo);
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
				bValid = FALSE;
				dValue = AGMRS_PRODUCT_DATA_INVALID;

				//升降轨标记, 1: 升轨, 0: 降轨
				//int ii = i;
				int ii = iHeight - 1 - i;
				int pos = ii*iWidth+j;

				// 取得通道数据
				wDataCh1	= ppwData[ch[1]][pos] & 0x0FFF;
				wDataCh2	= ppwData[ch[2]][pos] & 0x0FFF;
				wDataCh3	= ppwData[ch[3]][pos] & 0x0FFF;
				wDataCh4	= ppwData[ch[4]][pos] & 0x0FFF;
				wDataCh5	= ppwData[ch[5]][pos] & 0x0FFF;
				if(iProductID == AGMRS_PRODUCT_CODE_MISC_CLOUD)
				{
				}

				if(iProductID == AGMRS_PRODUCT_CODE_MISC_FIRE)
				{
					wDataCh3	= ppwData[ch[3]][pos] & 0x0FFF;
					wDataCh4	= ppwData[ch[4]][pos] & 0x0FFF;
				}
				if(iProductID == AGMRS_PRODUCT_CODE_MISC_SNOW)
				{
				}

				// 无效数据
				if(wDataCh1==0x0FFF || wDataCh2==0x0FFF || wDataCh3==0x0FFF || wDataCh4==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
					continue;
				}

				// 为积雪指数算法提供数据
				//wDataCh[4]	= wDataCh4;
				//wDataCh[6]	= wDataCh6;

				// 取得像元点类型
				int iPixelType = this->GetPixelType(ii,j);		// y,x

				switch(iPixelType)
				{
				case PixelType::Cloud:		// 云 
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_CLOUD)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					}
					break;

				case PixelType::Fog:		// 雾
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_FOG)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_FOG;
					}
					break;
				case PixelType::FireNaked:		// 明火点
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE)
					{
						iFireCount++;
						m_ppfProduct[i][j] = FireType::FireNaked;	// 明火 1

						// 格点转换为经纬度(i,j)是产品行列
						pAvhrrData->xy2LonLat(j,i,lon,lat,1);

						// 火点
						FirePoint firePoint;
						firePoint.Id = iFireCount;				// id
						firePoint.Type = FireType::FireNaked;	// 类型
						firePoint.Row = i;
						firePoint.Col = j;
						firePoint.Lon = (float)lon;
						firePoint.Lat = (float)lat;
						firePoint.Temperature = 0.1f * wDataCh3;	// 温度
						firePoint.Area = 0.1f * wDataCh3 / 515;		// 面积(计算亚象元面积)
						firePoint.Group = 0;
						firePoint.GroupName = _T("Group_0");
						firePoint.Flag = 0;
						pFirePointVector->push_back(firePoint);
					}
					break;

				case PixelType::FireEmber:		// 过火点
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE)
					{
						iFireCount++;
						m_ppfProduct[i][j] = FireType::FireEmber;

						// 格点转换为经纬度(i,j)是产品行列
						pAvhrrData->xy2LonLat(j,i,lon,lat,1);

						// 火点
						FirePoint firePoint;
						firePoint.Id = iFireCount;				// id
						firePoint.Type = FireType::FireEmber;	// 类型
						firePoint.Row = i;
						firePoint.Col = j;
						firePoint.Lon = (float)lon;
						firePoint.Lat = (float)lat;
						firePoint.Temperature = 0.1f * wDataCh4;	// 温度
						firePoint.Area = 0.1f * wDataCh4 / 500;	// 面积(计算亚象元面积)
						firePoint.Group = 0;
						firePoint.GroupName = _T("Group_0");
						firePoint.Flag = 0;
						pFirePointVector->push_back(firePoint);
					}
					break;

				case PixelType::Snow:		// 雪
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_SNOW)
					{
						iSnowCount++;
						dBaseValue = 0.0; dScale = 1.0;
						dValue = oAvhrrAlgorithm.Ndsi();	// 积雪指数
						m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);		// trueValue = value / dScale + dBaseValue;
					}
					break;

				case PixelType::Ice:		// 冰
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_SNOW)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_ICE;
					}
					break;

				case PixelType::Water:		// 水 
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_WATER)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					}
					break;

				case PixelType::Land:		// 陆地
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

		if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE && iFireCount==0)
		{
			::MessageBox(NULL,_T("没有火点"),_T("信息"),MB_OK);
			return FALSE;
		}
		else if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE)
		{
			// 火点编组(根据火点行列值分组，行或列值相邻的组号相同)
			this->Grouping(pFirePointVector);
		}

		if(iProductID==AGMRS_PRODUCT_CODE_MISC_SNOW && iSnowCount==0)
		{
			::MessageBox(NULL,_T("没有积雪"),_T("信息"),MB_OK);
			return FALSE;
		}

		///------------------------------------------------
		// 填充产品数据信息
		KDataInfo* pProductDataInfo = pAvhrrData->GetProductDataInfo();

		// 1.格式代码
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;				// important

		// 2.日期
		pProductDataInfo->wYear				= pDataInfo->Year();
		pProductDataInfo->wMonth			= pDataInfo->Month();
		pProductDataInfo->wDay				= pDataInfo->Day();
		pProductDataInfo->wHour				= pDataInfo->Hour();
		pProductDataInfo->wMinute			= pDataInfo->Minute();
		pProductDataInfo->wSecond			= 0;
		pProductDataInfo->dwMillisecond		= 0;

		// 3.数据描述
		pProductDataInfo->wElementCode		= 0;	//ElementCode_Index;
		pProductDataInfo->wProductCode		= iProductID;
		pProductDataInfo->wDataType			= DataType_Float;
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
		KString strPathName(pAvhrrData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileName = strPathName.Mid(pos+1);		// 取得文件名(无路径)

		KString strProduct(_T(""));
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_MISC_CLOUD:
			strProduct = _T("cloud");
			break;

		case AGMRS_PRODUCT_CODE_MISC_WATER:
			strProduct = _T("water");
			break;

		case AGMRS_PRODUCT_CODE_MISC_FIRE:
			strProduct = _T("fire");
			break;

		case AGMRS_PRODUCT_CODE_MISC_FOG:
			strProduct = _T("fog");
			break;

		case AGMRS_PRODUCT_CODE_MISC_SNOW:
			strProduct = _T("snow");
			break;
		}

		// 注释信息
		TCHAR szComment[MAX_PATH];
		_stprintf(szComment,_T("%s_%4d%02d%02d.%02d%02d%02d.000.024.%d")
			,strProduct.GetString()
			,pDataInfo->Year()
			,pDataInfo->Month()
			,pDataInfo->Day()
			,pDataInfo->Hour()
			,pDataInfo->Minute()
			,pDataInfo->Second()
			,iProductID
			);
		pProductDataInfo->SetComment(szComment);

		// 火点
		if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE && pFirePointVector && pFirePointVector->size() != 0)
		{
			pProductDataInfo->TotalNum()		= pFirePointVector->size();
			pProductDataInfo->ElementNumbers()	= 4;
			pProductDataInfo->Flag()			= 1;
			pProductDataInfo->ContourNumbers()	= 0;
			pProductDataInfo->ClipArea()		= 0;
		}

		// 6. 在外部设置调色板和调色板类型
		// ...

		///------------------------------------------------

		return TRUE;
	}

}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------
