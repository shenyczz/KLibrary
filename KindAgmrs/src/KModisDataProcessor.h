/******************************************************************************
 ** KModisDataProcessor.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KModisDataProcessor - MODIS ���ݴ�����
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

#include "KLDFileProcessor.h"

namespace Agmrs { namespace Modis 
{

	class KIND_EXT_CLASS KModisDataProcessor : public KLDFileProcessor
	{
	public:
		KModisDataProcessor(void);
		virtual ~KModisDataProcessor(void);

	public:
		// ��Ʒ���� - ȫ����   - ABE ()
		virtual BOOL Abe(int iProductID);
		// ��Ʒ���� - �ر��¶� - LST ()
		virtual BOOL Lst(int iMethod);
		// ��Ʒ���� - ֲ��ָ�� - VIX ()
		//virtual BOOL Vix(int iProductID);
		// ��Ʒ���� - �ɺ�ָ�� - DIX ()
		virtual BOOL Dix(int iProductID);
		// ��Ʒ���� - ������ - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID);

		// ȡ����Ԫ����
		virtual int GetPixelType(int i,int j);
		// �Ƿ����(����)
		virtual BOOL IsClearNight(int i,int j);

	private:
		BOOL Dix_Ati(int iProductID);
		BOOL Dix_Tvdi(int iProductID);
		BOOL Dix_Pdi(int iProductID);
		BOOL Dix_Mpdi(int iProductID);

		// ��ȡ�����¶�
		//void GetTB(double* va, int& na, int ch, int row0, int col0, int rows, int cols);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
