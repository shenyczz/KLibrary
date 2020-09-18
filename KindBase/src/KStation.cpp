#include "StdAfx.h"
#include "KStation.h"


KStation::KStation(void)
{
	m_strStationID = _T("");
	m_dLon = 0;
	m_dLat = 0;
	m_dAlt = 0;
	m_iLevel = 0;
	m_strStationName = _T("");
}


KStation::~KStation(void)
{
}
