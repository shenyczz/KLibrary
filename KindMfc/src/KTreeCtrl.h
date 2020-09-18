/******************************************************************************
 ** Header File: KTreeCtrl.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KTreeCtrl - ���ؼ�
 ** Version:
 ** Function:
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	//1.���������
	KTreeCtrl m_TreeCtrl;
	
	//2.��ʼ����(�����ڶԻ���� OnInitDialog() ������)
	//-----------------------------
	m_TreeCtrl.ModifyStyle(0,TVS_CHECKBOXES);
	m_TreeCtrl.ModifyStyle(0,TVS_LINESATROOT);
	//-----------------------------
	m_TreeCtrl.EnableColorFont();
	m_TreeCtrl.EnableMultiSelect();
	//-----------------------------
	//m_TreeCtrl.SetBkColor(RGB(0,0,0));
	//m_TreeCtrl.SetTextColor(RGB(192,192,192));
	//-----------------------------
	//m_TreeCtrl.SetBkImage(IDB_BITMAP1);
	CImageList ImageListSmall;
	ImageListSmall.Create(SFX_IDB_TREE_LAYER,16,8,RGB(192,192,192));
	m_TreeCtrl.SetImageList(&m_ImageListSmall,TVSIL_NORMAL);
	m_TreeCtrl.SetStateImage(SFX_IDB_TREE_STATE_EYE,RGB(255,0,255));
	//-----------------------------
	HTREEITEM htRoot,htChild[10];
	//-----------------------------
	//ROOT
	htRoot=m_sfxTree.InsertItem("Root",0,1,TVI_ROOT);
	m_TreeCtrl.SetItemColor(htRoot,RGB(255,0,0));
	m_TreeCtrl.SetItemBold(htRoot);
	//-----------------------------
	//Child_0
	htChild[0]=m_TreeCtrl.InsertItem("Child0",2,3,htRoot);
	m_TreeCtrl.SetItemColor(htChild[0],RGB(255,0,0));

	m_TreeCtrl.InsertItem("Child_01",4,5,htChild[0]);
	m_TreeCtrl.InsertItem("Child_02 �ܳ����ַ���... Very long string...",4,5,htChild[0]);
	//-----------------------------
	//Child_1
	htChild[1]=m_TreeCtrl.InsertItem("Child1",2,3,htRoot);
	m_TreeCtrl.SetItemColor(htChild[1],RGB(0,255,0));

	m_TreeCtrl.InsertItem("Child_11",4,5,htChild[1]);
	m_TreeCtrl.InsertItem("Child_12",4,5,htChild[1]);
	//-----------------------------
	//...
	//-----------------------------
	m_TreeCtrl.Expand(htRoot,TVE_EXPAND);
	//m_TreeCtrl.Expand(htChild[0],TVE_EXPAND);
	//m_TreeCtrl.Expand(htChild[1],TVE_EXPAND);
	//-----------------------------

******************************************************************************/
#pragma once

namespace Gui
{

	#define TREE_CTRL_CHECKBOX_TIMERID	300
	#define TREE_CTRL_LABEL_TIMERID		301

	// CheckBox������Ϣ
	// �����ÿؼ��Ĵ���Ҫ��Ӧ����Ϣ
	#define WM_KIND_TREE_CTRL_CHECKBOX	(WM_USER + TREE_CTRL_CHECKBOX_TIMERID)	// CheckBox ������Ϣ

	// Label ������Ϣ
	// �����ÿؼ��Ĵ���Ҫ��Ӧ����Ϣ
	#define WM_KIND_TREE_CTRL_LABEL		(WM_USER + TREE_CTRL_LABEL_TIMERID)		// Label ������Ϣ


	struct TreeCtrlChecked
	{
		CTreeCtrl*  pTreeCtrl;		//CTreeCtrl ָ��
		HTREEITEM   hTreeItem;		//item ���
		CPoint      point;			//�����λ��
		DWORD       dwItemData;		//CTreeCtrl �� Data
	};

	// KTreeCtrl

	class KIND_EXT_CLASS KTreeCtrl : public CTreeCtrl
	{
	public:
		struct ColorFont
		{
			COLORREF color;
			LOGFONT logfont; 
		};
		typedef CMap< void*, void*, ColorFont,ColorFont&> CMapColorFont ;

		DECLARE_DYNAMIC(KTreeCtrl)

	public:
		KTreeCtrl();
		virtual ~KTreeCtrl();

	protected:
		virtual void PreSubclassWindow();
		virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	protected:
		afx_msg void OnPaint();
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);
		afx_msg BOOL OnQueryNewPalette();
		afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
		afx_msg void OnNMClick(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnTimer(UINT_PTR nIDEvent);
		afx_msg void OnTvnItemexpanding(NMHDR *pNMHDR, LRESULT *pResult);
		afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
		afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);

		DECLARE_MESSAGE_MAP()

