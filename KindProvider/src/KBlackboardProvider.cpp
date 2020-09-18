#include "StdAfx.h"
#include "KBlackboardProvider.h"


KBlackboardProvider::KBlackboardProvider(void)
{
}

KBlackboardProvider::KBlackboardProvider(LPCTSTR lpszConnectionString) throw(KException)
	: KProvider(lpszConnectionString)
{
	SetType(ProviderType_AgmrsData);
	if(!Open(lpszConnectionString))
	{
		throw KException(_T("Open connection string error."));
	}
}

KBlackboardProvider::~KBlackboardProvider(void)
{
}

// ´ò¿ª
BOOL KBlackboardProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = false;
	if(!lpszConnectionString)
	{
		m_bOpen = true;

		m_pDataObject = new KBlackboardData();
		m_pDataObject->GetDataInfo()->SetComment(_T("Í¼Ôª±à¼­"));
	}
	else
	{

	}

	return KProvider::Open(lpszConnectionString);
}

const KExtents& KBlackboardProvider::GetExtents()
{
	KBlackboardData* pBlackboardData = (KBlackboardData*)this->GetDataObject();
	this->SetExtents(pBlackboardData->GetExtents());
	return KObject::GetExtents();
}
