#pragma once

#include "KPalette.h"

class KIND_EXT_CLASS KLinearPalette : public KPalette
{
public:
	KLinearPalette(void);
	virtual ~KLinearPalette(void);

public:
	// ��¡
	virtual KLinearPalette* Clone();
	// ��ɫ
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);
};

