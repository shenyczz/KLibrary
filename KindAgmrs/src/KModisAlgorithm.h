/******************************************************************************
 ** KModisAlgorithm.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KModisAlgorithm - MODIS 数据反演算法
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
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
		// 全反照率
		double Abe();
		//水汽
		virtual double Vapour();

	public:
		//-------------------------------------------------
		//                  地表温度
		//-------------------------------------------------
		virtual double Lst(int iMethod = 0);
		private:
			double Lst_Qin(double r31,double r32,double tao31,double tao32);
			double Lst_Becker(double r31,double r32,double tao31,double tao32,double vapour,double angle);
			double Lst_Sobrino(double r31,double r32,double tao31,double tao32,double vapour,double angle);
			double Lst_regress();

	public:
		//-------------------------------------------------
		//                  植被指数
		//-------------------------------------------------
		// 归一化植被指数
		//virtual double Ndvi();
		// 比值植被指数
		//virtual double Rvi();
		// 差值植被指数
		//virtual double Dvi();
		// 土壤调整植被指数
		//virtual double Savi();
		// 增强型植被指数
		//virtual double Evi();

		// 植被指数导出产品
		//virtual double Lai();

		//-------------------------------------------------
		//                  干旱指数
		//-------------------------------------------------
		// 地表含水量指数
		virtual double Swci();
		// 垂直干旱指数
		virtual double Pdi(double fm);

		//-------------------------------------------------
		//                  其他指数
		//-------------------------------------------------
		// 归一化积雪指数
		virtual double Ndsi();
	};

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
