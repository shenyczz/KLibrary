// KLayerDockablePane.cpp : 实现文件
//

#include "stdafx.h"
#include "KLayerDockablePane.h"

#include "KGisDocumentBase.h"
#include "KGisViewBase.h"

#include "KLayerProperty.h"

namespace DockablePanes
{

	// KLayerDockablePane

	IMPLEMENT_DYNAMIC(KLayerDockablePane, KDockablePane)

	KLayerDockablePane::KLayerDockablePane()
	{
		m_pTreeCtrl = new KTreeCtrl();

		m_hTreeRoot = NULL;
		for(int i=0;i<50;i++)
		{
			m_hTreeChild[i] = NULL;
		}

		this->SetMenuID(KIDR_MENU_LAYER);
	}

	KLayerDockablePane::~KLayerDockablePane()
	{
		_delete(m_pTreeCtrl);
	}


	BEGIN_MESSAGE_MAP(KLayerDockablePane, KDockablePane)
		//-------------------------------------------------
		// 来自 KTreeCtrl 的消息
		// WM_KIND_TREE_CTRL_CHECKBOX
		ON_MESSAGE(WM_KIND_TREE_CTRL_CHECKBOX,OnTreeCtrlCheckBox)
		// WM_KIND_TREE_CTRL_LABEL
		ON_MESSAGE(WM_KIND_TREE_CTRL_LABEL,OnTreeCtrlLabel)
		//-------------------------------------------------
		// 刷新图层窗口消息 WM_USER_REFRESH_LAYER_WND
		ON_MESSAGE(WM_USER_REFRESH_LAYER_WND,OnRefreshLayerWnd)
		//-------------------------------------------------
		// 图层菜单
		ON_COMMAND_RANGE(KID_LAYER_BEG,KID_LAYER_END,OnLayerOperate)
		ON_UPDATE_COMMAND_UI_RANGE(KID_LAYER_BEG,KID_LAYER_END,OnUpdateLayerOperate)
		//-------------------------------------------------
	END_MESSAGE_MAP()



	// KLayerDockablePane 消息处理程序

	// 来自 KTreeCtrl WM_KIND_TREE_CTRL_CHECKBOX 消息
	sfx_msg LRESULT KLayerDockablePane::OnTreeCtrlCheckBox(WPARAM wParam,LPARAM lParam)
	{
		TreeCtrlChecked* pTreeCtrlChecked = (TreeCtrlChecked*)lParam;

		CPoint      point       = pTreeCtrlChecked->point;
		DWORD       dwItemData  = pTreeCtrlChecked->dwItemData;
		HTREEITEM	hTreeItem	= pTreeCtrlChecked->hTreeItem;
		CTreeCtrl*	pTreeCtrl   = pTreeCtrlChecked->pTreeCtrl;

		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pView = pFrame->GetActiveView();
		if(!pView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return -1L;

		KGisDocumentBase* pDoc = (KGisDocumentBase*)((KGisViewBase*)pView)->GetDocument();
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return -1L;

		pMapEngine->EnableRedraw();

		HTREEITEM hRoot		= pTreeCtrl->GetRootItem();
		HTREEITEM hCurrent	= hRoot;
		while(hCurrent)
		{
			if(hCurrent == hRoot)
			{
				hCurrent = pTreeCtrl->GetNextItem(hCurrent,TVGN_CHILD);
			}
			else
			{
				KLayer* pLayer=(KLayer*)pTreeCtrl->GetItemData(hCurrent);
				pLayer->GetRender()->GetLayerProperty()->EnableVisible(pTreeCtrl->GetCheck(hCurrent)==TRUE);
				hCurrent = pTreeCtrl->GetNextSiblingItem(hCurrent);
			}
		}

		pFrame->SetActiveView(pView);
		pView->Invalidate(false);

		return 0L;
	}
	// 来自 KTreeCtrl WM_KIND_TREE_CTRL_LABEL 消息
	// 当在树中点击标签时(选择图层),刷新对应的属性窗口
	sfx_msg  LRESULT KLayerDockablePane::OnTreeCtrlLabel(WPARAM wParam,LPARAM lParam)
	{
		TreeCtrlChecked* pTreeCtrlChecked = (TreeCtrlChecked*)lParam;

		CPoint      point       = pTreeCtrlChecked->point;
		DWORD       dwItemData  = pTreeCtrlChecked->dwItemData;
		HTREEITEM	hTreeItem	= pTreeCtrlChecked->hTreeItem;
		CTreeCtrl*	pTreeCtrl   = pTreeCtrlChecked->pTreeCtrl;

		KLayer* pLayer=(KLayer*)pTreeCtrl->GetItemData(hTreeItem);

		// pLayer == NULL 时清除属性窗口
		//if(pLayer == NULL)
		//	return -1L;

		// 取得属性窗口
		CWnd* pWndProperty = NULL;
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndProperty = ((KFrameWndEx*)pFrame)->GetPropertyWnd();
		}
		if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			pWndProperty = ((KMDIFrameWndEx*)pFrame)->GetPropertyWnd();
		}

