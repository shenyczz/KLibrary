#pragma once

#include "KProvider.h"

class KIND_EXT_CLASS KRadarDataProvider : public KProvider
{
public:
	KRadarDataProvider(void);
	KRadarDataProvider(LPCTSTR lpszConnectionString) throw(KException);
	virtual ~KRadarDataProvider(void);

public:
	// ´ò¿ª
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);
};

