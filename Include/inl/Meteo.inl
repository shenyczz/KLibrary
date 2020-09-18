#ifndef __INCLUDED_METEO_INL__
#define __INCLUDED_METEO_INL__

#pragma pack(1)

// վ��
//
struct Station
{
	TCHAR szStationID[33];		// ���Ա���32��ANSI�ַ�
	double dLon;
	double dLat;
	double dAlt;
	TCHAR szStationName[65];	// ���Ա���64��ANSI�ַ�

	Station()
	{
		_tcscpy(szStationID,_T(""));
		dLon = 0;
		dLat = 0;
		dAlt = 0;
		_tcscpy(szStationName,_T(""));
	}

	void SetStationID(LPCTSTR lpszStationID)
	{
		memset(szStationID,0,sizeof(TCHAR)*33);
		_tcsncpy(szStationID,lpszStationID,33);
		szStationID[32] = _T('0');
	}
	LPCTSTR GetStationID() { return szStationID; }

	void SetStationName(LPCTSTR lpszStationName)
	{
		memset(szStationName,0,sizeof(TCHAR)*65);
		_tcsncpy(szStationName,lpszStationName,65);
		szStationID[64] = _T('0');
	}
	LPCTSTR GetStationName() { return szStationName; }
};

#pragma pack()

#endif	//#ifndef __INCLUDED_METEO_INL__

