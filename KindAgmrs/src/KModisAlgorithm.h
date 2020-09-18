/******************************************************************************
 ** KModisAlgorithm.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KModisAlgorithm - MODIS ���ݷ����㷨
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

#include "KLDAlgorithm.h"

namespace Agmrs { namespace Modis 
{

	class KIND_EXT_CLASS KModisAlgorithm : public KLDAlgorithm
	{
	public:
		KModisAlgorithm(void);
		virtual ~KModisAlgorithm(void);

	public:
		// ȫ������
		double Abe();
		//ˮ��
		virtual double Vapour();

	public:
		//-------------------------------------------------
		//                  �ر��¶�
		//-------------------------------------------------
		virtual double Lst(int iMethod = 0);
		private:
			double Lst_Qin(double r31,double r32,double tao31,double tao32);
			double Lst_Becker(double r31,double r32,double tao31,double tao32,double vapour,double angle);
			double Lst_Sobrino(double r31,double r32,double tao31,double tao32,double vapour,double angle);
			double Lst_regress();

	public:
		//-------------------------------------------------
		//                  ֲ��ָ��
		//-------------------------------------------------
		// ��һ��ֲ��ָ��
		//virtual double Ndvi();
		// ��ֲֵ��ָ��
		//virtual double Rvi();
		// ��ֲֵ��ָ��
		//virtual double Dvi();
		// ��������ֲ��ָ��
		//virtual double Savi();
		// ��ǿ��ֲ��ָ��
		//virtual double Evi();

		// ֲ��ָ��������Ʒ
		//virtual double Lai();

		//-------------------------------------------------
		//                  �ɺ�ָ��
		//-------------------------------------------------
		// �ر�ˮ��ָ��
		virtual double Swci();
		// ��ֱ�ɺ�ָ��
		virtual double Pdi(double fm);

		//-------------------------------------------------
		//                  ����ָ��
		//-------------------------------------------------
		// ��һ����ѩָ��
		virtual double Ndsi();
	};

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
