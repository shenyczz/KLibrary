#include "StdAfx.h"
#include "KAgmrsData.h"
#include "KAgmrsDataProcessor.h"


namespace Agmrs
{
	typedef T2DMemAllocator<Data2GrayParam>		KData2GrayParam2DMemAllocator;

	KAgmrsDataProcessor::KAgmrsDataProcessor(void)
	{
		m_ppfProduct = NULL;

		m_pFirePointVector = NULL;

		m_pData2GrayParam = new Data2GrayParam[500];
		memset(m_pData2GrayParam, 0, sizeof(Data2GrayParam) * 500);

		memset(&m_DataIndexOfChannel[0],-1,sizeof(int)*200);
	}

	KAgmrsDataProcessor::~KAgmrsDataProcessor(void)
	{
		_delete(m_pFirePointVector);
		_deletea(m_pData2GrayParam);

		KFloat2DMemAllocator::FreeMemory2D(m_ppfProduct);
	}

	// �������ת��Ϊ��ɫ
	// ң���������û�е�ɫ��ʹ��ϵͳ�ṩ��Ĭ�ϵ�ɫ��
	// ���û��Ĭ�ϵ�ɫ��������ɻҶ�����
	COLORREF* KAgmrsDataProcessor::Data2Colors(COLORREF clrBack)
	{
		// ���ݶ���
		KDataObject* pDataObject = (KDataObject*)this->GetOwner();
		if(!pDataObject)
			return NULL;

		// ������Ϣ
		KDataInfo* pDataInfo = pDataObject->GetDataInfo();
		if(!pDataInfo)
			return NULL;

		// ����
		double* pData = pDataObject->GetVData();
		if(!pData)
			return NULL;

		// ��ɫ��
		KPalette* pPalette = pDataObject->GetPalette();
		if(!pPalette)
		{
			int iProductCode = pDataInfo->ProductCode();
			pPalette = KAgmrsData::GetAgmrsProductPalette(iProductCode, PaletteType_Segment);
			pDataObject->SetPalette(pPalette);
		}

		return KDataProcessor::Data2Colors(clrBack);
	}

	BOOL firesort(FirePoint firePoint1,FirePoint firePoint2)
	{
		return firePoint1.Group < firePoint2.Group;	// ����
	}

	// ����(�ݹ�)
	void KAgmrsDataProcessor::Grouping(FirePointVector* pFirePointVector)
	{
		int i;
		int iCount = (int)pFirePointVector->size();
		//if(iCount>10000)
		//	return;

		if(iCount==0)
			return;

		// ����������
		ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

		int iGroup = 0;
		FirePoint* pFirePoint0 = &pFirePointVector->at(0);
		pFirePoint0->Flag = 1;
		pFirePoint0->Group = ++iGroup;
		if(pProgressParam) pProgressParam->SetInfo(_T("������ ... "));
		for(i=0;i<iCount;i++)
		{
			FirePoint* pFirePoint = &pFirePointVector->at(i);
			if(pFirePoint->Flag!=0) // ���˵�ʹ�ù���
				continue;

			BOOL bFind = grouping(pFirePoint,pFirePointVector);
			if(!bFind)
			{
				pFirePoint->Flag = 1;
				pFirePoint->Group = ++iGroup;
			}

			// ��ʾ����
			if(m_pCallbackFunc)
			{
				if(m_pCallbackParam)
				{
					ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
					pProgressParam->lTotalNums = iCount;
					pProgressParam->lCurrentPos = i;
				}
				m_pCallbackFunc(m_pCallbackParam);
			}
		}

		//-------------------------------------------------
		// ��������
		list<FirePoint> firePointList;
		for(i=0;i<iCount;i++)
		{
			FirePoint firePoint = pFirePointVector->at(i);
			firePointList.push_back(firePoint);
		}

		pFirePointVector->clear();
		firePointList.sort(firesort);
		list<FirePoint>::iterator itr;
		for(itr=firePointList.begin();itr!=firePointList.end();itr++)
		{
			FirePoint firePoint = (*itr);
			pFirePointVector->push_back(firePoint);
		}

		firePointList.clear();
		//-------------------------------------------------
		return;
	}

	BOOL KAgmrsDataProcessor::grouping(FirePoint* pFirePoint, FirePointVector* pFirePointVector)
	{
		int i;
		int row0 = pFirePoint->Row;
		int col0 = pFirePoint->Col;
		int iCount = (int)pFirePointVector->size();
		BOOL bFind = FALSE;
		for(i=0;i<iCount;i++)
		{
			FirePoint* pFirePointGrouped = &pFirePointVector->at(i);
			if(pFirePointGrouped->Flag==0)	// ���˵�û��ʹ�ù���
				continue;

			int row1 = pFirePointGrouped->Row;
			int col1 = pFirePointGrouped->Col;

			if(row0==row1 && col0==col1)	// ���˵���ͬ�ĵ�
				continue;

			int drow = row0-row1;
			int dcol = col0-col1;

			BOOL c1 = (drow==-1) && (dcol==-1 || dcol==0 || dcol==1);
			BOOL c2 = (drow== 0) && (dcol==-1 || dcol==1);
			BOOL c3 = (drow== 1) && (dcol==-1 || dcol==0 || dcol==1);

			if(c1 || c2 || c3)
			{
				bFind = TRUE;
				pFirePoint->Group = pFirePointGrouped->Group;
				pFirePoint->Flag = 1;
				break;
			}
		}

		return bFind;
	}

