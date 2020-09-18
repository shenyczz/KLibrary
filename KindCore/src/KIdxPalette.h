/******************************************************************************
 ** KIdxPalette.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KIdxPalette
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

#include "KPalette.h"

class KIND_EXT_CLASS KIdxPalette : public KPalette
{
public:
	KIdxPalette(void);
	virtual ~KIdxPalette(void);

public:
	// ¿ËÂ¡
	virtual KPalette* Clone();
	// ÑÕÉ«
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);
};

