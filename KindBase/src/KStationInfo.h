#pragma once

#include "KStation.h"

class KIND_EXT_CLASS KStationInfo : public KStation
{
public:
	KStationInfo(void);
	virtual ~KStationInfo(void);

public:
	int& ElementNum()				{ return m_iElementNum; }
	double& ElementValue(int index)	{ return m_dElementValue[index]; }

protected:
	int m_iElementNum;
	double m_dElementValue[110];
};

