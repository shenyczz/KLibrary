#include "StdAfx.h"
#include "KRadarDataProvider.h"


KRadarDataProvider::KRadarDataProvider(void)
{
}

KRadarDataProvider::KRadarDataProvider(LPCTSTR lpszConnectionString)
	: KProvider(lpszConnectionString)
{
	SetType(ProviderType_RadarData);
	if(!Open(lpszConnectionString))
	{
		throw KException(_T("Open connection string error."));
	}
}


KRadarDataProvider::~KRadarDataProvider(void)
{
}

// ��
BOOL KRadarDataProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = false;

	if(KRdaData::IsRdaData(lpszConnectionString))
	{
	}
	else if(KRpgData::IsRpgData(lpszConnectionString))
	{
		KRpgData* pRpgData = new KRpgData();
		_delete(m_pDataObject);
		m_pDataObject = pRpgData;

		if(pRpgData->LoadData(lpszConnectionString))
		{
			// ���ɵ�����������(���������)
			m_bOpen = pRpgData->ToRangeBin() ? true: false;
		}
	}

	return KProvider::Open(lpszConnectionString);
}
