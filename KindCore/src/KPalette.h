/******************************************************************************
 ** KPalette.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPalette
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
******************************************************************************/

#pragma once

#include "KObject.h"
#include "KPaletteItem.h"

enum PaletteType : int
{
	PaletteType_Unknown		= 0,	// 未知
	PaletteType_Standard	= 1,	// 标准调色板
	PaletteType_Index		= 2,	// 索引调色板
	PaletteType_Rainbow		= 3,	// 彩虹调色板
	PaletteType_Segment		= 4,	// 分段调色板
	PaletteType_Linear		= 5,	// 线性调色板
};

class KIND_EXT_CLASS KPalette : public KObject, public TPtrCollection<KPaletteItem*>
{
public:
	KPalette(void);
	virtual ~KPalette(void);

public:
	// 克隆
	virtual KPalette* Clone();
	// 颜色
	virtual COLORREF GetColor(double dValue,COLORREF clrDefault = 0);

public:
	// 默认色
	COLORREF& DefaultColor() { return m_clrDefault; }
	// 透明色
	COLORREF& TransparentColor() { return m_clrTransparent; }
	// 是否有注释
	BOOL& HasComment() { return m_bHasComment; }
	// 是否有透明色
	BOOL& HasTransparentColor() { return m_bHasTransparentColor; }
	// 取得调色板颜色入口数量
	int GetColorEntries() { return this->Count(); }
	// 特定颜色数量
	const int& SpecificColorNumbers() { return this->m_iSpecificColorNumbers; }
	// 特定数据最小值
	DWORD& SpecificDataMin()	{ return this->m_dwSpecificDataMin; }
	// 特定数据最大值
	DWORD& SpecificDataMax()	{ return this->m_dwSpecificDataMax; }

public:
	// 添加颜色
	virtual BOOL AddColor(double dValue, COLORREF color, LPCTSTR pszComment = _T(""));
	virtual BOOL AddColor(double dValue, BYTE r, BYTE g, BYTE b, LPCTSTR pszComment = _T(""));
	virtual BOOL AddColor(double dValue, BYTE a, BYTE r, BYTE g, BYTE b, LPCTSTR pszComment = _T(""));
	virtual BOOL AddColor(const VARGB& vargb, LPCTSTR pszComment);

public:
	// 指定值的索引
	int FindIndex(double dValue);
	// 指定值的最临近索引
	int FindNearestIndex(double dValue);
	// 有效条目数量
	int NumbersOfValidItem(double spv_min = 0x8000);

public:
	// 数据范围(用于线性梯度调色板)
	void SetValueRange(double vMin,double vMax);
	void GetValueRange(double& vMin,double& vMax);

protected:
	// 取得索引颜色
	COLORREF GetIdxColor(double dValue,COLORREF clrDefault);
	// 取得分段颜色
	COLORREF GetSegColor(double dValue,COLORREF clrDefault);
	// 取得抖动颜色
	COLORREF GetDitColor(double dValue,COLORREF clrDefault);

	// 根据 value 的位置找出在颜色表中对应的颜色,
	COLORREF LookupColor(double value,COLORREF clrDefault);

protected:
	// 有效条目数量(分段调色板属性)
	int m_iValidItemNumbers;
	// 默认色
	COLORREF m_clrDefault;
	// 透明色
	COLORREF m_clrTransparent;
	// 是否有注释
	BOOL m_bHasComment;
	// 是否有透明色
	BOOL m_bHasTransparentColor;
	// 特定颜色数量
	int m_iSpecificColorNumbers;
	// 特定数据最小值
	DWORD m_dwSpecificDataMin;
	// 特定数据最大值
	DWORD m_dwSpecificDataMax;

protected:
	// 用于彩虹调色板
	double m_dValueMin;
	double m_dValueMax;
};
