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
		// ȡ���ֶ�����
		int GetFieldCount() { return Count(); }

	public:
		// ȡ���ֶ�
		KField* GetField(int index) const;
		KField* GetField(LPCTSTR pszFieldName) const;

		// ȡ�ü�¼����
		int GetRecordLength();

		// ��¡
		KFields* Clone();
	};

}// namespace XBase - end
//---------------------------------------------------------
