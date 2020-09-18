/******************************************************************************
 ** Header File: AdoDatabase.h
 **
 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KAdoDatabase
 ** Version:
 ** Function: 操作数据库 mdb
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
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
		// 连接字符串
		CString GetConnectionString() { return m_strConnection; }
		void SetConnectionString(LPCTSTR lpstrConnection) { m_strConnection = lpstrConnection; }

		// 连接超时
		long GetConnectionTimeout() { return m_nConnectionTimeout; }
		void SetConnectionTimeout(long nConnectionTimeout = 30) { m_nConnectionTimeout = nConnectionTimeout; }

		// 连接模式
		int GetConnectionMode() { return m_pConnection->GetMode(); }
		void SetConnectionMode(int nConnectMode) { m_pConnection->PutMode((ADODB::ConnectModeEnum)nConnectMode); }

		// 连接指针
		_ConnectionPtr GetConnection() { return m_pConnection; }

		// 影响到的记录数量
		int GetRecordsAffected() { return m_nRecordsAffected; }

		// 事务
		long BeginTransaction() { return m_pConnection->BeginTrans(); }
		long CommitTransaction() { return m_pConnection->CommitTrans(); }
		long RollbackTransaction() { return m_pConnection->RollbackTrans(); }

		// 错误信息
		DWORD GetLastError() { return m_dwLastError; }
		CString GetLastErrorString() { return m_strLastError; }
		CString GetErrorDescription() { return m_strErrorDescription; }

	public:
		// 打开连接
		BOOL Open(LPCTSTR lpstrConnection = _T(""), LPCTSTR lpstrUserID = _T(""), LPCTSTR lpstrPassword = _T(""));

		// 关闭连接
		void Close();

		// 运行并返回影响的记录数量
		int Execute(LPCTSTR lpstrExec);

		// 是否打开
		BOOL IsOpen();

	protected:
		// 倾倒错误
		void dump_com_error(_com_error &e);

	protected:
		_ConnectionPtr m_pConnection;		// 连接只能指针

		CString m_strConnection;			// 连接字符串
		long m_nConnectionTimeout;			// 连接时间延时

		int m_nRecordsAffected;				// 影响的记录数量

		CString m_strLastError;				// 最后的错误信息
		CString m_strErrorDescription;		// 错误描述
		DWORD m_dwLastError;				// 错误代码

	public:
		// 取得记录数量
		static long GetRecordCount(_RecordsetPtr pRecordset);
	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
