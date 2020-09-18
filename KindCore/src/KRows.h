/******************************************************************************
 ** KRows.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KRows
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

#include "KRow.h"

namespace XBase
{
	class KIND_EXT_CLASS KRows : public KObject, public TPtrCollection<KRow*>
	{
	public:
		KRows(void);
		KRows(const KRows& rows);
		KRows(LPCTSTR lpszName);
		virtual ~KRows(void);

	public:
		int GetRowCount()			{ return this->Count(); }
		KRow* GetRow(int index)		{ return this->GetAt(index); }
		KRow* GetRowById(int id)	{ return this->GetAt(id-1); }

	public:
		// 克隆
		KRows* Clone();
		// 取得字段集合
		KFields* GetFields();

		// 根据字段名称和字段值取得记录序号
		int RecordOf(LPCTSTR lpszFieldName,double dValue);
		int RecordOf(LPCTSTR lpszFieldName,LPCTSTR lpszValue);
	};

}// namespace XBase - end
//---------------------------------------------------------
