#pragma once

namespace Gui
{
}// namespace Gui - end

// KEditCtrl

namespace Gui
{

	class KIND_EXT_CLASS KEditCtrl : public CEdit
	{
		DECLARE_DYNAMIC(KEditCtrl)

	public:
		KEditCtrl();
		virtual ~KEditCtrl();

	protected:
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);

		DECLARE_MESSAGE_MAP()

	public:
		COLORREF GetBackColor()				{ return m_clrBack; }
		void SetBackColor(COLORREF clrBack)	{ m_clrBack = clrBack; }

		COLORREF GetForeColor()				{ return m_clrFore; }
		void SetForeColor(COLORREF clrFore)	{ m_clrFore = clrFore; }

	public:
		void AppendText(LPCTSTR lpszText, BOOL bLineScroll = FALSE);

	protected:
		COLORREF	m_clrBack;	//±³¾°ÑÕÉ«
		COLORREF	m_clrFore;	//Ç°¾°ÑÕÉ«
	};

}// namespace Gui - end