	// ����TVDI
	BOOL KAgmrsDataProcessor::Dix_Tvdi(KDataInfo* pDataInfo,float* pDataNdvi,float* pDataLst,float*& pDataTvdi)
	{
		int i,j;

		int iWidth = pDataInfo->xNum;
		int iHeight = pDataInfo->yNum;

		// �ص�����
		CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
		// ����������
		ProgressParam* pCallbackParam = (ProgressParam*)this->CallbackParam();

		// 1.������Чֲ��ָ������
		// ȡ�� ndvi [0.01,0.80]*100->[1,80] ��Χ�ڶ�Ӧ�� (i,j) ����
		if(pCallbackParam) pCallbackParam->SetInfo(_T("������Чֲ��ָ������..."));
		vector<POINT> ptArray[100];
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				float fNdvi = *(pDataNdvi + i * iWidth + j);
				WORD wNdvi = (WORD)(fNdvi*100);
				for(int v=1;v<=80;v++)
				{
					if(wNdvi == v)
					{
						POINT pt;	pt.x = j;	pt.y = i;
						ptArray[v].push_back(pt);			// ����ȫ�� v ֵ������
					}
				}
			}//j

			// ��ʾ����
			if(pCallbackFunc)
			{
				if(pCallbackParam)
				{
					pCallbackParam->lTotalNums = iHeight;
					pCallbackParam->lCurrentPos = i;
				}

				pCallbackFunc(pCallbackParam);
			}

		}//i

		// 2.����ֲ��ָ���Ͷ�Ӧ�ĵر��¶�
		if(pCallbackParam) pCallbackParam->SetInfo(_T("����ֲ��ָ���Ͷ�Ӧ�ĵر��¶�..."));
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

				float fLst = *(pDataLst + i * iWidth + j);	// K

				if( fLst>0+273.15 && fLst<100+273.15 )	// 0-500��
				{
					fLstMin[v] = min(fLstMin[v],fLst);
					fLstMax[v] = max(fLstMax[v],fLst);
				}

			}//m

			//---------------------------------------------
			// ��ʾ���� - BEG
			if(pCallbackFunc)
			{
				if(pCallbackParam)
				{
					pCallbackParam->lTotalNums = 80;
					pCallbackParam->lCurrentPos = v;
				}

				pCallbackFunc(pCallbackParam);
			}
			// ��ʾ���� - END
			//---------------------------------------------

		}//v

		// 3.������Ч��������
		BOOL bMin[100], bMax[100];
		memset(&bMin[0],FALSE,sizeof(BOOL)*100);
		memset(&bMax[0],FALSE,sizeof(BOOL)*100);
		int iNumberOfMin = 0;	// ��Сֵ����
		int iNumberOfMax = 0;	// ���ֵ����

		for(int v=1;v<=80;v++)
		{
			bMin[v] = TRUE
				&& !(fabs(fLstMin[v] - 99999) < EPS)
				&& (fLstMin[v]>(0+273.15) && fLstMin[v]<(100+273.15))
				;

			if(bMin[v])
				iNumberOfMin++;

			bMax[v] = TRUE
				&& !(fabs(fLstMax[v] + 99999) < EPS)
				&& (fLstMax[v]>(0+273.15) && fLstMax[v]<(100+273.15))
				;

			if(bMax[v])
				iNumberOfMax++;
		}

		int imin = 0,imax = 0;
		double dNdviMin[100],dNdviMax[100];
		double dTsmin[100],dTsmax[100];
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

		}//i

		// �ع�
		//--------------------------------------------
		//     y = a0 + a1*x
		// TSmin = a1 + b1*NDVI
		// TSmax = a2 + b2*NDVI
		//--------------------------------------------
		double a1,b1,a2,b2;
		double c1[2],c2[2],dt1[6],dt2[6];
		KRegression::Linear1D(&dNdviMin[0],&dTsmin[0],iNumberOfMin,c1,dt1);
		KRegression::Linear1D(&dNdviMax[0],&dTsmax[0],iNumberOfMax,c2,dt2);

		a1 = c1[0];		b1 = c1[1];
		a2 = c2[0];		b2 = c2[1];

		// ����
		_deletea(pDataTvdi);
		pDataTvdi = new float[iWidth*iHeight];
		if(!pDataTvdi)
			return FALSE;

		memset(pDataTvdi,0,sizeof(float)*iWidth*iHeight);

		double dTvdi = 0;
		if(pCallbackParam) pCallbackParam->SetInfo(_T("����TVDI ..."));
		for(i=0;i<iHeight;i++)
		{
			for(j=0;j<iWidth;j++)
			{
				int pos = i * iWidth + j;
				double ndvi = *(pDataNdvi + pos);	// (0~1)
				double Ts = *(pDataLst + pos);		// K

				// 1.NDVI = ��Ч����
				if(fabs(ndvi-pDataInfo->InvalidData())<EPS)
				{
					// 1.1 TS = ��Ч����;
					if(fabs(Ts-pDataInfo->InvalidData())<EPS)
					{
						dTvdi = pDataInfo->InvalidData();
					}
					// 1.2 TS = ����ֵ
					else if(Ts>AXIN_SPECIAL_DATA_BEG && Ts<AXIN_SPECIAL_DATA_END)
					{
						dTvdi = Ts;
					}
					// 1.3 TS = ����ֵ
					else if(Ts>0 && Ts<100+273.15)
					{
						dTvdi = pDataInfo->InvalidData();
					}
				}
				// 2.NDVI = ����ֵ
				if(ndvi>AXIN_SPECIAL_DATA_BEG && ndvi<AXIN_SPECIAL_DATA_END)
				{
					// 2.1 TS = ��Ч����;
					if(fabs(Ts-pDataInfo->InvalidData())<EPS)
					{
						dTvdi = ndvi;
					}
					// 2.2 TS = ����ֵ
					else if(Ts>AXIN_SPECIAL_DATA_BEG && Ts<AXIN_SPECIAL_DATA_END)
					{
						dTvdi = max(ndvi,Ts);
					}
					// 2.3 TS = ����ֵ
					else if(Ts>0 && Ts<100+273.15)
					{
						dTvdi = ndvi;
					}
				}
				// 3.NDVI = ����ֵ
				if(ndvi>0 && ndvi<1)
				{
					// 3.1 TS = ��Ч����;
					if(fabs(Ts-pDataInfo->InvalidData())<EPS)
					{
						dTvdi = pDataInfo->InvalidData();
					}
					// 3.2 TS = ����ֵ
					else if(Ts>AXIN_SPECIAL_DATA_BEG && Ts<AXIN_SPECIAL_DATA_END)
					{
						dTvdi = ndvi,Ts;
					}
					// 3.3 TS = ����ֵ
					else if(Ts>0 && Ts<100+273.15)
					{
						double Tsmin = a1 + b1*ndvi;			// K
						double Tsmax = a2 + b2*ndvi;			// K
						if(fabs(Tsmax-Tsmin)<EPS || Tsmax<Tsmin)
						{
							dTvdi = pDataInfo->InvalidData();
						}
						else
						{
							// ֵԽ��Խ�ɺ�
							dTvdi = (Ts - Tsmin) / (Tsmax - Tsmin);
						}
					}
				}

				*(pDataTvdi + pos) = (float)dTvdi;

			}//j

			//---------------------------------------------
			// ��ʾ���� - BEG
			if(pCallbackFunc)
			{
				if(pCallbackParam)
				{
					pCallbackParam->lTotalNums = iHeight;
					pCallbackParam->lCurrentPos = i;
				}

				pCallbackFunc(pCallbackParam);
			}
			// ��ʾ���� - END
			//---------------------------------------------
		}//i

		return TRUE;
	}

	// ��ȡ�����¶�
	// va - ����
	// na - ����ά��
	// (row0,col0)
	void KAgmrsDataProcessor::GetBackgroundTemperature(double* va, int& na, int ch, int row0, int col0, int rows, int cols)
	{
		int i,j;

		// ���ݶ���
		KAgmrsData* pKAgmrsData = (KAgmrsData*)GetOwner();
		// ���ݶ�����Ϣ
		KDataInfo* pDataInfo = pKAgmrsData->GetDataInfo();
		// ����ָ��
		PWORD* ppwData = pKAgmrsData->GetData();

		//���ݳߴ�
		int iWidth = pDataInfo->Width();
		int iHeight = pDataInfo->Height();

		int index = 0;
		for(i=row0;i<row0+rows;i++)
		{
			for(j=col0;j<col0+cols;j++)
			{
				if(i>=iHeight || j>=iWidth)
					break;

				try
				{
					WORD wData	= ppwData[ch][i*iWidth+j] & 0x0FFF;
					if(wData>0 && wData<0x0FFF)
					{
						//vat[index] = wData;
						va[index] = 0.1 * wData;
						index++;
					}
				}
				catch(...)
				{
					break;
				}

			}//next j

			int x=0;
			x++;

		}//next i

		na = index;

		return;
	}


//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------
