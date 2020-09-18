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
 **   Author: ������.֣�� (shenyczz@163.com)
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
		// ��¡
		KRow* Clone();
		// ȡ�ü�¼����
		int GetLength() { return this->GetRecordLength(); }
		// ȡ�ü�¼ƫ�Ƶ�ַ
		LONG GetOffset();
		// ת��Ϊ�ַ���
		char* ToString();

	private:
		char* m_pszBuffer;
	};

}// namespace XBase - end
//---------------------------------------------------------
