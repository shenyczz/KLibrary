/******************************************************************************
 ** KAxinDataProvider.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KAxinDataProvider
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ÉêÓÀ³½.Ö£ÖÝ (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KProvider.h"

class KIND_EXT_CLASS KAxinDataProvider : public KProvider
{
public:
	KAxinDataProvider(void);
	KAxinDataProvider(LPCTSTR lpszConnectionString) throw(KException);
	virtual ~KAxinDataProvider(void);

public:
	// ´ò¿ª
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);

public:
	virtual const KExtents& GetExtents();

private:
	BOOL openStationData(LPCTSTR lpszConnectionString);
	BOOL openGridData_Contour(LPCTSTR lpszConnectionString);
	BOOL openGridData_Image(LPCTSTR lpszConnectionString);
	BOOL openGridData_WindVector(LPCTSTR lpszConnectionString);
	BOOL openImageCtrlData(LPCTSTR lpszConnectionString);
};

