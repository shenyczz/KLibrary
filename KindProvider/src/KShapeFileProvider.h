/******************************************************************************
 ** KShapeFileProvider.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KShapeFileProvider
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

class KIND_EXT_CLASS KShapeFileProvider : public KProvider
{
public:
	KShapeFileProvider(void);
	KShapeFileProvider(const KShapeFileProvider& shapeFileProvider);
	KShapeFileProvider(LPCTSTR lpszConnectionString) throw(KException);
	virtual ~KShapeFileProvider(void);

public:
	// ´ò¿ª
	virtual BOOL Open(LPCTSTR lpszConnectionString = NULL);

public:
	virtual const KExtents& GetExtents();
};
