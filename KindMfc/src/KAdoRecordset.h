/******************************************************************************
 ** Header File: AdoRecordset.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KAdoRecordset
 ** Version:
 ** Function: 操作数据库记录集
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	//1. 
		KAdoRecordset adoRecordset(KAdoDatabase* pAdoDatabase);

******************************************************************************/

#pragma once

#include "KAdoDatabase.h"
#include "KAdoCommand.h"

namespace Ado
{

	class KIND_EXT_CLASS KAdoRecordset
	{
	public:
		enum AdoOpenEnum
		{
			eAdoOpenUnknown		= 0,
			eAdoOpenQuery		= 1,	// 查询
			eAdoOpenTable		= 2,	// 表
			eAdoOpenStoredProc	= 3		// 存储过程
		};

		enum AdoDbEditEnum
		{
			eAdoDbEditNone	= 0,
			eAdoDbEditNew	= 1,
			eAdoDbEdit		= 2
		};

	public:
		KAdoRecordset(KAdoDatabase* pAdoDatabase);
		virtual ~KAdoRecordset(void);

	public:
		BOOL IsEof() { return m_pRecordset->EndOfFile == VARIANT_TRUE; }
		BOOL IsEOF() { return m_pRecordset->EndOfFile == VARIANT_TRUE; }

		BOOL IsBof() { return m_pRecordset->BOF == VARIANT_TRUE; }
		BOOL IsBOF() { return m_pRecordset->BOF == VARIANT_TRUE; }

		BOOL IsConnectionOpen() { return m_pConnection != NULL && m_pConnection->GetState() != adStateClosed; }

		void MoveFirst() { m_pRecordset->MoveFirst(); }
		void MoveNext() { m_pRecordset->MoveNext(); }
		void MovePrevious() { m_pRecordset->MovePrevious(); }
		void MoveLast() { m_pRecordset->MoveLast(); }

		void SetAbsolutePage(int nPage) { m_pRecordset->PutAbsolutePage((enum PositionEnum)nPage); }
		long GetAbsolutePage() { return (long)m_pRecordset->GetAbsolutePage(); }

		void SetPageSize(int nSize) { m_pRecordset->PutPageSize(nSize); }
		long GetPageSize() { return m_pRecordset->GetPageSize(); }

		void SetAbsolutePosition(int nPosition) { m_pRecordset->PutAbsolutePosition((enum PositionEnum)nPosition); }
		long GetAbsolutePosition() { return (long)m_pRecordset->GetAbsolutePosition(); }

		long GetPageCount() { return (long)m_pRecordset->GetPageCount(); }

		_RecordsetPtr GetRecordset() { return m_pRecordset; }

		_ConnectionPtr GetActiveConnection() { return m_pConnection; }

		void SetQuery(LPCSTR strQuery) { m_strQuery = strQuery; }
		CString GetQuery() { return m_strQuery; }

		long GetFieldCount() { return m_pRecordset->Fields->GetCount(); }

		_variant_t GetBookmark() { return m_pRecordset->Bookmark; }

		CString GetLastErrorString() { return m_strLastError; }
		DWORD GetLastError() { return m_dwLastError; }

	public:
		BOOL IsOpen();

	public:
		BOOL Open(LPCTSTR lpstrExec = _T(""), int nOption = eAdoOpenUnknown);

		BOOL OpenSchema(int nSchema, LPCTSTR SchemaID = _T(""));

		BOOL Requery();

		long GetRecordCount();

		void Close();

		BOOL Update();
		void CancelUpdate();

		BOOL Execute(KAdoCommand* pAdoCommand);

		BOOL SaveAsXML(LPCTSTR lpstrXMLFile);
		BOOL OpenXML(LPCTSTR lpstrXMLFile);

		BOOL SetFilter(LPCTSTR strFilter);
		BOOL SetSort(LPCTSTR strCriteria);

		BOOL Clone(KAdoRecordset &pRs);

		BOOL Find(LPCTSTR lpFind, int nSearchDirection=adSearchForward);
		BOOL FindFirst(LPCTSTR lpszFind);
		BOOL FindNext();

		BOOL Delete();
		BOOL SetBookmark();

		BOOL AddNew();
		void Edit();

		BOOL AddNew(CADORecordBinding &pAdoRecordBinding);
		BOOL RecordBinding(CADORecordBinding &pAdoRecordBinding);

		CString GetString(LPCTSTR lpCols, LPCTSTR lpRows, LPCTSTR lpNull, long numRows);

		BOOL GetFieldInfo(FieldPtr pField, AdoFieldInfo* pAdoFieldInfo);
		BOOL GetFieldInfo(int nIndex, AdoFieldInfo* pAdoFieldInfo);
		BOOL GetFieldInfo(LPCTSTR lpFieldName, AdoFieldInfo* pAdoFieldInfo);

		BOOL GetChunk(FieldPtr pField, CString& strValue);
		BOOL GetChunk(int nIndex, CString& strValue);
		BOOL GetChunk(LPCTSTR lpFieldName, CString& strValue);

