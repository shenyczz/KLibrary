#include "StdAfx.h"
#include "KAdoParameter.h"
#include "KAdoException.h"

namespace Ado
{

	KAdoParameter::KAdoParameter(void)
	{
		m_strName = _T("");	// 参数名称 

		m_pParameter.CreateInstance(__uuidof(Parameter));
		m_pParameter->Type = ADODB::DataTypeEnum::adSmallInt; // adEmpty 会出错
		m_pParameter->Direction = ADODB::ParameterDirectionEnum::adParamInput;
		m_pParameter->Size = 0;
		m_pParameter->Precision = 0;
		m_pParameter->NumericScale = 0;

		//__declspec(property(get=GetValue,put=PutValue)) _variant_t Value;
	}
	KAdoParameter::KAdoParameter(CString strName, short value, int nParameterDirection)
	{
		m_pParameter.CreateInstance(__uuidof(Parameter));

		this->SetName(strName);
		this->SetType(ADODB::DataTypeEnum::adSmallInt);
		this->SetDirection(nParameterDirection);
		this->SetSize(sizeof(short));
		this->SetPrecision(0);
		this->SetScale(0);
		this->SetValue(value);
	}
	KAdoParameter::KAdoParameter(CString strName, long value, int nParameterDirection)
	{
		m_pParameter.CreateInstance(__uuidof(Parameter));

		this->SetName(strName);
		this->SetType(ADODB::DataTypeEnum::adInteger);
		this->SetDirection(nParameterDirection);
		this->SetSize(sizeof(long));
		this->SetPrecision(0);
		this->SetScale(0);
		this->SetValue(value);
	}
	KAdoParameter::KAdoParameter(CString strName, float value, int nParameterDirection)
	{
		m_pParameter.CreateInstance(__uuidof(Parameter));

		this->SetName(strName);
		this->SetType(ADODB::DataTypeEnum::adSingle);
		this->SetDirection(nParameterDirection);
		this->SetSize(sizeof(float));
		this->SetPrecision(0);
		this->SetScale(0);
		this->SetValue(value);
	}
	KAdoParameter::KAdoParameter(CString strName, double value, int nParameterDirection)
	{
		m_pParameter.CreateInstance(__uuidof(Parameter));

		this->SetName(strName);
		this->SetType(ADODB::DataTypeEnum::adDouble);
		this->SetDirection(nParameterDirection);
		this->SetSize(sizeof(double));
		this->SetPrecision(0);
		this->SetScale(0);
		this->SetValue(value);
	}
	KAdoParameter::KAdoParameter(CString strName, CString value, int nParameterDirection)
	{
		m_pParameter.CreateInstance(__uuidof(Parameter));

		this->SetName(strName);
		this->SetType(ADODB::DataTypeEnum::adBSTR);
		this->SetDirection(nParameterDirection);
		this->SetSize(value.GetLength());
		this->SetPrecision(0);
		this->SetScale(0);
		this->SetValue(value);
	}
	KAdoParameter::KAdoParameter(CString strName, COleDateTime value, int nParameterDirection)
	{
		m_pParameter.CreateInstance(__uuidof(Parameter));

		this->SetName(strName);
		this->SetType(ADODB::DataTypeEnum::adDate);
		this->SetDirection(nParameterDirection);
		this->SetSize(sizeof(DATE));
		this->SetPrecision(0);
		this->SetScale(0);
		this->SetValue(value);
	}

	KAdoParameter::~KAdoParameter(void)
	{
		m_pParameter.Release();
		m_pParameter = NULL;
	}

