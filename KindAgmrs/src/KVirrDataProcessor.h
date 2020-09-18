/******************************************************************************
 ** KVirrDataProcessor.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KVirrDataProcessor
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

#include "KLDFileProcessor.h"

namespace Agmrs { namespace Virr
{

	class KIND_EXT_CLASS KVirrDataProcessor : public KLDFileProcessor
	{
	public:
		KVirrDataProcessor(void);
		virtual ~KVirrDataProcessor(void);

	public:
		// 取得像元类型
		virtual int GetPixelType(int i,int j);
		// 产品生成 - 地表温度
		virtual BOOL Lst(int iMethod);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------

