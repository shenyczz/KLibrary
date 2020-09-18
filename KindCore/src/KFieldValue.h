/******************************************************************************
 ** KFieldValue.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KFieldValue
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

#include "KObject.h"

namespace XBase
{

	class KIND_EXT_CLASS KFieldValue : public KObject
	{
	public:
		KFieldValue(void);
		KFieldValue(const KFieldValue& fldv);
		virtual ~KFieldValue(void);

	public:
		// ��¡
		KFieldValue* Clone();

	public:
		// �����ֶ�ֵ
		virtual void SetValue(_variant_t var);
		virtual void SetValue(int value);
		virtual void SetValue(float value);
		virtual void SetValue(double value);
		virtual void SetValue(LPCTSTR value);

		// ��ȡ�ֶ�ֵ
		virtual _variant_t& GetValue();
		virtual int GetIntergerValue();
		virtual float GetFloatValue();
		virtual double GetDoubleValue();
		virtual LPCTSTR GetStringValue();

	private:
		// �������ֶ�ֵ
		// float  �ֶο�� = 9  (����С����)
		// double �ֶο�� = 18 (����С����)
		double m_dValue;
		// �ַ����ֶ�ֵ
		LPTSTR m_pszValue;

		_variant_t m_Variant;
	};

//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
