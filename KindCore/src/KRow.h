/******************************************************************************
 ** KRow.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KRow
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

#include "KFields.h"

namespace XBase
{
	class KIND_EXT_CLASS KRow : public KFields
	{
	public:
		KRow(void);
		KRow(const KRow& row);
		KRow(int id);
		virtual ~KRow(void);

	public:
		// 克隆
		KRow* Clone();
		// 取得记录长度
		int GetLength() { return this->GetRecordLength(); }
		// 取得记录偏移地址
		LONG GetOffset();
		// 转换为字符串
		char* ToString();

	private:
		char* m_pszBuffer;
	};

}// namespace XBase - end
//---------------------------------------------------------
