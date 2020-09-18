/******************************************************************************
 ** Header File: KOleDbDataLink.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: C
 ** Version:
 ** Function:
 ** Explain:

 ** Author: Carlos Antollini <cantollini@hotmail.com>
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender: ShenYongchen(shenyczz@163.com) […Í”¿≥Ω]
 ** DateTime:
 ** Explain:

 ** Usage:

	//1. 
		use

******************************************************************************/

#pragma once

namespace Ado
{

	class KIND_EXT_CLASS KOleDbDataLink
	{
	public:
		KOleDbDataLink(void);
		virtual ~KOleDbDataLink(void);

	public:
		CString New(HWND hWnd = NULL);
		void Edit(HWND hWnd, LPCSTR lpstrConnectionString);

	private:
		IDataSourceLocatorPtr m_pDataLink;
	};

//---------------------------------------------------------
} // namespace Ado - end
//}---------------------------------------------------------