		if(pWndProperty->GetSafeHwnd())
		{
			pWndProperty->PostMessage(WM_USER_REFRESH_PROPERTY_WND,0,(LPARAM)pLayer);
		}

		return 0L;
	}

	// 用户向图层窗口发送 WM_USER_REFRESH_LAYER_WND 来刷新图层窗口
	sfx_msg LRESULT KLayerDockablePane::OnRefreshLayerWnd(WPARAM wParam,LPARAM lParam)
	{
		CView* pView = (CView*)lParam;
		if(!pView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
		{
			// 清空图层窗口
			this->Refresh(NULL);
		}
		else
		{
			this->Refresh(pView,((BOOL)wParam) ? true : false);
		}

		return 0;
	}

	// 设置快捷菜单
	void KLayerDockablePane::SetMenuID(UINT uMenuID)
	{
		m_pTreeCtrl->SetMenuID(uMenuID);

		//CWinApp* pWinApp = AfxGetApp();
		//if(pWinApp->IsKindOf(RUNTIME_CLASS(CWinAppEx)))
		//{
		//	BOOL bNameValid;
		//	CString strName;
		//	bNameValid = strName.LoadString(KIDS_MENU_LAYER);
		//	ASSERT(bNameValid);

		//	CContextMenuManager* pContextMenuManager = ((CWinAppEx*)pWinApp)->GetContextMenuManager();
		//	pContextMenuManager->AddMenu(strName, KIDR_MENU_LAYER);
		//	return;
		//}
	}

	// 取得选中的图层
	KLayer* KLayerDockablePane::GetSelectedLayer()
	{
		KTreeCtrl* pTreeCtrl = m_pTreeCtrl;
		if(!pTreeCtrl || !pTreeCtrl->GetSafeHwnd())
			return NULL;

		HTREEITEM hTreeItem = NULL;

		// 取得选中的树条目
		hTreeItem = pTreeCtrl->GetSelectedItem();
		if(!hTreeItem)
			return NULL;

		// 取得条目对应的图层对象
		KLayer* pLayer=(KLayer*)pTreeCtrl->GetItemData(hTreeItem);
		if(!pLayer)
			return NULL;

		return pLayer;
	}
	// 选中图层
	void KLayerDockablePane::SelectLayer(KLayer* pLayer)
	{
		KTreeCtrl* pTreeCtrl = m_pTreeCtrl;
		if(!pTreeCtrl || !pTreeCtrl->GetSafeHwnd())
			return;

		for (HTREEITEM hItem= pTreeCtrl->GetRootItem(); hItem!=NULL; hItem = pTreeCtrl->GetNextItem(hItem))
		{
			KLayer* pLayerTemp=(KLayer*)pTreeCtrl->GetItemData(hItem);
			if(pLayerTemp==pLayer)
			{
				pTreeCtrl->SelectItem(hItem);
			}
		}
	}

	// 建立停靠窗口内部控件
	// 出错返回-1
	BOOL KLayerDockablePane::CreateCtrl()
	{
		CRect rectDummy;
		rectDummy.SetRectEmpty();

		if(!m_pTreeCtrl->Create(WS_VISIBLE | WS_CHILD, rectDummy, this, 2))
			return FALSE;

		m_wndToolBar.Create(this, AFX_DEFAULT_TOOLBAR_STYLE, KIDR_TOOLBAR_LAYER);
		m_wndToolBar.LoadToolBar(KIDR_TOOLBAR_LAYER, 0, 0, TRUE);
		//m_wndToolBar.CleanUpLockedImages();
		//m_wndToolBar.LoadBitmap(KIND_IDB_TOOLBAR_LAYER, 0, 0, TRUE);

		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() | CBRS_TOOLTIPS | CBRS_FLYBY);
		m_wndToolBar.SetPaneStyle(m_wndToolBar.GetPaneStyle() & ~(CBRS_GRIPPER | CBRS_SIZE_DYNAMIC | CBRS_BORDER_TOP | CBRS_BORDER_BOTTOM | CBRS_BORDER_LEFT | CBRS_BORDER_RIGHT));
		m_wndToolBar.SetOwner(this);

		// 所有命令将通过此控件路由，而不是通过主框架路由:
		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

		return TRUE;
	}
	// 设置控件字体
	void KLayerDockablePane::SetCtrlFont()
	{
		//m_Font.CreateStockObject(DEFAULT_GUI_FONT);

		NONCLIENTMETRICS info;
		info.cbSize = sizeof(info);
		afxGlobalData.GetNonClientMetrics(info);

		LOGFONT lf;
		afxGlobalData.fontRegular.GetLogFont(&lf);

		lf.lfHeight = info.lfMenuFont.lfHeight;
		lf.lfWeight = info.lfMenuFont.lfWeight;
		lf.lfItalic = info.lfMenuFont.lfItalic;

		m_Font.DeleteObject();
		m_Font.CreateFontIndirect(&lf);
		if(m_pTreeCtrl)
			m_pTreeCtrl->SetFont(&m_Font);
	}
	// 调整布局
	void KLayerDockablePane::AdjustLayout()
	{
		KDockablePane::AdjustLayout();

		if (this->GetSafeHwnd() == NULL)
			return;

		if(m_pTreeCtrl == NULL)
			return;

		CRect rectClient;
		this->GetClientRect(rectClient);

		// 工具条高度
		int cyTlb = 16;
		if(m_wndToolBar.GetSafeHwnd())
		{
			cyTlb = m_wndToolBar.CalcFixedLayout(FALSE, TRUE).cy;
		}

		m_wndToolBar.SetWindowPos(
			NULL, 
			rectClient.left, 
			rectClient.top, 
			rectClient.Width(),
			cyTlb,
			SWP_NOACTIVATE | SWP_NOZORDER);

		m_pTreeCtrl->SetWindowPos(
			NULL,
			rectClient.left, 
			rectClient.top + (cyTlb), 
			rectClient.Width(),
			rectClient.Height() - (cyTlb), 
			SWP_NOACTIVATE | SWP_NOZORDER);
	}
	// 刷新停靠窗口(必要时在派生类重载)
	void KLayerDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		KTreeCtrl* pTreeCtrl = m_pTreeCtrl;
		if(!pTreeCtrl || !pTreeCtrl->GetSafeHwnd())
			return;

		if(lpvData==NULL)
		{
			// 保存新指针
			m_lpvData = lpvData;
			// 清除所有条目
			pTreeCtrl->DeleteAllItems();
			pTreeCtrl->Invalidate(TRUE);
			return;
		}

		// 相同的对象不更新属性窗口
		if(!bForceRefresh && lpvData && m_lpvData==lpvData)
			return;

		// 保存新指针
		m_lpvData = lpvData;
		// 清除所有条目
		pTreeCtrl->DeleteAllItems();
		pTreeCtrl->Invalidate(TRUE);

		//
		// 下面刷新
		//
		// 视图
		KGisViewBase* pView = (KGisViewBase*)lpvData;
		if(!pView)
			return;

		// 文档
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// 地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// 图层集
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		// 树控件
		pTreeCtrl->ModifyStyle(0,TVS_CHECKBOXES);	// 使具有CheckBox
		pTreeCtrl->ModifyStyle(0,TVS_LINESATROOT);	//

		//pTreeCtrl->EnableMultiSelect();	//允许多选
		//pTreeCtrl->EnableColorFont();	//Enable Color_Font

		// 设置 ImageList
		m_ImageListLayer.DeleteImageList();

		//m_ImageListLayer.Create(KIDB_BITMAP_LAYER,16,8,RGB(192,192,192));
		//pTreeCtrl->SetImageList(&m_ImageListLayer,TVSIL_NORMAL);

		m_ImageListLayer.Create(16,16,ILC_COLOR|ILC_MASK,16,16);
		CBitmap bitmap;
		bitmap.Attach(KResource::LoadBitmap(KIDB_BITMAP_LAYER));
		m_ImageListLayer.Add(&bitmap,RGB(192,192,192));
		bitmap.DeleteObject();

		pTreeCtrl->SetImageList(&m_ImageListLayer,TVSIL_NORMAL);

		// 状态图片
		pTreeCtrl->SetStateImage(KResource::LoadBitmap(KIDB_BITMAP_STATE),RGB(255,0,255));

		//TreeCtrl.SetBkImage(IDB_METEOR);
		//pTreeCtrl->SetBkColor(pDoc->GetGParameter()->GetColortabData()->GetSystemColor(SNYC_CTI_BACKGROUND));
		//pTreeCtrl->SetTextColor(pDoc->GetGParameter()->GetColortabData()->GetSystemColor(SNYC_CTI_FORE));
		//pTreeCtrl->SetStateImage(SFX_IDB_TREE_STATE_EYE,RGB(255,0,255));

		// 树根
		m_hTreeRoot = pTreeCtrl->InsertItem(_T("图层控制"),0,1,TVI_ROOT);
		pTreeCtrl->SetCheck(m_hTreeRoot,TRUE);
		pTreeCtrl->SetItemData(m_hTreeRoot, (DWORD)(KLayer*)NULL);
		pTreeCtrl->SetItemColor(m_hTreeRoot,255);
		//pTreeCtrl->EnableItemBold(m_hTreeRoot);

		HTREEITEM hSelectItem = NULL;

		// 1.添加各固定图层
		int iLayerCount = pLayerPtrCollection->Count();
		for(int i=0;i<iLayerCount;i++)
		{
			// 图层
			KLayer* pLayer = pLayerPtrCollection->GetAt(i);
			if(!pLayer)
				continue;

			// 渲染器
			KRender* pRender = pLayer->GetRender();
			if(!pRender)
				continue;

			// 图层属性
			KLayerProperty* pLayerProperty = pRender->GetLayerProperty();
			if(!pLayerProperty)
				continue;

			// 允许移动的图层在后面添加
			//if(pLayerProperty->IsAllowDelete())
			if(pLayerProperty->IsAllowMove())
				continue;

			// 数据提供者
			KProvider* pProvider = pLayerProperty->GetProvider();
			if(!pProvider)
				continue;

			m_hTreeChild[i] = pTreeCtrl->InsertItem(pProvider->GetComment(),2,3,m_hTreeRoot);
			pTreeCtrl->SetItemData(m_hTreeChild[i], (DWORD)pLayer);
			pTreeCtrl->SetCheck(m_hTreeChild[i],pLayerProperty->IsVisible());
			pTreeCtrl->EnableItemBold(m_hTreeChild[i],FALSE);

			// 不可删除图层
			if(!pLayerProperty->IsAllowDelete())
			{
				pTreeCtrl->SetItemColor(m_hTreeChild[i],RGB(192,192,192));
				//bool bAllowMove = pLayerProperty->IsAllowMove();
				//pTreeCtrl->SetItemColor(m_hTreeChild[i], bAllowMove ? RGB(192,192,255) : RGB(192,192,192));
			}
			else
			{
				pTreeCtrl->SetItemColor(m_hTreeChild[i],pLayerProperty->GetColor());
			}

			if(i==iLayerCount-1)
			{
				hSelectItem = m_hTreeChild[i];
			}
		}

		// 2.倒序添加非固定图层
		for(int i=iLayerCount-1;i>=0;i--)
		{
			// 图层
			KLayer* pLayer = pLayerPtrCollection->GetAt(i);
			if(!pLayer)
				continue;

			// 渲染器
			KRender* pRender = pLayer->GetRender();
			if(!pRender)
				continue;

			// 图层属性
			KLayerProperty* pLayerProperty = pRender->GetLayerProperty();
			if(!pLayerProperty)
				continue;

			// 允许删除的图层在后面添加
			//if(!pLayerProperty->IsAllowDelete())
			if(!pLayerProperty->IsAllowMove())
				continue;

			// 数据提供者
			KProvider* pProvider = pLayerProperty->GetProvider();
			if(!pProvider)
				continue;

			m_hTreeChild[i] = pTreeCtrl->InsertItem(pProvider->GetComment(),2,3,m_hTreeRoot);
			pTreeCtrl->SetItemData(m_hTreeChild[i], (DWORD)pLayer);
			pTreeCtrl->SetCheck(m_hTreeChild[i],pLayerProperty->IsVisible());

			pTreeCtrl->EnableItemBold(m_hTreeChild[i],FALSE);

			//不可修改图层
			if(!pLayerProperty->IsAllowDelete())
			{
				pTreeCtrl->SetItemColor(m_hTreeChild[i],RGB(192,192,192));
				//bool bAllowMove = pLayerProperty->IsAllowMove();
				//pTreeCtrl->SetItemColor(m_hTreeChild[i], bAllowMove ? RGB(0,0,192) : RGB(192,192,192));
			}
			else
			{
				pTreeCtrl->SetItemColor(m_hTreeChild[i],pLayerProperty->GetColor());
			}

			if(i==iLayerCount-1)
			{
				hSelectItem = m_hTreeChild[i];
			}

		}

		//pTreeCtrl->SelectItem(m_hTreeRoot);
		pTreeCtrl->SelectItem(hSelectItem);
		pTreeCtrl->Expand(m_hTreeRoot,TVE_EXPAND);

		return;
	}

	// 图层操作
	sfx_msg void KLayerDockablePane::OnLayerOperate(UINT id)
	{
		switch(id)
		{
		case KID_LAYER_DELETE:			// 删除图层
			this->DeleteLayer();
			break;

		case KID_LAYER_MOVE_UP:			// 上移图层
			this->MoveLayerDn();
			break;

		case KID_LAYER_MOVE_DN:			// 下移图层
			this->MoveLayerUp();
			break;

		case KID_LAYER_MOVE_FIRST:		// 图层置顶
			this->MoveLayerLast();
			break;

		case KID_LAYER_MOVE_LAST:		// 图层置底
			this->MoveLayerFirst();
			break;
		}

		return;
	}
	sfx_msg void KLayerDockablePane::OnUpdateLayerOperate(CCmdUI* pCmdUI)
	{
		UINT id = pCmdUI->m_nID;

		switch(id)
		{
		case KID_LAYER_DELETE:		// 删除
			break;
		case KID_LAYER_MOVE_UP:		// 上移
			break;
		case KID_LAYER_MOVE_DN:		// 下移
			break;
		}

		return;
	}

	// 删除选中的图层
	void KLayerDockablePane::DeleteLayer()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		KLayerProperty* pLayerProperty = pLayer->GetRender()->GetLayerProperty();
		if(!pLayerProperty)
			return;

		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pView = pFrame->GetActiveView();
		if(!pView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		KGisDocumentBase* pDoc = (KGisDocumentBase*)((KGisViewBase*)pView)->GetDocument();
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// 不允许删除
		if(!pLayerProperty->IsAllowDelete())
		{
			AfxMessageBox(_T("本图层不允许删除!"));
			return;
		}
		else
		{
			// 删除指定图层
			pDoc->RemoveLayer(pLayer);
			// 刷新树控件
			Refresh(m_lpvData,TRUE);
		}

		// 重新绘制视图
		pMapEngine->EnableRedraw(TRUE);
   		pFrame->SetActiveView(pView);
		pView->Invalidate(false);

	}
	// 上移图层
	void KLayerDockablePane::MoveLayerUp()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// 视图
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// 文档
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// 框架
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// 地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// 图层集
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveUp(pLayer, pLayerPtrCollection))
		{
			// 强制刷新
			this->Refresh(m_lpvData,TRUE);
			// 选择原选择的图层
			this->SelectLayer(pLayer);

			// 重新绘制视图
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}
	// 下移图层
	void KLayerDockablePane::MoveLayerDn()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// 视图
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// 文档
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// 框架
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// 地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// 图层集
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveDn(pLayer, pLayerPtrCollection))
		{
			// 强制刷新
			this->Refresh(m_lpvData,TRUE);
			// 选择原选择的图层
			this->SelectLayer(pLayer);

			// 重新绘制视图
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}
	// 上移图层为最顶层
	void KLayerDockablePane::MoveLayerFirst()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// 视图
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// 文档
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// 框架
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// 地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// 图层集
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveFirst(pLayer, pLayerPtrCollection))
		{
			// 强制刷新
			this->Refresh(m_lpvData,TRUE);
			// 选择原选择的图层
			this->SelectLayer(pLayer);

			// 重新绘制视图
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}
	// 下移图层为最底层
	void KLayerDockablePane::MoveLayerLast()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// 视图
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// 文档
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// 框架
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// 地图控件
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// 图层集
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveLast(pLayer, pLayerPtrCollection))
		{
			// 强制刷新
			this->Refresh(m_lpvData,TRUE);
			// 选择原选择的图层
			this->SelectLayer(pLayer);

			// 重新绘制视图
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}

	bool KLayerDockablePane::MoveUp(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// 不允许删除
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// 不允许移动
		if(!pLayerProperty->IsAllowMove())
			return false;

		// 图层索引
		int index = pLayerPtrCollection->IndexOf(pLayer);
		if(index<=0)
			return false;

		int indexUp = index - 1;
		KLayer* pLayerTemp = pLayerPtrCollection->GetAt(indexUp);
		//if(!pLayerTemp || !pLayerTemp->GetProperty() || !((KLayerProperty*)pLayerTemp->GetProperty())->IsAllowDelete())
		if(!pLayerTemp || !pLayerTemp->GetProperty() || !((KLayerProperty*)pLayerTemp->GetProperty())->IsAllowMove())
			return false;

		pLayerPtrCollection->at(indexUp) = pLayer;
		pLayerPtrCollection->at(index) = pLayerTemp;

		return true;
	}
	bool KLayerDockablePane::MoveDn(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// 不允许删除
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// 不允许移动
		if(!pLayerProperty->IsAllowMove())
			return false;

		int iCount = pLayerPtrCollection->Count();
		int index = pLayerPtrCollection->IndexOf(pLayer);
		if(index>=iCount-1)
			return false;

		int indexDn = index+1;
		KLayer* pLayerTemp = pLayerPtrCollection->GetAt(indexDn);
		//if(!pLayerTemp || !pLayerTemp->GetProperty() || !((KLayerProperty*)pLayerTemp->GetProperty())->IsAllowDelete())
		if(!pLayerTemp || !pLayerTemp->GetProperty() || !((KLayerProperty*)pLayerTemp->GetProperty())->IsAllowMove())
			return false;

		pLayerPtrCollection->at(indexDn) = pLayer;
		pLayerPtrCollection->at(index) = pLayerTemp;

		return true;
	}
	bool KLayerDockablePane::MoveFirst(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// 不允许删除
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// 不允许移动
		if(!pLayerProperty->IsAllowMove())
			return false;

		// 总图层数量
		int iCount = pLayerPtrCollection->Count();
		// 移动的图层索引
		int index = pLayerPtrCollection->IndexOf(pLayer);

		// 查找第一个非固定图层
		int indexFirst = -1;
		for(int i=0;i<iCount;i++)
		{
			//if(((KLayerProperty*)pLayerPtrCollection->GetAt(i)->GetProperty())->IsAllowDelete())
			if(((KLayerProperty*)pLayerPtrCollection->GetAt(i)->GetProperty())->IsAllowMove())
			{
				indexFirst = i;
				break;
			}
		}

		if(indexFirst<0 || index<=indexFirst)
			return false;

		for(int i=index;i>=indexFirst+1;i--)
		{
			pLayerPtrCollection->at(i) = pLayerPtrCollection->at(i-1);
		}

		pLayerPtrCollection->at(indexFirst) = pLayer;

		return true;
	}
	bool KLayerDockablePane::MoveLast(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection)
	{
		// 图层属性
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// 不允许删除
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// 不允许移动
		if(!pLayerProperty->IsAllowMove())
			return false;

		// 总图层数量
		int iCount = pLayerPtrCollection->Count();
		// 移动的图层索引
		int index = pLayerPtrCollection->IndexOf(pLayer);
		// 是最后图层
		if(index == iCount-1)
			return false;

		for(int i=index;i<iCount-1;i++)
		{
			pLayerPtrCollection->at(i) = pLayerPtrCollection->at(i+1);
		}

		pLayerPtrCollection->at(iCount-1) = pLayer;

		return true;
	}

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

