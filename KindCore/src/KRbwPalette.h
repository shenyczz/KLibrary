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
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
 **		//1.声明类对象
 **		KRbwPalette rbwPal;
 **
 **		//2.设置彩虹类型
 **		rbwPal.SetRainbowStyles(RainbowStyles eRainbowStyle);
 **
 **		//3.设置数据范围
 **		rbwPal.SetValueRange(vmin,vmax);
 **
 **		//3.根据数据值取得颜色
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
		GreenToRed,		// 绿色过度到红色

		Max = 19
	};

public:
	KRbwPalette(void);
	KRbwPalette(RainbowStyle eRainbowStyle);
	virtual ~KRbwPalette(void);

public:
	// 克隆
	virtual KPalette* Clone();
	// 颜色
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);

public:
	// 彩虹类型
	void SetRainbowStyle(RainbowStyle eStyle);
	RainbowStyle GetRainbowStyle(RainbowStyle eStyle) { return m_eRainbowStyle; }

private:
	RainbowStyle m_eRainbowStyle;

};
