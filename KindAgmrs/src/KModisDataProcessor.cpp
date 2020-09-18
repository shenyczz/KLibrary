#include "StdAfx.h"
#include "KModisDataProcessor.h"

#include "KModisData.h"
#include "KModisAlgorithm.h"

namespace Agmrs { namespace Modis 
{

	KModisDataProcessor::KModisDataProcessor(void)
	{
		this->m_pDataAlgorithm = new KModisAlgorithm();
	}

	KModisDataProcessor::~KModisDataProcessor(void)
	{
	}

	// ��Ʒ���� - ȫ����
	// 1 2 3 4 8 9 10 11 14 15 17 19
	BOOL KModisDataProcessor::Abe(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ������Ϣ
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();
		if(!ppwData)
			return FALSE;

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ͨ������
		WORD wDataCh[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];							// AGMRS_MODIS_DATA_CHANNEL_NUM_MAX=40
		memset(&wDataCh[0],0,sizeof(WORD)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		// ͨ����
		int ch[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		for(i=1;i<=36;i++)
		{
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KModisAlgorithm oModisAlgorithm;
		oModisAlgorithm.SetData(&wDataCh[0]);

		// �ж�ͨ���Ƿ����
		// 1 2 3 4 8 9 10 11 14 15 17 19
		BOOL bChannelOk = TRUE 
						  && ch[1]>=0 && ch[2]>=0 && ch[3]>=0 && ch[4]>=0 && ch[8]>=0 && ch[9]>=0
						  && ch[10]>=0 && ch[11]>=0 && ch[14]>=0 && ch[15]>=0 && ch[17]>=0 && ch[19]>=0
						  ;
		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Abe"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float)*iWidth*iHeight);

		// ����ȫ����
		// 1 2 3 4 8 9 10 11 14 15 17 19
		WORD wDataCh1(0),wDataCh2(0),wDataCh3(0),wDataCh4(0),wDataCh8(0),wDataCh9(0);
		WORD wDataCh10(0),wDataCh11(0),wDataCh14(0),wDataCh15(0),wDataCh17(0),wDataCh19(0);
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		double dBaseValue = 0;
		double dScale = 1.0;

#ifdef _DEBUG
		//=================================================
		int count = 0;

		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================
#endif

		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("����ȫ����..."));
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

				// 1 2 3 4 8 9 10 11 14 15 17 19
				wDataCh1 = ppwData[ch[1]][pos] & 0x0FFF;		wDataCh1 = wDataCh1>=0x0FFE ? 0 : wDataCh1;
				wDataCh2 = ppwData[ch[2]][pos] & 0x0FFF;		wDataCh2 = wDataCh2>=0x0FFE ? 0 : wDataCh2;
				wDataCh3 = ppwData[ch[3]][pos] & 0x0FFF;		wDataCh3 = wDataCh3>=0x0FFE ? 0 : wDataCh3;
				wDataCh4 = ppwData[ch[4]][pos] & 0x0FFF;		wDataCh4 = wDataCh4>=0x0FFE ? 0 : wDataCh4;
				wDataCh8 = ppwData[ch[8]][pos] & 0x0FFF;		wDataCh8 = wDataCh8>=0x0FFE ? 0 : wDataCh8;
				wDataCh9 = ppwData[ch[9]][pos] & 0x0FFF;		wDataCh9 = wDataCh9>=0x0FFE ? 0 : wDataCh9;

				wDataCh10 = ppwData[ch[10]][pos] & 0x0FFF;		wDataCh10 = wDataCh10>=0xFFE ? 0 : wDataCh10;
				wDataCh11 = ppwData[ch[11]][pos] & 0x0FFF;		wDataCh11 = wDataCh11>=0xFFE ? 0 : wDataCh11;
				wDataCh14 = ppwData[ch[14]][pos] & 0x0FFF;		wDataCh14 = wDataCh14>=0xFFE ? 0 : wDataCh14;
				wDataCh15 = ppwData[ch[15]][pos] & 0x0FFF;		wDataCh15 = wDataCh15>=0xFFE ? 0 : wDataCh15;
				wDataCh17 = ppwData[ch[17]][pos] & 0x0FFF;		wDataCh17 = wDataCh17>=0xFFE ? 0 : wDataCh17;
				wDataCh19 = ppwData[ch[19]][pos] & 0x0FFF;		wDataCh19 = wDataCh19>=0xFFE ? 0 : wDataCh19;

				// ��Ч��֮
				//if(wDataCh1>=0xFFE || wDataCh2>=0xFFE || wDataCh3>=0xFFE || wDataCh4>=0xFFE || wDataCh8>=0xFFE || wDataCh9>=0xFFE
				//	|| wDataCh10>=0xFFE || wDataCh11>=0xFFE || wDataCh14>=0xFFE || wDataCh15>=0xFFE || wDataCh17>=0xFFE || wDataCh19>=0xFFE)
				//{
				//	m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
				//	continue;
				//}

				// Ϊ�㷨�ṩ����
				wDataCh[1]	= wDataCh1;
				wDataCh[2]	= wDataCh2;
				wDataCh[3]	= wDataCh3;
				wDataCh[4]	= wDataCh4;
				wDataCh[8]	= wDataCh8;
				wDataCh[9]	= wDataCh9;

				wDataCh[10] = wDataCh10;
				wDataCh[11] = wDataCh11;
				wDataCh[14] = wDataCh14;
				wDataCh[15] = wDataCh15;
				wDataCh[17] = wDataCh17;
				wDataCh[19] = wDataCh19;

				// ȡ����Ԫ������
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Water:		// ˮ
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					break;

				case PixelType::Cloud:		// ��
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					break;

				case PixelType::Unknown:	// δ֪ -9999
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					break;

				case PixelType::Land:	// ½��
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					dValue = oModisAlgorithm.Abe();
					bValid = (dValue>0 && dValue<1);
					if(bValid)
					{
						// trueValue = value / dScale + dBaseValue;
						m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);

#ifdef _DEBUG
						//=============================
						count++;
						dtmin = min(dtmin,dValue);
						dtmax = max(dtmax,dValue);
						//=============================
#endif
					}
					break;
				}

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


		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.����
		pProductDataInfo->wYear				= pDataInfo->Year();
		pProductDataInfo->wMonth			= pDataInfo->Month();
		pProductDataInfo->wDay				= pDataInfo->Day();
		pProductDataInfo->wHour				= pDataInfo->Hour();
		pProductDataInfo->wMinute			= pDataInfo->Minute();
		pProductDataInfo->wSecond			= 0;
		pProductDataInfo->dwMillisecond		= 0;

		// 3.��������
		//pProductDataInfo->wElementCode	= eElementStyle_Temperature;	// important
		pProductDataInfo->wProductCode		= AGMRS_PRODUCT_CODE_MISC_ABE;
		pProductDataInfo->wDataType			= DataType_Float;				// important
		pProductDataInfo->InvalidData()		= AXIN_INVALID_DATA;
		pProductDataInfo->dBaseValue		= dBaseValue;
		pProductDataInfo->dScale			= dScale;

		// 4.������(Important)
		pProductDataInfo->xNum				= pDataInfo->Width();
		pProductDataInfo->xMin				= pDataInfo->MinX();
		pProductDataInfo->xMax				= pDataInfo->MaxX();
		pProductDataInfo->xInterval			= pDataInfo->XInterval();

		pProductDataInfo->yNum				= pDataInfo->Height();
		pProductDataInfo->yMin				= pDataInfo->MinY();
		pProductDataInfo->yMax				= pDataInfo->MaxY();
		pProductDataInfo->yInterval			= pDataInfo->YInterval();

		// 5.ע����Ϣ(�������)
		KString strPathName(pModisData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileName = strPathName.Mid(pos+1);

		TCHAR szTemp[MAX_PATH];
		_stprintf(szTemp,_T("MISC_abe(%d)_%s"),iProductID, strFileName.GetString());
		pProductDataInfo->SetComment(szTemp);

		// 6. ���ⲿ���õ�ɫ��(��ɫ������, ��ɫ, ע��, ��ɫ���͸��ɫ)(Important)
		// ...

		return TRUE;
	}

	// ��Ʒ���� - �ر��¶�
	// ʹ�� 1,2,19,31,32ͨ������
	BOOL KModisDataProcessor::Lst(int iMethod)
	{
		int i,j;


		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ������Ϣ
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();
		if(!ppwData)
			return FALSE;

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		// ͨ������
		WORD wDataCh[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];							// AGMRS_MODIS_DATA_CHANNEL_NUM_MAX=40
		memset(&wDataCh[0],0,sizeof(WORD)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		// ͨ����
		int ch[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		for(i=1;i<=36;i++)
		{
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KModisAlgorithm oModisAlgorithm;
		oModisAlgorithm.SetData(&wDataCh[0]);

		// �ж�ͨ���Ƿ����
		BOOL bChannelOk = TRUE 
						  && ch[31]>=0
						  && ch[32]>=0
						  ;
		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Lst"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float)*iWidth*iHeight);	// 

		// ����ر��¶�
		WORD wDataCh1(0),wDataCh2(0);
		WORD wDataCh19(0);
		WORD wDataCh31(0),wDataCh32(0);
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
		pProgressParam->SetInfo(_T("����ر��¶�..."));
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

				wDataCh1 = ppwData[ch[1]][pos] & 0x0FFF;	wDataCh1 = wDataCh1>=0xFFE ? 0 : wDataCh1;
				wDataCh2 = ppwData[ch[2]][pos] & 0x0FFF;	wDataCh2 = wDataCh2>=0xFFE ? 0 : wDataCh2;

				wDataCh19 = ppwData[ch[19]][pos] & 0x0FFF;	wDataCh19 = wDataCh19>=0xFFE ? 0 : wDataCh19;

				wDataCh31 = ppwData[ch[31]][pos] & 0x0FFF;	wDataCh31 = wDataCh31>=0xFFE ? 0 : wDataCh31;
				wDataCh32 = ppwData[ch[32]][pos] & 0x0FFF;	wDataCh32 = wDataCh32>=0xFFE ? 0 : wDataCh32;

				// ��Ч��֮
				if(wDataCh1==0x0FFF || wDataCh2==0x0FFF || wDataCh19==0x0FFF || wDataCh31==0x0FFF || wDataCh32==0x0FFF)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}
				if(wDataCh1==0x0FFE || wDataCh2==0x0FFE || wDataCh19==0x0FFE || wDataCh31==0x0FFE || wDataCh32==0x0FFE)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}

				// Ϊ�㷨�ṩ����
				wDataCh[1]	= wDataCh1;
				wDataCh[2]	= wDataCh2;
				wDataCh[19] = wDataCh19;
				wDataCh[31] = wDataCh31;
				wDataCh[32] = wDataCh32;

				// ȡ����Ԫ������
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Land:	// ½��
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					dValue = oModisAlgorithm.Lst(iMethod) - 273.15;		// ���ؾ����¶�(ת��Ϊ���϶�)
					// �Ƿ���Ч (-20-100��)
					bValid = (dValue>-20 && dValue<100);
					if(bValid)
					{
						double vapour = oModisAlgorithm.Vapour();
						//=============================
						dtmin = min(dtmin,vapour);
						dtmax = max(dtmax,vapour);
						//=============================
						m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);		// trueValue = value / dScale + dBaseValue;
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

		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.����
		pProductDataInfo->wYear			= pDataInfo->wYear;
		pProductDataInfo->wMonth		= pDataInfo->wMonth;
		pProductDataInfo->wDay			= pDataInfo->wDay;
		pProductDataInfo->wHour			= pDataInfo->wHour;
		pProductDataInfo->wMinute		= pDataInfo->wMinute;
		pProductDataInfo->wSecond		= 0;
		pProductDataInfo->dwMillisecond	= 0;

		// 3.��������
		//pProductDataInfo->wElementCode		= eElementStyle_Temperature_C;	// important
		pProductDataInfo->wProductCode	= AGMRS_PRODUCT_CODE_MISC_LST;
		pProductDataInfo->wDataType		= DataType_Float;				// important
		pProductDataInfo->InvalidData()	= AXIN_INVALID_DATA;
		pProductDataInfo->dBaseValue	= dBaseValue;
		pProductDataInfo->dScale		= dScale;

		// 4.������(Important)
		pProductDataInfo->xNum			= pDataInfo->Width();
		pProductDataInfo->xMin			= pDataInfo->MinLon();
		pProductDataInfo->xMax			= pDataInfo->MaxLon();
		pProductDataInfo->xInterval		= pDataInfo->LonInterval();

		pProductDataInfo->yNum			= pDataInfo->Height();
		pProductDataInfo->yMin			= pDataInfo->MinLat();
		pProductDataInfo->yMax			= pDataInfo->MaxLat();
		pProductDataInfo->yInterval		= pDataInfo->LatInterval();

		// 5.ע����Ϣ(�������)
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

		KString strPathName(pModisData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileName = strPathName.Mid(pos+1);
		TCHAR szTemp[MAX_PATH];
		_stprintf(szTemp,_T("LST_%s_%s"),strMethod.GetString(), strFileName.GetString());
		pProductDataInfo->SetComment(szTemp);

		// 6. ���ⲿ���õ�ɫ��(��ɫ������, ��ɫ, ע��, ��ɫ���͸��ɫ)(Important)
		// ...

		return TRUE;
	}

	// ��Ʒ���� - �ɺ�ָ��
	BOOL KModisDataProcessor::Dix(int iProductID)
	{
		if(iProductID == AGMRS_PRODUCT_CODE_DIX_ATI)
		{
			return Dix_Ati(iProductID);
		}
		else if(iProductID == AGMRS_PRODUCT_CODE_DIX_TVDI)
		{
			return Dix_Tvdi(iProductID);
		}
		else if(iProductID == AGMRS_PRODUCT_CODE_DIX_PDI)
		{
			return Dix_Pdi(iProductID);
		}
		else if(iProductID == AGMRS_PRODUCT_CODE_DIX_MPDI)
		{
			return Dix_Mpdi(iProductID);
		}

		int i,j;

		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ���ݶ���
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();
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
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KModisAlgorithm oModisAlgorithm;
		oModisAlgorithm.SetData(&wDataCh[0]);	// ���ü��������

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE; 
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_DIX_VSWI:
			bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[3]>=0);
			break;

		case AGMRS_PRODUCT_CODE_DIX_SWCI:
			bChannelOk = (ch[6]>=0) && (ch[7]>=0);
			break;
		}

		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Dix"),_T(""),MB_OK|MB_ICONINFORMATION); 
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
		//
		WORD wDataCh1(0),wDataCh2(0),wDataCh3(0);
		WORD wDataCh6(0),wDataCh7(0);

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

		if(pProgressParam){ pProgressParam->SetInfo(_T("����ɺ�ָ��...")); }
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
				wDataCh1	= ppwData[ch[1]][pos] & 0x0FFF;
				wDataCh2	= ppwData[ch[2]][pos] & 0x0FFF;
				if(AGMRS_PRODUCT_CODE_DIX_VSWI==iProductID)
				{
					wDataCh3	= ppwData[ch[3]][pos] & 0x0FFF;
				}
				if(AGMRS_PRODUCT_CODE_DIX_SWCI==iProductID)
				{
					wDataCh6	= ppwData[ch[6]][pos] & 0x0FFF;
					wDataCh7	= ppwData[ch[7]][pos] & 0x0FFF;
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
				wDataCh[6]	= wDataCh6;
				wDataCh[7]	= wDataCh7;
				//wDataCh[19] = wDataCh19;
				//wDataCh[31] = wDataCh31;
				//wDataCh[32] = wDataCh32;

				// ȡ����Ԫ������
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Land:
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
						switch(iProductID)
						{
						case AGMRS_PRODUCT_CODE_DIX_VSWI:
							{
								dBaseValue = 0.0; dScale = 1.0;
								double Ndvi = oModisAlgorithm.Ndvi();
								double Evi = oModisAlgorithm.Evi();
								double Ts = oModisAlgorithm.Lst();	// K

								if(Ts>0 && Ts<100+273.15)	// �¶ȵ���100��
								{// Ts != 0
									dValue = Ndvi*100 / Ts;		// K
									//dValue = Evi*100 / Ts;
								}
								else
								{
									dValue = 0;
								}
								bValid = (dValue>0 && dValue<1);
							}
							break;

						case AGMRS_PRODUCT_CODE_DIX_SWCI:
							dBaseValue = 0.0; dScale = 1.0;
							dValue = oModisAlgorithm.Swci();
							bValid = (dValue>0 && dValue<1);
							break;

						}

						// �Ƿ���Ч
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

		}// for(i)

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// necessary

		// 2.����
		pProductDataInfo->wYear				= pDataInfo->wYear;
		pProductDataInfo->wMonth			= pDataInfo->wMonth;
		pProductDataInfo->wDay				= pDataInfo->wDay;
		pProductDataInfo->wHour				= pDataInfo->wHour;
		pProductDataInfo->wMinute			= pDataInfo->wMinute;
		pProductDataInfo->wSecond			= 0;
		pProductDataInfo->dwMillisecond		= 0;

		// 3.��������
		pProductDataInfo->wElementCode		= 0;
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
		KString strPathName(pModisData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileNameWithoutPath = strPathName.Mid(pos+1);		// ȡ���ļ���(��·��)

		KString strProduct(_T(""));
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_DIX_VSWI:
			strProduct = _T("vswi");
			break;

		case AGMRS_PRODUCT_CODE_DIX_SWCI:
			strProduct = _T("swci");
			break;
		}

		TCHAR szComment[MAX_PATH];
		_stprintf(szComment,_T("DIX_%s(%d)_%s"),strProduct.GetString(), iProductID, strFileNameWithoutPath.GetString());
		pProductDataInfo->SetComment(szComment);

		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...

		///------------------------------------------------

		return TRUE;
	}

	// TVDI ��Ҫ�����ļ��������ļ�
	BOOL KModisDataProcessor::Dix_Ati(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KModisData* pOwner = (KModisData*)GetOwner();

		KModisData* pModisDataDay = NULL;
		KModisData* pModisDataNight = NULL;

		PFLOAT* ppfDayAbe = NULL;
		PFLOAT* ppfDayLst = NULL;
		PFLOAT* ppfNightLst = NULL;

		// �����ң�������ļ�
		TCHAR szDayFile[MAX_PATH] = _T("");
		_stprintf(szDayFile,pOwner->GetDayFile());

		// �������ݶ���
		pModisDataDay = new KModisData();
		pModisDataDay->RegiestCallbackFunc(pOwner->CallbackFunc(),pOwner->CallbackParam());

		KDataInfo* pDataInfoDay = pModisDataDay->GetDataInfo();

		if(!pModisDataDay->LoadData(szDayFile))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}

		if(!pModisDataDay->IsDay())
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			::MessageBox(NULL,_T("���ǰ��������ļ�!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		int iWidthDay = pDataInfoDay->Width();
		int iHeightDay = pDataInfoDay->Height();

		// ����ȫ����
		if(!pModisDataDay->Abe(AGMRS_PRODUCT_CODE_MISC_ABE))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		// �����ڴ汣�� ABE ����
		if(!KFloat2DMemAllocator::AllocMemory2D(ppfDayAbe,iHeightDay,iWidthDay))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		KAgmrsDataProcessor* pAgmrsDataProcessorDay = (KAgmrsDataProcessor*)pModisDataDay->GetDataProcessor();
		memcpy(*ppfDayAbe,*pAgmrsDataProcessorDay->GetProductData(),sizeof(FLOAT) * iWidthDay * iHeightDay);

		// �������ر��¶�
		if(!pModisDataDay->Lst(LstMethod::Becker))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		// �����ڴ汣�� LST ����
		if(!KFloat2DMemAllocator::AllocMemory2D(ppfDayLst,iHeightDay,iWidthDay))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		pAgmrsDataProcessorDay = (KAgmrsDataProcessor*)pModisDataDay->GetDataProcessor();
		memcpy(*ppfDayLst,*pAgmrsDataProcessorDay->GetProductData(),sizeof(FLOAT) * iWidthDay * iHeightDay);

		// ���ϵ��ļ�
		TCHAR szNightFile[MAX_PATH] = _T("");
		_stprintf(szNightFile,pOwner->GetNightFile());

		// �������ݶ���
		pModisDataNight = new KModisData();
		pModisDataNight->RegiestCallbackFunc(pOwner->CallbackFunc(),pOwner->CallbackParam());

		KDataInfo* pDataInfoNight = pModisDataNight->GetDataInfo();

		if(!pModisDataNight->LoadData(szNightFile))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			::MessageBox(NULL,_T("�������������ļ�!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		if(!pModisDataNight->IsNight())
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			::MessageBox(NULL,_T("�������������ļ�!"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		int iWidthNight = pDataInfoNight->Width();
		int iHeightNight = pDataInfoNight->Height();

		// �������ϵر��¶�
		if(!pModisDataNight->Lst(LstMethod::Becker))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}

		// �����ڴ汣�� LST ����
		if(!KFloat2DMemAllocator::AllocMemory2D(ppfNightLst,iHeightNight,iWidthNight))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		KAgmrsDataProcessor* pAgmrsDataProcessorNight = (KAgmrsDataProcessor*)pModisDataNight->GetDataProcessor();
		memcpy(*ppfNightLst,*pAgmrsDataProcessorNight->GetProductData(),sizeof(FLOAT) * iWidthNight * iHeightNight);

		//-------------------------------------------------
		// ���㹫������
		LDFHeaderCommon* pModisFileHeaderDay = (LDFHeaderCommon*)pModisDataDay->FileHeader();
		LDFHeaderCommon* pModisFileHeaderNight = (LDFHeaderCommon*)pModisDataNight->FileHeader();

		// ����������Сֵ����Ӧ�����ֵ
		float fCommonLonMin = max(pModisFileHeaderDay->fLonMin,pModisFileHeaderNight->fLonMin);
		float fCommonLonMax = min(pModisFileHeaderDay->fLonMax,pModisFileHeaderNight->fLonMax);

		float fCommonLatMin = max(pModisFileHeaderDay->fLatMin,pModisFileHeaderNight->fLatMin);
		float fCommonLatMax = min(pModisFileHeaderDay->fLatMax,pModisFileHeaderNight->fLatMax);

		if(fCommonLonMax<fCommonLonMin || fCommonLatMax<fCommonLatMin)
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			::MessageBox(NULL,_T("û�й�������"),_T("��Ϣ"),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		// ���㹫�������ڰ���ͼ������
		int xCommonMin,yCommonMin;
		if(!pModisDataDay->LonLat2xy(fCommonLonMin,fCommonLatMin,xCommonMin,yCommonMin))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		else
		{// ����
			xCommonMin = xCommonMin;
			yCommonMin = pModisFileHeaderDay->wHeight - 1 - yCommonMin;
		}

		int xCommonMax,yCommonMax;
		if(!pModisDataDay->LonLat2xy(fCommonLonMax,fCommonLatMax,xCommonMax,yCommonMax))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		else
		{// ����
			xCommonMax = xCommonMax;
			yCommonMax = pModisFileHeaderDay->wHeight - 1 - yCommonMax;
		}
		//-------------------------------------------------

		int iWidth = iWidthDay;
		int iHeight = iHeightDay;

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			_delete(pModisDataDay);
			_delete(pModisDataNight);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
			KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
			KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float) * iWidth * iHeight);


		// �����ȹ���ָ�� - ATI
		double dValue(0);
		BOOL bValid(FALSE);
		double dBaseValue = 0;
		double dScale = 1;

#ifdef _DEBUG
		//=================================================
		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================
#endif

		ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("�����ȹ���ָ��..."));
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				int ii = iHeight-1-i;

				// ������ڰ���ͼ�Ĺ�������������һ����
				if( !(j>=xCommonMin && j<=xCommonMax && i>=yCommonMin && i<=yCommonMax) )
				{
					this->m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					continue;
				}

				float ts = ppfDayLst[i][j];
				float abe = ppfDayAbe[i][j];

				// ���(j,i)��Ӧ�ĵ�������
				double lon,lat;
				if(!pModisDataDay->xy2LonLat(j,i,lon,lat,1))
				{
					this->m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					continue;
				}

				// ���ݾ�γ�ȼ����������ݵĸ������
				int in,jn;
				if(!pModisDataNight->LonLat2xy(lon,lat,jn,in,1))
				{
					this->m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					continue;
				}

				float nts = ppfNightLst[in][jn];

				//ȡ����Ԫ������
				int iPixelType = pModisDataDay->GetPixelType(ii,j);	// y,x
				switch(iPixelType)
				{
				case PixelType::Water:		// ˮ 
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_WATER;
					break;

				case PixelType::Cloud:		// �� 
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_CLOUD;
					break;

				case PixelType::Unknown:	// δ֪ 99999
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					break;

				case PixelType::Land:
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
					dBaseValue = 0; dScale = 1.0;
					//if(!pModisDataNight->IsClearNight(in,jn))
					//{
					//	dValue = 0;
					//}
					//else
					{
						if( (ts-nts)>0 )
						{
							dValue = (1-abe) / (ts-nts);
						}
						else
						{
							dValue = 0;
						}
					}

					// �Ƿ���Ч
					bValid = (dValue>0 && dValue<1);
					if(bValid)
					{
						m_ppfProduct[i][j] = (float)((dValue - dBaseValue) * dScale);		// trueValue = value / dScale + dBaseValue;
#ifdef _DEBUG
						//=============================
						dtmin = min(dtmin,dValue);
						dtmax = max(dtmax,dValue);
						//=============================
#endif
					}

					break;
				}


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

		// ���ݶ���
		//KDataInfo* pDataInfoDay = pModisDataDay->GetDataInfo();

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisDataDay->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.����
		pProductDataInfo->wYear				= pDataInfoDay->wYear;
		pProductDataInfo->wMonth			= pDataInfoDay->wMonth;
		pProductDataInfo->wDay				= pDataInfoDay->wDay;
		pProductDataInfo->wHour				= pDataInfoDay->wHour;
		pProductDataInfo->wMinute			= pDataInfoDay->wMinute;
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
		pProductDataInfo->xNum				= pDataInfoDay->Width();
		pProductDataInfo->xMin				= pDataInfoDay->MinLon();
		pProductDataInfo->xMax				= pDataInfoDay->MaxLon();
		pProductDataInfo->xInterval			= pDataInfoDay->LonInterval();

		pProductDataInfo->yNum				= pDataInfoDay->Height();
		pProductDataInfo->yMin				= pDataInfoDay->MinLat();
		pProductDataInfo->yMax				= pDataInfoDay->MaxLat();
		pProductDataInfo->yInterval			= pDataInfoDay->LatInterval();

		// 5.ע����Ϣ
		KString strPathName(pModisDataDay->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileNameWithoutPath = strPathName.Mid(pos+1);		// ȡ���ļ���(��·��)
		KString strProduct(_T("ati"));

		TCHAR szComment[MAX_PATH];
		_stprintf(szComment,_T("DIX_%s(%d)_%s"),strProduct.GetString(), iProductID, strFileNameWithoutPath.GetString());
		pProductDataInfo->SetComment(szComment);

		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...

		///------------------------------------------------

		// �ͷ��ڴ�
		_delete(pModisDataDay);
		_delete(pModisDataNight);
		KFloat2DMemAllocator::FreeMemory2D(ppfDayAbe);
		KFloat2DMemAllocator::FreeMemory2D(ppfDayLst);
		KFloat2DMemAllocator::FreeMemory2D(ppfNightLst);

		return TRUE;
	}

	// 
	BOOL KModisDataProcessor::Dix_Tvdi(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ���ݶ���
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();
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
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KModisAlgorithm oModisAlgorithm;
		oModisAlgorithm.SetData(&wDataCh[0]);	// ���ü��������

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE; 
		bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[31]>=0) && (ch[32]>=0);
		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Vix"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		// 1.���� NDVI
		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("���� NDVI ..."));
		PFLOAT* ppfNdvi = NULL;
		if(this->Vix(AGMRS_PRODUCT_CODE_VIX_NDVI))
		{
			// �����ڴ汣�� NDVI ����
			if(!KFloat2DMemAllocator::AllocMemory2D(ppfNdvi,iHeight,iWidth))
			{
				::MessageBox(NULL, _T("�ڴ���ɴ���! -- Dix_Tvdi"),_T(""),MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
			memcpy(*ppfNdvi,*m_ppfProduct,sizeof(FLOAT) * iWidth * iHeight);
		}
		if(!ppfNdvi)
			return FALSE;

		// 2.���� LST
		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("���� LST ..."));
		PFLOAT* ppfLst = NULL;
		if(this->Lst(LstMethod::Sobrino))
		{
			// �����ڴ汣�� LST ����
			if(!KFloat2DMemAllocator::AllocMemory2D(ppfLst,iHeight,iWidth))
			{
				::MessageBox(NULL, _T("�ڴ���ɴ���! -- Dix_Tvdi"),_T(""),MB_OK|MB_ICONINFORMATION);
				return FALSE;
			}
			memcpy(*ppfLst,*m_ppfProduct,sizeof(FLOAT) * iWidth * iHeight);
		}
		if(!ppfLst)
			return FALSE;

		// 3.������Чֲ��ָ������
		//   ȡ�� ndvi [0.01,0.80]*100->[1,80] ��Χ�ڶ�Ӧ�� (i,j) ����
		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
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

			int xxx=0;

		}// for(i)

		// �ͷ��ڴ�
		KFloat2DMemAllocator::FreeMemory2D(ppfLst);
		KFloat2DMemAllocator::FreeMemory2D(ppfNdvi);

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisData->GetProductDataInfo();

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
		KString strPathName(pModisData->GetFileName());
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

	// ��ֱ�ɺ�ָ�� - PDI
	BOOL KModisDataProcessor::Dix_Pdi(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ���ݶ���
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();
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
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KModisAlgorithm oModisAlgorithm;
		oModisAlgorithm.SetData(&wDataCh[0]);	// ���ü��������

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE; 
		bChannelOk = (ch[1]>=0) && (ch[2]>=0);
		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Dix_Pdi"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		// 1.�������ͨ��(ch1)��Ч��������...
		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("�������ͨ����Ч��������..."));
		vector<POINT> ptArray[100];
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				WORD wDataCh1 = (WORD)(0.1*ppwData[ch[1]][i*iWidth+j]+0.5);	// ͨ�����ݷŴ��� 10 ��?

				// 10% ~ 90%
				for(int v=10;v<=90;v++)
				{
					if(wDataCh1 == v)
					{
						POINT pt;
						pt.x = j;
						pt.y = i;
						ptArray[v].push_back(pt);
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

		// 2.�������ͨ��(RED - ch1)��Ч���ݶ�Ӧ��
		//     ������ͨ��(NIR - ch2)���ݵ����ֵ������Сֵ��
		WORD wDataCh2Min[100];
		WORD wDataCh2Max[100];
		memset(&wDataCh2Min[0],0xffff,sizeof(WORD)*100);
		memset(&wDataCh2Max[0],0x0000,sizeof(WORD)*100);

		// 10% ~ 90%
		for(int k=10;k<=90;k++)
		{
			vector<POINT> pa = ptArray[k];
			int size = pa.size();
			for(int m=0;m<size;m++)
			{
				POINT pt = pa[m];
				j = pt.x;
				i = pt.y;

				WORD wDataCh2 = (WORD)(0.1*ppwData[ch[2]][i*iWidth+j]+0.5);

				// 0% --> 100%
				//if( wDataCh2>0 && wDataCh2<=1000 )
				if( wDataCh2>0 && wDataCh2<=100 )
				{
					wDataCh2Min[k] = min(wDataCh2Min[k],wDataCh2);
					wDataCh2Max[k] = max(wDataCh2Max[k],wDataCh2);
				}
			}

		}

		// 3.������Ч��������
		BOOL bMin[100],bMax[100];
		memset(&bMin[0],FALSE,sizeof(BOOL)*100);
		memset(&bMax[0],FALSE,sizeof(BOOL)*100);
		int iNumOfMin(0),iNumOfMax(0);
		for(i=10;i<=90;i++)
		{
			bMin[i] = (wDataCh2Min[i]>0 && wDataCh2Min[i]<1000);
			if(bMin[i]) iNumOfMin++;

			bMax[i] = (wDataCh2Max[i]>0 && wDataCh2Max[i]<1000);
			if(bMax[i]) iNumOfMax++;

		}

		int imin(0),imax(0);
		double dDataCh1Min[100],dDataCh2Min[100];
		double dDataCh1Max[100],dDataCh2Max[100];

		for(i=0;i<100;i++)
		{

			if(bMin[i])
			{
				dDataCh1Min[imin] = (double)(i)/100;
				dDataCh2Min[imin] = (double)wDataCh2Min[i]/100;
				imin++;
			}
			if(bMax[i])
			{
				dDataCh1Max[imax] = (double)(i)/100;
				dDataCh2Max[imax] = (double)wDataCh2Max[i]/100;
				imax++;
			}
		}

		// ���㷽�̲���
		double a1,b1,c1[2],dt1[6];
		double a2,b2,c2[2],dt2[6];
		// ch2min = a + b*ch1
		//     y = a0 + a1*x
		KRegression::Linear1D(&dDataCh1Min[0],&dDataCh2Min[0],iNumOfMin,c1,dt1);
		KRegression::Linear1D(&dDataCh1Max[0],&dDataCh2Max[0],iNumOfMax,c2,dt2);

		a1 = c1[0];	b1 = c1[1];
		a2 = c2[0];	b2 = c2[1];

		if(b1<b2)
		{
			// b1 Ҫ���� b2
			::MessageBox(NULL, _T("b1<b2! -- Dix_Pdi"),_T(""),MB_OK|MB_ICONINFORMATION);
			//return FALSE;
		}

		double fm = (double)b1;

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float) * iWidth * iHeight);

		// ���㴹ֱ�ɺ�ָ�� - PDI
		WORD wDataCh1(0),wDataCh2(0);

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

		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("���� PDI ..."));
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
				wDataCh1	= ppwData[ch[1]][pos] & 0x0FFF;
				wDataCh2	= ppwData[ch[2]][pos] & 0x0FFF;

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

				// ȡ����Ԫ������
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Land:
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
						dBaseValue = 0; dScale = 1.0;
						double dNdvi = oModisAlgorithm.Ndvi();
						if(dNdvi>0 && dNdvi<1)
						{
							//double dTemp = 0.6003 * (wDataCh1 + 1.3324 * wDataCh2) * 0.1;	// �ع�

							//double fm = (double)b1;
							dValue = oModisAlgorithm.Pdi(fm);
							bValid = (dValue>-1 && dValue<1);

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
							else
							{
								m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
							}
						}
						else
						{
							m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
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

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.����
		pProductDataInfo->wYear				= pDataInfo->Year();
		pProductDataInfo->wMonth			= pDataInfo->Month();
		pProductDataInfo->wDay				= pDataInfo->Day();
		pProductDataInfo->wHour				= pDataInfo->Hour();
		pProductDataInfo->wMinute			= pDataInfo->Minute();
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
		KString strPathName(pModisData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileNameWithoutPath = strPathName.Mid(pos+1);		// ȡ���ļ���(��·��)
		KString strProduct(_T("pdi"));

		TCHAR szComment[MAX_PATH];
		_stprintf(szComment,_T("DIX_%s(%d)_%s"),strProduct.GetString(), iProductID, strFileNameWithoutPath.GetString());
		pProductDataInfo->SetComment(szComment);

		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...

		///------------------------------------------------

		return TRUE;
	}

	// �����Ĵ�ֱ�ɺ�ָ��
	BOOL KModisDataProcessor::Dix_Mpdi(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ���ݶ���
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();
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
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KModisAlgorithm oModisAlgorithm;
		oModisAlgorithm.SetData(&wDataCh[0]);	// ���ü��������

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE; 
		bChannelOk = (ch[1]>=0) && (ch[2]>=0);
		if(!bChannelOk)
		{
			::MessageBox(NULL, _T("����ͨ�����ݲ�ȫ! -- Dix_Pdi"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		// ���� NDVI
		if(!this->Vix(AGMRS_PRODUCT_CODE_VIX_NDVI))
		{
			::MessageBox(NULL, _T("���� NDVI����! -- Dix_Pdi"),_T(""),MB_OK|MB_ICONINFORMATION);
			return FALSE;
		}

		PFLOAT* ppfNdvi = this->GetProductData();

		// ���� NDVI �����Сֵ
		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("���� NDVI �����Сֵ..."));
		double dNdviMin(99999),dNdviMax(-99999);
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				double ndvi = ppfNdvi[i][j];
				if(ndvi>0 && ndvi<1)
				{
					dNdviMin = min(dNdviMin,ndvi);	// 0.001
					dNdviMax = max(dNdviMax,ndvi);	// 0.821
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

		// 1.�������ͨ��(ch1)��Ч��������...
		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("�������ͨ����Ч��������..."));
		vector<POINT> ptArray[100];
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				WORD wDataCh1 = (WORD)(0.1*ppwData[ch[1]][i*iWidth+j]+0.5);	// ͨ�����ݷŴ��� 10 ��?

				// 10% ~ 90%
				for(int v=10;v<=90;v++)
				{
					if(wDataCh1 == v)
					{
						POINT pt;	pt.x = j;	pt.y = i;
						ptArray[v].push_back(pt);
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

		// 2.�������ͨ��(RED - ch1)��Ч���ݶ�Ӧ��
		//     ������ͨ��(NIR - ch2)���ݵ����ֵ������Сֵ��
		WORD wDataCh2Min[100];
		WORD wDataCh2Max[100];
		memset(&wDataCh2Min[0],0xffff,sizeof(WORD)*100);
		memset(&wDataCh2Max[0],0x0000,sizeof(WORD)*100);

		// 10% ~ 90%
		for(int k=10;k<=90;k++)
		{
			vector<POINT> pa = ptArray[k];
			int size = pa.size();
			for(int m=0;m<size;m++)
			{
				POINT pt = pa[m];
				j = pt.x;
				i = pt.y;

				WORD wDataCh2 = (WORD)(0.1*ppwData[ch[2]][i*iWidth+j]+0.5);

				// 0% --> 100%
				//if( wDataCh2>0 && wDataCh2<=1000 )
				if( wDataCh2>0 && wDataCh2<=100 )
				{
					wDataCh2Min[k] = min(wDataCh2Min[k],wDataCh2);
					wDataCh2Max[k] = max(wDataCh2Max[k],wDataCh2);
				}
			}

		}

		// 3.������Ч��������
		BOOL bMin[100],bMax[100];
		memset(&bMin[0],FALSE,sizeof(BOOL)*100);
		memset(&bMax[0],FALSE,sizeof(BOOL)*100);
		int iNumOfMin(0),iNumOfMax(0);
		for(i=10;i<=90;i++)
		{
			bMin[i] = (wDataCh2Min[i]>0 && wDataCh2Min[i]<1000);
			if(bMin[i]) iNumOfMin++;

			bMax[i] = (wDataCh2Max[i]>0 && wDataCh2Max[i]<1000);
			if(bMax[i]) iNumOfMax++;

		}

		int imin(0),imax(0);
		double dDataCh1Min[100],dDataCh2Min[100];
		double dDataCh1Max[100],dDataCh2Max[100];

		for(i=0;i<100;i++)
		{

			if(bMin[i])
			{
				dDataCh1Min[imin] = (double)(i)/100;
				dDataCh2Min[imin] = (double)wDataCh2Min[i]/100;
				imin++;
			}
			if(bMax[i])
			{
				dDataCh1Max[imax] = (double)(i)/100;
				dDataCh2Max[imax] = (double)wDataCh2Max[i]/100;
				imax++;
			}
		}

		// ���㷽�̲���
		double a1,b1,c1[2],dt1[6];
		double a2,b2,c2[2],dt2[6];
		// ch2min = a + b*ch1
		//     y = a0 + a1*x
		KRegression::Linear1D(&dDataCh1Min[0],&dDataCh2Min[0],iNumOfMin,c1,dt1);
		KRegression::Linear1D(&dDataCh1Max[0],&dDataCh2Max[0],iNumOfMax,c2,dt2);

		a1 = c1[0];	b1 = c1[1];
		a2 = c2[0];	b2 = c2[1];

		if(b1<b2)
		{
			// b1 Ҫ���� b2
			::MessageBox(NULL, _T("b1<b2! -- Dix_Pdi"),_T(""),MB_OK|MB_ICONINFORMATION);
			//return FALSE;
		}

		double fm = b1;
		double fvm = b2;

		//Ϊ��Ʒ���ݷ����ڴ�
		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
		if(!KFloat2DMemAllocator::AllocMemory2D(m_ppfProduct,iHeight,iWidth))
		{
			return FALSE;
		}
		memset(*m_ppfProduct,0,sizeof(float) * iWidth * iHeight);

		// ���㴹ֱ�ɺ�ָ�� - PDI
		//
		WORD wDataCh1(0),wDataCh2(0);

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

		//ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
		pProgressParam->bReset = TRUE;
		pProgressParam->SetInfo(_T("���� MPDI ..."));
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
				wDataCh1	= ppwData[ch[1]][pos] & 0x0FFF;
				wDataCh2	= ppwData[ch[2]][pos] & 0x0FFF;

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

				// ȡ����Ԫ������
				int iPixelType = this->GetPixelType(ii,j);		// y,x
				switch(iPixelType)
				{
				case PixelType::Land:
					{
						m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
						dBaseValue = 0; dScale = 1.0;
						double dNdvi = oModisAlgorithm.Ndvi();
						if(dNdvi>dNdviMin && dNdvi<dNdviMax)
						{
							double pdi = oModisAlgorithm.Pdi(fm);
							double fv = (dNdviMax-dNdvi)/(dNdviMax-dNdviMin);	// ֲ�����Ƕ�
							double fvPdi = oModisAlgorithm.Pdi(fvm);

							dValue = (pdi - fv*fvPdi) / (1-fv);
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
							else
							{
								m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
							}
						}
						else
						{
							m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;
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
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID; AGMRS_PRODUCT_CODE_DIX_PDI;
					break;
				}

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

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisData->GetProductDataInfo();

		// 1.��ʽ����
		pProductDataInfo->wFormatCode		= AXIN_FMT_CODE_GRID_IMAGE;			// important

		// 2.����
		pProductDataInfo->wYear				= pDataInfo->Year();
		pProductDataInfo->wMonth			= pDataInfo->Month();
		pProductDataInfo->wDay				= pDataInfo->Day();
		pProductDataInfo->wHour				= pDataInfo->Hour();
		pProductDataInfo->wMinute			= pDataInfo->Minute();
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
		KString strPathName(pModisData->GetFileName());
		int pos = strPathName.ReverseFind(_T("\\"));
		KString strFileNameWithoutPath = strPathName.Mid(pos+1);		// ȡ���ļ���(��·��)
		KString strProduct(_T("mpdi"));

		TCHAR szComment[MAX_PATH];
		_stprintf(szComment,_T("DIX_%s(%d)_%s"),strProduct.GetString(), iProductID, strFileNameWithoutPath.GetString());
		pProductDataInfo->SetComment(szComment);

		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...

		///------------------------------------------------

		return TRUE;
	}

	// ������ - SIM (Single Item Monitor)
	// �ơ���ˮ�����
	BOOL KModisDataProcessor::Sim(int iProductID)
	{
		int i,j;

		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ������Ϣ
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();
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
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
			m_DataIndexOfChannel[i] = ch[i];
		}

		// �㷨����
		KModisAlgorithm oModisAlgorithm;
		oModisAlgorithm.SetData(&wDataCh[0]);	// ���ü��������

		// ������Ϣ
		TCHAR szInfo[MAX_PATH] = _T("");
		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		// �����Ҫ��ͨ��
		BOOL bChannelOk = FALSE; 
		switch(iProductID)
		{
		case AGMRS_PRODUCT_CODE_MISC_CLOUD:	// ��
			bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[34]>=0);
			break;

		case AGMRS_PRODUCT_CODE_MISC_FOG:	// ��
			bChannelOk = FALSE;
			break;

		case AGMRS_PRODUCT_CODE_MISC_SNOW:	// ѩ
			bChannelOk = ch[1]>=0 && ch[2]>=0 && ch[4]>=0 && ch[6]>=0
				 && ch[20]>=0 && ch[26]>=0 && ch[29]>=0
				 && ch[31]>=0 && ch[32]>=0
				;
			break;

		case AGMRS_PRODUCT_CODE_MISC_ICE:	// ��
			break;

		case AGMRS_PRODUCT_CODE_MISC_FIRE:	// ��� --> ��������ͨ��(���� => 1��2��23��31 ���� => 23��31)
			{
				_stprintf(szInfo,_T("������Ʒ ..."));

				if(pModisData->IsDay())
					bChannelOk = TRUE
								&& (ch[1]>=0)
								&& (ch[2]>=0)
								&& (ch[21]>=0)
								&& (ch[23]>=0)
								&& (ch[31]>=0)
								&& (ch[32]>=0)
								;
				else
					bChannelOk = (ch[1]>=0) && (ch[2]>=0) && (ch[23]>=0) && (ch[31]>=0);
			}
			break;

		case AGMRS_PRODUCT_CODE_MISC_WATER:	// ˮ��
			bChannelOk = (ch[1]>=0) && (ch[2]>=0);
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
		WORD wDataCh1(0),wDataCh2(0),wDataCh4(0),wDataCh6(0);
		WORD wDataCh20(0),wDataCh21(0),wDataCh23(0),wDataCh26(0),wDataCh29(0);
		WORD wDataCh31(0),wDataCh32(0),wDataCh34(0);
		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		BOOL bValid(FALSE);

		//=================================================
		double dtmax = -10000;
		double dtmin = +99999;
		//=================================================

		int iFireCount = 0;
		int iSnowCount = 0;

		double lon =0;
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
				if(iProductID == AGMRS_PRODUCT_CODE_MISC_CLOUD)
				{
					wDataCh34	= ppwData[ch[34]][pos] & 0x0FFF; wDataCh34 = wDataCh34>0x0ffe ? AGMRS_PRODUCT_DATA_INVALID : wDataCh34;
				}
				if(iProductID == AGMRS_PRODUCT_CODE_MISC_FIRE)
				{
					wDataCh21	= ppwData[ch[21]][pos] & 0x0FFF;
					wDataCh23	= ppwData[ch[23]][pos] & 0x0FFF;
					wDataCh31	= ppwData[ch[31]][pos] & 0x0FFF;
					wDataCh32	= ppwData[ch[32]][pos] & 0x0FFF;
				}
				if(iProductID == AGMRS_PRODUCT_CODE_MISC_SNOW)
				{
					wDataCh4	= ppwData[ch[4]][pos] & 0x0FFF;
					wDataCh6	= ppwData[ch[6]][pos] & 0x0FFF;
					wDataCh20	= ppwData[ch[20]][pos] & 0x0FFF;
					wDataCh26	= ppwData[ch[26]][pos] & 0x0FFF;
					wDataCh29	= ppwData[ch[29]][pos] & 0x0FFF;
					wDataCh31	= ppwData[ch[31]][pos] & 0x0FFF;
					wDataCh32	= ppwData[ch[32]][pos] & 0x0FFF;
				}

				// ��Ч����
				if(wDataCh1==0x0FFF || wDataCh2==0x0FFF || wDataCh4==0x0FFF || wDataCh6==0x0FFF
					 || wDataCh20==0x0FFF || wDataCh26==0x0FFF || wDataCh29==0x0FFF
					 || wDataCh31==0x0FFF || wDataCh32==0x0FFF
					)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}
				if(wDataCh1==0x0FFE || wDataCh2==0x0FFE || wDataCh4==0x0FFE || wDataCh6==0x0FFE
					 || wDataCh20==0x0FFE || wDataCh26==0x0FFE || wDataCh29==0x0FFE
					 || wDataCh31==0x0FFE || wDataCh32==0x0FFE
					)
				{
					m_ppfProduct[i][j] = AGMRS_PRODUCT_DATA_INVALID;	// ��Чֵ
					continue;
				}

				// Ϊ��ѩָ���㷨�ṩ����
				wDataCh[4]	= wDataCh4;
				wDataCh[6]	= wDataCh6;

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
						pModisData->xy2LonLat(j,i,lon,lat,1);

						// ���
						FirePoint firePoint;
						firePoint.Id = iFireCount;				// id
						firePoint.Type = FireType::FireNaked;	// ����
						firePoint.Row = i;
						firePoint.Col = j;
						firePoint.Lon = (float)lon;
						firePoint.Lat = (float)lat;
						firePoint.Temperature = 0.1f * wDataCh21;	// �¶�
						firePoint.Area = 0.1f * wDataCh21 / 500;	// ���(��������Ԫ���)
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
						pModisData->xy2LonLat(j,i,lon,lat,1);

						// ���
						FirePoint firePoint;
						firePoint.Id = iFireCount;				// id
						firePoint.Type = FireType::FireEmber;	// ����
						firePoint.Row = i;
						firePoint.Col = j;
						firePoint.Lon = (float)lon;
						firePoint.Lat = (float)lat;
						firePoint.Temperature = 0.1f * wDataCh21;	// �¶�
						firePoint.Area = 0.1f * wDataCh21 / 500;	// ���(��������Ԫ���)
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
						dValue = oModisAlgorithm.Ndsi();	// ��ѩָ��
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
			::MessageBox(NULL,_T("û�л��!"),_T("��Ϣ"),MB_OK);
			return FALSE;
		}
		else if(iProductID==AGMRS_PRODUCT_CODE_MISC_FIRE)
		{
			// ������(���ݻ������ֵ���飬�л���ֵ���ڵ������ͬ)
			this->Grouping(pFirePointVector);
		}

		if(iProductID==AGMRS_PRODUCT_CODE_MISC_SNOW && iSnowCount==0)
		{
			::MessageBox(NULL,_T("û�л�ѩ!"),_T("��Ϣ"),MB_OK);
			return FALSE;
		}

		///------------------------------------------------
		// ����Ʒ������Ϣ
		KDataInfo* pProductDataInfo = pModisData->GetProductDataInfo();

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
		KString strPathName(pModisData->GetFileName());
		int place = strPathName.ReverseFind(_T("\\"));
		KString strFileName = strPathName.Mid(place+1);		// ȡ���ļ���(��·��)

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
			pProductDataInfo->Flag()			= 1;	// ��ע��
			pProductDataInfo->ContourNumbers()	= 0;
			pProductDataInfo->ClipArea()		= 0;
		}

		// 6. ���ⲿ���õ�ɫ��͵�ɫ������
		// ...

		///------------------------------------------------

		return TRUE;
	}

	// ��Ԫ����
	// PixelType::Cloud,	// ��(1,2,31)
	// PixelType::Fog,		// ��()
	// PixelType::Fire,		// ��(1,2,21,31)
	// PixelType::Snow,		// ѩ(1,2,4,6,20,26,29,31,32)
	// PixelType::Ice,		// ��()
	// PixelType::Land,		// ½(1,2,4)
	// PixelType::Water,	// ˮ(1,2)
	// i - ��
	// j - ��
	int KModisDataProcessor::GetPixelType(int i,int j)
	{
		int iPixelType = PixelType::Unknown;

		// ���ݶ���
		KModisData* pModisData = (KModisData*)GetOwner();
		// ������Ϣ
		KDataInfo* pDataInfo = pModisData->GetDataInfo();
		// ����ָ��
		PWORD* ppwData = pModisData->GetData();

		//���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		if(i>=iHeight || j>=iWidth)
		{
			return PixelType::Unknown;
		}

		// �б���ֵ
		KModisThreshold threshold = *((KModisThreshold*)pModisData->GetThreshold());

		int ch1		= m_DataIndexOfChannel[1];		// %
		int ch2		= m_DataIndexOfChannel[2];		// %
		int ch4		= m_DataIndexOfChannel[4];		// %
		int ch6		= m_DataIndexOfChannel[6];		// %
		int ch20	= m_DataIndexOfChannel[20];		// K
		int ch21	= m_DataIndexOfChannel[21];		// K
		int ch23	= m_DataIndexOfChannel[23];		// K
		int ch26	= m_DataIndexOfChannel[26];		// %
		int ch29	= m_DataIndexOfChannel[29];		// %
		int ch31	= m_DataIndexOfChannel[31];		// K
		int ch32	= m_DataIndexOfChannel[32];		// K
		int ch34	= m_DataIndexOfChannel[34];		// %

		double dDataCh1(0),dDataCh2(0),dDataCh4(0),dDataCh6(0);
		double dDataCh20(0),dDataCh21(0),dDataCh26(0),dDataCh29(0),dDataCh31(0),dDataCh32(0),dDataCh34(0);

		if(ch1>=0)	dDataCh1	= 1.0 * ppwData[ch1][i*iWidth+j] / 10;		// %
		if(ch2>=0)	dDataCh2	= 1.0 * ppwData[ch2][i*iWidth+j] / 10;		// %
		if(ch4>=0)	dDataCh4	= 1.0 * ppwData[ch4][i*iWidth+j] / 10;		// %
		if(ch6>=0)	dDataCh6	= 1.0 * ppwData[ch6][i*iWidth+j] / 10;		// %

		if(ch20>=0)	dDataCh20	= 1.0 * ppwData[ch20][i*iWidth+j] / 10;		// K
		if(ch21>=0)	dDataCh21	= 1.0 * ppwData[ch21][i*iWidth+j] / 10;		// K
		if(ch26>=0)	dDataCh26	= 1.0 * ppwData[ch26][i*iWidth+j] / 10;		// %
		if(ch29>=0)	dDataCh29	= 1.0 * ppwData[ch29][i*iWidth+j] / 10;		// %
		if(ch31>=0)	dDataCh31	= 1.0 * ppwData[ch31][i*iWidth+j] / 10;		// K
		if(ch32>=0)	dDataCh32	= 1.0 * ppwData[ch32][i*iWidth+j] / 10;		// K
		if(ch34>=0)	dDataCh34	= 1.0 * ppwData[ch34][i*iWidth+j] / 10;		// %

		//-------------------------------------------------
		// 1.���ж��ƺ�ˮ��
		//-------------------------------------------------
		// ���� 31 ͨ��
		if(m_DataIndexOfChannel[1]>=0 && m_DataIndexOfChannel[2]>=0 && m_DataIndexOfChannel[31]>=0)
		{
			if (dDataCh1>threshold.dCloudRed && (dDataCh2 - dDataCh1)>threshold.dCloudNirRed && dDataCh31<threshold.dCloudT11um)
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
		// ������ 31 ͨ��
		else if(m_DataIndexOfChannel[1]>=0 && m_DataIndexOfChannel[2]>=0 && !(m_DataIndexOfChannel[31]>=0))
		{
			if (dDataCh1>threshold.dCloudRed && (dDataCh2 - dDataCh1)>threshold.dCloudNirRed)
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
		else
		{
			iPixelType = PixelType::Unknown;
		}
		//-------------------------------------------------
		// 2. �жϴ���
		if(iPixelType == PixelType::Cloud)
		{
			//...
		}
		//-------------------------------------------------
		// 3. �жϻ��
		BOOL bDay = pModisData->IsDay();
		// ����ų��� �������� T 4 <315K ��ҹ�� 305K ���� ��T 41 �� 5K �� 3K �������ض����ǻ��
		BOOL bNotFire = FALSE;	// һ�����ǻ��
		bNotFire = bDay
			? dDataCh21 < threshold.dFiringT3930nm || (dDataCh21 - dDataCh31) < threshold.dFiringT3930nm_T11um
			: dDataCh21 < threshold.dNightFiringT3930nm || (dDataCh21 - dDataCh31) < threshold.dNightFiringT3930nm_T11um;

		// ����ⷽ��ѡ��(��ֵ���������¶ȷ�)
		int fireMethod = this->GetFlag();

		// 3.1 ���� MODIS ����̽���㷨����:
		// �������б�Ch1<30  Ch2<30  Ch21>320  Ch21-Ch31>20
		// ���ϻ���б�Ch21>315  Ch21-Ch31>10
		// ���������б�NDVI=[-0.05��0.1]  ch31>278  ch1<8  ch2<8
		if(fireMethod == 1 && (iPixelType == PixelType::Land || iPixelType == PixelType::Cloud) && !bNotFire )
		{
			//3.1.1 �ж������
			BOOL bFireNaked = FALSE;
			if(bDay)
			{
				BOOL a = dDataCh1 < threshold.dFiringRed && dDataCh2 < threshold.dFiringNir;
				BOOL b = dDataCh21 > threshold.dFiringT3930nm;
				BOOL c = (dDataCh21 - dDataCh31) > threshold.dFiringT3930nm_T11um;
				bFireNaked = a && b && c;
			}
			else
			{
				BOOL b = dDataCh21 > threshold.dNightFiringT3930nm;
				BOOL c = (dDataCh21 - dDataCh31) > threshold.dNightFiringT3930nm_T11um;
				bFireNaked = b && c;
			}

			// 3.1.2 �жϰ����
			BOOL bFireDark	= FALSE;
			if(bFireNaked)
			{
				// waiting...
			}

			// 3.1.3 �жϹ����
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

		// 3.2 ���� MODIS ����̽���㷨���£� 
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
		// ���(��ҪMODISͨ��1,2,21,31��21,31�����¶�)
		// 
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
			GetBackgroundTemperature(t4a, na,ch21,r0,c0,np,np);	// ȡ��T4�ı����¶�
			GetBackgroundTemperature(t11a,na,ch31,r0,c0,np,np);	// ȡ��T11�ı����¶�

			// �ų����
			// ��ȡ������Ϣʱ�˳���������Ϊ�� ��T 41 = T 4 -T 11 �� 20 K ��ҹ��Ϊ 10K ���� T 4 �� 320K �� 315K ����
			// ...

			// ��������ͨ�����²�
			for(i=0;i<np*np;i++)
			{
				t41a[i] = t4a[i] - t11a[i];
			}

			// T4��T4b����T4b
			double T4 = dDataCh21;
			double T4b = KStatistic::RsAvg(t4a,na);		// �����¶�ƽ��ֵ - T4b
			double detT4b = KStatistic::RsSd(t4a,na);	// �����¶ȱ�׼ƫ�� - ��T4b
			if(detT4b<2) detT4b = 2;					// 

			// T11��T11b����T11b
			double T11 = dDataCh31;
			double T11b = KStatistic::RsAvg(t11a,na);
			double detT11b = KStatistic::RsSd(t11a,na);
			if(detT11b<2) detT11b = 2;

			// T41��T41b����T41b
			double T41 = T4 - T11;
			double T41b = KStatistic::RsAvg(t41a,na);	// �����¶�ƽ��ֵ - T41b
			double detT41b = KStatistic::RsSd(t41a,na);	// �����¶ȱ�׼ƫ�� - ��T41b
			if(detT41b<2) detT41b = 2;
			//---------------------------------------------
			// 3.2.1 �ж������
			BOOL bFireNaked	= FALSE;
			if(bDay)	// ����
			{
				// ͨ������1,2,21,31(1,2ͨ������ҫ���ų�)
				BOOL bHasData = ch1>=0 && ch2>=0 && ch21>=0 && ch31>=0;
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
				// ͨ������21,31
				BOOL bHasData = ch21>=0 && ch31>=0;
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

			// 3.2.2 �жϰ����
			BOOL bFireDark	= FALSE;
			if(bFireNaked)
			{
				// waiting...
			}

			// 3.2.3 ���жϹ����
			BOOL bFireEmber		= FALSE;
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
		// 
		// ��Ȫ(��ž�����ѧ����ѧԺ).��������.��4��
		if(0)
		{
			BOOL bSnow = FALSE;

			// 1. ����ˮ��
			//    0.555um < 10%
			BOOL bIsWater = dDataCh4 < 10;
			if(!bIsWater)
			{
				// 2. ��ȡ����ѩ
				//    0.555um > 30%
				//    0.555um > 0.865um
				BOOL bIsCloud_Snow = (dDataCh4 > threshold.dSnowCh4) && (dDataCh4 > dDataCh2);
				if(bIsCloud_Snow)
				{
					// 3. �����һ����ֻ�ѩָ��(NDSI)
					double dNdsi = (dDataCh4 - dDataCh6) / (dDataCh4 + dDataCh6);
					// 4. ��ѩ��Ԫ(��ѩ��������)
					//    NDSI > 0.4
					//    0.865um > 11%
					if(dNdsi>threshold.dNdsi && dDataCh2 > threshold.dSnowCh2)
					{
						// 5. �������(�и��ƺ;���)
						//    T8.55um - T11um > Tth1
						//    R1.375um > Rth2
						//BOOL bHCloud = (dDataCh29-dDataCh31) > threshold.dSnowCh29Ch31 && dDataCh26 > threshold.dSnowCh26;
						BOOL bHCloud = FALSE;
						if( !bHCloud )
						{
							// 6. �����е���
							//    T3.7um - T11um > Tth3;
							//BOOL bLCloud = (dDataCh20-dDataCh31) > threshold.dSnowCh20Ch31;
							BOOL bLCloud = FALSE;
							if(!bLCloud)
							{
								bSnow = TRUE;	// ��ѩ
							}
						}//if( !bHCloud )
					}//��ѩ��������
				}//if(bIsCloud_Snow)
			}//if(!bIsWater)

			if(bSnow)
			{
				iPixelType = PixelType::Snow;
			}
		}
		//-------------------------------------------------

		return iPixelType;
	}

	// �Ƿ����Ƶ�
	BOOL KModisDataProcessor::IsClearNight(int i,int j)
	{
		KModisData* pModisData = (KModisData*)GetOwner();
		KDataInfo* pDataInfo = pModisData->GetDataInfo();

		// ����ָ��
		PWORD* ppwData = pModisData->GetData();

		// ���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		if(i<0 || i>=iHeight || j<0 || j>=iWidth)
		{
			return FALSE;
		}

		// ͨ����(-1��ʾ������)
		int ch[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&ch[0],-1,sizeof(int)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		// ��������ͨ�����ڴ�ͨ��
		for(i=1;i<=36;i++)
		{
			int ch38 = pModisData->Channel2ChannelCode(i);
			ch[i] = pModisData->GetDataIndex(ch38);
		}

		if(ch[31]<0)
			return FALSE;

		KModisThreshold thd = *((KModisThreshold*)pModisData->GetThreshold());

		double dDataCh31 = 1.0 * ppwData[ch[31]][i*iWidth+j] / 10;	// K

		return (dDataCh31 > thd.dNightCloudT12umLo && dDataCh31 < thd.dNightCloudT12umHi);
	}

}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
