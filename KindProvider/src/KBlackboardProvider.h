/******************************************************************************
 ** KBlackboardProvider.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KBlackboardProvider
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

class KIND_EXT_CLASS KBlackboardProvider : public KProvider
{
public:
	KBlackboardProvider(void);
	KBlackboardProvider(LPCTSTR lpszConnectionString) throw(KException);
	virtual ~KBlackboardProvider(void);

public:
	// ´ò¿ª
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);

public:
	virtual const KExtents& GetExtents();
};

