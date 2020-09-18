#pragma once

#include "KLDFileProcessor.h"

namespace Agmrs { namespace Avhrr
{

	class KIND_EXT_CLASS KAvhrrDataProcessor : public KLDFileProcessor
	{
	public:
		KAvhrrDataProcessor(void);
		virtual ~KAvhrrDataProcessor(void);

	public:
		// 数据转换为 DIB
		virtual BOOL DataToDIB(int iRChannel = 0,int iGChannel = 0,int iBChannel = 0);

	public:
		// 产品生成 - 全照率
		virtual BOOL Abe(int iProductID) { return FALSE; }
		// 产品生成 - 地表温度
		virtual BOOL Lst(int iMethod) { return FALSE; }
		// 产品生成 - 植被指数
		virtual BOOL Vix(int iProductID);
		// 产品生成 - 干旱指数
		virtual BOOL Dix(int iProductID) { return FALSE; }
		// 单项监测 - SIM (Single Item Monitor)
		virtual BOOL Sim(int iProductID);

		// 取得像元类型
		virtual int GetPixelType(int i,int j);
		// 是否晴空(无云)
		virtual BOOL IsClearNight(int i,int j) { return FALSE; }

	private:
		// 数据转换为灰度
		void Data2Gray();
		// 计算通道数据转换为灰度数据的参数
		void CalcData2GrayParam();
		// 通道数据转换为灰度
		void ChannelData2Gray(Data2GrayParam* pData2GrayParam);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------