	BOOL KAdoParameter::SetValue(short iValue)
	{
		_variant_t vtVal;
		vtVal.vt = VT_I2;
		vtVal.iVal = iValue;

		try
		{
			m_pParameter->Value = vtVal;
			m_pParameter->Type = ADODB::DataTypeEnum::adSmallInt;
			m_pParameter->Size = sizeof(short);
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::SetValue(long lValue)
	{
		_variant_t vtVal;
		vtVal.vt = VT_I4;
		vtVal.lVal = lValue;

		try
		{
			m_pParameter->Size = sizeof(long);
			m_pParameter->Type = ADODB::DataTypeEnum::adInteger;
			m_pParameter->Value = vtVal;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::SetValue(float fValue)
	{
		_variant_t vtVal;
		vtVal.vt = VT_R4;
		vtVal.fltVal = fValue;

		try
		{
			m_pParameter->Size = sizeof(float);
			m_pParameter->Type = ADODB::DataTypeEnum::adSingle;
			m_pParameter->Value = vtVal;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::SetValue(double dValue)
	{
		_variant_t vtVal;
		vtVal.vt = VT_R8;
		vtVal.dblVal = dValue;

		try
		{
			m_pParameter->Size = sizeof(double);
			m_pParameter->Type = ADODB::DataTypeEnum::adDouble;
			m_pParameter->Value = vtVal;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::SetValue(CString strValue)
	{
		_variant_t vtVal;
		if(!strValue.IsEmpty())	vtVal.vt = VT_BSTR;
		else					vtVal.vt = VT_NULL;

		vtVal.bstrVal = strValue.AllocSysString();

		try
		{
			m_pParameter->Size = sizeof(char) * strValue.GetLength();
			m_pParameter->Type = ADODB::DataTypeEnum::adBSTR;
			m_pParameter->Value = vtVal;
			::SysFreeString(vtVal.bstrVal);
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			::SysFreeString(vtVal.bstrVal);
			return FALSE;
		}
	}
	BOOL KAdoParameter::SetValue(COleDateTime date_time)
	{
		_variant_t vtVal;
		vtVal.vt = VT_DATE;
		vtVal.date = date_time;

		try
		{
			m_pParameter->Size = sizeof(DATE);
			m_pParameter->Type = ADODB::DataTypeEnum::adDate;
			m_pParameter->Value = vtVal;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::SetValue(_variant_t vtValue)
	{
		_variant_t vtVal;
		vtVal.vt = VT_VARIANT;
		vtVal = vtValue;

		try
		{
			m_pParameter->Size = sizeof(VARIANT);
			m_pParameter->Value = vtValue;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoParameter::GetValue(short& iValue)
	{
		_variant_t vtVal;
		short nVal = 0;

		try
		{
			vtVal = m_pParameter->Value;
			switch(vtVal.vt)
			{
			case VT_BOOL:
				nVal = vtVal.boolVal;
				break;
			case VT_I1:
			case VT_UI1:
				nVal = vtVal.bVal;
				break;
			case VT_I2:
			case VT_UI2:
				nVal = vtVal.iVal;
				break;
			case VT_INT:
			case VT_UINT:
				nVal = vtVal.intVal;
				break;
			case VT_NULL:
			case VT_EMPTY:
				nVal = 0;
				break;
			default:
				nVal = vtVal.iVal;
			}	
			iValue = nVal;
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::GetValue(long& lValue)
	{
		_variant_t vtVal;
		long lVal = 0;

		try
		{
			vtVal = m_pParameter->Value;
			switch(vtVal.vt)
			{
			case VT_BOOL:
				lVal = vtVal.boolVal;
				break;
			case VT_I1:
			case VT_UI1:
				lVal = vtVal.bVal;
				break;
			case VT_I2:
			case VT_UI2:
				lVal = vtVal.iVal;
				break;
			case VT_INT:
			case VT_UINT:
				lVal = vtVal.intVal;
				break;
			case VT_I4:
			case VT_UI4:
				lVal = vtVal.lVal;
				break;
			case VT_NULL:
			case VT_EMPTY:
				lVal = 0;
				break;
			default:
				lVal = vtVal.lVal;
				break;
			}
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::GetValue(float& fValue)
	{
		_variant_t vtVal;
		double dblVal;
		try
		{
			vtVal = m_pParameter->Value;
			switch(vtVal.vt)
			{
			case VT_R4:
				dblVal = vtVal.fltVal;
				break;
			case VT_R8:
				dblVal = vtVal.dblVal;
				break;
			case VT_DECIMAL:
				dblVal = vtVal.decVal.Lo32;
				dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
				dblVal /= pow(10.0, (double)vtVal.decVal.scale); 
				break;
			case VT_UI1:
				dblVal = vtVal.iVal;
				break;
			case VT_I2:
			case VT_I4:
				dblVal = vtVal.lVal;
				break;
			case VT_INT:
				dblVal = vtVal.intVal;
				break;
			case VT_NULL:
			case VT_EMPTY:
				dblVal = 0;
				break;
			default:
				dblVal = 0;
			}
			fValue = static_cast<float>(dblVal);
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::GetValue(double& dValue)
	{
		_variant_t vtVal;
		double dblVal;
		try
		{
			vtVal = m_pParameter->Value;
			switch(vtVal.vt)
			{
			case VT_R4:
				dblVal = vtVal.fltVal;
				break;
			case VT_R8:
				dblVal = vtVal.dblVal;
				break;
			case VT_DECIMAL:
				dblVal = vtVal.decVal.Lo32;
				dblVal *= (vtVal.decVal.sign == 128)? -1 : 1;
				dblVal /= pow(10.0, vtVal.decVal.scale); 
				break;
			case VT_UI1:
				dblVal = vtVal.iVal;
				break;
			case VT_I2:
			case VT_I4:
				dblVal = vtVal.lVal;
				break;
			case VT_INT:
				dblVal = vtVal.intVal;
				break;
			case VT_NULL:
			case VT_EMPTY:
				dblVal = 0;
				break;
			default:
				dblVal = 0;
			}
			dValue = dblVal;
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::GetValue(CString& strValue, CString strDateFormat)
	{
		char buff[50];
		TCHAR tbuff[50];
		_variant_t vtVal;
		CString val;

		try
		{
			vtVal = m_pParameter->Value;
			switch(vtVal.vt) 
			{
			case VT_BSTR:
				val = vtVal.bstrVal;
				break;

			case VT_I2:
				_itot_s(vtVal.iVal,tbuff,50,10);
				val = buff;
				break;

			case VT_UI2:
				_itot_s(vtVal.uiVal,tbuff,50,10);
				val = buff;
				break;

			case VT_I4:
				_ltot_s(vtVal.lVal,tbuff,50,10);
				val = buff;
				break;

			case VT_UI4:
				_ltot_s(vtVal.ulVal,tbuff,50,10);
				val = buff;
				break;

			case VT_R4:
			   _gcvt_s(buff, 50, vtVal.fltVal, 10);
				val = buff;
				break;

			case VT_R8:
			   _gcvt_s(buff, 50, vtVal.dblVal, 10);
				val = buff;
				break;

			case VT_BOOL:
				val = vtVal.boolVal == VARIANT_TRUE? _T("T") : _T("F");
				break;

			case VT_DECIMAL:
				{
					double tmp = vtVal.decVal.Lo32;
					tmp *= (vtVal.decVal.sign == 128)? -1 : 1;
					tmp /= pow(10.0, vtVal.decVal.scale); 
				   _gcvt_s(buff, 50, tmp, 10);
					val = buff;
				}
				break;

			case VT_DATE:
				{
					COleDateTime dt(vtVal);
				
					if(strDateFormat.IsEmpty())
						strDateFormat = _T("%Y-%m-%d %H:%M:%S");

					val = dt.Format(strDateFormat);
				}
				break;

			case VT_CY:
				{
					vtVal.ChangeType(VT_R8);
 
					CString strTemp;
					strTemp.Format(_T("%f"), vtVal.dblVal);
 
					_TCHAR pszFormattedNumber[64];
 
					//	LOCALE_USER_DEFAULT
					if(GetCurrencyFormat(
							LOCALE_USER_DEFAULT,	// locale for which string is to be formatted 
							0,						// bit flag that controls the function's operation
							strTemp,				// pointer to input number string
							NULL,					// pointer to a formatting information structure
													//	NULL = machine default locale settings
							pszFormattedNumber,		// pointer to output buffer
							63))					// size of output buffer
					{
						strTemp = pszFormattedNumber;
					}

					val = strTemp;
				}
				break;

			case VT_EMPTY:
			case VT_NULL:
				val.Empty();
				break;

			default:
				val.Empty();
				return FALSE;
			}
			strValue = val;
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::GetValue(COleDateTime& datm)
	{
		_variant_t vtVal;

		try
		{
			vtVal = m_pParameter->Value;
			switch(vtVal.vt) 
			{
			case VT_DATE:
				{
					COleDateTime dt(vtVal);
					datm = dt;
				}
				break;
			case VT_EMPTY:
			case VT_NULL:
				datm.SetStatus(COleDateTime::null);
				break;
			default:
				return FALSE;
			}
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoParameter::GetValue(_variant_t& vtValue)
	{
		try
		{
			vtValue = m_pParameter->Value;
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	void KAdoParameter::dump_com_error(_com_error &e)
	{
		CString ErrorStr;

		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		ErrorStr.Format( _T("KAdoParameter Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
			e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
		m_strLastError = ErrorStr;
		m_dwLastError = e.Error();

		#ifdef _DEBUG
			AfxMessageBox(ErrorStr, MB_OK | MB_ICONERROR);
		#endif	

		throw KAdoException(e.Error(), e.Description());
	}

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
