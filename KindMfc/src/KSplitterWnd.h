//////////////////////////////////////////////////////////////////////
// KSplitterWnd.h : header file
// 
// �������ƣ��ָ����
// �汾˵����
// �� �� �ţ�
// ��    �ܣ�
//
// �� �� �ˣ�������
// ����ʱ�䣺2003.3
// �� �� �ߣ�
// �޸�ʱ�䣺
// �޸�˵����
// ��    ����
// ��    �����ο� codeguru.com
//
// ��    ����
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

