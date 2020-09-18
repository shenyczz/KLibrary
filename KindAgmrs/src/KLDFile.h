/******************************************************************************
 ** KLDFile.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KLDFile - 农业气象遥感数据本地文件
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
/******************************************************************************
关于通道、通道代码、数据索引的说明
通道		- Channel
通道代码	- ChannelCode
数据索引	- DataIndex

【通道】是科研人员口中的数据通道
【通道代码】局地文件通道索引数组中记录的代码。Mersi 的1通道的通道代码是51
【数据索引】是保存在内存中的对应通道代码的数据首地址的索引值
******************************************************************************/

#pragma once

#include "KAgmrsData.h"

namespace Agmrs { namespace LDFile
{

	class KIND_EXT_CLASS KLDFile : public KAgmrsData
	{
	public:
		KLDFile(void);
		~KLDFile(void);

	protected:
		// 装载数据
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);
		private:
			BOOL LoadData_bin_ld2(LPCTSTR lpszFile);
			BOOL LoadData_bin_ld3(LPCTSTR lpszFile);

	public:
		// 填充数据信息
		virtual void FillDataInfo(LPVOID lpvData);

	public:
		// 是白天数据
		virtual BOOL IsDay() { return ((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight == 0; }
		// 是晚上数据
		virtual BOOL IsNight() { return ((LDFHeaderCommon*)m_pFileHeader)->wDayOrNight == 1; }
		// 取得判别阈值
		virtual void* GetThreshold() { return NULL; }

	public:
		// 映射通道(通道号和数据索引对应)
		virtual void MapChannel();
		// 设置默认的显示通道
		virtual void SetDefaultDisplayChannel();
		// 根据通道代码取得通道数据索引
		virtual int GetDataIndex(int iChannelCode);
		// 通道名称转换为通道号(1-38)
		int ChannelName2ChannelCode(LPCTSTR lpszChannelName);
	};

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
