#include "StdAfx.h"
#include "KAdoRecordset.h"

#include "KAdoDatabase.h"
#include "KAdoCommand.h"
#include "KAdoException.h"

namespace Ado
{

	KAdoRecordset::KAdoRecordset(KAdoDatabase* pAdoDatabase)
	{
		m_pConnection = pAdoDatabase->GetConnection();
		m_pCommand.CreateInstance(__uuidof(Command));
		m_pRecordset.CreateInstance(__uuidof(Recordset));
		m_pRecBinding = NULL;

		m_strQuery = _T("");
		m_nEditStatus = eAdoDbEditNone;
		m_nSearchDirection = adSearchForward;

		m_strLastError = _T("");
		m_dwLastError = 0;
	}

	KAdoRecordset::~KAdoRecordset(void)
	{
		this->Close();

		if(m_pCommand)
		{
			m_pCommand.Release();
			m_pCommand = NULL;
		}

		if(m_pRecordset)
		{
			m_pRecordset.Release();
			m_pRecordset = NULL;
		}

		if(m_pRecBinding)
		{
			m_pRecBinding->Release();
			m_pRecBinding = NULL;
		}

		m_strQuery = _T("");
		m_nEditStatus = eAdoDbEditNone;

		m_strLastError = _T("");
		m_dwLastError = 0;
	}


	BOOL KAdoRecordset::Open(LPCTSTR lpstrExec, int nOption)
	{
		if(m_pConnection == NULL)
			return FALSE;

		if(m_pConnection->GetState() == adStateClosed)
			return FALSE;
	
		m_strQuery = lpstrExec;
		if(m_strQuery.IsEmpty())
			return FALSE;

		this->Close();

		m_strQuery.TrimLeft();
		// 是 SELECT 查询语句
		BOOL bIsSelect = m_strQuery.Mid(0, _tcslen(_T("Select "))).CompareNoCase(_T("select ")) == 0 && nOption == eAdoOpenUnknown;

		try
		{
			m_pRecordset->CursorType = adOpenStatic;		// 游标类型
			m_pRecordset->CursorLocation = adUseClient;		// 游标位置

			// 查询
			if(bIsSelect || nOption == eAdoOpenUnknown || nOption == eAdoOpenUnknown)
			{
				m_pRecordset->Open((LPCSTR)_bstr_t(m_strQuery), _variant_t((IDispatch*)m_pConnection, TRUE), adOpenStatic, adLockOptimistic, adCmdUnknown);
			}
			// 表
			else if(nOption == eAdoOpenTable)
			{
				m_pRecordset->Open((LPCSTR)_bstr_t(m_strQuery), _variant_t((IDispatch*)m_pConnection, TRUE), adOpenKeyset, adLockOptimistic, adCmdTable);
			}
			// 存储过程
			else if(nOption == eAdoOpenStoredProc)
			{
				m_pCommand->ActiveConnection = m_pConnection;
				m_pCommand->CommandType = adCmdStoredProc;
				m_pCommand->CommandText = _bstr_t(m_strQuery);
				m_pConnection->CursorLocation = adUseClient;
			
				//  m_pCommand->Execute( VARIANT * RecordsAffected, VARIANT * Parameters, long Options ) 
				//m_pRecordset = m_pCommand->Execute(NULL, NULL, adCmdText);
				m_pRecordset = m_pCommand->Execute(NULL, NULL, adCmdStoredProc);	// [4/3/2006 ShenYongChen]
			}
			else
			{
				TRACE( "Unknown parameter. %d", nOption);
				return FALSE;
			}
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}

		return m_pRecordset != NULL && m_pRecordset->GetState()!= adStateClosed;
	}

