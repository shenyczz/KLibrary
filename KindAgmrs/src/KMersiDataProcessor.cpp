#include "StdAfx.h"
#include "KMersiDataProcessor.h"

#include "KMersiData.h"
#include "KMersiAlgorithm.h"

namespace Agmrs { namespace Mersi
{

	KMersiDataProcessor::KMersiDataProcessor(void)
	{
		this->m_pDataAlgorithm = new KMersiAlgorithm();
	}

	KMersiDataProcessor::~KMersiDataProcessor(void)
	{
	}

	// 像元类型
	// i - 行
	// j - 列
	int KMersiDataProcessor::GetPixelType(int i,int j)
	{
		int iPixelType = PixelType::Unknown;
		//-------------------------------------------------
		// 数据对象
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		// 数据信息
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();
		// 数据指针
		PWORD* ppwData = pAgmrsData->GetData();

		//数据尺寸
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		if(i>=iHeight || j>=iWidth)
		{
			return PixelType::Unknown;
		}

		// 判别阈值
		KMersiThreshold threshold = *((KMersiThreshold*)pAgmrsData->GetThreshold());

		// 通道数据索引
		int iDataIndexOfRed		= m_DataIndexOfChannel[MersiChannel::Red];		// %
		int iDataIndexOfNir		= m_DataIndexOfChannel[MersiChannel::Nir];		// %
		int iDataIndexOfGrn		= m_DataIndexOfChannel[MersiChannel::Grn];		// %
		int iDataIndexOfT11um	= m_DataIndexOfChannel[MersiChannel::T11um];	// K


		double dDataRed(0),dDataNir(0),dDataGrn(0);
		double dDataT11um(0);

		if(iDataIndexOfRed>=0)		dDataRed	= 1.0 * ppwData[iDataIndexOfRed][i*iWidth+j] / 10;		// %
		if(iDataIndexOfNir>=0)		dDataNir	= 1.0 * ppwData[iDataIndexOfNir][i*iWidth+j] / 10;		// %
		if(iDataIndexOfGrn>=0)		dDataGrn	= 1.0 * ppwData[iDataIndexOfGrn][i*iWidth+j] / 10;		// %
		if(iDataIndexOfT11um>=0)	dDataT11um	= 1.0 * ppwData[iDataIndexOfT11um][i*iWidth+j] / 10;	// K

		double dDataNir_Red = dDataNir - dDataRed;

		if (dDataRed > threshold.dCloudRed
			&& dDataNir_Red > threshold.dCloudNirRed
			&& dDataNir_Red < threshold.dCloudNirRedMax
			&& dDataT11um < threshold.dCloudT11um
			)
		{
			iPixelType = PixelType::Cloud;
		}
		else if (dDataNir_Red < threshold.dWaterNirRed
			&& dDataNir_Red <= threshold.dWaterNirRedMin
			&& dDataRed < threshold.dWaterRed
			&& dDataNir < threshold.dWaterNir
			)
		{
			iPixelType = PixelType::Water;
		}
		else if (dDataNir_Red < threshold.dWaterNirRed
			&& dDataNir_Red > threshold.dWaterNirRedMin
			)
		{
			iPixelType = PixelType::Cloud;
		}
		else
		{
			iPixelType = PixelType::Land;
		}
		//-------------------------------------------------
		return iPixelType;
	}

	// MERSI 使用普适性单通道算法
	BOOL KMersiDataProcessor::Lst(int iMethod)
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

		// 算法数据
		WORD wDataAlg[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&wDataAlg[0],0,sizeof(WORD)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));

