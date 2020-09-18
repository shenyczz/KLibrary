#include "StdAfx.h"
#include "KAdoCommand.h"

#include "KAdoDatabase.h"
#include "KAdoParameter.h"
#include "KAdoException.h"

namespace Ado
{

	KAdoCommand::KAdoCommand(KAdoDatabase* pAdoDatabase)
	{
		m_pCommand.CreateInstance(__uuidof(Command));

		m_pAdoDatabase = pAdoDatabase;
		m_nCommandType = adCmdUnknown;
		m_strCommandText = _T("");
		m_pCommand->ActiveConnection = pAdoDatabase->GetConnection();

		m_nRecordsAffected = 0;
	}

	KAdoCommand::KAdoCommand(KAdoDatabase* pAdoDatabase, CString strCommandText, int nCommandType)
	{
		m_pCommand.CreateInstance(__uuidof(Command));

		m_pAdoDatabase = pAdoDatabase;
		m_nCommandType = nCommandType;
		m_strCommandText = strCommandText;
		m_pCommand->ActiveConnection = pAdoDatabase->GetConnection();

		m_pCommand->CommandType = (CommandTypeEnum)nCommandType;
		m_pCommand->CommandText = strCommandText.AllocSysString();

		m_nRecordsAffected = 0;
	}

	KAdoCommand::~KAdoCommand(void)
	{
		if(m_pCommand)
		{
			::SysFreeString(m_pCommand->CommandText);
			m_pCommand.Release();
			m_pCommand = NULL;
		}

		m_strCommandText = _T("");
	}

	BOOL KAdoCommand::AddParameter(KAdoParameter* pAdoParameter)
	{
		ASSERT(pAdoParameter->GetParameter() != NULL);

		try
		{
			m_pCommand->Parameters->Append(pAdoParameter->GetParameter());
			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	BOOL KAdoCommand::AddParameter(CString strName, short iValue,int nDirection)
	{
		_variant_t vtValue;
		vtValue.vt = VT_I2;
		vtValue.iVal = iValue;

		int nType = adSmallInt;
		long lSize = sizeof(short);

		return AddParameter(strName, vtValue, nType, lSize, nDirection, 0, 0);
	}

	BOOL KAdoCommand::AddParameter(CString strName, long lValue,int nDirection)
	{
		_variant_t vtValue;
		vtValue.vt = VT_I4;
		vtValue.lVal = lValue;

		int nType = adInteger;
		long lSize = sizeof(long);

		return AddParameter(strName, vtValue, nType, lSize, nDirection, 0, 0);
	}

	BOOL KAdoCommand::AddParameter(CString strName, float fValue, int nDirection)
	{
		_variant_t vtValue;

		vtValue.vt = VT_R4;
		vtValue.fltVal = fValue;

		int nType = adSingle;
		long lSize = sizeof(float);

		return AddParameter(strName, vtValue, nType, lSize, nDirection, 0, 0);
	}

	BOOL KAdoCommand::AddParameter(CString strName, double dValue, int nDirection)
	{

		_variant_t vtValue;

		vtValue.vt = VT_R8;
		vtValue.dblVal = dValue;

		int nType = adDouble;
		long lSize = sizeof(double);

		return AddParameter(strName, vtValue, nType, lSize, nDirection, 0, 0);
	}

	BOOL KAdoCommand::AddParameter(CString strName, CString strValue, int nDirection)
	{
		_variant_t vtValue;
		vtValue.vt = VT_BSTR;
		vtValue.bstrVal = strValue.AllocSysString();

		int nType = adBSTR;
		long lSize = strValue.GetLength();

		return AddParameter(strName, vtValue, nType, lSize, nDirection, 0, 0);
	}

	BOOL KAdoCommand::AddParameter(CString strName, COleDateTime date_time, int nDirection)
	{
		_variant_t vtValue;

		vtValue.vt = VT_DATE;
		vtValue.date = date_time;

		int nType = adDate;
		long lSize = sizeof(DATE);

		return AddParameter(strName, vtValue, nType, lSize, nDirection, 0, 0);
	}

	//=======================================================
	// ��    ��: AddParameter
	// ��    ��: public
	// ��    ��: Ϊ�������Ӳ���
	// ��    ��:
	//			 [i] strName	- ��������
	//			 [i] vtValue	- ������ֵ
	//			 [i] nType		- ������������ DataTypeEnum
	//			 [i] lSize		- ����ֵ�ĳ��� [example: sizeof(int)]
	//			 [i] nDirection	- �����Ĳ�������,Ĭ��=adParamInput
	//			 [i] nPrecision	- ����ֵ����(�ֶο��)��Ĭ��=0
	//			 [i] nScale		- ����ֵС��λ����Ĭ��=0
	//
	// ��    ��: 
	// ע    ��: �� nType=adNumeric �� adDecimal ʱ
	//           ���붨�� precision and scale  ��ֵ 
	//=======================================================
	BOOL KAdoCommand::AddParameter(CString strName, _variant_t vtValue, int nType, long lSize, int nDirection, int nPrecision, int nScale)
	{
		try
		{
			_bstr_t bstrTemp = strName.AllocSysString();
			//---------------------------------------------
			_ParameterPtr pParam = m_pCommand->CreateParameter
				(
					bstrTemp,
					(ADODB::DataTypeEnum)nType,
					(ADODB::ParameterDirectionEnum)nDirection,
					lSize,
					vtValue
				);

			pParam->PutPrecision(nPrecision);
			pParam->PutNumericScale(nScale);
			m_pCommand->Parameters->Append(pParam);
			//---------------------------------------------
			::SysFreeString(bstrTemp);

			return TRUE;
		}
		catch(_com_error& e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	// ����
	BOOL KAdoCommand::Execute()
	{
		_variant_t vRecords;
		m_nRecordsAffected = 0;
		try
		{
			m_pCommand->Execute(&vRecords, NULL, m_nCommandType);
			m_nRecordsAffected = vRecords.iVal;
			return TRUE;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	}

	void KAdoCommand::dump_com_error(_com_error &e)
	{
		CString ErrorStr;
	
	
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		ErrorStr.Format( _T("KAdoCommand Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
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
