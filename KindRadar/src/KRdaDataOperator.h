/******************************************************************************
 ** KRdaDataOperator.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRdaDataOperator - RAD 数据操作类
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

#include "KindRadar.inl"
#include "KWsr98DataOperator.h"

namespace Wsr98d { namespace Rda
{

	class KIND_EXT_CLASS KRdaDataOperator : public KWsr98DataOperator
	{
	public:
		KRdaDataOperator(void);
		virtual ~KRdaDataOperator(void);
	};

//--------------------------------------------------------
}}// namespace Wsr98d::Rda - end
//--------------------------------------------------------
