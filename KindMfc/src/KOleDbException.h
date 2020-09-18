/******************************************************************************
 ** Header File: OleDbDataLink.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KOleDbException 
 ** Version:
 ** Function:
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

	class KIND_EXT_CLASS KOleDbException
	{
	public:
		KOleDbException()
			:m_lErrorCode(0)
			,m_strError(_T(""))
		{}

		KOleDbException(long lErrorCode)
			:m_lErrorCode(lErrorCode)
			,m_strError(_T(""))
		{}

		KOleDbException(long lErrorCode, const CString& strError)
			:m_lErrorCode(lErrorCode)
			,m_strError(strError)
		{}

		KOleDbException(const CString& strError)
			:m_lErrorCode(0)
			,m_strError(strError)
		{}

		KOleDbException(long lErrorCode, const char* szError)
			:m_lErrorCode(lErrorCode)
			,m_strError(szError)
		{}

		KOleDbException(const char* szError)
			:m_lErrorCode(0)
			,m_strError(szError)
		{}

		~KOleDbException(void)
		{}

	public:
		// ����/��ȡ������Ϣ
		void SetErrorMessage(LPCTSTR lpstrError) { m_strError = lpstrError; }
		CString GetErrorMessage() const { return m_strError; }

		// ����/��ȡ�������
		void SetError(long lErrorCode) { m_lErrorCode = lErrorCode; }
		long GetError() { return m_lErrorCode; }

	protected:
		CString m_strError;	// ������Ϣ
		long m_lErrorCode;	// �������
	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