	public:
		// ���û�ȡӵ����
		void SetOwner(CWnd* pOwner) { m_pOwner = pOwner; }
		CWnd* GetOwner() { return m_pOwner; }
		// ����/��ȡ�Ƿ��ɫ�ı�
		void EnableColorFont(BOOL bColorFont = TRUE) { m_bColorFont = bColorFont; }
		BOOL IsColorFont() { return m_bColorFont; }

		// ����/��ȡ�Ƿ��ѡ
		void EnableMultiSelect(BOOL bMultiSelect=TRUE) {m_bMultiSelect=bMultiSelect;}
		BOOL IsEnableMultiSelect() {return m_bMultiSelect;}

		// ����/��ȡĳ��Ŀ�Ƿ����
		void EnableItemBold(HTREEITEM hItem, BOOL bBold = TRUE) { SetItemState(hItem,(bBold ? TVIS_BOLD : 0),TVIS_BOLD); }
		BOOL IsItemBold(HTREEITEM hItem) {return ( GetItemState(hItem,TVIS_BOLD) & TVIS_BOLD ) ? TRUE : FALSE; }

		// �˵�ID
		void SetMenuID(UINT uMenuID) { m_uMenuID = uMenuID; }
		UINT GetMenuID() { return m_uMenuID; }

	public:
		//���ñ���ͼ��
		BOOL SetBkImage(LPCTSTR lpszResourceName);
		BOOL SetBkImage(UINT nIDResource);

		//����/��ȡ item ��ɫ
		void SetItemColor(HTREEITEM hItem, COLORREF color);
		COLORREF GetItemColor(HTREEITEM hItem);

		//����/��ȡ item �߼�����
		void SetItemFont(HTREEITEM hItem, LOGFONT* pLogFont);
		BOOL GetItemFont(HTREEITEM hItem, LOGFONT* pLogFont);

		//����״̬ͼƬ
		void SetStateImage(UINT UnselectID,UINT SelectID,COLORREF clrMask);
		void SetStateImage(UINT nBitmapID,COLORREF clrMask);
		void SetStateImage(HBITMAP hBitmap,COLORREF clrMask);

		//�ı�item״̬(��ʱû�е���)
		void ChangeItemState();	

		//ȡ�õ�һ��ѡ�е� item �ľ��
		virtual HTREEITEM GetFirstSelectedItem();
		//ȡ����һ��ѡ�е� item �ľ��
		virtual HTREEITEM GetPrevSelectedItem(HTREEITEM hItem);
		//ȡ����һ��ѡ�е� item �ľ��
		virtual HTREEITEM GetNextSelectedItem(HTREEITEM hItem);
		//ȡ��ĩһ��ѡ�е� item �ľ��
		virtual HTREEITEM GetLastSelectedItem();

	public:
		//ȡ����һ�� item �ľ��
		virtual HTREEITEM GetPrevItem(HTREEITEM hItem);

		//ȡ����һ�� item �ľ��
		virtual HTREEITEM GetNextItem(HTREEITEM hItem);

		//ȡ�����һ���� item �ľ��
		virtual HTREEITEM GetLastItem(HTREEITEM hItem);

		//�����ı����� item,����������
		virtual HTREEITEM FindItem(LPCTSTR lpszSearch,
			BOOL bCaseSensitive = FALSE,
			BOOL bDownDir = TRUE,
			BOOL bWholeWord = FALSE,
			HTREEITEM hItemFrom=NULL);

		// ���ҵ��� item ��Ч
		virtual BOOL IsFindValid(HTREEITEM hti);

		//ȡ������ѡ��
		void ClearSelection();
		
		//ѡ��ĳһ��Χ�ڵ� item
		void SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo);

		//�������Checked��������������Ů������Ů��Checked,��֮��Ȼ
		void SetAllChildCheck(HTREEITEM hTreeItemParent,BOOL bCheck);

		// �������Checked�������丸���游��Checked
		void SetParentCheck(HTREEITEM hTreeItemChild);

		//��鱾��������Ƿ�Checked
		BOOL IsChildChecked(HTREEITEM hTreeItemParent);

	private:
		//���Ʋ�ɫ�ı�
		void DrawColorFont(CDC* pDC);
		//���Ʊ���ͼƬ
		void DrawBackImage(CDC* pDevDC,CDC* pMemDC);

	protected:
		BOOL		m_bColorFont;		// �Ƿ��ɫ�ı�
		BOOL		m_bMultiSelect;		// �Ƿ��ѡ

		CBitmap		m_bitmap;
		CPoint      m_point;
		CPalette	m_pal;
		CImageList	m_ImageListState;	//

		int			m_cxBitmap;			//����λͼ�Ŀ��
		int			m_cyBitmap;			//����λͼ�ĸ߶�

		CMapColorFont m_mapColorFont;
		UINT        m_uTimerID;
		HTREEITEM	m_hItemFirstSel;	// �ڶ�ѡ״̬��ѡ�еĵ�һ����Ŀ

		TreeCtrlChecked* m_pTreeCtrlChecked;

		CWnd* m_pOwner;

		UINT m_uMenuID;
	};

//---------------------------------------------------------
}// namespace Gui - end
//---------------------------------------------------------
