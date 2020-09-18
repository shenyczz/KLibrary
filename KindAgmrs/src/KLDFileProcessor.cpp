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

	// ����ת��Ϊ DIB
	BOOL KLDFileProcessor::DataToDIB(int iRChannel, int iGChannel , int iBChannel)
	{
		int i,j;

		// ȡ�����ݶ���
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		if(!pAgmrsData)
			return FALSE;

		// ���ݿ�
		if(pAgmrsData->IsEmpty())
			return FALSE;

		// ������Ϣ
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();
		if(!pDataInfo)
			return FALSE;

		// ����ת��Ϊ�Ҷ�
		this->Data2Gray();

		// �����ں�ͨ��ֵ
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

		// ѡ������ͨ��
		pAgmrsData->GetDataChannel(&chr,&chg,&chb);

		// ͨ����ת��Ϊͨ������
		int chrCode = pAgmrsData->Channel2ChannelCode(chr);
		int chgCode = pAgmrsData->Channel2ChannelCode(chg);
		int chbCode = pAgmrsData->Channel2ChannelCode(chb);

		// ͨ��ͨ������ת��Ϊ��������
		int rDataIndex = pAgmrsData->GetDataIndex(chrCode);
		int gDataIndex = pAgmrsData->GetDataIndex(chgCode);
		int bDataIndex = pAgmrsData->GetDataIndex(chbCode);

		// ͨ������ = [0,19]
		if(rDataIndex<0 || gDataIndex<0 || bDataIndex<0)
			return FALSE;

		//���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// 24λ DIB ÿ�е��ֽ�
		DWORD dwBytesPerLine = DIBWIDTHBYTES(iWidth * 8 * 3);

		// ���ݳߴ�(24λ)
		DWORD dwSizeImage = dwBytesPerLine * iHeight;

		// ����DIB ��С(�޵�ɫ������)
		DWORD dwDIBSize = sizeof(BITMAPINFOHEADER) + 0 + dwSizeImage;

		// һ�η��� DIB �ڴ�
		PBYTE pDib = new BYTE[dwDIBSize];
		memset(pDib, 0, sizeof(BYTE) * dwDIBSize);

		// ���� BITMAPINFOHRADER
		PBITMAPINFO pBitmapInfo = (PBITMAPINFO)pDib;
		pBitmapInfo->bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);
		pBitmapInfo->bmiHeader.biWidth			= iWidth;
		pBitmapInfo->bmiHeader.biHeight			= iHeight;
		pBitmapInfo->bmiHeader.biPlanes			= 1;
		pBitmapInfo->bmiHeader.biBitCount		= 24;
		pBitmapInfo->bmiHeader.biCompression	= BI_RGB;
		pBitmapInfo->bmiHeader.biSizeImage		= dwSizeImage;	//��BI_RGB��ʽʱ���Ϊ0
		pBitmapInfo->bmiHeader.biXPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biYPelsPerMeter	= 0;
		pBitmapInfo->bmiHeader.biClrUsed		= 0;
		pBitmapInfo->bmiHeader.biClrImportant	= 0;

		// DIB ����ָ��
		PBYTE pDIBits = pDib + sizeof(BITMAPINFOHEADER) + 0 ;

		// ���ݺͻҶ�
		PWORD* ppwData = pAgmrsData->GetData();
		PBYTE* ppucDataGray = pAgmrsData->GetDataGray();

		// ͨ������ת��ΪDIB
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) { pProgressParam->SetInfo(_T("ͨ������ת��ΪDIB...")); }
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

				//��������, 1: ����, 0: ����
				int ii = (iHeight-1-i);

				*(pDIBits + ii*dwBytesPerLine+j*3 + 2) = byRGray;	// red
				*(pDIBits + ii*dwBytesPerLine+j*3 + 1) = byGGray;	// green
				*(pDIBits + ii*dwBytesPerLine+j*3 + 0) = byBGray;	// blue

			}//for(j)

			// ��ʾ����
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

		// ������ KDib ��
		if(!m_pDib_org)
		{
			m_pDib_org = new KDib();
		}
		m_pDib_org->AttachMemory(pDib,TRUE);	// TRUE - �� KDib �ͷ��ڴ�

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

	// ����ת��Ϊ�Ҷ�
	void KLDFileProcessor::Data2Gray()
	{
		// ����ͨ������ת��Ϊ�Ҷ����ݵĲ���
		CalcData2GrayParam();

		// ȡ�����ݶ���
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// ͨ����
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ͨ������ת��Ϊ�Ҷ�ֵ
		for(int ch=0;ch<iChannelNums;ch++)
		{
			ChannelData2Gray(&m_pData2GrayParam[ch]);
		}

		return;
	}

	// ����ͨ������ת��Ϊ�Ҷ����ݵĲ���
	void KLDFileProcessor::CalcData2GrayParam()
	{
		int i,ch,value;

		// ȡ�����ݶ���
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		int iAvailPoint = 0;		// ��Ч����
		WORD wMaxValue = 0;			// ���ֵ
		WORD wMinValue = 0xFFFF;	// ��Сֵ

		WORD wUpThreshold = AGMRS_MERSI_DATA_VALUE_MAX;	// ͨ����������ֵ
		WORD wDnThreshold = 0;							// ͨ����������ֵ

		// ͨ����
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ����ͨ�������ֽ���
		int iDataSizePerChannel = pDataInfo->Width() * pDataInfo->Height();

		// ͨ��ֵ�ܶ�
		int* iValueIntensity = new int[AGMRS_MERSI_DATA_VALUE_MAX+10];

		// �洢�ۼƸ���
		float* fProbability = new float[AGMRS_MERSI_DATA_VALUE_MAX+10];

		// ��ʼ��ͨ������ת��Ϊ�Ҷ����ݵĲ���
		memset(m_pData2GrayParam, 0, sizeof(Data2GrayParam) * AGMRS_MERSI_DATA_CHANNEL_NUMBERS);

		// ͨ������ָ��
		PWORD* ppwData = pAgmrsData->GetData();

		// �����ͨ������
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) pProgressParam->SetInfo(_T("ͨ������ת��Ϊ�Ҷ�ֵ..."));
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
				if(wData < AGMRS_MERSI_DATA_VALUE_MAX)			//0x0fff = 4095 ��Чֵ
				{
					// ��¼���ֵ����Сֵ
					wMaxValue = wData>wMaxValue ? wData : wMaxValue;
					wMinValue = wData<wMinValue ? wData : wMinValue;

					// ��Ӧͨ��ֵ������ +1
					iValueIntensity[wData]++;	// ͨ��ֵ�ܶ�
				}

			}// next i

			// ��Ч���ݵ�����
			iAvailPoint = iDataSizePerChannel - iValueIntensity[0];	// ��ȥֵΪ0�ĵ�

			//�����ۼƸ���
			fProbability[0] = 0.0f;
			fProbability[1] = (float)iValueIntensity[1] / iAvailPoint;	// ������������Ч����
			for(value=2; value<=AGMRS_MERSI_DATA_VALUE_MAX; value++)
			{
				fProbability[value] = fProbability[value-1] + (float)iValueIntensity[value] / iAvailPoint;
			}

			// ���˵������١��ǳ�С��ֵ���õ�ͨ����������ֵ
			for(value=0; value<AGMRS_MERSI_DATA_VALUE_MAX; value++)
			{
				if(fProbability[value] > 0.012)
				{
					wDnThreshold = value; // ͨ����������ֵ
					break;
				}
			}

			// ���˵������١�ֵ�ܴ��ֵ���õ�ͨ����������ֵ
			for(value=AGMRS_MERSI_DATA_VALUE_MAX; value>=0; value--)
			{
				if(fProbability[value] < 0.92)
				{
					wUpThreshold = value; // ͨ����������ֵ
					break;
				}
			}

			// ͨ������ת��Ϊ�Ҷ����ݵĲ���
			m_pData2GrayParam[ch].iChannel			= ch;			// ͨ����
			m_pData2GrayParam[ch].iEnhanceType		= 2;			// ��ǿ��ʽ(0-ֱ�ߣ�1-ָ����2-���� ...)
			m_pData2GrayParam[ch].wMinValue			= wMinValue;	// ��Сֵ
			m_pData2GrayParam[ch].wMaxValue			= wMaxValue;	// ���ֵ
			m_pData2GrayParam[ch].wThreshold_min	= wDnThreshold;	// ȥ��һЩСֵ��ļ�Сֵ(ͨ����������ֵ)
			m_pData2GrayParam[ch].wThreshold_max	= wUpThreshold;	// ȥ��һЩ��ֵ��ļ���ֵ(ͨ����������ֵ)
			m_pData2GrayParam[ch].cMinGray			= 50;			// ��С�Ҷ�
			m_pData2GrayParam[ch].cMaxGray			= 255;			// ���Ҷ�
			m_pData2GrayParam[ch].cLessDownGray		= 50;			// ����С�Ҷ�
			m_pData2GrayParam[ch].cExceedUpGray		= 255;			// �����Ҷ�
			m_pData2GrayParam[ch].bAntiPhase		= FALSE;		// �Ƿ���

			// ��MODIS����
			if(pAgmrsData->IsModisData())
			{
				if(ch >= 19)
				{// ����
					m_pData2GrayParam[ch].bAntiPhase = TRUE;
				}

				if(ch == 24)
				{
					m_pData2GrayParam[ch].iEnhanceType = 1;		// ��ǿ��ʽ(0-ֱ�ߣ�1-ָ����2-���� ...)
					m_pData2GrayParam[ch].bAntiPhase = FALSE;
				}
			}

			if(m_pData2GrayParam[ch].bAntiPhase)
			{//����
				m_pData2GrayParam[ch].cLessDownGray = m_pData2GrayParam[ch].cMaxGray;		// ����С�Ҷ�
				m_pData2GrayParam[ch].cExceedUpGray = m_pData2GrayParam[ch].cMinGray;		// �����Ҷ�
			}
			else
			{
				m_pData2GrayParam[ch].cLessDownGray = m_pData2GrayParam[ch].cMinGray;		// ����С�Ҷ�
				m_pData2GrayParam[ch].cExceedUpGray = m_pData2GrayParam[ch].cMaxGray;		// �����Ҷ�
			}

			// ��ʾ����
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

		// �ڴ����
		_deletea(iValueIntensity);
		_deletea(fProbability);

		return;
	}

	// ͨ������ת��Ϊ�Ҷ�
	void KLDFileProcessor::ChannelData2Gray(Data2GrayParam* pData2GrayParam)
	{
		int i,value;

		int ch = pData2GrayParam->iChannel;						// ͨ����
		int iEnhanceType = pData2GrayParam->iEnhanceType;		// ��ǿ��ʽ(0-ֱ�ߣ�1-ָ����2-���� ...)
		WORD wMinValue = pData2GrayParam->wMinValue;			// ��Сֵ
		WORD wMaxValue = pData2GrayParam->wMaxValue;			// ���ֵ
		WORD wDnThreshold = pData2GrayParam->wThreshold_min;	// ȥ��һЩСֵ��ļ�Сֵ
		WORD wUpThreshold = pData2GrayParam->wThreshold_max;	// ȥ��һЩ��ֵ��ļ���ֵ
		BYTE cMinGray = pData2GrayParam->cMinGray;				// ��С�Ҷ�
		BYTE cMaxGray = pData2GrayParam->cMaxGray;				// ���Ҷ�
		BYTE cLessDownGray = pData2GrayParam->cLessDownGray;	// ����С�Ҷ�
		BYTE cExceedUpGray = pData2GrayParam->cExceedUpGray;	// �����Ҷ�
		BOOL bAntiPhase = pData2GrayParam->bAntiPhase;			// �Ƿ���

		// ȡ�����ݶ���
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// ͨ������ָ��
		PWORD* ppwData = pAgmrsData->GetData();

		// ͨ�����ݻҶ�ָ��
		PBYTE* ppucDataGray = pAgmrsData->GetDataGray();
		memset(ppucDataGray[ch], cExceedUpGray, AGMRS_MERSI_DATA_VALUE_MAX);

		// ͨ�����ݳߴ�
		int iSizeOfData = pDataInfo->Width() * pDataInfo->Height();
		for(i=0; i<iSizeOfData; i++)
		{
			WORD wData = ppwData[ch][i] & 0x0FFF;
			//ͨ��ֵΪ0�ĻҶ�Ϊ0
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
				// ���������ǿ����
			}
		}

		WORD wUpValue = wUpThreshold;
		WORD wDownValue = wDnThreshold;

		//Assert(wUpValue-wDownValue>0);
		// �� DEBUG ��������ʱ������,���� Release ʱ����
		// �˷���һ��ʱ��

		double a,b;
		switch(iEnhanceType)
		{
		case 0:	// ������ǿ
			a = (double)(cMaxGray-cMinGray) / (wUpValue-wDownValue);
			b = (double)cMinGray-a*wDownValue;
			for(value=wDownValue; value<wUpValue+1; value++)
			{
				ppucDataGray[ch][value] = bAntiPhase ? cMaxGray-(BYTE)(a*value+b) : (BYTE)(a*value+b);
			}
			break;

		case 1:	// ָ����ǿ y=a*exp(b*x)  ,ȡ��(1,min),(255,max)���a,b
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

		case 2:	// ������ǿ
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

	// ��Ʒ���� - ֲ��ָ��
	// ��(0,1)֮��
	BOOL KLDFileProcessor::Vix(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		// ������Ϣ
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pAgmrsData->GetData();
		if(!ppwData)
			return FALSE;

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// �����㷨����
		WORD wDataAlg[AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX];
		memset(&wDataAlg[0],0,sizeof(WORD)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		KAgmrsAlgorithm* pAgmrsAlgorithm = (KAgmrsAlgorithm*)this->GetDataAlgorithm();
		pAgmrsAlgorithm->SetData(&wDataAlg[0]);

		//-------------------------------------------------
		if(iProductID == AGMRS_PRODUCT_CODE_VIX_LAI)
		{
			// �鿴LAI��������
			pAgmrsAlgorithm->DateTime = pDataInfo->DateTime();
			if(!pAgmrsAlgorithm->ConfigLai())
			{
				MessageBox(NULL,_T("ConfigLai error"),_T("LAI"),MB_OK);
				return FALSE;
			}
		}
		//-------------------------------------------------

		// ��������ͨ������������
		memset(&m_DataIndexOfChannel[0],-1,sizeof(int)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		for(i=1;i<=36;i++)	// 36��MODIS��������ν����
		{
			int iChannelCode = pAgmrsData->Channel2ChannelCode(i);
			m_DataIndexOfChannel[i] = pAgmrsData->GetDataIndex(iChannelCode);
		}

		// ���ú�⣬������⡢���Ⲩ�η���������ͨ��
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

		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE;
		_tstring strInfo = _T("");
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_VIX_NDVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("���� NDVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_DVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("���� DVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_RVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("���� RVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_SAVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("���� SAVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_EVI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0 && (m_DataIndexOfChannel[iBluChannel]>=0));
			strInfo = _T("���� EVI ...");
			break;

			// ֲ��ָ��������Ʒ
		case AGMRS_PRODUCT_CODE_VIX_LAI:
			bChannelOk = (m_DataIndexOfChannel[iRedChannel]>=0) && (m_DataIndexOfChannel[iNirChannel]>=0);
			strInfo = _T("���� LAI ...");
			break;
		}

		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Vix"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float)*iWidth*iHeight);

		// ����ֲ��ָ��
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

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
		if(pProgressParam) { pProgressParam->SetInfo(strInfo.c_str()); }

		// ����
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				bValid = FALSE;
				dValue = AGMRS_PRODUCT_DATA_INVALID;

				//��������, 1: ����, 0: ����
				//int ii = i;
				int ii = (iHeight - 1) - i;

				int pos = ii*iWidth+j;

				// ȡ��ͨ������
				wDataRed = ppwData[m_DataIndexOfChannel[iRedChannel]][pos] & 0x0FFF;
				wDataRed = wDataRed>=0x0FFE ? 0 : wDataRed;
				wDataNir = ppwData[m_DataIndexOfChannel[iNirChannel]][pos] & 0x0FFF;
				wDataNir = wDataNir>=0x0FFE ? 0 : wDataNir; 
				if(AGMRS_PRODUCT_CODE_VIX_EVI==iProductID)
				{
					wDataBlu = ppwData[m_DataIndexOfChannel[iBluChannel]][pos] & 0x0FFF;
					wDataBlu = wDataBlu>=0x0FFE ? 0 : wDataBlu; 
				}

				// ��Ч����
				if(wDataRed==0x0FFF || wDataNir==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}
				if(wDataRed==0x0FFE || wDataNir==0x0FFE)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}

				// Ϊ�㷨�ṩ����
				wDataAlg[KAgmrsAlgorithm::Red]	= wDataRed;
				wDataAlg[KAgmrsAlgorithm::Nir]	= wDataNir;
				wDataAlg[KAgmrsAlgorithm::Blu]	= wDataBlu;

				// ȡ����Ԫ������
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Land:	// ½��
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
							// ���ü���LAI�������
							// (1)ʱ��
							pAgmrsAlgorithm->DateTime = pDataInfo->DateTime();
							// (2)���̲���(��Ӧ�����ṩ)
							// pAgmrsAlgorithm->SetLaiConfig(KStringArray& lc);
							//-----------------------------

							dValue = pAgmrsAlgorithm->Lai();
							break;
						}

						// �Ƿ���Ч
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

				case PixelType::Water:		// ˮ
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					break;

				case PixelType::Cloud:		// ��
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					break;

				case PixelType::Unknown:	// δ֪ -9999
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					break;
				}

			}// for(j)

			// ��ʾ����
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
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode	= AXIN_FMT_CODE_GRID_IMAGE;	// important

		// 2.����
		pProductDataInfo->wYear			= pDataInfo->Year();
		pProductDataInfo->wMonth		= pDataInfo->Month();
		pProductDataInfo->wDay			= pDataInfo->Day();
		pProductDataInfo->wHour			= pDataInfo->Hour();
		pProductDataInfo->wMinute		= pDataInfo->Minute();
		pProductDataInfo->wSecond		= 0;
		pProductDataInfo->dwMillisecond	= 0;

		// 3.��������
		pProductDataInfo->wElementCode	= 0;
		pProductDataInfo->wProductCode	= iProductID;
		pProductDataInfo->wDataType		= DataType_Float;
		pProductDataInfo->InvalidData()	= AXIN_INVALID_DATA;
		pProductDataInfo->dBaseValue	= dBaseValue;
		pProductDataInfo->dScale		= dScale;

		// 4.������
		pProductDataInfo->xNum			= pDataInfo->Width();
		pProductDataInfo->xMin			= pDataInfo->MinLon();
		pProductDataInfo->xMax			= pDataInfo->MaxLon();
		pProductDataInfo->xInterval		= pDataInfo->LonInterval();

		pProductDataInfo->yNum			= pDataInfo->Height();
		pProductDataInfo->yMin			= pDataInfo->MinLat();
		pProductDataInfo->yMax			= pDataInfo->MaxLat();
		pProductDataInfo->yInterval		= pDataInfo->LatInterval();

		// ע����Ϣ
		TCHAR szTemp[MAX_PATH];
		_stprintf(szTemp,_T("%s"),pAgmrsData->NameProduct(iProductID));
		pProductDataInfo->SetComment(szTemp);


		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
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

	// �¶�ֲ���ɺ�ָ��
	// ��Ҫ NDVI �� LST
	// TVDI = (Ts-Tsmin) / (Tsmax-Tsmin)
	BOOL KLDFileProcessor::Dix_Tvdi(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KAgmrsData* pAgmrsData = (KAgmrsData*)GetOwner();
		// ���ݶ���
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pAgmrsData->GetData();
		if(!ppwData)
			return FALSE;

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ��������ͨ�����ڴ�ͨ��
		for(i=1;i<=36;i++)
		{
			int iChannelCode = pAgmrsData->Channel2ChannelCode(i);
			m_DataIndexOfChannel[i] = pAgmrsData->GetDataIndex(iChannelCode);
		}

		// �����㷨����
		WORD wDataAlg[AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX];
		memset(&wDataAlg[0],0,sizeof(WORD)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		KAgmrsAlgorithm* pAgmrsAlgorithm = (KAgmrsAlgorithm*)this->GetDataAlgorithm();
		pAgmrsAlgorithm->SetData(&wDataAlg[0]);

		// ��������ͨ������������
		memset(&m_DataIndexOfChannel[0],-1,sizeof(int)*(AGMRS_LOCAL_DATA_CHANNEL_NUM_MAX));
		for(i=1;i<=36;i++)	// 36��MODIS��������ν����
		{
			int iChannelCode = pAgmrsData->Channel2ChannelCode(i);
			m_DataIndexOfChannel[i] = pAgmrsData->GetDataIndex(iChannelCode);
		}

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		PFLOAT* ppfLst = NULL;
		PFLOAT* ppfNdvi = NULL;

		// 1.���� LST
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("���� LST ..."));
		if(this->Lst(LstMethod::Sobrino))
		{
			// �����ڴ汣�� LST ����
			if(!KFloat2DMemAllocator::AllocMemory2D(ppfLst,iHeight,iWidth))
			{
				::MessageBox(NULL, _T("�ڴ���ɴ���! -- Dix_Tvdi_01"),_T(""),MB_OK|MB_ICONINFORMATION);
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

		// 2.���� NDVI
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("���� NDVI ..."));
		if(this->Vix(AGMRS_PRODUCT_CODE_VIX_NDVI))
		{
			// �����ڴ汣�� NDVI ����
			if(!KFloat2DMemAllocator::AllocMemory2D(ppfNdvi,iHeight,iWidth))
			{
				::MessageBox(NULL, _T("�ڴ���ɴ���! -- Dix_Tvdi_02"),_T(""),MB_OK|MB_ICONINFORMATION);
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



		// 3.������Чֲ��ָ������
		//   ȡ�� ndvi [0.01,0.80]*100->[1,80] ��Χ�ڶ�Ӧ�� (i,j) ����
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("������Чֲ��ָ������..."));
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
						ptArray[v].push_back(pt);			// ����ȫ�� v ֵ������
					}
				}
			}

			// ��ʾ����
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

		// 4.����ֲ��ָ���Ͷ�Ӧ�ĵر��¶�
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("����ֲ��ָ���Ͷ�Ӧ�ĵر��¶�..."));
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

			// ��ʾ����
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

		// ������Ч��������
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

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			::MessageBox(NULL, _T("�ڴ���ɴ���! -- Dix_Tvdi_03"),_T(""),MB_OK|MB_ICONINFORMATION);
			KFloat2DMemAllocator::FreeMemory2D(ppfLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float) * iWidth * iHeight);

		// ���� TVDI
		//
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		double dBaseValue = 0;	// ����ֵ
		double dScale = 1.0;	// �Ŵ�ϵ��

