/******************************************************************************
 ** KSegPalette.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KSegPalette
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
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

class KIND_EXT_CLASS KSegPalette : public KPalette
{
public:
	KSegPalette(void);
	virtual ~KSegPalette(void);

public:

public:
	// ��¡
	virtual KPalette* Clone();
	// ��ɫ
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);
};

