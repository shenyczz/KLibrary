#include "StdAfx.h"
#include "KPaletteItem.h"


KPaletteItem::KPaletteItem(void)
	:m_pszComment(0)
{
}

KPaletteItem::KPaletteItem(const KPaletteItem& item)
	:VARGB(item.V,item.A,item.R,item.G,item.B)
	,m_pszComment(0)
{
	KString::SetString(m_pszComment, item.m_pszComment);
}

KPaletteItem::KPaletteItem(const VARGB& vargb)
	:VARGB(vargb)
	,m_pszComment(0)
{
}

KPaletteItem::KPaletteItem(const VARGB& vargb, LPCTSTR pszComment)
	:VARGB(vargb)
	,m_pszComment(0)
{
	KString::SetString(m_pszComment,pszComment);
}

KPaletteItem::KPaletteItem(float v, COLORREF color)
	:VARGB(v,color)
	,m_pszComment(0)
{
}

KPaletteItem::KPaletteItem(float v, COLORREF color, LPCTSTR pszComment)
	:VARGB(v,color)
	,m_pszComment(0)
{
	KString::SetString(m_pszComment,pszComment);
}

KPaletteItem::KPaletteItem(float v, int r, int g, int b)
	:VARGB(v,r,g,b)
	,m_pszComment(0)
{
}

KPaletteItem::KPaletteItem(float v, int r, int g, int b, LPCTSTR pszComment)
	:VARGB(v,r,g,b)
	,m_pszComment(0)
{
	KString::SetString(m_pszComment,pszComment);
}

KPaletteItem::KPaletteItem(float v, int a, int r, int g, int b)
	:VARGB(v,a,r,g,b)
	,m_pszComment(0)
{
}

KPaletteItem::KPaletteItem(float v, int a, int r, int g, int b, LPCTSTR pszComment)
	:VARGB(v,a,r,g,b)
	,m_pszComment(0)
{
	KString::SetString(m_pszComment,pszComment);
}

KPaletteItem::~KPaletteItem(void)
{
	_deletea(m_pszComment);
}

// ¿ËÂ¡
KPaletteItem* KPaletteItem::Clone()
{
	return new KPaletteItem(*this);
}
