#pragma once

#include "KLDFile.h"

namespace Agmrs { namespace Mersi
{

	class KIND_EXT_CLASS KMersiData : public KLDFile
	{
	public:
		KMersiData(void);
		virtual ~KMersiData(void);

	public:
		// 取得判别阈值
		virtual void* GetThreshold(){ return GetMersiThreshold(); }

	public:
		// 设置默认的显示通道
		virtual void SetDefaultDisplayChannel();
		// 通道号转换为通道代码
		virtual int Channel2ChannelCode(int iChannel);
		// 通道代码转换为通道号
		virtual int ChannelCode2Channel(int iChannelCode);
		// 通道代码转换为通道名称
		virtual LPCTSTR ChannelCode2String(int iChannelCode);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------
