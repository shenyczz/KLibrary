/******************************************************************************
 ** KModisDataProcessor.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KModisDataProcessor - MODIS 数据处理器
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

#include "KLDFileProcessor.h"

namespace Agmrs { namespace Modis 
{

	class KIND_EXT_CLASS KModisDataProcessor : public KLDFileProcessor
	{
	public:
		KModisDataProcessor(void);
		virtual ~KModisDataProcessor(void);

	public:
		// 产品生成 - 全照率   - ABE ()
		virtual BOOL Abe(int iProductID);
		// 产品生成 - 地表温度 - LST ()
		virtual BOOL Lst(int iMethod);
		// 产品生成 - 植被指数 - VIX ()
		//virtual BOOL Vix(int iProductID);
		// 产品生成 - 干旱指数 - DIX ()
		virtual BOOL Dix(int iProductID);
		// 产品生成 - 单项监测 - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID);

		// 取得像元类型
		virtual int GetPixelType(int i,int j);
		// 是否晴空(无云)
		virtual BOOL IsClearNight(int i,int j);

	private:
		BOOL Dix_Ati(int iProductID);
		BOOL Dix_Tvdi(int iProductID);
		BOOL Dix_Pdi(int iProductID);
		BOOL Dix_Mpdi(int iProductID);

		// 提取背景温度
		//void GetTB(double* va, int& na, int ch, int row0, int col0, int rows, int cols);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
