#include "StdAfx.h"
#include "KJetEngine.h"

namespace Ado
{
	/*
	KJetEngine::KJetEngine(void)
	{
	}

	KJetEngine::~KJetEngine(void)
	{
	}

	//static
	//ѹ�����ݿ�
	// strDatabaseSource Դ���ݿ�����
	// strDatabaseDestination Ŀ�����ݿ�����
	BOOL KJetEngine::CompactDatabase(CString strDatabaseSource, CString strDatabaseDestination)
	{
		try
		{
			::CoInitialize(NULL);
			IJetEnginePtr jet(__uuidof(JetEngine));		
			HRESULT hr = jet->CompactDatabase(_bstr_t(strDatabaseSource.GetBuffer(0)), _bstr_t(strDatabaseDestination.GetBuffer(0)));
			jet.Release();
			::CoUninitialize();
			return hr == S_OK;
		}
		catch(_com_error) 
		{       
			::CoUninitialize();
			return FALSE;
		} 
	}

	//static
	//ˢ�»�����
	BOOL KJetEngine::RefreshCache(ADODB::_Connection *pconn)
	{
		try
		{
			::CoInitialize(NULL);
			IJetEnginePtr jet(__uuidof(JetEngine)); 
			HRESULT hr = jet->RefreshCache(pconn);
			::CoUninitialize();
			return hr == S_OK;
		}
		catch(_com_error) 
		{ 
			::CoUninitialize();
			return FALSE;
		} 
	
		return FALSE;
	}
	*/
//---------------------------------------------------------
}// namespace Ado - end
//---------------------------------------------------------
