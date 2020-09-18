// SfxSplitterWnd.cpp : implementation file
//

#include "stdafx.h"
#include "KSplitterWnd.h"
//---

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// KSplitterWnd
namespace Gui 
{

	KSplitterWnd::KSplitterWnd()
	{
		/*
		m_cxSplitter = m_cySplitter = 2;4;
		m_cxBorderShare = m_cyBorderShare = 1;
		m_cxSplitterGap = m_cySplitterGap = 2;4 + 1 + 1;
		m_cxBorder = m_cyBorder = 1;
		*/

		m_bFlatSplitter = FALSE;
		m_nHiddenCol	= -1;
		m_nHiddenRow	= -1;

	}

	KSplitterWnd::~KSplitterWnd()
	{
	}


	BEGIN_MESSAGE_MAP(KSplitterWnd, CSplitterWnd)
		//{{AFX_MSG_MAP(KSplitterWnd)
		ON_WM_PAINT()
		//}}AFX_MSG_MAP
	END_MESSAGE_MAP()


	/////////////////////////////////////////////////////////////////////////////
	// KSplitterWnd message handlers

	void KSplitterWnd::OnPaint() 
	{
		// TODO: Add your message handler code here
		if( m_bFlatSplitter )
		{
			ASSERT_VALID(this);
			CPaintDC dc(this);
			
			CRect rectClient;
			GetClientRect(&rectClient);
			
			CRect rectInside;
			GetInsideRect(rectInside);
			rectInside.InflateRect(4,4);
			
			// draw the splitter boxes
			if (m_bHasVScroll && m_nRows < m_nMaxRows)
			{
				OnDrawSplitter(&dc, splitBox,
					CRect(rectInside.right, rectClient.top,
					rectClient.right, rectClient.top + m_cySplitter));
			}
			
			if (m_bHasHScroll && m_nCols < m_nMaxCols)
			{
				OnDrawSplitter(&dc, splitBox,
					CRect(rectClient.left, rectInside.bottom,
					rectClient.left + m_cxSplitter, rectClient.bottom));
			}
			
			// extend split bars to window border (past margins)
			DrawAllSplitBars(&dc, rectInside.right, rectInside.bottom);
			
			// draw splitter intersections (inside only)
			GetInsideRect(rectInside);
			dc.IntersectClipRect(rectInside);
			CRect rect;
			rect.top = rectInside.top;
			for (int row = 0; row < m_nRows - 1; row++)
			{
				rect.top += m_pRowInfo[row].nCurSize + m_cyBorderShare;
				rect.bottom = rect.top + m_cySplitter;
				rect.left = rectInside.left;
				for (int col = 0; col < m_nCols - 1; col++)
				{
					rect.left += m_pColInfo[col].nCurSize + m_cxBorderShare;
					rect.right = rect.left + m_cxSplitter;
					OnDrawSplitter(&dc, splitIntersection, rect);
					rect.left = rect.right + m_cxBorderShare;
				}
				rect.top = rect.bottom + m_cxBorderShare;
			}
		}
		else {
			CSplitterWnd::OnPaint();
		}
	}

	void KSplitterWnd::OnDrawSplitter(CDC* pDC, ESplitType nType,const CRect& rectArg)
	{
		if( m_bFlatSplitter )
		{
			// if pDC == NULL, then just invalidate
			if (pDC == NULL)
			{
				RedrawWindow(rectArg, NULL, RDW_INVALIDATE|RDW_NOCHILDREN);
				return;
			}
			ASSERT_VALID(pDC);

			// otherwise, actually draw
			CRect rect = rectArg;
			bool bWin4 = (BYTE)::GetVersion() >= 4;
			COLORREF clrBtnFace			= ::GetSysColor(COLOR_BTNFACE);
			COLORREF clrBtnShadow		= ::GetSysColor(COLOR_BTNSHADOW);
			COLORREF clrBtnHilite		= ::GetSysColor(COLOR_BTNHIGHLIGHT);
			COLORREF clrBtnText			= ::GetSysColor(COLOR_BTNTEXT);
			COLORREF clrWindowFrame		= ::GetSysColor(COLOR_WINDOWFRAME);
			switch (nType)
			{
			case splitBorder:
				ASSERT(bWin4);
				pDC->Draw3dRect(rect, clrBtnFace, clrBtnFace);
				rect.InflateRect(-CX_BORDER, -CY_BORDER);
				pDC->Draw3dRect(rect, clrBtnShadow, clrBtnHilite);
				return;

			case splitIntersection:
				ASSERT(!bWin4);
				break;

			case splitBox:
				if (bWin4)
				{
					pDC->Draw3dRect(rect, clrBtnFace, clrWindowFrame);
					rect.InflateRect(-CX_BORDER, -CY_BORDER);
					pDC->Draw3dRect(rect, clrBtnHilite, clrBtnShadow);
					rect.InflateRect(-CX_BORDER, -CY_BORDER);
					break;
				}
				// fall through...
			case splitBar:
				if (!bWin4)
				{
					pDC->Draw3dRect(rect, clrBtnHilite, clrBtnShadow);
					rect.InflateRect(-CX_BORDER, -CY_BORDER);
				}
				break;

			default:
				ASSERT(FALSE);  // unknown splitter type
			}

			// fill the middle
			pDC->FillSolidRect(rect, clrBtnFace);
		}

		else 
		{
			CSplitterWnd::OnDrawSplitter(pDC, nType, rectArg);
		}
	}

