#pragma once

#include "KObject.h"
#include "KString.h"

class KIND_EXT_CLASS KStation : public KObject
{
public:
	KStation(void);
	virtual ~KStation(void);

public:
	KString& StationID()	{ return m_strStationID; }
	KString& StationName()	{ return m_strStationName; }

	double& Lon()			{ return m_dLon; }
	double& Lat()			{ return m_dLat; }
	double& Alt()			{ return m_dAlt; }

	int& Level()			{ return m_iLevel; }

protected:
	KString m_strStationID;
	double m_dLon;
	double m_dLat;
	double m_dAlt;
	int	m_iLevel;				// Õ¾µã¼¶±ð
	KString m_strStationName;
};

