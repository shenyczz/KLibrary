#include "StdAfx.h"
#include "KFieldValue.h"

namespace XBase
{

	KFieldValue::KFieldValue(void)
		:KObject()
		,m_dValue(0)
		,m_pszValue(0)
		,m_Variant(0)
	{
	}
	KFieldValue::KFieldValue(const KFieldValue& fldv)
		:KObject(fldv)
		,m_dValue(fldv.m_dValue)
		,m_pszValue(0)
		,m_Variant(fldv.m_Variant)
	{
		KString::SetString(m_pszValue,fldv.m_pszValue);
	}

	KFieldValue::~KFieldValue(void)
	{
		_deletea(m_pszValue);
	}

	// 克隆
	KFieldValue* KFieldValue::Clone()
	{
		return new KFieldValue(*this);
	}

	// 设置字段值
	void KFieldValue::SetValue(_variant_t var)
	{
		switch(var.vt)
		{
		case VT_EMPTY:		// 
		case VT_NULL:
			break;

		case VT_I1:
			m_dValue = (char)var.cVal;
			break;
		case VT_I2:
			m_dValue = (short)var.iVal;
			break;
		case VT_I4:
			m_dValue = (long)var.lVal;
			break;
		case VT_INT:
			m_dValue = (INT)var.intVal;
			break;
		case VT_UI1:
			m_dValue = (BYTE)var.intVal;
			break;
		case VT_UI2:
			m_dValue = (WORD)var.intVal;
			break;
		case VT_UI4:
			m_dValue = (DWORD)var.intVal;
			break;
		case VT_UINT:
			m_dValue = (UINT)var.intVal;
			break;

		case VT_R4:
			m_dValue = (float)var.fltVal;
		case VT_R8:
			m_dValue = (double)var.dblVal;
			break;

		case VT_BSTR:
			this->SetValue(var.bstrVal);
			break;
		}

		return;
	}
	void KFieldValue::SetValue(int value)
	{
		m_dValue = value;
	}
	void KFieldValue::SetValue(float value)
	{
		m_dValue = value;
	}
	void KFieldValue::SetValue(double value)
	{
		m_dValue = value;
	}
	void KFieldValue::SetValue(LPCTSTR value)
	{
		KString::SetString(m_pszValue,value);
	}

	// 获取字段值
	_variant_t& KFieldValue::GetValue()
	{
		FieldType eFieldType = (FieldType)this->GetType();

		switch(eFieldType)
		{
		case FieldType_Character:
			m_Variant = _variant_t(m_pszValue);
			break;

		default:
			m_Variant = _variant_t(m_dValue);
			break;
		}

		return m_Variant;
	}
	int KFieldValue::GetIntergerValue()
	{
		return static_cast<int>(m_dValue+0.00001);
	}
	float KFieldValue::GetFloatValue()
	{
		return static_cast<float>(m_dValue);
	}
	double KFieldValue::GetDoubleValue()
	{
		return m_dValue;
	}
	LPCTSTR KFieldValue::GetStringValue()
	{
		return m_pszValue;
	}

}// namespace XBase - end
//---------------------------------------------------------