	void KSplitterWnd::HideColumn(int nColHide)
	{
		ASSERT_VALID( this );
		ASSERT( m_nCols > 1 );
		ASSERT( nColHide < m_nCols );
		ASSERT( m_nHiddenCol == -1 );
		m_nHiddenCol = nColHide;
		
		// if the column has an active window -- change it
		int nActiveRow, nActiveCol;
		if( GetActivePane( &nActiveRow, &nActiveCol ) != NULL )
		{
			if(	nActiveCol == nColHide )
			{
				if( ++nActiveCol >= m_nCols )
					nActiveCol = 0;
				SetActivePane( nActiveRow, nActiveCol );
			}
		}

		// hide all column panes
		for( int nRow = 0; nRow < m_nRows; nRow++)
		{
			CWnd* pPaneHide = GetPane(nRow, nColHide);
			ASSERT( pPaneHide != NULL );

			pPaneHide->ShowWindow(SW_HIDE);
			pPaneHide->SetDlgCtrlID( AFX_IDW_PANE_FIRST+m_nCols + 16*nRow );
			
			for( int nCol = nColHide + 1; nCol < m_nCols; nCol++ )
			{
				CWnd* pPane = GetPane( nRow, nCol );
				ASSERT( pPane != NULL );

				pPane->SetDlgCtrlID( IdFromRowCol( nRow, nCol - 1 ));
			}
		}

		m_nCols--;
		m_pColInfo[m_nCols].nCurSize = m_pColInfo[nColHide].nCurSize;
		RecalcLayout();
	}

	void KSplitterWnd::HideRow(int nRowHide)
	{
		ASSERT_VALID( this );
		ASSERT( m_nRows > 1 );
		ASSERT( nRowHide < m_nRows );
		ASSERT( m_nHiddenRow == -1 );
		m_nHiddenRow = nRowHide;
		
		int nActiveRow, nActiveCol;

		// if the nRow has an active window -- change it
		if( GetActivePane( &nActiveRow, &nActiveCol ) != NULL )
		{
			if( nActiveRow == nRowHide )
			{
				if( ++nActiveRow >= m_nRows )
					nActiveRow = 0;
				SetActivePane( nActiveRow, nActiveCol );
			}
		}

		// hide all nRow panes.
		for( int nCol = 0; nCol < m_nCols; ++nCol )
		{
			CWnd* pPaneHide = GetPane( nRowHide, nCol );
			ASSERT( pPaneHide != NULL );

			pPaneHide->ShowWindow( SW_HIDE );
			pPaneHide->SetDlgCtrlID( AFX_IDW_PANE_FIRST+nCol + 16*m_nRows );
			
			for( int nRow = nRowHide+1; nRow < m_nRows; ++nRow )
			{
				CWnd* pPane = GetPane( nRow, nCol );
				ASSERT( pPane != NULL );

				pPane->SetDlgCtrlID( IdFromRowCol( nRow-1, nCol ));
			}
		}

		m_nRows--;
		m_pRowInfo[m_nRows].nCurSize = m_pRowInfo[nRowHide].nCurSize;
		RecalcLayout();
	}

	BOOL KSplitterWnd::ReplaceView(int nRow, int nCol, CRuntimeClass *pViewClass)
	{
		if(GetRowCount()==1 && GetColumnCount()==1)
			GetActivePane(&nRow,&nCol);

		CView *pView = DYNAMIC_DOWNCAST( CView, GetPane( nRow, nCol ));
		ASSERT_KINDOF( CView, pView );

		if( pView->IsKindOf( pViewClass ))
			return FALSE;

		// Get pointer to CDocument object so that it can be used in the creation
		// process of the new view
		CDocument *pDoc = pView->GetDocument();

		int nID = pView->GetDlgCtrlID();
		int nWidth, nHeight, nMinWidth, nMinHeight;

		GetRowInfo( nRow, nWidth, nMinWidth );
		GetColumnInfo( nCol, nHeight, nMinHeight );

		// Delete existing view
		DeleteView(nRow, nCol);

		// Create new view
		CCreateContext context;
		context.m_pCurrentDoc = pDoc;
		CreateView( nRow, nCol, pViewClass, CSize( nWidth, nHeight ), &context );
		pView = DYNAMIC_DOWNCAST( CView, GetPane( nRow, nCol ));
		ASSERT_KINDOF( CView, pView );

		pView->SetDlgCtrlID( nID );

		SetRowInfo( nRow, nWidth, nMinWidth );
		SetColumnInfo( nCol, nHeight, nMinHeight );
		RecalcLayout();

		return TRUE;
	}

