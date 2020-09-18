/******************************************************************************
 ** Header File: AdoParameter.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KAdoParameter
 ** Version:
 ** Function: ADO 参数类
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	//1. 
		use

******************************************************************************/

#pragma once

namespace Ado
{

	class KIND_EXT_CLASS KAdoParameter
	{
	public:
		KAdoParameter(void);
		KAdoParameter(CString strName, short value, int nParameterDirection = ADODB::ParameterDirectionEnum::adParamInput);
		KAdoParameter(CString strName, long value, int nParameterDirection = ADODB::ParameterDirectionEnum::adParamInput);
		KAdoParameter(CString strName, float value, int nParameterDirection = ADODB::ParameterDirectionEnum::adParamInput);
		KAdoParameter(CString strName, double value, int nParameterDirection = ADODB::ParameterDirectionEnum::adParamInput);
		KAdoParameter(CString strName, CString value, int nParameterDirection = ADODB::ParameterDirectionEnum::adParamInput);
		KAdoParameter(CString strName, COleDateTime value, int nParameterDirection = ADODB::ParameterDirectionEnum::adParamInput);

		virtual ~KAdoParameter(void);

		// 属性访问器
		//__declspec(property(get=GetValue,put=PutValue)) _variant_t Value;

	public:
		// 1.参数名称
		CString GetName() { return m_strName; }
		void SetName(CString strName)
		{
			m_strName = strName;
			if(m_pParameter)
			{
				_bstr_t bstrTemp = strName.AllocSysString();
				m_pParameter->Name = bstrTemp;
				::SysFreeString(bstrTemp);
			}
		}
		// 2.参数数据类型(DataTypeEnum)
		int GetType() { return m_pParameter->Type; }
		void SetType(int nType) { m_pParameter->Type = (ADODB::DataTypeEnum)nType; }
		// 3.参数方向(ParameterDirectionEnum)
		int GetDirection() { return m_pParameter->Direction; }
		void SetDirection(int nDirection) { m_pParameter->Direction = (ADODB::ParameterDirectionEnum)nDirection; }
		// 4.参数字节长度: long->sizeof(long) char[]-> char[]/sizeof(char)
		long GetSize() { return (long)m_pParameter->Size; }
		void SetSize(long lSize) { m_pParameter->Size = lSize; }
		// 5.参数数据精度(如果数据类型为 adNumeric 或 adDecimal，必须指明参数数据精度和小数位数)
		int GetPrecision() { return m_pParameter->GetPrecision(); }
		void SetPrecision(int nPrecision) { m_pParameter->PutPrecision(nPrecision); }
		// 6.字段小数部份的位数(如果数据类型为 adNumeric 或 adDecimal，必须指明参数数据精度和小数位数)
		int GetScale() { return m_pParameter->GetNumericScale(); }
		void SetScale(int nScale) { m_pParameter->PutNumericScale(nScale); }
		// 7.参数的值
		BOOL GetValue(short& iValue);
		BOOL GetValue(long& lValue);
		BOOL GetValue(float& fValue);
		BOOL GetValue(double& dValue);
		BOOL GetValue(CString& strValue, CString strDateFormat = _T(""));
		BOOL GetValue(COleDateTime& time);
		BOOL GetValue(_variant_t& vtValue);

		BOOL SetValue(short iValue);
		BOOL SetValue(long lValue);
		BOOL SetValue(float fValue);
		BOOL SetValue(double dValue);
		BOOL SetValue(CString strValue);
		BOOL SetValue(COleDateTime date_time);
		BOOL SetValue(_variant_t vtValue);

	public:
		// 取得参数智能指针
		_ParameterPtr GetParameter() { return m_pParameter; }

	public:


	protected:
		void dump_com_error(_com_error &e);
	
	protected:
		// 参数智能指针
		_ParameterPtr m_pParameter;
	
	protected:
		CString m_strName;		// 1.参数名称 

		CString m_strLastError;
		DWORD m_dwLastError;

	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
