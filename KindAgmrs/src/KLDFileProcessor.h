#pragma once

#include "KAgmrsDataProcessor.h"

namespace Agmrs { namespace LDFile
{

	class KIND_EXT_CLASS KLDFileProcessor : public KAgmrsDataProcessor
	{
	public:
		KLDFileProcessor(void);
		~KLDFileProcessor(void);

	public:
		// 数据转换为 DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0);

	protected:
		// 数据转换为灰度
		void Data2Gray();
		// 计算通道数据转换为灰度数据的参数
		void CalcData2GrayParam();
		// 通道数据转换为灰度
		void ChannelData2Gray(Data2GrayParam* pData2GrayParam);

	public:
		// 产品生成 - 植被指数
		virtual BOOL Vix(int iProductID);
		// 产品生成 - 干旱指数
		virtual BOOL Dix(int iProductID);

		// 单项监测 - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID) { return FALSE; }

		// 取得像元类型
		virtual int GetPixelType(int i,int j){ return PixelType::Unknown; }
		// 是否晴空(无云)
		virtual BOOL IsClearNight(int i,int j) { return FALSE; }

		// 合并数据通道
		virtual BOOL ChannelCombine(ChannelCombineInfo* pChannelCombineInfo1,
			                        ChannelCombineInfo* pChannelCombineInfo2,
									LPCTSTR lpszOutputFile,
									KExtents* pExtents) { return FALSE; }

		// 产品生成 - 地表温度
		virtual BOOL Lst(int iMethod) { return FALSE; }
		// 产品生成 - 全照率
		virtual BOOL Abe(int iProductID) { return FALSE; }

	protected:
		virtual BOOL Dix_Def(int iProductID) { return FALSE; }
		virtual BOOL Dix_Ati(int iProductID) { return FALSE; }
		virtual BOOL Dix_Tvdi(int iProductID);
		virtual BOOL Dix_Pdi(int iProductID) { return FALSE; }
		virtual BOOL Dix_Mpdi(int iProductID) { return FALSE; }
		virtual BOOL Dix_Vswi(int iProductID) { return FALSE; }
	};

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
