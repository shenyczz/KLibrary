/******************************************************************************
 ** Header File: AdoParameter.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KAdoParameter
 ** Version:
 ** Function: ADO ������
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [������]
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

		// ���Է�����
		//__declspec(property(get=GetValue,put=PutValue)) _variant_t Value;

	public:
		// 1.��������
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
		// 2.������������(DataTypeEnum)
		int GetType() { return m_pParameter->Type; }
		void SetType(int nType) { m_pParameter->Type = (ADODB::DataTypeEnum)nType; }
		// 3.��������(ParameterDirectionEnum)
		int GetDirection() { return m_pParameter->Direction; }
		void SetDirection(int nDirection) { m_pParameter->Direction = (ADODB::ParameterDirectionEnum)nDirection; }
		// 4.�����ֽڳ���: long->sizeof(long) char[]-> char[]/sizeof(char)
		long GetSize() { return (long)m_pParameter->Size; }
		void SetSize(long lSize) { m_pParameter->Size = lSize; }
		// 5.�������ݾ���(�����������Ϊ adNumeric �� adDecimal������ָ���������ݾ��Ⱥ�С��λ��)
		int GetPrecision() { return m_pParameter->GetPrecision(); }
		void SetPrecision(int nPrecision) { m_pParameter->PutPrecision(nPrecision); }
		// 6.�ֶ�С�����ݵ�λ��(�����������Ϊ adNumeric �� adDecimal������ָ���������ݾ��Ⱥ�С��λ��)
		int GetScale() { return m_pParameter->GetNumericScale(); }
		void SetScale(int nScale) { m_pParameter->PutNumericScale(nScale); }
		// 7.������ֵ
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
		// ȡ�ò�������ָ��
		_ParameterPtr GetParameter() { return m_pParameter; }

	public:


	protected:
		void dump_com_error(_com_error &e);
	
	protected:
		// ��������ָ��
		_ParameterPtr m_pParameter;
	
	protected:
		CString m_strName;		// 1.�������� 

		CString m_strLastError;
		DWORD m_dwLastError;

	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