	BOOL KAdoRecordset::OpenSchema(int nSchema, LPCTSTR SchemaID)
	{
		try
		{
			_variant_t vtSchemaID = vtMissing;

			if(_tcslen(SchemaID) != 0)
			{
				vtSchemaID = SchemaID;
				nSchema = adSchemaProviderSpecific;
			}
			
			m_pRecordset = m_pConnection->OpenSchema((enum SchemaEnum)nSchema, vtMissing, vtSchemaID);
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoRecordset::Requery()
	{
		if(IsOpen())
		{
			try
			{
				m_pRecordset->Requery(adExecuteRecord);	// ExecuteOptionEnum
			}
			catch(_com_error &e)
			{
				dump_com_error(e);
				return FALSE;
			}
		}
		return TRUE;
	}

	long KAdoRecordset::GetRecordCount()
	{
		long nRows = 0;
	
		nRows = (long)m_pRecordset->GetRecordCount();

		if(nRows == -1)
		{
			nRows = 0;
			if(m_pRecordset->EndOfFile != VARIANT_TRUE)
				m_pRecordset->MoveFirst();
		
			while(m_pRecordset->EndOfFile != VARIANT_TRUE)
			{
				nRows++;
				m_pRecordset->MoveNext();
			}
			if(nRows > 0)
				m_pRecordset->MoveFirst();
		}
	
		return nRows;
	}

	void KAdoRecordset::Close()
	{
		if(IsOpen())
		{
			if (m_nEditStatus != dbEditNone)
			{
				  CancelUpdate();
			}

			m_pRecordset->PutSort(_T(""));
			m_pRecordset->Close();	
		}
	}

	BOOL KAdoRecordset::IsOpen()
	{
		if(m_pRecordset != NULL && IsConnectionOpen())
		{
			return m_pRecordset->GetState() != adStateClosed;
		}

		return FALSE;
	}

	BOOL KAdoRecordset::Update()
	{
		BOOL bret = TRUE;

		if(m_nEditStatus != dbEditNone)
		{
			try
			{
				if(m_pRecordset->Update() != S_OK)
				{
					bret = FALSE;
				}
			}
			catch(_com_error &e)
			{
				dump_com_error(e);
				bret = FALSE;
			}

			if(!bret)
			{
				m_pRecordset->CancelUpdate();
			}
			m_nEditStatus = eAdoDbEditNone;
		}

		return bret;
	}

	void KAdoRecordset::CancelUpdate()
	{
		m_pRecordset->CancelUpdate();
		m_nEditStatus = eAdoDbEditNone;
	}

	void KAdoRecordset::dump_com_error(_com_error &e)
	{
		CString ErrorStr;
	
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		ErrorStr.Format( _T("KAdoRecordset Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
			e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription );
		m_strLastError = _T("Query = " + GetQuery() + '\n' + ErrorStr);
		m_dwLastError = e.Error();

		#ifdef _DEBUG
			AfxMessageBox( ErrorStr, MB_OK | MB_ICONERROR );
		#endif	

		throw KAdoException(e.Error(), e.Description());
	}

	///===============================================
	/// 字段操作 - beg
	//判断 NULL 字段
	BOOL KAdoRecordset::IsFieldNull(int nIndex)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			return vtFld.vt == VT_NULL;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::IsFieldNull(LPCTSTR lpFieldName)
	{
		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			return vtFld.vt == VT_NULL;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//判断 EMPTY 字段
	BOOL KAdoRecordset::IsFieldEmpty(int nIndex)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
	
		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			return (vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL);
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::IsFieldEmpty(LPCTSTR lpFieldName)
	{
		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			return (vtFld.vt == VT_EMPTY || vtFld.vt == VT_NULL);
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}


	//设置字段值为空
	BOOL KAdoRecordset::SetFieldEmpty(int nIndex)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
		vtFld.vt = VT_EMPTY;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldEmpty(LPCTSTR lpFieldName)
	{
		_variant_t vtFld;
		vtFld.vt = VT_EMPTY;
	
		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置通用型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, _variant_t vtValue)
	{
		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
	
		return PutFieldValue(vtIndex, vtValue);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue)
	{	
		return PutFieldValue(lpFieldName, vtValue);
	}

	//设置字符串型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, CString strValue)
	{
		BOOL bValue(FALSE);

		//字段索引
		_variant_t vtIndex;

		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		//字段
		_variant_t vtFld;

		if(!strValue.IsEmpty())
			vtFld.vt = VT_BSTR;
		else
			vtFld.vt = VT_NULL;

		vtFld.bstrVal = strValue.AllocSysString();

		bValue = PutFieldValue(vtIndex, vtFld);

		//注意内存回收
		SysFreeString(vtFld.bstrVal);

		return TRUE;
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, CString strValue)
	{
		BOOL bValue(FALSE);

		_variant_t vtFld;

		if(!strValue.IsEmpty())
			vtFld.vt = VT_BSTR;
		else
			vtFld.vt = VT_NULL;

		vtFld.bstrVal = strValue.AllocSysString();

		bValue =  PutFieldValue(lpFieldName, vtFld);

		//注意内存回收
		SysFreeString(vtFld.bstrVal);

		return bValue;
	}

	//设置短整型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, short iValue)
	{
		_variant_t vtIndex;

		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
	
		vtFld.vt = VT_I2;
		vtFld.iVal = iValue;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, short iValue)
	{
		_variant_t vtFld;
	
		vtFld.vt = VT_I2;
		vtFld.iVal = iValue;
	
	
		return PutFieldValue(lpFieldName, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(int nIndex, unsigned short uiValue)
	{
		_variant_t vtIndex;

		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
	
		vtFld.vt = VT_UI2;
		vtFld.uiVal = uiValue;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, unsigned short uiValue)
	{
		_variant_t vtFld;
	
		vtFld.vt = VT_UI2;
		vtFld.uiVal = uiValue;
	
	
		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置长整型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, long lValue)
	{
		_variant_t vtIndex;

		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
	
		vtFld.vt = VT_I4;
		vtFld.lVal = lValue;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, long lValue)
	{
		_variant_t vtFld;
	
		vtFld.vt = VT_I4;
		vtFld.lVal = lValue;
	
		return PutFieldValue(lpFieldName, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(int nIndex, unsigned long ulValue)
	{
		_variant_t vtIndex;

		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
	
		vtFld.vt = VT_UI4;
		vtFld.ulVal = ulValue;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, unsigned long ulValue)
	{
		_variant_t vtFld;
	
		vtFld.vt = VT_UI4;
		vtFld.ulVal = ulValue;
	
		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置单精度浮点型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, float fValue)
	{
		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
		vtFld.vt = VT_R4;
		vtFld.fltVal = fValue;

		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, float fValue)
	{
		_variant_t vtFld;
		vtFld.vt = VT_R4;
		vtFld.fltVal = fValue;

		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置双精度浮点型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, double dValue)
	{
		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		_variant_t vtFld;
		vtFld.vt = VT_R8;
		vtFld.dblVal = dValue;

		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, double dValue)
	{
		_variant_t vtFld;
		vtFld.vt = VT_R8;
		vtFld.dblVal = dValue;

		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置逻辑型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, BOOL bValue)
	{
		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
	
		_variant_t vtFld;
		vtFld.vt = VT_BOOL;
		vtFld.boolVal = bValue;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, BOOL bValue)
	{
		_variant_t vtFld;
		vtFld.vt = VT_BOOL;
		vtFld.boolVal = bValue;
	
		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置日期时间型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, COleDateTime dtValue)
	{
		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
	
		_variant_t vtFld;
		vtFld.vt = VT_DATE;
		vtFld.date = dtValue;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, COleDateTime dtValue)
	{
		_variant_t vtFld;
		vtFld.vt = VT_DATE;
		vtFld.date = dtValue;
	
		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置货币型字段值
	BOOL KAdoRecordset::SetFieldValue(int nIndex, COleCurrency cyValue)
	{
		if(cyValue.m_status == COleCurrency::invalid)
			return FALSE;

		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
	
		_variant_t vtFld;
		
		vtFld.vt = VT_CY;
		vtFld.cyVal = cyValue.m_cur;
	
		return PutFieldValue(vtIndex, vtFld);
	}
	BOOL KAdoRecordset::SetFieldValue(LPCTSTR lpFieldName, COleCurrency cyValue)
	{
		if(cyValue.m_status == COleCurrency::invalid)
			return FALSE;

		_variant_t vtFld;

		vtFld.vt = VT_CY;
		vtFld.cyVal = cyValue.m_cur;	
		
		return PutFieldValue(lpFieldName, vtFld);
	}

	//设置通用型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, _variant_t& vtValue)
	{
		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
	
		try
		{
			vtValue = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue)
	{
		try
		{
			vtValue = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//获取字符串型字段值
	//  CString strDateFormat - A formatting time string similar to the strftime formatting string. The more common are: 
	//	%a - Abbreviated weekday name 
	//	%A - Full weekday name 
	//	%b - Abbreviated month name 
	//	%B - Full month name 
	//	%c - Date and time representation appropriate for locale 
	//	%d - Day of month as decimal number (01 - 31) 
	//	%H - Hour in 24-hour format (00 - 23) 
	//	%I - Hour in 12-hour format (01 - 12) 
	//	%j - Day of year as decimal number (001 - 366) 
	//	%m - Month as decimal number (01 - 12) 
	//	%M - Minute as decimal number (00 - 59) 
	//	%p - Current locale’s A.M./P.M. indicator for 12-hour clock 
	//	%S - Second as decimal number (00 - 59) 
	//	%U - Week of year as decimal number, with Sunday as first day of week (00 - 53) 
	//	%w - Weekday as decimal number (0 - 6; Sunday is 0) 
	//	%W - Week of year as decimal number, with Monday as first day of week (00 - 53) 
	//	%x - Date representation for current locale 
	//	%X - Time representation for current locale 
	//	%y - Year without century, as decimal number (00 - 99) 
	//	%Y - Year with century, as decimal number 
	BOOL KAdoRecordset::GetFieldValue(int nIndex, CString& strValue,CString strDateFormat)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		char buff[50] = "";
		TCHAR tbuff[50] = _T("");
		CString val(_T(""));
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			switch(vtFld.vt) 
			{
			case VT_BSTR:
				val = vtFld.bstrVal;
				break;

			case VT_I2:
				_itot_s(vtFld.iVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_UI2:
				_itot_s(vtFld.uiVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_I4:
				_ltot_s(vtFld.lVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_UI4:
				_ltot_s(vtFld.ulVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_R4:
			   _gcvt_s(buff, 50, vtFld.fltVal, 10);
				val = buff;
				break;

			case VT_R8:
			   _gcvt_s(buff, 50, vtFld.dblVal, 10);
				val = buff;
				break;

			case VT_BOOL:
				val = vtFld.boolVal == VARIANT_TRUE? _T("T") : _T("F");
				break;

			case VT_DECIMAL:
				{
					double tmp = vtFld.decVal.Lo32;
					tmp *= (vtFld.decVal.sign == 128)? -1 : 1;
					tmp /= pow(10.0, vtFld.decVal.scale); 
				   _gcvt_s(buff, 50, tmp, 10);
					val = buff;
				}
				break;

			case VT_DATE:
				{
					COleDateTime dt(vtFld);
				
					if(strDateFormat.IsEmpty())
						strDateFormat = _T("%Y-%m-%d %H:%M:%S");

					val = dt.Format(strDateFormat);
				}
				break;

			case VT_CY:
				{
					vtFld.ChangeType(VT_R8);
 
					CString strTemp;
					strTemp.Format(_T("%f"), vtFld.dblVal);
 
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
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, CString& strValue,CString strDateFormat)
	{
		char buff[50] = "";
		TCHAR tbuff[50] = _T("");
		CString val(_T(""));
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			switch(vtFld.vt) 
			{
			case VT_BSTR:
				val = vtFld.bstrVal;
				break;

			case VT_I2:
				_itot_s(vtFld.iVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_UI2:
				_itot_s(vtFld.uiVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_I4:
				_ltot_s(vtFld.lVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_UI4:
				_ltot_s(vtFld.ulVal,tbuff,50,10);
				val = tbuff;
				break;

			case VT_R4:
			   _gcvt_s(buff, 50, vtFld.fltVal, 10);
				val = buff;
				break;

			case VT_R8:
			   _gcvt_s(buff, 50, vtFld.dblVal, 10);
				val = buff;
				break;

			case VT_BOOL:
				val = vtFld.boolVal == VARIANT_TRUE? _T("T") : _T("F");
				break;

			case VT_DECIMAL:
				{
					double tmp = vtFld.decVal.Lo32;
					tmp *= (vtFld.decVal.sign == 128)? -1 : 1;
					tmp /= pow(10.0, vtFld.decVal.scale); 
				   _gcvt_s(buff, 50, tmp, 10);
					val = buff;
				}
				break;

			case VT_DATE:
				{
					COleDateTime dt(vtFld);
				
					if(strDateFormat.IsEmpty())
						strDateFormat = _T("%Y-%m-%d %H:%M:%S");

					val = dt.Format(strDateFormat);
				}
				break;

			case VT_CY:
				{
					vtFld.ChangeType(VT_R8);
 
					CString strTemp;
					strTemp.Format(_T("%f"), vtFld.dblVal);
 
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
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//获取短整型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, short& iValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		int val = NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			switch(vtFld.vt)
			{
			case VT_BOOL:
				val = vtFld.boolVal;
				break;

			case VT_I2:
			case VT_UI1:
				val = vtFld.iVal;
				break;

			case VT_INT:
				val = vtFld.intVal;
				break;

			case VT_NULL:
			case VT_EMPTY:
				val = 0;
				break;

			default:
				val = vtFld.iVal;
			}

			iValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, short& iValue)
	{
		int val = NULL;

		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			switch(vtFld.vt)
			{
			case VT_BOOL:
				val = vtFld.boolVal;
				break;

			case VT_I2:
			case VT_UI1:
				val = vtFld.iVal;
				break;

			case VT_INT:
				val = vtFld.intVal;
				break;

			case VT_NULL:
			case VT_EMPTY:
				val = 0;
				break;

			default:
				val = vtFld.iVal;
			}

			iValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(int nIndex, unsigned short& uiValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		int val = NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			switch(vtFld.vt)
			{
			case VT_BOOL:
				val = vtFld.boolVal;
				break;

			case VT_UI2:
				val = vtFld.uiVal;
				break;

			case VT_INT:
				val = vtFld.intVal;
				break;

			case VT_NULL:
			case VT_EMPTY:
				val = 0;
				break;

			default:
				val = vtFld.iVal;
			}

			uiValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, unsigned short& uiValue)
	{
		int val = NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			switch(vtFld.vt)
			{
			case VT_BOOL:
				val = vtFld.boolVal;
				break;

			case VT_UI2:
				val = vtFld.uiVal;
				break;

			case VT_INT:
				val = vtFld.intVal;
				break;

			case VT_NULL:
			case VT_EMPTY:
				val = 0;
				break;

			default:
				val = vtFld.iVal;
			}

			uiValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//获取长整型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, long& lValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		long val = NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			{
				val = vtFld.lVal;
			}

			lValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, long& lValue)
	{
		long val = NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			{
				val = vtFld.lVal;
			}

			lValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(int nIndex, unsigned long& ulValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		long val = NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			{
				val = vtFld.ulVal;
			}

			ulValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue)
	{
		long val = NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			if(vtFld.vt != VT_NULL && vtFld.vt != VT_EMPTY)
			{
				val = vtFld.ulVal;
			}

			ulValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//获取单精度浮点型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, float& fValue)
	{
		double dValue(0);

		if(!GetFieldValue(nIndex,dValue))
		{
			return FALSE;
		}

		fValue = static_cast<float>(dValue);

		return TRUE;
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, float& fValue)
	{
		double dValue(0);

		if(!GetFieldValue(lpFieldName,dValue))
		{
			return FALSE;
		}

		fValue = static_cast<float>(dValue);

		return TRUE;
	}

	//获取双精度浮点型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, double& dValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		double val = (double)NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			switch(vtFld.vt)
			{
			case VT_R4:
				val = vtFld.fltVal;
				break;

			case VT_R8:
				val = vtFld.dblVal;
				break;

			case VT_DECIMAL:
				val = vtFld.decVal.Lo32;
				val *= (vtFld.decVal.sign == 128)? -1 : 1;
				val /= pow(10.0, vtFld.decVal.scale); 
				break;

			case VT_I1:
				val = vtFld.cVal;
				break;

			case VT_UI1:
				val = vtFld.bVal;
				break;

			case VT_I2:
				val = vtFld.iVal;
				break;

			case VT_I4:
				val = vtFld.lVal;
				break;

			case VT_UI2:
				val = vtFld.uiVal;
				break;

			case VT_UI4:
				val = vtFld.ulVal;
				break;

			case VT_INT:
				val = vtFld.intVal;
				break;

			case VT_CY:   //Added by John Andy Johnson!!!!
				vtFld.ChangeType(VT_R8);
				val = vtFld.dblVal;
				break;

			case VT_NULL:
			case VT_EMPTY:
				val = 0;
				break;

			default:
				val = 0;
				break;
			}

			dValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, double& dValue)
	{
		double val = (double)NULL;
		_variant_t vtFld;

		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			switch(vtFld.vt)
			{
			case VT_R4:
				val = vtFld.fltVal;
				break;

			case VT_R8:
				val = vtFld.dblVal;
				break;

			case VT_DECIMAL:
				val = vtFld.decVal.Lo32;
				val *= (vtFld.decVal.sign == 128)? -1 : 1;
				val /= pow(10.0, vtFld.decVal.scale); 
				break;

			case VT_I1:
				val = vtFld.cVal;
				break;

			case VT_UI1:
				val = vtFld.bVal;
				break;

			case VT_I2:
				val = vtFld.iVal;
				break;

			case VT_I4:
				val = vtFld.lVal;
				break;

			case VT_UI2:
				val = vtFld.uiVal;
				break;

			case VT_UI4:
				val = vtFld.ulVal;
				break;

			case VT_INT:
				val = vtFld.intVal;
				break;

			case VT_CY:   //Added by John Andy Johnson!!!!
				vtFld.ChangeType(VT_R8);
				val = vtFld.dblVal;
				break;

			case VT_NULL:
			case VT_EMPTY:
				val = 0;
				break;

			default:
				val = 0;
				break;
			}

			dValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//获取逻辑型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, BOOL& bValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		BOOL val(FALSE);
		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			switch(vtFld.vt) 
			{
			case VT_BOOL:
				val = vtFld.boolVal == VARIANT_TRUE? TRUE: FALSE;
				break;
			case VT_EMPTY:
			case VT_NULL:
				val = FALSE;
				break;
			default:
				return FALSE;
			}

			bValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, BOOL& bValue)
	{
		BOOL val(FALSE);
		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			switch(vtFld.vt) 
			{
			case VT_BOOL:
				val = vtFld.boolVal == VARIANT_TRUE? TRUE: FALSE;
				break;
			case VT_EMPTY:
			case VT_NULL:
				val = FALSE;
				break;
			default:
				return FALSE;
			}

			bValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//获取日期时间型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, COleDateTime& dtValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		COleDateTime val;
		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			switch(vtFld.vt) 
			{
			case VT_DATE:
				{
					COleDateTime dt(vtFld);
					val = dt;
				}
				break;
			case VT_EMPTY:
			case VT_NULL:
				val.SetStatus(COleDateTime::null);
				break;
			default:
				return FALSE;
			}

			dtValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, COleDateTime& dtValue)
	{
		COleDateTime val;
		_variant_t vtFld;
	
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			switch(vtFld.vt) 
			{
			case VT_DATE:
				{
					COleDateTime dt(vtFld);
					val = dt;
				}
				break;
			case VT_EMPTY:
			case VT_NULL:
				val.SetStatus(COleDateTime::null);
				break;
			default:
				return FALSE;
			}

			dtValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	//获取货币型字段值
	BOOL KAdoRecordset::GetFieldValue(int nIndex, COleCurrency& cyValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;
	
		COleCurrency val;
		_variant_t vtFld;
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(vtIndex)->Value;
			switch(vtFld.vt) 
			{
			case VT_CY:
				val = (CURRENCY)vtFld.cyVal;
				break;
			case VT_EMPTY:
			case VT_NULL:
				{
				val = COleCurrency();
				val.m_status = COleCurrency::null;
				}
				break;
			default:
				return FALSE;
			}

			cyValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::GetFieldValue(LPCTSTR lpFieldName, COleCurrency& cyValue)
	{
		COleCurrency val;
		_variant_t vtFld;
		try
		{
			vtFld = m_pRecordset->Fields->GetItem(lpFieldName)->Value;
			switch(vtFld.vt) 
			{
			case VT_CY:
				val = (CURRENCY)vtFld.cyVal;
				break;
			case VT_EMPTY:
			case VT_NULL:
				{
				val = COleCurrency();
				val.m_status = COleCurrency::null;
				}
				break;
			default:
				return FALSE;
			}

			cyValue = val;

			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	// private
	// 字段赋值 - PutFieldValue
	BOOL KAdoRecordset::PutFieldValue(_variant_t vtIndex, _variant_t vtFld)
	{
		if(m_nEditStatus == dbEditNone)
		{
			return FALSE;
		}

		try
		{
			m_pRecordset->Fields->GetItem(vtIndex)->Value = vtFld;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}
	BOOL KAdoRecordset::PutFieldValue(LPCTSTR lpFieldName, _variant_t vtFld)
	{
		if(m_nEditStatus == eAdoDbEditNone)
		{//unable edit
			return FALSE;
		}
	
		try
		{
			m_pRecordset->Fields->GetItem(lpFieldName)->Value = vtFld; 
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;	
		}
	}
	/// 字段操作 - end
	///===============================================

	BOOL KAdoRecordset::Execute(KAdoCommand* pAdoCommand)
	{
		if(IsOpen())
			Close();

		m_pConnection = pAdoCommand->GetCommand()->ActiveConnection;

		try
		{
			m_pConnection->CursorLocation = adUseClient;
			m_pRecordset = pAdoCommand->GetCommand()->Execute(NULL, NULL, pAdoCommand->GetCommandType());
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoRecordset::SaveAsXML(LPCTSTR lpstrXMLFile)
	{
		HRESULT hr;

		ASSERT(IsOpen());
	
		try
		{
			hr = m_pRecordset->Save(lpstrXMLFile, adPersistXML);
			return hr == S_OK;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
		return TRUE;
	}

	BOOL KAdoRecordset::OpenXML(LPCTSTR lpstrXMLFile)
	{
		HRESULT hr = S_OK;

		if(IsOpen())
			Close();

		try
		{
			hr = m_pRecordset->Open(lpstrXMLFile, _T("Provider=MSPersist;"), adOpenForwardOnly, adLockOptimistic, adCmdFile);
			return hr == S_OK;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoRecordset::SetFilter(LPCTSTR strFilter)
	{
		ASSERT(IsOpen());
	
		try
		{
			m_pRecordset->PutFilter(strFilter);
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoRecordset::SetSort(LPCTSTR strCriteria)
	{
		ASSERT(IsOpen());
	
		try
		{
			m_pRecordset->PutSort(strCriteria);
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoRecordset::Clone(KAdoRecordset &pRs)
	{
		try
		{
			pRs.m_pRecordset = m_pRecordset->Clone(adLockUnspecified);
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoRecordset::Find(LPCTSTR lpFind, int nSearchDirection)
	{
		BOOL bValue(FALSE);

		m_strFind = lpFind;
		m_nSearchDirection = nSearchDirection;

		ASSERT(!m_strFind.IsEmpty());

		m_pRecordset->Find(_bstr_t(m_strFind), 0, (SearchDirectionEnum)nSearchDirection, "");

		if(nSearchDirection == adSearchForward)
		{
			//m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchForward, "");
			if(!IsEof())
			{
				m_varBookFind = m_pRecordset->Bookmark;
				bValue = TRUE;
			}
		}
		else if(nSearchDirection == adSearchBackward)
		{
			//m_pRecordset->Find(_bstr_t(m_strFind), 0, adSearchBackward, "");
			if(!IsBof())
			{
				m_varBookFind = m_pRecordset->Bookmark;
				bValue = TRUE;
			}
		}
		else
		{
			TRACE("Unknown parameter. %d", nSearchDirection);
			m_nSearchDirection = adSearchForward;

			bValue = FALSE;
		}
	
		return bValue;
	}

	BOOL KAdoRecordset::FindFirst(LPCTSTR lpszFind)
	{
		m_pRecordset->MoveFirst();
		return Find(lpszFind);
	}

	BOOL KAdoRecordset::FindNext()
	{
		m_pRecordset->Find(_bstr_t(m_strFind), 1, (SearchDirectionEnum)m_nSearchDirection, m_varBookFind);

		if(m_nSearchDirection == adSearchForward)
		{
			//m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchForward, m_varBookFind);
			if(!IsEof())
			{
				m_varBookFind = m_pRecordset->Bookmark;
				return TRUE;
			}
		}
		else
		{
			//m_pRecordset->Find(_bstr_t(m_strFind), 1, adSearchBackward, m_varBookFind);
			if(!IsBof())
			{
				m_varBookFind = m_pRecordset->Bookmark;
				return TRUE;
			}
		}
		return FALSE;
	}

	BOOL KAdoRecordset::Delete()
	{
		if(m_pRecordset->Delete(adAffectCurrent) != S_OK)
			return FALSE;

		if(m_pRecordset->Update() != S_OK)
			return FALSE;
	
		m_nEditStatus = eAdoDbEditNone;
		return TRUE;
	}

	BOOL KAdoRecordset::SetBookmark()
	{
		if(m_varBookmark.vt != VT_EMPTY)
		{
			m_pRecordset->Bookmark = m_varBookmark;
			return TRUE;
		}
		return FALSE;
	}

	BOOL KAdoRecordset::AddNew()
	{
		m_nEditStatus = eAdoDbEditNone;
		if(m_pRecordset->AddNew() != S_OK)
			return FALSE;

		m_nEditStatus = eAdoDbEditNew;
		return TRUE;
	}

	void KAdoRecordset::Edit()
	{
		m_nEditStatus = eAdoDbEdit;
	}

	BOOL KAdoRecordset::AddNew(CADORecordBinding &pAdoRecordBinding)
	{
		try
		{
			if(m_pRecBinding->AddNew(&pAdoRecordBinding) != S_OK)
			{
				return FALSE;
			}
			else
			{
				m_pRecBinding->Update(&pAdoRecordBinding);
				return TRUE;
			}
			
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}	
	}

	BOOL KAdoRecordset::RecordBinding(CADORecordBinding &pAdoRecordBinding)
	{
		HRESULT hr;
		m_pRecBinding = NULL;

		//Open the binding interface.
		if(FAILED(hr = m_pRecordset->QueryInterface(__uuidof(IADORecordBinding), (LPVOID*)&m_pRecBinding )))
		{
			_com_issue_error(hr);
			return FALSE;
		}
	
		//Bind the recordset to class
		if(FAILED(hr = m_pRecBinding->BindToRecordset(&pAdoRecordBinding)))
		{
			_com_issue_error(hr);
			return FALSE;
		}
		return TRUE;
	}

	CString KAdoRecordset::GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows)
	{
		_bstr_t varOutput;
		_bstr_t varNull("");
		_bstr_t varCols("\t");
		_bstr_t varRows("\r");

		if(_tcslen(lpCols) != 0)
			varCols = _bstr_t(lpCols);

		if(_tcslen(lpRows) != 0)
			varRows = _bstr_t(lpRows);
	
		if(numRows == 0)
			numRows =(long)GetRecordCount();			
			
		varOutput = m_pRecordset->GetString(adClipString, numRows, varCols, varRows, varNull);

		return (LPCTSTR)varOutput;
	}

	BOOL KAdoRecordset::GetFieldInfo(FieldPtr pField, AdoFieldInfo* pAdoFieldInfo)
	{
		memset(pAdoFieldInfo, 0, sizeof(AdoFieldInfo));

		_tcscpy_s(pAdoFieldInfo->m_szName, 30, (LPCTSTR)pField->GetName());

		pAdoFieldInfo->m_lDefinedSize = (long)pField->GetDefinedSize();
		pAdoFieldInfo->m_nType = pField->GetType();
		pAdoFieldInfo->m_lAttributes = pField->GetAttributes();

		if(!IsEof())
		{
			pAdoFieldInfo->m_lSize = (long)pField->GetActualSize();
		}

		return TRUE;
	}

	BOOL KAdoRecordset::GetFieldInfo(int nIndex, AdoFieldInfo* pAdoFieldInfo)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

		return GetFieldInfo(pField, pAdoFieldInfo);
	}

	BOOL KAdoRecordset::GetFieldInfo(LPCTSTR lpFieldName, AdoFieldInfo* pAdoFieldInfo)
	{
		FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);
	
		return GetFieldInfo(pField, pAdoFieldInfo);
	}

	BOOL KAdoRecordset::GetChunk(FieldPtr pField, CString& strValue)
	{
		CString str;
		long lngSize, lngOffSet = 0;
		_variant_t varChunk;

		lngSize = (long)pField->ActualSize;
	
		str.Empty();
		while(lngOffSet < lngSize)
		{ 
			try
			{
				varChunk = pField->GetChunk(ADO_CHUNK_SIZE);
			
				str += varChunk.bstrVal;
				lngOffSet += ADO_CHUNK_SIZE;
			}
			catch(_com_error &e)
			{
				dump_com_error(e);
				return FALSE;
			}
		}

		lngOffSet = 0;
		strValue = str;
		return TRUE;
	}

	BOOL KAdoRecordset::GetChunk(int nIndex, CString& strValue)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);
	
		return GetChunk(pField, strValue);
	}

	BOOL KAdoRecordset::GetChunk(LPCTSTR lpFieldName, CString& strValue)
	{
		FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);
	
		return GetChunk(pField, strValue);
	}


	BOOL KAdoRecordset::GetChunk(FieldPtr pField, LPVOID lpData)
	{
		long lngSize, lngOffSet = 0;
		_variant_t varChunk;    
		UCHAR chData;
		HRESULT hr;
		long lBytesCopied = 0;

		lngSize = (long)pField->ActualSize;
	
		while(lngOffSet < lngSize)
		{ 
			try
			{
				varChunk = pField->GetChunk(ADO_CHUNK_SIZE);

				//Copy the data only upto the Actual Size of Field.  
				for(long lIndex = 0; lIndex < ADO_CHUNK_SIZE; lIndex++)
				{
					hr= SafeArrayGetElement(varChunk.parray, &lIndex, &chData);
					if(SUCCEEDED(hr))
					{
						//Take BYTE by BYTE and advance Memory Location
						//hr = SafeArrayPutElement((SAFEARRAY FAR*)lpData, &lBytesCopied ,&chData); 
						((UCHAR*)lpData)[lBytesCopied] = chData;
						lBytesCopied++;
					}
					else
						break;
				}
				lngOffSet += ADO_CHUNK_SIZE;
			}
			catch(_com_error &e)
			{
				dump_com_error(e);
				return FALSE;
			}
		}

		lngOffSet = 0;
		return TRUE;
	}

	BOOL KAdoRecordset::GetChunk(int nIndex, LPVOID lpData)
	{
		_variant_t vtIndex;
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

		return GetChunk(pField, lpData);
	}

	BOOL KAdoRecordset::GetChunk(LPCTSTR lpFieldName, LPVOID lpData)
	{
		FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

		return GetChunk(pField, lpData);
	}

	BOOL KAdoRecordset::AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes)
	{
		HRESULT hr;
		_variant_t varChunk;
		long lngOffset = 0;
		UCHAR chData;
		SAFEARRAY FAR *psa = NULL;
		SAFEARRAYBOUND rgsabound[1];

		try
		{
			//Create a safe array to store the array of BYTES 
			rgsabound[0].lLbound = 0;
			rgsabound[0].cElements = nBytes;
			psa = SafeArrayCreate(VT_UI1,1,rgsabound);

			while(lngOffset < (long)nBytes)
			{
				chData	= ((UCHAR*)lpData)[lngOffset];
				hr = SafeArrayPutElement(psa, &lngOffset, &chData);

				if(FAILED(hr))
					return FALSE;
			
				lngOffset++;
			}
			lngOffset = 0;

			//Assign the Safe array  to a variant. 
			varChunk.vt = VT_ARRAY|VT_UI1;
			varChunk.parray = psa;

			hr = pField->AppendChunk(varChunk);

			if(SUCCEEDED(hr)) return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}

		return FALSE;
	}

	BOOL KAdoRecordset::AppendChunk(int nIndex, LPVOID lpData, UINT nBytes)
	{
		_variant_t vtIndex;
	
		vtIndex.vt = VT_I2;
		vtIndex.iVal = nIndex;

		FieldPtr pField = m_pRecordset->Fields->GetItem(vtIndex);

		return AppendChunk(pField, lpData, nBytes);
	}

	BOOL KAdoRecordset::AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes)
	{

		FieldPtr pField = m_pRecordset->Fields->GetItem(lpFieldName);

		return AppendChunk(pField, lpData, nBytes);
	}

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------

