/******************************************************************************
 ** KPaletteItem.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPaletteItem
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

#include "KString.h"

class KIND_EXT_CLASS KPaletteItem : public VARGB
{
public:
	KPaletteItem(void);
	KPaletteItem(const KPaletteItem& item);
	KPaletteItem(const VARGB& vargb);
	KPaletteItem(const VARGB& vargb, LPCTSTR pszComment);
	KPaletteItem(float v, COLORREF color);
	KPaletteItem(float v, COLORREF color, LPCTSTR pszComment);
	KPaletteItem(float v, int r, int g, int b);
	KPaletteItem(float v, int r, int g, int b, LPCTSTR pszComment);
	KPaletteItem(float v, int a, int r, int g, int b);
	KPaletteItem(float v, int a, int r, int g, int b, LPCTSTR pszComment);

	virtual ~KPaletteItem(void);

public:
	// ×¢ÊÍ
	void SetComment(LPCTSTR pszComment) { KString::SetString(m_pszComment,pszComment); }
	LPCTSTR GetComment() { return m_pszComment; }

public:
	// ¿ËÂ¡
	KPaletteItem* Clone();

protected:
	LPTSTR m_pszComment;
};
