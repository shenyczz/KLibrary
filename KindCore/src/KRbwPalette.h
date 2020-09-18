/******************************************************************************
 ** KRbwPalette.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KRbwPalette
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
 **		//1.���������
 **		KRbwPalette rbwPal;
 **
 **		//2.���òʺ�����
 **		rbwPal.SetRainbowStyles(RainbowStyles eRainbowStyle);
 **
 **		//3.�������ݷ�Χ
 **		rbwPal.SetValueRange(vmin,vmax);
 **
 **		//3.��������ֵȡ����ɫ
 **		rbwPal.GetColor(double dValue);
 **
******************************************************************************/

#pragma once

#include "KPalette.h"

class KIND_EXT_CLASS KRbwPalette : public KPalette
{
public:
	enum RainbowStyle : int
	{
		Default = 0,
		Rainbow,
		InverseRainbow,
		GrADS,
		GreenToRed,		// ��ɫ���ȵ���ɫ

		Max = 19
	};

public:
	KRbwPalette(void);
	KRbwPalette(RainbowStyle eRainbowStyle);
	virtual ~KRbwPalette(void);

public:
	// ��¡
	virtual KPalette* Clone();
	// ��ɫ
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);

public:
	// �ʺ�����
	void SetRainbowStyle(RainbowStyle eStyle);
	RainbowStyle GetRainbowStyle(RainbowStyle eStyle) { return m_eRainbowStyle; }

private:
	RainbowStyle m_eRainbowStyle;

};
