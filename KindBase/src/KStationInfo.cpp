#include "StdAfx.h"
#include "KStationInfo.h"


KStationInfo::KStationInfo(void)
{
	m_iElementNum = 0;
	for(int i=0;i<110;i++)
	{
		m_dElementValue[i] = 0;
	}
}


KStationInfo::~KStationInfo(void)
{
}
