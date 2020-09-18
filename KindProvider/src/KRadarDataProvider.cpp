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

// 打开
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
			// 生成地理坐标数据(距离库链表)
			m_bOpen = pRpgData->ToRangeBin() ? true: false;
		}
	}

	return KProvider::Open(lpszConnectionString);
}
