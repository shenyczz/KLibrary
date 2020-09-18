#pragma once

#include "KProvider.h"

class KIND_EXT_CLASS KAgmrsDataProvider : public KProvider
{
public:
	KAgmrsDataProvider(void);
	KAgmrsDataProvider(LPCTSTR lpszConnectionString) throw(KException);
	virtual ~KAgmrsDataProvider(void);

public:
	virtual const KExtents& GetExtents();
	// ´ò¿ª
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);
};

