/******************************************************************************
 ** Header File: KTreeCtrl.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KTreeCtrl - 树控件
 ** Version:
 ** Function:
 ** Explain:

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	//1.声明类对象
	KTreeCtrl m_TreeCtrl;
	
	//2.初始化树(可以在对话框的 OnInitDialog() 函数中)
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
	m_TreeCtrl.InsertItem("Child_02 很长的字符串... Very long string...",4,5,htChild[0]);
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

	// CheckBox单击消息
	// 包含该控件的窗体要响应本消息
	#define WM_KIND_TREE_CTRL_CHECKBOX	(WM_USER + TREE_CTRL_CHECKBOX_TIMERID)	// CheckBox 单击消息

	// Label 单击消息
	// 包含该控件的窗体要响应本消息
	#define WM_KIND_TREE_CTRL_LABEL		(WM_USER + TREE_CTRL_LABEL_TIMERID)		// Label 单击消息


	struct TreeCtrlChecked
	{
		CTreeCtrl*  pTreeCtrl;		//CTreeCtrl 指针
		HTREEITEM   hTreeItem;		//item 句柄
		CPoint      point;			//鼠标点击位置
		DWORD       dwItemData;		//CTreeCtrl 的 Data
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
		// 设置获取拥有者
		void SetOwner(CWnd* pOwner) { m_pOwner = pOwner; }
		CWnd* GetOwner() { return m_pOwner; }
		// 设置/获取是否彩色文本
		void EnableColorFont(BOOL bColorFont = TRUE) { m_bColorFont = bColorFont; }
		BOOL IsColorFont() { return m_bColorFont; }

		// 设置/获取是否多选
		void EnableMultiSelect(BOOL bMultiSelect=TRUE) {m_bMultiSelect=bMultiSelect;}
		BOOL IsEnableMultiSelect() {return m_bMultiSelect;}

		// 设置/获取某条目是否粗体
		void EnableItemBold(HTREEITEM hItem, BOOL bBold = TRUE) { SetItemState(hItem,(bBold ? TVIS_BOLD : 0),TVIS_BOLD); }
		BOOL IsItemBold(HTREEITEM hItem) {return ( GetItemState(hItem,TVIS_BOLD) & TVIS_BOLD ) ? TRUE : FALSE; }

		// 菜单ID
		void SetMenuID(UINT uMenuID) { m_uMenuID = uMenuID; }
		UINT GetMenuID() { return m_uMenuID; }

	public:
		//设置背景图像
		BOOL SetBkImage(LPCTSTR lpszResourceName);
		BOOL SetBkImage(UINT nIDResource);

		//设置/获取 item 颜色
		void SetItemColor(HTREEITEM hItem, COLORREF color);
		COLORREF GetItemColor(HTREEITEM hItem);

		//设置/获取 item 逻辑字体
		void SetItemFont(HTREEITEM hItem, LOGFONT* pLogFont);
		BOOL GetItemFont(HTREEITEM hItem, LOGFONT* pLogFont);

		//设置状态图片
		void SetStateImage(UINT UnselectID,UINT SelectID,COLORREF clrMask);
		void SetStateImage(UINT nBitmapID,COLORREF clrMask);
		void SetStateImage(HBITMAP hBitmap,COLORREF clrMask);

		//改变item状态(暂时没有调用)
		void ChangeItemState();	

		//取得第一个选中的 item 的句柄
		virtual HTREEITEM GetFirstSelectedItem();
		//取得上一个选中的 item 的句柄
		virtual HTREEITEM GetPrevSelectedItem(HTREEITEM hItem);
		//取得下一个选中的 item 的句柄
		virtual HTREEITEM GetNextSelectedItem(HTREEITEM hItem);
		//取得末一个选中的 item 的句柄
		virtual HTREEITEM GetLastSelectedItem();

	public:
		//取得上一个 item 的句柄
		virtual HTREEITEM GetPrevItem(HTREEITEM hItem);

		//取得下一个 item 的句柄
		virtual HTREEITEM GetNextItem(HTREEITEM hItem);

		//取得最后一个的 item 的句柄
		virtual HTREEITEM GetLastItem(HTREEITEM hItem);

		//根据文本查找 item,并返回其句柄
		virtual HTREEITEM FindItem(LPCTSTR lpszSearch,
			BOOL bCaseSensitive = FALSE,
			BOOL bDownDir = TRUE,
			BOOL bWholeWord = FALSE,
			HTREEITEM hItemFrom=NULL);

		// 查找到的 item 有效
		virtual BOOL IsFindValid(HTREEITEM hti);

		//取消所有选择
		void ClearSelection();
		
		//选择某一范围内的 item
		void SelectItems(HTREEITEM hItemFrom, HTREEITEM hItemTo);

		//如果本层Checked则设置其所有子女、孙子女等Checked,反之依然
		void SetAllChildCheck(HTREEITEM hTreeItemParent,BOOL bCheck);

		// 如果本层Checked则设置其父、祖父等Checked
		void SetParentCheck(HTREEITEM hTreeItemChild);

		//检查本层的子树是否Checked
		BOOL IsChildChecked(HTREEITEM hTreeItemParent);

	private:
		//绘制彩色文本
		void DrawColorFont(CDC* pDC);
		//绘制背景图片
		void DrawBackImage(CDC* pDevDC,CDC* pMemDC);

	protected:
		BOOL		m_bColorFont;		// 是否彩色文本
		BOOL		m_bMultiSelect;		// 是否多选

		CBitmap		m_bitmap;
		CPoint      m_point;
		CPalette	m_pal;
		CImageList	m_ImageListState;	//

		int			m_cxBitmap;			//背景位图的宽度
		int			m_cyBitmap;			//背景位图的高度

		CMapColorFont m_mapColorFont;
		UINT        m_uTimerID;
		HTREEITEM	m_hItemFirstSel;	// 在多选状态下选中的第一个条目

		TreeCtrlChecked* m_pTreeCtrlChecked;

		CWnd* m_pOwner;

		UINT m_uMenuID;
	};

//---------------------------------------------------------
}// namespace Gui - end
//---------------------------------------------------------