#ifdef _DEBUG
		//=================================================
		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================
#endif

		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("�ɺ�ָ������..."));
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				bValid = FALSE;
				dValue = AGMRS_PRODUCT_DATA_INVALID;

				//��������, 1: ����, 0: ����
				int ii = iHeight - 1 - i;

				float fLst = ppfLst[i][j];
				float fNdvi = ppfNdvi[i][j];
				BOOL bLst = (fLst>0+273.15 && fLst<100+273.15);	// (0,100)
				BOOL bNdvi = fNdvi>0 && fNdvi<1.0;				// (0,1)

				//ȡ����Ԫ������
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
							{// ֵԽ��Խ�ɺ�
								dValue = (Ts - Tsmin) / (Tsmax - Tsmin);
							}
						}
						else
						{
							dValue = 0;
						}

						// �Ƿ���Ч
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

				case PixelType::Water:		// ˮ 
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					break;

				case PixelType::Cloud:		// �� 
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					break;

				case PixelType::Unknown:	// δ֪ 99999
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					break;
				}

			}// for(j)

			// ��ʾ����
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

		// �ͷ��ڴ�
		KFloat2DMemAllocator::FreeMemory2D(ppfLst);
		KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);
		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.����
		pProductDataInfo->wYear				= pDataInfo->wYear;
		pProductDataInfo->wMonth			= pDataInfo->wMonth;
		pProductDataInfo->wDay				= pDataInfo->wDay;
		pProductDataInfo->wHour				= pDataInfo->wHour;
		pProductDataInfo->wMinute			= pDataInfo->wMinute;
		pProductDataInfo->wSecond			= 0;
		pProductDataInfo->dwMillisecond		= 0;

		// 3.��������
		//pProductDataInfo->wElementCode	= eElementStyle_Index;			// important
		pProductDataInfo->wProductCode		= iProductID;
		pProductDataInfo->wDataType			= DataType_Float;				// important
		pProductDataInfo->InvalidData()		= AXIN_INVALID_DATA;
		pProductDataInfo->dBaseValue		= dBaseValue;
		pProductDataInfo->dScale			= dScale;

		// 4.������
		pProductDataInfo->xNum				= pDataInfo->Width();
		pProductDataInfo->xMin				= pDataInfo->MinLon();
		pProductDataInfo->xMax				= pDataInfo->MaxLon();
		pProductDataInfo->xInterval			= pDataInfo->LonInterval();

		pProductDataInfo->yNum				= pDataInfo->Height();
		pProductDataInfo->yMin				= pDataInfo->MinLat();
		pProductDataInfo->yMax				= pDataInfo->MaxLat();
		pProductDataInfo->yInterval			= pDataInfo->LatInterval();

		// 5.ע����Ϣ
		KString strPathName(pAgmrsData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileNameWithoutPath = strPathName.Mid(pos+1);		// ȡ���ļ���(��·��)
		KString strProduct(_T("tvdi"));

		TCHAR szComment[MAX_PATH];
		_stprintf(szComment,_T("DIX_%s(%d)_%s"),strProduct.GetString(), iProductID, strFileNameWithoutPath.GetString());
		pProductDataInfo->SetComment(szComment);

		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...

		///------------------------------------------------

		return TRUE;
	}


//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
