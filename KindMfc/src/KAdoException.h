/******************************************************************************
 ** Header File: KAdoException.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: CAdoException - Ado 异常类
 ** Version:
 ** Function:
 ** Explain: reference codeguru.com

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

	class KIND_EXT_CLASS KAdoException
	{
	public:
		KAdoException(void)
			:m_lErrorCode(0)
			,m_strError(_T(""))
		{}

		KAdoException(long lErrorCode)
			:m_lErrorCode(lErrorCode)
			,m_strError(_T(""))
		{}

		KAdoException(long lErrorCode, const CString& strError) 
			:m_lErrorCode(lErrorCode)
			,m_strError(strError)
		{}

		KAdoException(const CString& strError)
			:m_lErrorCode(0)
			,m_strError(strError)
		{}

		KAdoException(long lErrorCode, const char* szError)
			:m_lErrorCode(lErrorCode)
			,m_strError(szError)
		{}

		KAdoException(const char* szError)
			:m_lErrorCode(0)
			,m_strError(szError)
		{}

		virtual ~KAdoException(void)
		{}

	public:
		// 设置/获取错误信息
		void SetErrorMessage(LPCTSTR lpstrError) { m_strError = lpstrError; }
		CString GetErrorMessage() const { return m_strError; }

		// 设置/获取错误代码
		void SetError(long lErrorCode) { m_lErrorCode = lErrorCode; }
		long GetError() { return m_lErrorCode; }

	protected:
		CString m_strError;	// 错误信息
		long m_lErrorCode;	// 错误代码
	};

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
