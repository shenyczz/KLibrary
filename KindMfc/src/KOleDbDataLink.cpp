#include "StdAfx.h"
#include "KOleDbDataLink.h"

#include "KOleDbException.h"

namespace Ado
{

	KOleDbDataLink::KOleDbDataLink(void)
	{
		m_pDataLink = NULL;
	}

	KOleDbDataLink::~KOleDbDataLink(void)
	{
		m_pDataLink = NULL;
	}

	CString KOleDbDataLink::New(HWND hWnd /*= NULL*/)
	{
		m_pDataLink.CreateInstance(__uuidof(DataLinks));
		try
		{
			if(hWnd != NULL) m_pDataLink->PuthWnd(reinterpret_cast<long>(hWnd));
			IDispatchPtr pDisp = m_pDataLink->PromptNew();
		
			_ConnectionPtr conn = pDisp;

			CString strReturn = (LPCTSTR)(BSTR)conn->GetConnectionString().copy();

			m_pDataLink.Release();
			m_pDataLink = NULL;

			return strReturn;
		}
		catch(_com_error &e)
		{
			throw KOleDbException(e.WCode(), e.Description());
		}
	}

	void KOleDbDataLink::Edit(HWND hWnd, LPCSTR lpstrConnectionString)
	{
		BOOL bRet;
		IDispatch* pDispatch = NULL;
		_ConnectionPtr pAdoConnection = NULL;

		m_pDataLink.CreateInstance(__uuidof(DataLinks));
		pAdoConnection.CreateInstance(__uuidof(ADODB::Connection));
	
		try
		{
			m_pDataLink->PuthWnd(reinterpret_cast<long>(hWnd));
			pAdoConnection->PutConnectionString(_bstr_t(lpstrConnectionString));
			pAdoConnection.QueryInterface(IID_IDispatch, (LPVOID*)&pDispatch);

			bRet = m_pDataLink->PromptEdit(&pDispatch) == VARIANT_TRUE;

			m_pDataLink.Release();
			pAdoConnection.Release();
		}
		catch(_com_error &e)
		{
			throw KOleDbException(e.WCode(), e.Description());
		}
	}

//---------------------------------------------------------
} // namespace Ado - end
//---------------------------------------------------------
