#pragma once

#include "KLDFile.h"

namespace Agmrs { namespace Avhrr
{

	class KIND_EXT_CLASS KAvhrrData : public KLDFile
	{
	public:
		KAvhrrData(void);
		virtual ~KAvhrrData(void);

	public:
		// 取得判别阈值
		virtual void* GetThreshold(){ return GetAvhrrThreshold(); }

	protected:
		// 装载数据
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);
		private:
		BOOL LoadData_bin_ld2(LPCTSTR lpszFile);
		BOOL LoadData_bin_ld3(LPCTSTR lpszFile);

	public:
		// 映射通道(通道号和数据索引对应)
		//virtual void MapChannel();
		// 根据通道代码区的通道数据索引
		//virtual int GetDataIndex(int iChannelCode);

		// 设置默认的显示通道
		virtual void SetDefaultDisplayChannel();
		// 通道号转换为通道代码
		virtual int Channel2ChannelCode(int iChannel);
		// 通道代码转换为通道号
		virtual int ChannelCode2Channel(int iChannelCode);
		// 通道转换为字符串
		virtual LPCTSTR ChannelCode2String(int iChannelCode);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Avhrr - end
//---------------------------------------------------------
