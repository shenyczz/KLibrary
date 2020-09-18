/******************************************************************************
 ** KVirrData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KVirrData
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KLDFile.h"

namespace Agmrs { namespace Virr
{

	class KIND_EXT_CLASS KVirrData : public KLDFile
	{
	public:
		KVirrData(void);
		virtual ~KVirrData(void);

	public:
		// 取得判别阈值
		virtual void* GetThreshold(){ return GetVirrThreshold(); }

	public:
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
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------
