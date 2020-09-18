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

	// ����ת��Ϊ DIB
	BOOL KAvhrrDataProcessor::DataToDIB(int iRChannel,int iGChannel,int iBChannel)
	{
		int i,j;

		// ȡ�����ݶ���
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		if(!pAvhrrData)
			return FALSE;

		// ���ݿ�
		if(pAvhrrData->IsEmpty())
			return FALSE;

		// ������Ϣ
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();
		if(!pDataInfo)
			return FALSE;

		// ����ת��Ϊ�Ҷ�
		this->Data2Gray();

		// �����ں�ͨ��ֵ
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

		// ѡ������ͨ��
		pAvhrrData->GetDataChannel(&chr,&chg,&chb);

		// ͨ����ת��Ϊͨ������
		int chrCode = pAvhrrData->Channel2ChannelCode(chr);
		int chgCode = pAvhrrData->Channel2ChannelCode(chg);
		int chbCode = pAvhrrData->Channel2ChannelCode(chb);

		// ͨ��ͨ������ת��Ϊ��������
		int rDataIndex = pAvhrrData->GetDataIndex(chrCode);
		int gDataIndex = pAvhrrData->GetDataIndex(chgCode);
		int bDataIndex = pAvhrrData->GetDataIndex(chbCode);

		// ͨ������ = [0,9]
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
		PWORD* ppwData = pAvhrrData->GetData();
		PBYTE* ppucDataGray = pAvhrrData->GetDataGray();

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
	void KAvhrrDataProcessor::Data2Gray()
	{
		// ����ͨ������ת��Ϊ�Ҷ����ݵĲ���
		CalcData2GrayParam();

		// ȡ�����ݶ���
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// ͨ����
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ͨ������ת��Ϊ�Ҷ�ֵ
		for(int ch=0;ch<iChannelNums;ch++)	// 0-19
		{
			ChannelData2Gray(&m_pData2GrayParam[ch]);
		}

		return;
	}

	// ����ͨ������ת��Ϊ�Ҷ����ݵĲ���
	void KAvhrrDataProcessor::CalcData2GrayParam()
	{
		int i,ch,value;

		// ȡ�����ݶ���
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		int iAvailPoint = 0;		// ��Ч����
		WORD wMaxValue = 0;			// ���ֵ
		WORD wMinValue = 0xFFFF;	// ��Сֵ

		WORD wUpThreshold = AGMRS_VIRR_DATA_VALUE_MAX;	// ͨ����������ֵ
		WORD wDnThreshold = 0;							// ͨ����������ֵ

		// ͨ����
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ����ͨ�������ֽ���
		int iDataSizePerChannel = pDataInfo->Width() * pDataInfo->Height();

		// ͨ��ֵ�ܶ�
		int* iValueIntensity = new int[AGMRS_VIRR_DATA_VALUE_MAX+10];

		// �洢�ۼƸ���
		float* fProbability = new float[AGMRS_VIRR_DATA_VALUE_MAX+10];

		// ��ʼ��ͨ������ת��Ϊ�Ҷ����ݵĲ���
		memset(m_pData2GrayParam, 0, sizeof(Data2GrayParam) * AGMRS_VIRR_DATA_CHANNEL_NUMBERS);

		// ͨ������ָ��
		PWORD* ppwData = pAvhrrData->GetData();

		// �����ͨ������
		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		if(pProgressParam) pProgressParam->SetInfo(_T("ͨ������ת��Ϊ�Ҷ�ֵ..."));
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
				if(wData < AGMRS_VIRR_DATA_VALUE_MAX)			//0x0fff = 4095 ��Чֵ
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
			for(value=2; value<=AGMRS_VIRR_DATA_VALUE_MAX; value++)
			{
				fProbability[value] = fProbability[value-1] + (float)iValueIntensity[value] / iAvailPoint;
			}

			// ���˵������١��ǳ�С��ֵ���õ�ͨ����������ֵ
			for(value=0; value<AGMRS_VIRR_DATA_VALUE_MAX; value++)
			{
				if(fProbability[value] > 0.012)
				{
					wDnThreshold = value; // ͨ����������ֵ
					break;
				}
			}

			// ���˵������١�ֵ�ܴ��ֵ���õ�ͨ����������ֵ
			for(value=AGMRS_VIRR_DATA_VALUE_MAX; value>=0; value--)
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

			/*
			if(ch >= 19)
			{// ����
				m_Data2GrayParam[ch].bAntiPhase = TRUE;
			}

			if(ch == 24)
			{
				m_Data2GrayParam[ch].iEnhanceType = 1;		// ��ǿ��ʽ(0-ֱ�ߣ�1-ָ����2-���� ...)
				m_Data2GrayParam[ch].bAntiPhase = FALSE;
			}
			*/

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
	void KAvhrrDataProcessor::ChannelData2Gray(Data2GrayParam* pData2GrayParam)
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
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// ͨ������ָ��
		PWORD* ppwData = pAvhrrData->GetData();

		// ͨ�����ݻҶ�ָ��
		PBYTE* ppucDataGray = pAvhrrData->GetDataGray();
		memset(ppucDataGray[ch], cExceedUpGray, AGMRS_VIRR_DATA_VALUE_MAX);

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

	// ��Ԫ����
	// PixelType::Cloud,	// ��
	// PixelType::Fog,		// ��
	// PixelType::Fire,		// ��
	// PixelType::Snow,		// ѩ
	// PixelType::Ice,		// ��
	// PixelType::Land,		// ½
	// PixelType::Water,	// ˮ(1,2)
	int KAvhrrDataProcessor::GetPixelType(int i,int j)
	{
		int iPixelType = PixelType::Unknown;
		//-------------------------------------------------
		// ���ݶ���
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		// ������Ϣ
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();
		// ����ָ��
		PWORD* ppwData = pAvhrrData->GetData();

		//���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		if(i<0 || j<0 || i>=iHeight || j>=iWidth)
			return PixelType::Unknown;

		// �б���ֵ
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
		// 1.���ж��ƺ�ˮ��
		if(m_DataIndexOfChannel[1]>=0 && m_DataIndexOfChannel[2]>=0 && m_DataIndexOfChannel[4]>=0)	// ���� 4 ͨ��
		{
			if (dDataCh1>threshold.dCloudRed && (dDataCh2 - dDataCh1)>threshold.dCloudNirRed && dDataCh4<threshold.dCloudT11um)
			{//��
				iPixelType = PixelType::Cloud;
			}
			else if (dDataCh1<threshold.dWaterRed && dDataCh2<threshold.dWaterNir && (dDataCh2 - dDataCh1)<threshold.dWaterNirRed)
			{//ˮ��
				iPixelType = PixelType::Water;
			}
			else
			{//½��
				iPixelType = PixelType::Land;
			}
		}
		// ������ 4 ͨ��
		else if(m_DataIndexOfChannel[1]>=0 && m_DataIndexOfChannel[2]>=0)
		{
			if (dDataCh1>threshold.dCloudRed && (dDataCh2 - dDataCh1)>threshold.dCloudNirRed)
			{//��
				iPixelType = PixelType::Cloud;
			}
			//{��			}
			else if (dDataCh1<threshold.dWaterRed && dDataCh2<threshold.dWaterNir && (dDataCh2 - dDataCh1)<threshold.dWaterNirRed)
			{//ˮ��
				iPixelType = PixelType::Water;
			}
			else
			{//½��
				iPixelType = PixelType::Land;
			}
		}
		else
		{
			iPixelType = PixelType::Unknown;
		}
		//-------------------------------------------------
		// 2. �жϴ���(�������)
		if(iPixelType == PixelType::Cloud)
		{
		}
		//-------------------------------------------------
		// 3. �жϻ��
		BOOL bDay = pAvhrrData->IsDay();
		// ����ų��� �������� T 4 <315K ��ҹ�� 305K ���� ��T 41 �� 5K �� 3K �������ض����ǻ��
		BOOL bNotFire = FALSE;	// һ�����ǻ��
		bNotFire = bDay
			? dDataCh3 < threshold.dFiringT3930nm || (dDataCh3 - dDataCh4) < threshold.dFiringT3930nm_T11um
			: dDataCh3 < threshold.dNightFiringT3930nm || (dDataCh3 - dDataCh4) < threshold.dNightFiringT3930nm;

		int fireMethod = this->GetFlag();

		// 3.1 ���� AVHRR ����̽���㷨���£�
		// A: R1<20%
		// B: R2<25%
		// C: T3>315K(����Ϊ293K)
		// D: T3-T4>10K(����Ϊ8K)
		// ����R1��R2Ϊ1��2ͨ���ķ�����ֵ��T3��T4Ϊ3��4ͨ����������ֵ��
		// ������������������Ϳ��Խ��õ�ȷ��Ϊ��㣺
		// Fire = A && B && C && D
		if(fireMethod == 1 && (iPixelType == PixelType::Land || iPixelType == PixelType::Cloud) && !bNotFire )
		{
			// 1.�ж������
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

			// 2.���жϰ����
			BOOL bFireDark	= FALSE;
			if(bFireNaked)
			{
				// waiting...
			}

			// 3.���жϹ����
			BOOL bFireEmber	= FALSE;
			if(bFireNaked && !bFireDark)
			{
				// waiting...
			}

			// ��ֵ��������
			if(bFireNaked)
				iPixelType = PixelType::FireNaked;	// ����
			else if(bFireEmber)
				iPixelType = PixelType::FireEmber;	// ����
			else if(bFireDark)
				iPixelType = PixelType::FireDark;	// ����
			else
				iPixelType = iPixelType;			// ����
		}

		// 3.2 ���� AVHRR ����̽���㷨���£�
		// ���� 5 ���߼�����(A �� B �� a �� b �� X)
		// A�� T4 > T4b + 4 * ��T4b		a: ��T41 > ��T41b + 4 * ��T41b 
		// B�� T4 > 320K (ҹ�� 315K)	b: T41 > 20K (ҹ�� 10K)
		// X�� T4 > 360K (ҹ�� 330K)
		// Z:  ���Ҫ����̫��ҫ��
		//
		// ������������������Ϳ��Խ��õ�ȷ��Ϊ��㣺
		// 1. ��ҫ��
		// 2. (A or B) and (a or b) or (X)
		// 
		// ���(��ҪAVHRRͨ��1,2,3,4��3,4�����¶�)
		// ���� B��b �ǳ��淽��������, �����ۺ�������ʽ(A or B) and (a or b) or (X)
		// �����������Ļ������Ӧ�ò����ڳ��淽�������Ļ������
		//-------------------------------------------------
		// [A]: T4 > T4b + 4 * ��T4b
		// T4		-> �������������Ĳ���Ϊ 4umͨ���¶�(K)
		// T4b		-> �������������Ĳ���Ϊ 4umͨ�������¶�(K)
		// ��T4b	-> ����������T4b�ı�׼ƫ��,���С��2K��2K���

		// [a]: ��T41 > ��T41b + 4 * ��T41b 
		// ��T41	-> T4 - T11
		// ��T41b	-> T4b - T11b
		// ��T41b	-> ����������T41b�ı�׼ƫ��

		// [B]: T4 > 320K (ҹ�� 315K)
		// T4		-> �������������Ĳ���Ϊ 4umͨ���¶�(K)

		// [b]: T41 > 20K (ҹ�� 10K)
		// T41		-> T4 - T11

		// [X]: T4 > 360K (ҹ�� 330K)
		// T4		-> �������������Ĳ���Ϊ 4umͨ���¶�(K)

		// [Z]: ���Ҫ����̫��ҫ�ߣ���
		//      0.64um �� 0.86um ����ͨ���ķ����ʶ����� 30%
		//      ҫ�߽�С��40��

		// T11		-> ���Ĳ���Ϊ 11umͨ���¶�(K)
		// T11b		-> ���Ĳ���Ϊ 11umͨ�������¶�(K)
		//-------------------------------------------------
		if((fireMethod == 2 || fireMethod == 3) && (iPixelType == PixelType::Land || iPixelType == PixelType::Cloud) && !bNotFire )
		{
			//---------------------------------------------
			// �����㹻������
			int na = 5000;			// ����ά��(�����㹻������)
			double t4a[5000];		// T4		�����¶�����
			double t11a[5000];		// T11		�����¶�����
			double t41a[5000];		// T4-T11	�����¶�����
			memset(t4a, 0,sizeof(double)*na);
			memset(t11a,0,sizeof(double)*na);
			memset(t41a,0,sizeof(double)*na);

			// �������ڳߴ�
			int np = 20;			// ���������� np*np = 400
			int r0 = i-np;			// ��ʼ��
			if(r0<0) r0 = 0;
			int c0 = j-np;			// ��ʼ��
			if(c0<0) c0 = 0;

			// ȡ��T4��T11�ı����¶�
			GetBackgroundTemperature(t4a, na,ch3,r0,c0,np,np);	// ȡ��T4�ı����¶�
			GetBackgroundTemperature(t11a,na,ch4,r0,c0,np,np);	// ȡ��T11�ı����¶�
			//---------------------------------------------

			// �ų����
			// ��ȡ������Ϣʱ�˳���������Ϊ�� ��T 41 = T 4 -T 11 �� 20 K ��ҹ��Ϊ 10K ���� T 4 �� 320K �� 315K ����
			// ...

			// ��������ͨ�����²�
			for(i=0;i<np*np;i++)
			{
				t41a[i] = t4a[i] - t11a[i];
			}

			// T4��T4b����T4b
			double T4 = dDataCh3;
			double T4b = KStatistic::RsAvg(t4a,na);		// �����¶�ƽ��ֵ - T4b
			double detT4b = KStatistic::RsSd(t4a,na);	// �����¶ȱ�׼ƫ�� - ��T4b
			if(detT4b<2) detT4b = 2;					// 

			// T11��T11b����T11b
			double T11 = dDataCh4;
			double T11b = KStatistic::RsAvg(t11a,na);
			double detT11b = KStatistic::RsSd(t11a,na);
			if(detT11b<2) detT11b = 2;

			// T41��T41b����T41b
			double T41 = T4 - T11;
			double T41b = KStatistic::RsAvg(t41a,na);	// �����¶�ƽ��ֵ - T41b
			double detT41b = KStatistic::RsSd(t41a,na);	// �����¶ȱ�׼ƫ�� - ��T41b
			if(detT41b<2) detT41b = 2;
			//---------------------------------------------
			// 1.���ж������
			BOOL bFireNaked	= FALSE;
			if(bDay)	// ����
			{
				// ͨ������1,2,3,4(1,2ͨ������ҫ���ų�)
				BOOL bHasData = ch1>=0 && ch2>=0 && ch3>=0 && ch4>=0;
				if(bHasData)
				{
					// A. T4 > T4b + 4 * ��T4b
					BOOL bCondition_A = T4 > T4b + 4 * detT4b;

					// a. ��T41 > ��T41b + 4 * ��T41b 
					BOOL bCondition_a = T41 > T41b + 4 * detT41b;

					// B. T4 > 320K (ҹ�� 315K)
					BOOL bCondition_B = T4 > threshold.dFiringT3930nm;
					if(fireMethod == 2) bCondition_B = FALSE;

					// b. T41 > 20K (ҹ�� 10K)
					BOOL bCondition_b = T41 > threshold.dFiringT3930nm_T11um;
					if(fireMethod == 2) bCondition_b = FALSE;

					// X. T4 > 360K (ҹ�� 330K)
					BOOL bCondition_X = T4 > 360;

					// �ۺ�����
					//�� A or B �� and �� a or b �� or �� X �� 
					bFireNaked = (bCondition_A || bCondition_B) && (bCondition_a || bCondition_b) || bCondition_X;

					// Z. ̫��ҫ���˳�
					//    0.64um �� 0.86um ����ͨ���ķ����ʶ����� 30%
					//    ҫ�߽�С��40��
					//BOOL bSolarFlare = dDataCh1 > 30 && dDataCh2 > 30;
					BOOL bSolarFlare = dDataCh1 > threshold.dFiringRed && dDataCh2 > threshold.dFiringNir;
					// �˳�ҫ��
					bFireNaked = bFireNaked && !bSolarFlare;
				}
			}
			else	// ����
			{
				// ͨ������3,4
				BOOL bHasData = ch3>=0 && ch4>=0;
				if(bHasData)
				{
					// A. T4 > T4b + 4 * ��T4b
					BOOL bCondition_A = T4 > T4b + 4 * detT4b;

					// a. ��T41 > ��T41b + 4 * ��T41b 
					BOOL bCondition_a = T41 > T41b + 4 * detT41b;

					// B. T4 > 320K (ҹ�� 315K)
					BOOL bCondition_B = T4 > threshold.dNightFiringT3930nm;

					// b. T41 > 20K (ҹ�� 10K)
					BOOL bCondition_b = T41 > threshold.dNightFiringT3930nm_T11um;

					// X. T4 > 360K (ҹ�� 330K)
					BOOL bCondition_X = T4 > 330;

					// �ۺ�����
					//�� A or B �� and �� a or b �� or �� X �� 
					bFireNaked = (bCondition_A || bCondition_B) && (bCondition_a || bCondition_b) || bCondition_X;
				}
			}

			// 2.���жϰ����
			BOOL bFireDark	= FALSE;
			if(bFireNaked)
			{
				// waiting...
			}

			// 3.���жϹ����
			BOOL bFireEmber	= FALSE;
			if(bFireNaked && !bFireDark)
			{
				// waiting...
			}

			// ��ֵ��������
			if(bFireNaked)
				iPixelType = PixelType::FireNaked;	// ����
			else if(bFireEmber)
				iPixelType = PixelType::FireEmber;	// ����
			else if(bFireDark)
				iPixelType = PixelType::FireDark;	// ����
			else
				iPixelType = iPixelType;			// ����
		}
		//-------------------------------------------------
		// 4. ��ѩ�ж�
		//-------------------------------------------------
		return iPixelType;
	}

	// ��Ʒ���� - ֲ��ָ��
	// ��(0,1)֮��
	BOOL KAvhrrDataProcessor::Vix(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		// ���ݶ���
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pAvhrrData->GetData();
		if(!ppwData)
			return FALSE;

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ͨ������
		WORD wDataCh[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];	// AGMRS_MODIS_DATA_CHANNEL_NUM_MAX=40
		memset(&wDataCh[0],0,sizeof(WORD)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		// ͨ����(-1��ʾ������)
		int ch[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		// ��������ͨ�����ڴ�ͨ��
		for(i=1;i<=36;i++)
		{
			int ch38 = pAvhrrData->Channel2ChannelCode(i);
			ch[i] = pAvhrrData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KAvhrrAlgorithm oAvhrrAlgorithm;
		oAvhrrAlgorithm.SetData(&wDataCh[0]);	// Ϊ�㷨���ü��������

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE;
		_tstring strInfo = _T("");
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_VIX_NDVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("���� NDVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_DVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("���� DVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_RVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("���� RVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_SAVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			strInfo = _T("���� SAVI ...");
			break;

		case AGMRS_PRODUCT_CODE_VIX_EVI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[3]>=0);
			strInfo = _T("���� EVI ...");
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

				//��������, 1: ����, 0: ����
				//int ii = i;
				int ii = iHeight - 1 - i;

				int pos = ii*iWidth+j;

				// ȡ��ͨ������
				wDataCh1	= ppwData[ch[1]][pos] & 0x0FFF;			wDataCh1 = wDataCh1>=0x0FFE ? 0 : wDataCh1;
				wDataCh2	= ppwData[ch[2]][pos] & 0x0FFF;			wDataCh2 = wDataCh2>=0x0FFE ? 0 : wDataCh2; 
				if(AGMRS_PRODUCT_CODE_VIX_EVI==iProductID)
				{
					wDataCh3	= ppwData[ch[3]][pos] & 0x0FFF;		wDataCh3 = wDataCh3>=0x0FFE ? 0 : wDataCh3; 
				}

				// ��Ч����
				if(wDataCh1==0x0FFF || wDataCh2==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}
				if(wDataCh1==0x0FFE || wDataCh2==0x0FFE)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}

				// Ϊ�㷨�ṩ����
				wDataCh[1]	= wDataCh1;
				wDataCh[2]	= wDataCh2;
				wDataCh[3]	= wDataCh3;

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

						// �Ƿ���Ч
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

		}// for(i)

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pAvhrrData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode	= AXIN_FMT_CODE_GRID_IMAGE;			// important

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
		_stprintf(szTemp,_T("%s"),pAvhrrData->NameProduct(iProductID));
		pProductDataInfo->SetComment(szTemp);


		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...
		///------------------------------------------------

		return TRUE;
	}

	// ��Ʒ���� - ������ - SIM (Single Item Monitor)
	BOOL KAvhrrDataProcessor::Sim(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KAvhrrData* pAvhrrData = (KAvhrrData*)GetOwner();
		// ������Ϣ
		KDataInfo* pDataInfo = pAvhrrData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pAvhrrData->GetData();
		if(!ppwData)
			return FALSE;

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ͨ������
		WORD wDataCh[AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX];
		memset(&wDataCh[0],0,sizeof(WORD)*(AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX));

		// ͨ����(-1��ʾ������)
		int ch[AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_AVHRR_DATA_CHANNEL_NUM_MAX));

		// ��������ͨ�����ڴ�ͨ��
		for(i=1;i<=AGMRS_AVHRR_DATA_CHANNEL_NUM;i++)
		{
			int iChannelCode = pAvhrrData->Channel2ChannelCode(i);
			ch[i] = pAvhrrData->GetDataIndex(iChannelCode);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KAvhrrAlgorithm oAvhrrAlgorithm;
		oAvhrrAlgorithm.SetData(&wDataCh[0]);	// Ϊ�㷨���ü��������

		// ������Ϣ
		TCHAR szInfo[MAX_PATH] = _T("");
		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();


		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE; 
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_MISC_CLOUD:	// ��
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_FOG:	// ��
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_SNOW:	// ѩ
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_ICE:	// ��
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_FIRE:	// ���
			{
				_stprintf(szInfo,_T("������Ʒ ..."));
				if(pAvhrrData->IsDay())
					bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[3]>=0) && (ch[4]>=0);
				else
					bChannelOk = (ch[3]>=0) && (ch[4]>=0);
			}
			break;

		case AGMRS_PRODUCT_CODE_MISC_WATER:	// ˮ��
			//bChannelOk = (ch[1]>=0) && (ch[2]>=0);
			break;
		}

		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Sim"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float)*iWidth*iHeight);

		// �����㼯���ڴ�
		FirePointVector* pFirePointVector = NULL;
		if(iProductID == AGMRS_PRODUCT_CODE_MISC_FIRE)
		{
			_delete(m_pFirePointVector);
			m_pFirePointVector = new FirePointVector();
			pFirePointVector = m_pFirePointVector;
		}

		// ����ֲ��ָ��
		WORD wDataCh1(0),wDataCh2(0),wDataCh3(0),wDataCh4(0),wDataCh5(0);
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		int iFireCount = 0;		// �������
		int iSnowCount = 0;		// ѩ������

		double lon =0 ;
		double lat = 0;

		double dBaseValue = 0;
		double dScale = 1.0;

		if(pProgressParam) pProgressParam->SetInfo(szInfo);
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
				bValid = FALSE;
				dValue = AGMRS_PRODUCT_DATA_INVALID;

				//��������, 1: ����, 0: ����
				//int ii = i;
				int ii = iHeight - 1 - i;
				int pos = ii*iWidth+j;

				// ȡ��ͨ������
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

				// ��Ч����
				if(wDataCh1==0x0FFF || wDataCh2==0x0FFF || wDataCh3==0x0FFF || wDataCh4==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}

				// Ϊ��ѩָ���㷨�ṩ����
				//wDataCh[4]	= wDataCh4;
				//wDataCh[6]	= wDataCh6;

				// ȡ����Ԫ������
				int iPixelType = this->GetPixelType(ii,j);		// y,x

				switch(iPixelType)
				{
				case PixelType::Cloud:		// �� 
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_CLOUD)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					}
					break;

				case PixelType::Fog:		// ��
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_FOG)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_FOG;
					}
					break;
				case PixelType::FireNaked:		// �����
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE)
					{
						iFireCount++;
						m_ppfProduct[i][j] = FireType::FireNaked;	// ���� 1

						// ���ת��Ϊ��γ��(i,j)�ǲ�Ʒ����
						pAvhrrData->xy2LonLat(j,i,lon,lat,1);

						// ���
						FirePoint firePoint;
						firePoint.Id = iFireCount;				// id
						firePoint.Type = FireType::FireNaked;	// ����
						firePoint.Row = i;
						firePoint.Col = j;
						firePoint.Lon = (float)lon;
						firePoint.Lat = (float)lat;
						firePoint.Temperature = 0.1f * wDataCh3;	// �¶�
						firePoint.Area = 0.1f * wDataCh3 / 515;		// ���(��������Ԫ���)
						firePoint.Group = 0;
						firePoint.GroupName = _T("Group_0");
						firePoint.Flag = 0;
						pFirePointVector->push_back(firePoint);
					}
					break;

				case PixelType::FireEmber:		// �����
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE)
					{
						iFireCount++;
						m_ppfProduct[i][j] = FireType::FireEmber;

						// ���ת��Ϊ��γ��(i,j)�ǲ�Ʒ����
						pAvhrrData->xy2LonLat(j,i,lon,lat,1);

						// ���
						FirePoint firePoint;
						firePoint.Id = iFireCount;				// id
						firePoint.Type = FireType::FireEmber;	// ����
						firePoint.Row = i;
						firePoint.Col = j;
						firePoint.Lon = (float)lon;
						firePoint.Lat = (float)lat;
						firePoint.Temperature = 0.1f * wDataCh4;	// �¶�
						firePoint.Area = 0.1f * wDataCh4 / 500;	// ���(��������Ԫ���)
						firePoint.Group = 0;
						firePoint.GroupName = _T("Group_0");
						firePoint.Flag = 0;
						pFirePointVector->push_back(firePoint);
					}
					break;

				case PixelType::Snow:		// ѩ
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_SNOW)
					{
						iSnowCount++;
						dBaseValue = 0.0; dScale = 1.0;
						dValue = oAvhrrAlgorithm.Ndsi();	// ��ѩָ��
						m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);		// trueValue = value / dScale + dBaseValue;
					}
					break;

				case PixelType::Ice:		// ��
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_SNOW)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_ICE;
					}
					break;

				case PixelType::Water:		// ˮ 
					if(iProductID==AGMRS_PRODUCT_CODE_MISC_WATER)
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					}
					break;

				case PixelType::Land:		// ½��
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

		if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE && iFireCount==0)
		{
			::MessageBox(NULL,_T("û�л��"),_T("��Ϣ"),MB_OK);
			return FALSE;
		}
		else if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE)
		{
			// ������(���ݻ������ֵ���飬�л���ֵ���ڵ������ͬ)
			this->Grouping(pFirePointVector);
		}

		if(iProductID==AGMRS_PRODUCT_CODE_MISC_SNOW && iSnowCount==0)
		{
			::MessageBox(NULL,_T("û�л�ѩ"),_T("��Ϣ"),MB_OK);
			return FALSE;
		}

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pAvhrrData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;				// important

		// 2.����
		pProductDataInfo->wYear				= pDataInfo->Year();
		pProductDataInfo->wMonth			= pDataInfo->Month();
		pProductDataInfo->wDay				= pDataInfo->Day();
		pProductDataInfo->wHour				= pDataInfo->Hour();
		pProductDataInfo->wMinute			= pDataInfo->Minute();
		pProductDataInfo->wSecond			= 0;
		pProductDataInfo->dwMillisecond		= 0;

		// 3.��������
		pProductDataInfo->wElementCode		= 0;	//ElementCode_Index;
		pProductDataInfo->wProductCode		= iProductID;
		pProductDataInfo->wDataType			= DataType_Float;
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
		KString strPathName(pAvhrrData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileName = strPathName.Mid(pos+1);		// ȡ���ļ���(��·��)

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

		// ע����Ϣ
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

		// ���
		if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE && pFirePointVector && pFirePointVector->size() != 0)
		{
			pProductDataInfo->TotalNum()		= pFirePointVector->size();
			pProductDataInfo->ElementNumbers()	= 4;
			pProductDataInfo->Flag()			= 1;
			pProductDataInfo->ContourNumbers()	= 0;
			pProductDataInfo->ClipArea()		= 0;
		}

		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...

		///------------------------------------------------

		return TRUE;
	}

}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------
