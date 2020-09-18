#pragma once

#include "KPalette.h"

class KIND_EXT_CLASS KLinearPalette : public KPalette
{
public:
	KLinearPalette(void);
	virtual ~KLinearPalette(void);

public:
	// ¿ËÂ¡
	virtual KLinearPalette* Clone();
	// ÑÕÉ«
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);
};

