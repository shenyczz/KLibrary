/******************************************************************************
 ** KRdaData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRdaData - RDA 数据类
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
		// 内存
		BOOL PrepareMemory();
		void FreeMemory();

		// 搜集数据
		void RefCollect();
		void VelCollect();
		void SpwCollect();

	private:
		// 解码函数
		double DecodeRef(unsigned char code);					// 反射率解码
		double DecodeVel(unsigned char code, short ResType);	// 速度解码
		double DecodeSpw(unsigned char code);					// 普宽解码
		double DecodeElv(unsigned short code);					// 仰角解码

	private:
		BOOL m_bVolBeg;			// 体扫开始
		BOOL m_bVolEnd;			// 体扫结束

		int m_iElvIndexCur;		// 当前仰角索引
		int m_iAzmIndexCur;		// 当前方位角索引

		float* m_pRefData;		// 反射率数据
		float* m_pVelData;		// 速度数据
		float* m_pSpwData;		// 谱宽数据
		float* m_pElvData;		// 仰角数据

		RdaRadialDataPacket* m_pRdaRadialDataPacket;	// RDA 的一条径向雷达波束数据包

		//KRadialList* m_RadialListRef[RDA_DATA_MAX_CUTS];	// 强度径向数据集
		//KRadialList* m_RadialListVel[RDA_DATA_MAX_CUTS];	// 速度径向数据集
		//KRadialList* m_RadialListSpw[RDA_DATA_MAX_CUTS];	// 谱宽径向数据集

	public:
		static bool IsRdaData(LPCTSTR lpszFile);

	};

//--------------------------------------------------------
}}// namespace Wsr98d::Rda - end
//--------------------------------------------------------