		BOOL GetChunk(FieldPtr pField, LPVOID lpData);
		BOOL GetChunk(int nIndex, LPVOID lpData);
		BOOL GetChunk(LPCTSTR lpFieldName, LPVOID lpData);

		BOOL AppendChunk(FieldPtr pField, LPVOID lpData, UINT nBytes);
		BOOL AppendChunk(int nIndex, LPVOID lpData, UINT nBytes);
		BOOL AppendChunk(LPCTSTR lpFieldName, LPVOID lpData, UINT nBytes);

	public:
		//判断 NULL 字段
		virtual BOOL IsFieldNull(int nIndex);
		virtual BOOL IsFieldNull(LPCTSTR lpFieldName);

		//判断 EMPTY 字段
		virtual BOOL IsFieldEmpty(int nIndex);
		virtual BOOL IsFieldEmpty(LPCTSTR lpFieldName);

	public:
		//设置字段值为空
		virtual BOOL SetFieldEmpty(int nIndex);
		virtual BOOL SetFieldEmpty(LPCTSTR lpFieldName);

		//设置通用型字段值
		virtual BOOL SetFieldValue(int nIndex, _variant_t vtValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, _variant_t vtValue);

		//设置字符串型字段值
		virtual BOOL SetFieldValue(int nIndex, CString strValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, CString strValue);

		//设置短整型字段值
		virtual BOOL SetFieldValue(int nIndex, short iValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, short iValue);
		virtual BOOL SetFieldValue(int nIndex, unsigned short uiValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, unsigned short uiValue);

		//设置长整型字段值
		virtual BOOL SetFieldValue(int nIndex, long lValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, long lValue);
		virtual BOOL SetFieldValue(int nIndex, unsigned long ulValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, unsigned long ulValue);

		//设置单精度浮点型字段值
		virtual BOOL SetFieldValue(int nIndex, float fValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, float fValue);

		//设置双精度浮点型字段值
		virtual BOOL SetFieldValue(int nIndex, double dValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, double dValue);

		//设置逻辑型字段值
		virtual BOOL SetFieldValue(int nIndex, BOOL bValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, BOOL bValue);

		//设置日期时间型字段值
		virtual BOOL SetFieldValue(int nIndex, COleDateTime dtValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, COleDateTime dtValue);

		//设置货币型字段值
		virtual BOOL SetFieldValue(int nIndex, COleCurrency cyValue);
		virtual BOOL SetFieldValue(LPCTSTR lpFieldName, COleCurrency cyValue);

		private:
		virtual BOOL PutFieldValue(_variant_t vtIndex, _variant_t vtFld);
		virtual BOOL PutFieldValue(LPCTSTR lpFieldName, _variant_t vtFld);

	public:
		//获取通用整型字段值
		virtual BOOL GetFieldValue(int nIndex, _variant_t& vtValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, _variant_t& vtValue);

		//获取字符串型字段值
		virtual BOOL GetFieldValue(int nIndex, CString& strValue,CString strDateFormat = _T(""));
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, CString& strValue,CString strDateFormat = _T(""));

		//获取短整型字段值
		virtual BOOL GetFieldValue(int nIndex, short& iValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, short& iValue);
		virtual BOOL GetFieldValue(int nIndex, unsigned short& uiValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, unsigned short& uiValue);

		//获取长整型字段值
		virtual BOOL GetFieldValue(int nIndex, long& lValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, long& lValue);
		virtual BOOL GetFieldValue(int nIndex, unsigned long& ulValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, unsigned long& ulValue);

		//获取单精度浮点型字段值
		virtual BOOL GetFieldValue(int nIndex, float& fValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, float& fValue);

		//获取双精度浮点型字段值
		virtual BOOL GetFieldValue(int nIndex, double& dValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, double& dValue);

		//获取逻辑型字段值
		virtual BOOL GetFieldValue(int nIndex, BOOL& bValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, BOOL& bValue);

		//获取日期时间型字段值
		virtual BOOL GetFieldValue(int nIndex, COleDateTime& dtValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, COleDateTime& dtValue);

		//获取货币型字段值
		virtual BOOL GetFieldValue(int nIndex, COleCurrency& cyValue);
		virtual BOOL GetFieldValue(LPCTSTR lpFieldName, COleCurrency& cyValue);

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

	protected:
		void dump_com_error(_com_error &e);

	public:
		_ConnectionPtr m_pConnection;		// 连接
		_CommandPtr m_pCommand;				// 命令
		_RecordsetPtr m_pRecordset;			// 记录集

		IADORecordBinding *m_pRecBinding;	// 绑定记录集
	
	protected:
		int m_nSearchDirection;
		CString m_strFind;

		_variant_t m_varBookFind;
		_variant_t m_varBookmark;

		int m_nEditStatus;

		CString m_strQuery;

		CString m_strLastError;
		DWORD m_dwLastError;


	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
