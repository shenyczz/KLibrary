/******************************************************************************
 ** KAgmrsDataProcessor.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KAgmrsDataProcessor - ũҵ����ң�����ݴ�����
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              //1. �������ݶ���

******************************************************************************/

#pragma once

#include "KAgmrsAlgorithm.h"

namespace Agmrs
{
	typedef vector<FirePoint> FirePointVector;

	class KIND_EXT_CLASS KAgmrsDataProcessor : public KDataProcessor
	{
	public:

	public:
		KAgmrsDataProcessor(void);
		virtual ~KAgmrsDataProcessor(void);

	public:
		// ����TVDI
		BOOL Dix_Tvdi(KDataInfo* pDataInfo,float* pDataNdvi,float* pDataLst,float*& pDataTvdi);

	public:
		// ��Ʒ
		PFLOAT* GetProductData() { return m_ppfProduct; }
		// ȡ�û�㼯��
		FirePointVector* GetFirePointVector() { return m_pFirePointVector; }
		// �㷨����ָ��
		//KAgmrsAlgorithm* GetAgmrsAlgorithm() { return m_pAgmrsAlgorithm; }

	#pragma region --���麯��--

	public:
		// ����ת��Ϊ DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0) = 0;

		// ��Ʒ���� - ȫ����
		virtual BOOL Abe(int iProductID) = 0;
		// ��Ʒ���� - �ر��¶�
		virtual BOOL Lst(int iMethod) = 0;
		// ��Ʒ���� - ֲ��ָ��
		virtual BOOL Vix(int iProductID) = 0;
		// ��Ʒ���� - �ɺ�ָ��
		virtual BOOL Dix(int iProductID) = 0;
		// ������ - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID) = 0;

		// ȡ����Ԫ����
		virtual int GetPixelType(int i,int j) = 0;
		// �Ƿ����(����)
		virtual BOOL IsClearNight(int i,int j) = 0;

		// �ϲ�����ͨ��
		virtual BOOL ChannelCombine(ChannelCombineInfo* pChannelCombineInfo1,
			ChannelCombineInfo* pChannelCombineInfo2,
			LPCTSTR lpszOutputFile,
			KExtents* pExtents) = 0;

	#pragma endregion

	protected:
		// �������ת��Ϊ��ɫ
		virtual COLORREF* Data2Colors(COLORREF clrBack);

		// ����
		void Grouping(FirePointVector* pFirePointVector);
		BOOL grouping(FirePoint* pFirePoint, FirePointVector* pGroupPointVector);

		// ��ȡ�����¶�
		// va - ����
		// na - ����ά��
		void GetBackgroundTemperature(double* va, int& na, int ch, int row0, int col0, int rows, int cols);

	protected:
		// ��Ʒ����
		PFLOAT* m_ppfProduct;
		// ����ת��Ϊ�Ҷ�ֵʹ�õĲ����ṹ����(��500ͨ��)
		Data2GrayParam* m_pData2GrayParam;
		// ��㼯��
		FirePointVector* m_pFirePointVector;
		// �㷨����
		//KAgmrsAlgorithm* m_pAgmrsAlgorithm;

		// ͨ��������������
		int m_DataIndexOfChannel[200];
	};

//---------------------------------------------------------
}// namespace Agmrs - end
//---------------------------------------------------------