	// ÇÐ»»ÊÔÍ¼
	void KSplitterWnd::SwitchView(int nRow, int nCol, CRuntimeClass *pNewView)
	{
		if(GetRowCount()==1 && GetColumnCount()==1)
			GetActivePane(&nRow,&nCol);
		CRect rect;	
		GetPane(nRow,nCol)->GetClientRect(&rect);

		POSITION pos=AfxGetApp()->GetFirstDocTemplatePosition();
		CDocTemplate* pDocTemplate=AfxGetApp()->GetNextDocTemplate(pos);

		pos=pDocTemplate->GetFirstDocPosition();
		CDocument* pDoc=pDocTemplate->GetNextDoc(pos);
		//---
		CCreateContext context;
		context.m_pCurrentDoc=pDoc;
		context.m_pCurrentFrame=(CFrameWnd*)AfxGetApp()->m_pMainWnd;
		context.m_pNewViewClass=pNewView;
		context.m_pNewDocTemplate=context.m_pCurrentDoc->GetDocTemplate();
		context.m_pLastView=(CView*)GetPane(nRow,nCol);
		//---
		DeleteView(nRow,nCol);
		CreateView(nRow,nCol,pNewView,CSize(rect.Width(),rect.Height()),&context);
		CView* pView=(CView*)GetPane(nRow,nCol);
		pView->GetParentFrame()->RecalcLayout();
		RecalcLayout();
		pView->OnInitialUpdate();
		SetActivePane(nRow,nCol);
	}

	void KSplitterWnd::ShowColumn()
	{
		ASSERT_VALID( this );
		ASSERT( m_nCols < m_nMaxCols );
		ASSERT( m_nHiddenCol != -1 );

		int nShowCol = m_nHiddenCol;
		m_nHiddenCol = -1;

		int cxNew = m_pColInfo[m_nCols].nCurSize;
		m_nCols++;  // add a column
		
		ASSERT( m_nCols == m_nMaxCols );
		
		int nCol;

		// Show the hidden column
		for( int nRow = 0; nRow < m_nRows; ++nRow )
		{
			CWnd* pPaneShow = GetDlgItem( AFX_IDW_PANE_FIRST+m_nCols + 16*nRow );
			ASSERT( pPaneShow != NULL );
			pPaneShow->ShowWindow( SW_SHOWNA );

			for( nCol = m_nCols - 2; nCol >= nShowCol; --nCol )
			{
				CWnd* pPane = GetPane( nRow, nCol );
				ASSERT( pPane != NULL );
				pPane->SetDlgCtrlID( IdFromRowCol( nRow, nCol + 1 ));
			}

			pPaneShow->SetDlgCtrlID( IdFromRowCol( nRow, nShowCol ));
		}

		// new panes have been created -- recalculate layout
		for( nCol = nShowCol+1; nCol < m_nCols; nCol++ )
			m_pColInfo[nCol].nIdealSize = m_pColInfo[nCol - 1].nCurSize;

		m_pColInfo[nShowCol].nIdealSize = cxNew;
		RecalcLayout();
	}

	void KSplitterWnd::ShowRow()
	{
		ASSERT_VALID( this );
		ASSERT( m_nRows < m_nMaxRows );
		ASSERT( m_nHiddenRow != -1 );

		int nShowRow = m_nHiddenRow;
		m_nHiddenRow = -1;

		int cyNew = m_pRowInfo[m_nRows].nCurSize;
		m_nRows++;  // add a nRow
		
		ASSERT( m_nRows == m_nMaxRows );
		
		int nRow;

		// Show the hidden nRow
		for( int nCol = 0; nCol < m_nCols; ++nCol )
		{
			CWnd* pPaneShow = GetDlgItem( AFX_IDW_PANE_FIRST+nCol + 16*m_nRows );
			ASSERT( pPaneShow != NULL );
			pPaneShow->ShowWindow( SW_SHOWNA );

			for( nRow = m_nRows - 2; nRow >= nShowRow; --nRow )
			{
				CWnd* pPane = GetPane( nRow, nCol );
				ASSERT( pPane != NULL );
				pPane->SetDlgCtrlID( IdFromRowCol( nRow + 1, nCol ));
			}

			pPaneShow->SetDlgCtrlID( IdFromRowCol( nShowRow, nCol ));
		}

		// new panes have been created -- recalculate layout
		for( nRow = nShowRow+1; nRow < m_nRows; nRow++ )
			m_pRowInfo[nRow].nIdealSize = m_pRowInfo[nRow - 1].nCurSize;

		m_pRowInfo[nShowRow].nIdealSize = cyNew;
		RecalcLayout();
	}

//---------------------------------------------------------
}// namespace Gui - end
//---------------------------------------------------------
