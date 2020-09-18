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

#include "KPoint.h"

namespace Geometries
{

	class KIND_EXT_CLASS KLabel : public KGeometry
	{
	public:
		KLabel(void);
		virtual ~KLabel(void);

	public:
		// ��ע�ı�λ��(��������)
		KPoint& Location() { return m_point; }

		// ��ע�ı�
		LPCTSTR GetText() { return m_strText; }
		void SetText(LPCTSTR pszText) { m_strText = pszText; }

		// ��������
		LPCTSTR GetFontName() { return m_lf.lfFaceName; }
		void SetFontName(LPCTSTR pszFontName) { _tcscpy(m_lf.lfFaceName,pszFontName); }

		// ������ɫ
		void SetFontColor(COLORREF clr) { m_FontColor = clr; }
		COLORREF GetFontColor() { return m_FontColor; }

		// �߼�����
		LOGFONT GetLogFont() { return m_lf; }
		void SetLogFont(LOGFONT lf) { m_lf = lf; }

	public:
		// �����С
		double GetFontPoint();
		void SetFontPoint(double dFontPoint);

		// ������
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

