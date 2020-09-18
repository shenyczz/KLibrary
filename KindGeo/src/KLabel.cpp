#include "StdAfx.h"
#include "KLabel.h"

namespace Geometries
{

	KLabel::KLabel(void)
		:m_point(0,0)
		,m_wFontStyle(FontStyle::FontStyleRegular)
	{
		SetType(GeometryType_Label);
		this->SetFontName(_T("宋体"));

		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		::GetObject(hFont,sizeof(LOGFONT),&m_lf);
	}


	KLabel::~KLabel(void)
	{
	}

	// 字体大小
	void KLabel::SetFontPoint(double dFontPoint)
	{
		m_lf.lfHeight = -(LONG)(1.0 * 12.0 / 9 * 9);
	}
	double KLabel::GetFontPoint()
	{ 
		double dFontPoint = 9;
		dFontPoint = -1.0 * m_lf.lfHeight / 12 * 9;
		return dFontPoint;
	}

	// 字体风格
	void KLabel::SetFontStyle(WORD wFontStyle)
	{
		m_wFontStyle = wFontStyle;
	}
	WORD KLabel::GetFontStyle()
	{
		WORD wFontStyle = 0;

		if(m_lf.lfWeight > 400) wFontStyle |= 0x01;
		if(m_lf.lfItalic)		wFontStyle |= 0x02;
		if(m_lf.lfUnderline)	wFontStyle |= 0x04;
		if(m_lf.lfStrikeOut)	wFontStyle |= 0x08;

		return wFontStyle;
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
