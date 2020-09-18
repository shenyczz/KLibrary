/******************************************************************************
 ** KModisData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KModisData - MODIS 数据
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

		// 1. 生成对象
		KModisData* pModisData = new KModisData();
		// 1.1 装载数据
		pModisData->LoadData(pszFile);

		// 2. 数据处理器
		KModisDataProcessor* pModisDataProcessor = (KModisDataProcessor*)pModisData->GetDataProcessor();
		// 2.1 数据转换为图像
		pModisDataProcessor->DataToDIB();
		// 2.2 生成产品
		pModisDataProcessor->Lst(iMethod);

		// 3. 产品保存
		PWORD* ppwProduct = pAgmrsDataProcessor->GetProductData();
		KDataInfo* pDataInfo = pAgmrsData->GetDataInfo();
		KdHelper::CreateKdFile(fileName, *ppwProduct, pDataInfo, palFile)

******************************************************************************/

#pragma once

#include "KLDFile.h"

namespace Agmrs { namespace Modis 
{

	class KIND_EXT_CLASS KModisData : public KLDFile
	{
	public:
		KModisData(void);
		virtual ~KModisData(void);

	public:
		// 取得判别阈值
		virtual void* GetThreshold() { return GetModisThreshold(); }

	public:
		// 设置默认的显示通道
		virtual void SetDefaultDisplayChannel();
		///===========================================
		// 1 2 .............. 12     13           14      15 ................ 36		// 习惯通道
		// 1 2 .............. 12  [13 + 14]   [15 + 16]   17 ................ 38		// 实际通道
		// 0 1 .............. 11  [12 + 13]   [14 + 15]   16 ................ 37		// 索引通道
		///===========================================
		// 通道号转换为通道代码(1-36 to 1-38)
		virtual int Channel2ChannelCode(int iChannel);
		// 通道代码转换为通道号(1-38 to 1-36)
		virtual int ChannelCode2Channel(int iChannelCode);
		// 通道代码转换为通道名称
		LPCTSTR ChannelCode2String(int iChannelCode);

	};

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
