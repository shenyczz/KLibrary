/******************************************************************************
 ** Header File: AdoCommand.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KAdoCommand
 ** Version:
 ** Function: ADO 命令类
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	//SQL SCRIPT...
	Create Procedure sp_OutputTest
	@IN1 int,
	@OutInt int Output,
	@OutChar varchar(20) Output
	As   
		SELECT    
			@OutChar = 'Hello World'
		SELECT    
			@OutInt = 10 * @IN1
		return (90)
	GO


	//Visual C++ Code...

	CADOParameter pParamRetVal(CADORecordset::typeInteger, sizeof(int),
							   CADOParameter::paramReturnValue);
	CADOParameter pParamIn(CADORecordset::typeInteger, sizeof(int));
	CADOParameter pParamOutInt(CADORecordset::typeInteger, sizeof(int),
							   CADOParameter::paramOutput);
	CADOParameter pParamOutChar(CADORecordset::typeChar, sizeof(char) * 20,
								CADOParameter::paramOutput);

	pParamIn.SetValue(2);

	CADOCommand pCmd(&pDb, "sp_OutputTest");

	pCmd.AddParameter(&pParamRetVal);
	pCmd.AddParameter(&pParamIn);
	pCmd.AddParameter(&pParamOutInt);
	pCmd.AddParameter(&pParamOutChar);

	CADORecordset* prs = new CADORecordset(pDb);

	if(pRs->Execute(&pCmd))
	{
		int nVal = 0;
		int nRetVal = 0;
		CString str = _T("");

		pParamRetVal.GetValue(nRetVal);
		pParamOutInt.GetValue(nVal);
		pParamOutChar.GetValue(str);
	}

	delete pRS;

	1.
	KAdoCommand adoCommand(pAdoDatabase);
	2.
	adoCommand.SetCommandType(type);
	3.
	adoCommand.SetCommandText(text);

******************************************************************************/

#pragma once

#include "KAdoDatabase.h"
#include "KAdoParameter.h"

namespace Ado
{

	class KIND_EXT_CLASS KAdoCommand
	{
	public:
		KAdoCommand(KAdoDatabase* pAdoDatabase);
		KAdoCommand(KAdoDatabase* pAdoDatabase, CString strCommandText, int nCommandType = adCmdStoredProc);
		virtual ~KAdoCommand(void);

	public:
		// 命令类型
		int GetCommandType() { return m_nCommandType; }
		void SetCommandType(int nCommandType)
		{
			m_nCommandType = nCommandType;
			m_pCommand->CommandType = (CommandTypeEnum)m_nCommandType;
		}

		// 命令文本
		CString GetCommandText() { return m_strCommandText; }
		void SetCommandText(CString strCommandText)
		{
			if(strCommandText.IsEmpty())
				return;

			m_strCommandText = strCommandText;
			::SysFreeString(m_pCommand->CommandText);
			m_pCommand->CommandText = m_strCommandText.AllocSysString();
		}

		// 超时(毫秒)
		long GetTimeout(){ return m_pCommand->GetCommandTimeout(); }
		void SetTimeout(long nTimeOut) { m_pCommand->PutCommandTimeout(nTimeOut); }

		int GetRecordsAffected() { return m_nRecordsAffected; }
		_CommandPtr GetCommand() { return m_pCommand; }

	public:
		// 添加参数
		BOOL AddParameter(KAdoParameter* pAdoParameter);
		BOOL AddParameter(CString strName, short iValue, int nDirection = adParamInput);
		BOOL AddParameter(CString strName, long iValue, int nDirection = adParamInput);
		BOOL AddParameter(CString strName, float fValue, int nDirection = adParamInput);
		BOOL AddParameter(CString strName, double dValue, int nDirection = adParamInput);
		BOOL AddParameter(CString strName, CString strValue, int nDirection = adParamInput);
		BOOL AddParameter(CString strName, COleDateTime date_time, int nDirection = adParamInput);
		BOOL AddParameter(CString strName, _variant_t vtValue, int nType, long lSize, int nDirection, int nPrecision, int nScale);

	public:
		// 运行
		BOOL Execute();

	protected:
		void dump_com_error(_com_error &e);

	protected:
		KAdoDatabase* m_pAdoDatabase;
		int m_nCommandType;
		CString m_strCommandText;
		int m_nRecordsAffected;

		_CommandPtr m_pCommand;

		CString m_strLastError;
		DWORD m_dwLastError;
	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
