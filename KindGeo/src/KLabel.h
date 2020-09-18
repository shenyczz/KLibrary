/******************************************************************************
 ** KLabel.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLabel
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

#include "KPoint.h"

namespace Geometries
{

	class KIND_EXT_CLASS KLabel : public KGeometry
	{
	public:
		KLabel(void);
		virtual ~KLabel(void);

	public:
		// 标注文本位置(世界坐标)
		KPoint& Location() { return m_point; }

		// 标注文本
		LPCTSTR GetText() { return m_strText; }
		void SetText(LPCTSTR pszText) { m_strText = pszText; }

		// 字体名称
		LPCTSTR GetFontName() { return m_lf.lfFaceName; }
		void SetFontName(LPCTSTR pszFontName) { _tcscpy(m_lf.lfFaceName,pszFontName); }

		// 字体颜色
		void SetFontColor(COLORREF clr) { m_FontColor = clr; }
		COLORREF GetFontColor() { return m_FontColor; }

		// 逻辑字体
		LOGFONT GetLogFont() { return m_lf; }
		void SetLogFont(LOGFONT lf) { m_lf = lf; }

	public:
		// 字体大小
		double GetFontPoint();
		void SetFontPoint(double dFontPoint);

		// 字体风格
		WORD GetFontStyle();
		void SetFontStyle(WORD wFontStyle);

	protected:
		KPoint m_point;
		KString m_strText;
		LOGFONT m_lf;
		WORD m_wFontStyle;
		COLORREF m_FontColor;
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------

