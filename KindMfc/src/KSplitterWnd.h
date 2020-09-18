//////////////////////////////////////////////////////////////////////
// KSplitterWnd.h : header file
// 
// 程序名称：分割窗口类
// 版本说明：
// 版 本 号：
// 功    能：
//
// 开 发 人：申永辰
// 开发时间：2003.3
// 修 改 者：
// 修改时间：
// 修改说明：
// 关    联：
// 其    他：参考 codeguru.com
//
// 用    法：
//	
//	ShowRow();
//	ShowColumn();
//	HideRow(int nRowHide);
//	HideColumn(int nColHide);
//
//	// Switch the View in certain SplitterWnd
//	SwitchView(int nRow, int nCol, CRuntimeClass *pNewView);
//
//	SetFlatStyle(BOOL bFlat=TRUE);
//
/////////////////////////////////////////////////////////////////////////////

#ifndef __INCLUDED_KSPLITTERWND_H__
#define __INCLUDED_KSPLITTERWND_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CX_BORDER   1
#define CY_BORDER   1

namespace Gui
{

	class KIND_EXT_CLASS KSplitterWnd : public CSplitterWnd
	{
	// Construction
	public:
		KSplitterWnd();

	// Attributes
	public:

	// Operations
	public:

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(KSplitterWnd)
		protected:
		virtual void OnDrawSplitter(CDC* pDC, ESplitType nType, const CRect& rect);
		//}}AFX_VIRTUAL

	// Implementation
	public:
		virtual ~KSplitterWnd();

		// Generated message map functions
	protected:
		//{{AFX_MSG(KSplitterWnd)
		afx_msg void OnPaint();
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	//---
	public:
		void HideRow(int nRowHide);
		void HideColumn(int nColHide);
		void ShowRow();
		void ShowColumn();
		void SwitchView(int nRow, int nCol, CRuntimeClass *pNewView);
		BOOL ReplaceView(int nRow, int nCol, CRuntimeClass *pViewClass);

		inline void SetFlatStyle(BOOL bFlat=TRUE){ m_bFlatSplitter = bFlat; }

	protected:
		int		m_nHiddenCol;		// Index of the hidden column.
		int		m_nHiddenRow;		// Index of the hidden row.
		BOOL	m_bFlatSplitter;	// true if the SPLS_FLATEDGE style was used during creation.
	};

//---------------------------------------------------------
}// namespace Gui - end
//---------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // #ifndef __INCLUDED_KSPLITTERWND_H__

