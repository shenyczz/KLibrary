/******************************************************************************
 ** KRdaData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRdaData - RDA ������
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

              //1. 
              use

******************************************************************************/

#pragma once

#include "KWsr98Data.h"

namespace Wsr98d { namespace Rda
{

	class KIND_EXT_CLASS KRdaData : public KWsr98Data
	{
	public:
		KRdaData(void);
		virtual ~KRdaData(void);

	public:
		virtual BOOL LoadData(LPCTSTR lpszFile, LPVOID lpvData = 0);

	protected:
		// �ڴ�
		BOOL PrepareMemory();
		void FreeMemory();

		// �Ѽ�����
		void RefCollect();
		void VelCollect();
		void SpwCollect();

	private:
		// ���뺯��
		double DecodeRef(unsigned char code);					// �����ʽ���
		double DecodeVel(unsigned char code, short ResType);	// �ٶȽ���
		double DecodeSpw(unsigned char code);					// �տ����
		double DecodeElv(unsigned short code);					// ���ǽ���

	private:
		BOOL m_bVolBeg;			// ��ɨ��ʼ
		BOOL m_bVolEnd;			// ��ɨ����

		int m_iElvIndexCur;		// ��ǰ��������
		int m_iAzmIndexCur;		// ��ǰ��λ������

		float* m_pRefData;		// ����������
		float* m_pVelData;		// �ٶ�����
		float* m_pSpwData;		// �׿�����
		float* m_pElvData;		// ��������

		RdaRadialDataPacket* m_pRdaRadialDataPacket;	// RDA ��һ�������״ﲨ�����ݰ�

		//KRadialList* m_RadialListRef[RDA_DATA_MAX_CUTS];	// ǿ�Ⱦ������ݼ�
		//KRadialList* m_RadialListVel[RDA_DATA_MAX_CUTS];	// �ٶȾ������ݼ�
		//KRadialList* m_RadialListSpw[RDA_DATA_MAX_CUTS];	// �׿������ݼ�

	public:
		static bool IsRdaData(LPCTSTR lpszFile);

	};

//--------------------------------------------------------
}}// namespace Wsr98d::Rda - end
//--------------------------------------------------------
