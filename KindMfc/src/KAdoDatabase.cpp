#include "StdAfx.h"
#include "KAdoDatabase.h"

#include "KAdoException.h"

namespace Ado
{

	KAdoDatabase::KAdoDatabase()
	{
		::CoInitialize(NULL);
		m_pConnection.CreateInstance(__uuidof(Connection));
		
		m_strConnection = _T("");

		m_nRecordsAffected = 0;
		m_nConnectionTimeout = 30;

		m_strLastError = _T("");
		m_dwLastError = 0;
	}

	KAdoDatabase::~KAdoDatabase(void)
	{
		Close();
		m_pConnection.Release();
		m_pConnection = NULL;
		m_strConnection = _T("");

		m_strLastError = _T("");
		m_dwLastError = 0;

		::CoUninitialize();
	}

	//=======================================================
	// ��    ��: Open
	// ��    ��: public
	//
	// ��    ��: �����ݿ�
	//
	// ��    ��:
	//			 [i] lpstrConnection - �����ַ���,Ĭ��=_T("")
	//			 [i] lpstrUserID - �û�ID,Ĭ��=_T("")
	//			 [i] lpstrPassword - �û�����,Ĭ��=_T("")
	//
	// ��    ��: 
	//			 TRUE - �ɹ�
	//			 FALSE - �ɹ�
	//
	// ע    ��: 
	//
	//=======================================================
	BOOL KAdoDatabase::Open(LPCTSTR lpstrConnection, LPCTSTR lpstrUserID, LPCTSTR lpstrPassword)
	{
		if(!lpstrConnection || _tcscmp(lpstrConnection,_T("")) !=0 )
		{
			m_strConnection = lpstrConnection;
		}

		if(IsOpen())
			this->Close();

		try
		{
			if(m_nConnectionTimeout != 0)
			{
				m_pConnection->PutConnectionTimeout(m_nConnectionTimeout);
			}
			HRESULT hr = m_pConnection->Open(_bstr_t(m_strConnection), _bstr_t(lpstrUserID), _bstr_t(lpstrPassword), NULL);
			return hr == S_OK;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
			return FALSE;
		}
	
		return TRUE;
	}

	// �ر�����
	void KAdoDatabase::Close()
	{
		if(IsOpen())
		{
			m_pConnection->Close();
		}
	}

	// ���в�����Ӱ��ļ�¼����
	int KAdoDatabase::Execute(LPCTSTR lpstrExec)
	{
		ASSERT(m_pConnection != NULL);
		ASSERT(_tcscmp(lpstrExec, _T("")) != 0);

		_variant_t vRecords;
	
		m_nRecordsAffected = 0;

		try
		{
			m_pConnection->CursorLocation = adUseClient;
			m_pConnection->Execute(_bstr_t(lpstrExec), &vRecords, adExecuteNoRecords);
			m_nRecordsAffected = vRecords.iVal;
		}
		catch(_com_error &e)
		{
			dump_com_error(e);
		}

		return m_nRecordsAffected;
	}

	// �Ƿ��
	BOOL KAdoDatabase::IsOpen()
	{
		BOOL bValue(FALSE);
		//-------------------------------------------------
		if(m_pConnection )
		{
			bValue = (m_pConnection->GetState() != adStateClosed);
		}
		//-------------------------------------------------
		return bValue;
	}

	// �׳�����
	void KAdoDatabase::dump_com_error(_com_error &e)
	{
		CString ErrorStr;
	
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		ErrorStr.Format( _T("KAdoDatabase Error\n\tCode = %08lx\n\tCode meaning = %s\n\tSource = %s\n\tDescription = %s\n"),
			e.Error(), e.ErrorMessage(), (LPCSTR)bstrSource, (LPCSTR)bstrDescription);

		m_strErrorDescription = (LPCSTR)bstrDescription ;
		m_strLastError = _T("Connection String = " + GetConnectionString() + '\n' + ErrorStr);
		m_dwLastError = e.Error(); 

		#ifdef _DEBUG
			//AfxMessageBox(ErrorStr, MB_OK | MB_ICONERROR );
		#endif

		throw KAdoException(e.Error(), e.Description());
	}

	// static
	// ȡ�ü�¼����
	long KAdoDatabase::GetRecordCount(_RecordsetPtr pRecordset)
	{
		long numRows = 0;
		numRows = (long)pRecordset->GetRecordCount();

		if(numRows == -1)
		{
			if(pRecordset->EndOfFile != VARIANT_TRUE)
				pRecordset->MoveFirst();

			while(pRecordset->EndOfFile != VARIANT_TRUE)
			{
				numRows++;
				pRecordset->MoveNext();
			}

			if(numRows > 0)
				pRecordset->MoveFirst();
		}

		return numRows;
	}

//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
