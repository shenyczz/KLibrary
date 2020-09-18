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
		// 克隆
		KFieldValue* Clone();

	public:
		// 设置字段值
		virtual void SetValue(_variant_t var);
		virtual void SetValue(int value);
		virtual void SetValue(float value);
		virtual void SetValue(double value);
		virtual void SetValue(LPCTSTR value);

		// 获取字段值
		virtual _variant_t& GetValue();
		virtual int GetIntergerValue();
		virtual float GetFloatValue();
		virtual double GetDoubleValue();
		virtual LPCTSTR GetStringValue();

	private:
		// 浮点型字段值
		// float  字段宽度 = 9  (包括小数点)
		// double 字段宽度 = 18 (包括小数点)
		double m_dValue;
		// 字符型字段值
		LPTSTR m_pszValue;

		_variant_t m_Variant;
	};

//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
