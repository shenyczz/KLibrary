/******************************************************************************
 ** KFields.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KFields
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

#include "KField.h"
#include "TCollection.h"

namespace XBase
{

	class KIND_EXT_CLASS KFields : public KObject, public TPtrCollection<KField*>
	{
	public:
		KFields(void);
		KFields(const KFields& fields);
		virtual ~KFields(void);

	public:
		// 取得字段数量
		int GetFieldCount() { return Count(); }

	public:
		// 取得字段
		KField* GetField(int index) const;
		KField* GetField(LPCTSTR pszFieldName) const;

		// 取得记录长度
		int GetRecordLength();

		// 克隆
		KFields* Clone();
	};

}// namespace XBase - end
//---------------------------------------------------------
