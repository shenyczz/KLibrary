/******************************************************************************
 ** Header File: AdoDatabase.h
 **
 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KAdoDatabase
 ** Version:
 ** Function: �������ݿ� mdb
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	// 1. 
		KAdoDatabase adoDatabase;
		adoDatabase.Open(lpstrConnection = _T(""), lpstrUserID = _T(""), lpstrPassword = _T(""));

******************************************************************************/

#pragma once

namespace Ado
{

	class KIND_EXT_CLASS KAdoDatabase
	{
	public:
		KAdoDatabase();
		virtual ~KAdoDatabase(void);

	public:
		// �����ַ���
		CString GetConnectionString() { return m_strConnection; }
		void SetConnectionString(LPCTSTR lpstrConnection) { m_strConnection = lpstrConnection; }

		// ���ӳ�ʱ
		long GetConnectionTimeout() { return m_nConnectionTimeout; }
		void SetConnectionTimeout(long nConnectionTimeout = 30) { m_nConnectionTimeout = nConnectionTimeout; }

		// ����ģʽ
		int GetConnectionMode() { return m_pConnection->GetMode(); }
		void SetConnectionMode(int nConnectMode) { m_pConnection->PutMode((ADODB::ConnectModeEnum)nConnectMode); }

		// ����ָ��
		_ConnectionPtr GetConnection() { return m_pConnection; }

		// Ӱ�쵽�ļ�¼����
		int GetRecordsAffected() { return m_nRecordsAffected; }

		// ����
		long BeginTransaction() { return m_pConnection->BeginTrans(); }
		long CommitTransaction() { return m_pConnection->CommitTrans(); }
		long RollbackTransaction() { return m_pConnection->RollbackTrans(); }

		// ������Ϣ
		DWORD GetLastError() { return m_dwLastError; }
		CString GetLastErrorString() { return m_strLastError; }
		CString GetErrorDescription() { return m_strErrorDescription; }

	public:
		// ������
		BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""));

		// �ر�����
		void Close();

		// ���в�����Ӱ��ļ�¼����
		int Execute(LPCTSTR lpstrExec);

		// �Ƿ��
		BOOL IsOpen();

	protected:
		// �㵹����
		void dump_com_error(_com_error &e);

	protected:
		_ConnectionPtr m_pConnection;		// ����ֻ��ָ��

		CString m_strConnection;			// �����ַ���
		long m_nConnectionTimeout;			// ����ʱ����ʱ

		int m_nRecordsAffected;				// Ӱ��ļ�¼����

		CString m_strLastError;				// ���Ĵ�����Ϣ
		CString m_strErrorDescription;		// ��������
		DWORD m_dwLastError;				// �������

	public:
		// ȡ�ü�¼����
		static long GetRecordCount(_RecordsetPtr pRecordset);
	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