		// 通道号
		int ch[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));

		for(i=1;i<=36;i++)
		{
			int iChannelCode = pAgmrsData->Channel2ChannelCode(i);
			ch[i] = pAgmrsData->GetDataIndex(iChannelCode);
			m_DataIndexOfChannel[i] = ch[i]; // 备份用于像元类型判别
		}

		// 算法对象
		KAgmrsAlgorithm* pAgmrsAlgorithm = (KAgmrsAlgorithm*)this->GetDataAlgorithm();
		pAgmrsAlgorithm->SetData(&wDataAlg[0]);

		// 判断通道是否存在
		BOOL bChannelOk = TRUE 
						  && ch[MersiChannel::Red]>=0
						  && ch[MersiChannel::Nir]>=0
						  && ch[MersiChannel::C940nm]>=0
						  && ch[MersiChannel::T11um]>=0
						  ;
		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("所需通道数据不全! -- Lst"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		//为产品数据分配内存
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
			return FALSE;
		else
			memset(*m_ppfProduct,0,sizeof(float)*iWidth*iHeight);

		// 计算地表温度
		WORD wDataRed(0),wDataNir(0);
		WORD wDataT11um(0);
		WORD wDataC940(0);	// 0.940um通道反射率 virr没有 0.940um通道
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		//=================================================
		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================

		double dBaseValue = -273.15;
		double dScale = 1.0;

		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("计算地表温度..."));
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

				wDataRed	= ppwData[ch[MersiChannel::Red]][pos] & 0x0FFF;		wDataRed	= wDataRed>=0xFFE ? 0 : wDataRed;
				wDataNir	= ppwData[ch[MersiChannel::Nir]][pos] & 0x0FFF;		wDataNir	= wDataNir>=0xFFE ? 0 : wDataNir;
				wDataC940 = ppwData[ch[MersiChannel::C940nm]][pos] & 0x0FFF;	wDataC940 = wDataC940 >= 0xFFE ? 0 : wDataC940;
				wDataT11um	= ppwData[ch[MersiChannel::T11um]][pos] & 0x0FFF;	wDataT11um	= wDataT11um>=0xFFE ? 0 : wDataT11um;

				// 无效数之
				if(wDataRed==0x0FFF || wDataNir==0x0FFF || wDataC940==0x0FFF || wDataT11um==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
					continue;
				}
				if(wDataRed==0x0FFE || wDataNir==0x0FFE || wDataC940==0x0FFE || wDataT11um==0x0FFE)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// 无效值
					continue;
				}

				// 为算法提供数据
				wDataAlg[KAgmrsAlgorithm::Red]	= wDataRed;
				wDataAlg[KAgmrsAlgorithm::Nir]	= wDataNir;
				wDataAlg[KAgmrsAlgorithm::C940]	= wDataC940;
				wDataAlg[KAgmrsAlgorithm::T11um] = wDataT11um;

				// 取得像元点类型
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Land:	// 陆地
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					dValue = pAgmrsAlgorithm->Lst(iMethod) - 273.15;		// 返回值是绝对温度,转换为摄氏度
					// 是否有效 (-20-100度)
					bValid = (dValue>-20 && dValue<100);
					if(bValid)
					{
						//=============================
						dtmin = min(dtmin,dValue);
						dtmax = max(dtmax,dValue);
						//=============================
						m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);
						// trueValue = value / dScale + dBaseValue;
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

		// 填充产品数据信息
		KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

		// 1.格式代码
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.日期
		pProductDataInfo->wYear			= pDataInfo->wYear;
		pProductDataInfo->wMonth		= pDataInfo->wMonth;
		pProductDataInfo->wDay			= pDataInfo->wDay;
		pProductDataInfo->wHour			= pDataInfo->wHour;
		pProductDataInfo->wMinute		= pDataInfo->wMinute;
		pProductDataInfo->wSecond		= 0;
		pProductDataInfo->dwMillisecond	= 0;

		// 3.数据描述
		//pProductDataInfo->wElementCode		= eElementStyle_Temperature_C;	// important
		pProductDataInfo->wProductCode	= AGMRS_PRODUCT_CODE_MISC_LST;
		pProductDataInfo->wDataType		= DataType_Float;				// important
		pProductDataInfo->InvalidData()	= AXIN_INVALID_DATA;
		pProductDataInfo->dBaseValue	= dBaseValue;
		pProductDataInfo->dScale		= dScale;

		// 4.格点参数(Important)
		pProductDataInfo->xNum			= pDataInfo->Width();
		pProductDataInfo->xMin			= pDataInfo->MinLon();
		pProductDataInfo->xMax			= pDataInfo->MaxLon();
		pProductDataInfo->xInterval		= pDataInfo->LonInterval();

		pProductDataInfo->yNum			= pDataInfo->Height();
		pProductDataInfo->yMin			= pDataInfo->MinLat();
		pProductDataInfo->yMax			= pDataInfo->MaxLat();
		pProductDataInfo->yInterval		= pDataInfo->LatInterval();

		// 5.注释信息(最好设置)
		KString strMethod(_T(""));
		int eMethod = iMethod;
		switch(eMethod)
		{
		case LstMethod::Qin:
			strMethod = _T("Qin");
			break;
		case LstMethod::Becker:
			strMethod = _T("Becker");
			break;
		case LstMethod::Sobrino:
			strMethod = _T("Sobrino");
			break;
		case LstMethod::Regress:
			strMethod = _T("Regress");
			break;
		}

		KString strPathName(pAgmrsData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileName = strPathName.Mid(pos+1);
		TCHAR szTemp[MAX_PATH];
		//_stprintf(szTemp,_T("LST_%s_%s"),strMethod.GetString(), strFileName.GetString());
		_stprintf(szTemp,_T("LST_%s"), strFileName.GetString());
		pProductDataInfo->SetComment(szTemp);

		// 6. 在外部设置调色板(调色板类型, 颜色, 注释, 调色板的透明色)(Important)
		// ...

		return TRUE;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------
