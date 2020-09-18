#pragma once

#include "KProvider.h"

class KIND_EXT_CLASS KDiamondDataProvider : public KProvider
{
public:
	KDiamondDataProvider(void);
	KDiamondDataProvider(LPCTSTR lpszConnectionString) throw(KException);
	virtual ~KDiamondDataProvider(void);

public:
	// ´ò¿ª
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);

private:
	BOOL openStationData(LPCTSTR lpszConnectionString);
	BOOL openGridData_Contour(LPCTSTR lpszConnectionString);
	//BOOL openGridData_Image(LPCTSTR lpszConnectionString);
	//BOOL openGridData_WindVector(LPCTSTR lpszConnectionString);
	//BOOL openImageCtrlData(LPCTSTR lpszConnectionString);
};

