// KLayerDockablePane.cpp : ʵ���ļ�
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
		// ���� KTreeCtrl ����Ϣ
		// WM_KIND_TREE_CTRL_CHECKBOX
		ON_MESSAGE(WM_KIND_TREE_CTRL_CHECKBOX,OnTreeCtrlCheckBox)
		// WM_KIND_TREE_CTRL_LABEL
		ON_MESSAGE(WM_KIND_TREE_CTRL_LABEL,OnTreeCtrlLabel)
		//-------------------------------------------------
		// ˢ��ͼ�㴰����Ϣ WM_USER_REFRESH_LAYER_WND
		ON_MESSAGE(WM_USER_REFRESH_LAYER_WND,OnRefreshLayerWnd)
		//-------------------------------------------------
		// ͼ��˵�
		ON_COMMAND_RANGE(KID_LAYER_BEG,KID_LAYER_END,OnLayerOperate)
		ON_UPDATE_COMMAND_UI_RANGE(KID_LAYER_BEG,KID_LAYER_END,OnUpdateLayerOperate)
		//-------------------------------------------------
	END_MESSAGE_MAP()



	// KLayerDockablePane ��Ϣ�������

	// ���� KTreeCtrl WM_KIND_TREE_CTRL_CHECKBOX ��Ϣ
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
	// ���� KTreeCtrl WM_KIND_TREE_CTRL_LABEL ��Ϣ
	// �������е����ǩʱ(ѡ��ͼ��),ˢ�¶�Ӧ�����Դ���
	sfx_msg  LRESULT KLayerDockablePane::OnTreeCtrlLabel(WPARAM wParam,LPARAM lParam)
	{
		TreeCtrlChecked* pTreeCtrlChecked = (TreeCtrlChecked*)lParam;

		CPoint      point       = pTreeCtrlChecked->point;
		DWORD       dwItemData  = pTreeCtrlChecked->dwItemData;
		HTREEITEM	hTreeItem	= pTreeCtrlChecked->hTreeItem;
		CTreeCtrl*	pTreeCtrl   = pTreeCtrlChecked->pTreeCtrl;

		KLayer* pLayer=(KLayer*)pTreeCtrl->GetItemData(hTreeItem);

		// pLayer == NULL ʱ������Դ���
		//if(pLayer == NULL)
		//	return -1L;

		// ȡ�����Դ���
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

	// �û���ͼ�㴰�ڷ��� WM_USER_REFRESH_LAYER_WND ��ˢ��ͼ�㴰��
	sfx_msg LRESULT KLayerDockablePane::OnRefreshLayerWnd(WPARAM wParam,LPARAM lParam)
	{
		CView* pView = (CView*)lParam;
		if(!pView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
		{
			// ���ͼ�㴰��
			this->Refresh(NULL);
		}
		else
		{
			this->Refresh(pView,((BOOL)wParam) ? true : false);
		}

		return 0;
	}

	// ���ÿ�ݲ˵�
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

	// ȡ��ѡ�е�ͼ��
	KLayer* KLayerDockablePane::GetSelectedLayer()
	{
		KTreeCtrl* pTreeCtrl = m_pTreeCtrl;
		if(!pTreeCtrl || !pTreeCtrl->GetSafeHwnd())
			return NULL;

		HTREEITEM hTreeItem = NULL;

		// ȡ��ѡ�е�����Ŀ
		hTreeItem = pTreeCtrl->GetSelectedItem();
		if(!hTreeItem)
			return NULL;

		// ȡ����Ŀ��Ӧ��ͼ�����
		KLayer* pLayer=(KLayer*)pTreeCtrl->GetItemData(hTreeItem);
		if(!pLayer)
			return NULL;

		return pLayer;
	}
	// ѡ��ͼ��
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

	// ����ͣ�������ڲ��ؼ�
	// ������-1
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

		// �������ͨ���˿ؼ�·�ɣ�������ͨ�������·��:
		m_wndToolBar.SetRouteCommandsViaFrame(FALSE);

		return TRUE;
	}
	// ���ÿؼ�����
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
	// ��������
	void KLayerDockablePane::AdjustLayout()
	{
		KDockablePane::AdjustLayout();

		if (this->GetSafeHwnd() == NULL)
			return;

		if(m_pTreeCtrl == NULL)
			return;

		CRect rectClient;
		this->GetClientRect(rectClient);

		// �������߶�
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
	// ˢ��ͣ������(��Ҫʱ������������)
	void KLayerDockablePane::Refresh(LPVOID lpvData, BOOL bForceRefresh)
	{
		KTreeCtrl* pTreeCtrl = m_pTreeCtrl;
		if(!pTreeCtrl || !pTreeCtrl->GetSafeHwnd())
			return;

		if(lpvData==NULL)
		{
			// ������ָ��
			m_lpvData = lpvData;
			// ���������Ŀ
			pTreeCtrl->DeleteAllItems();
			pTreeCtrl->Invalidate(TRUE);
			return;
		}

		// ��ͬ�Ķ��󲻸������Դ���
		if(!bForceRefresh && lpvData && m_lpvData==lpvData)
			return;

		// ������ָ��
		m_lpvData = lpvData;
		// ���������Ŀ
		pTreeCtrl->DeleteAllItems();
		pTreeCtrl->Invalidate(TRUE);

		//
		// ����ˢ��
		//
		// ��ͼ
		KGisViewBase* pView = (KGisViewBase*)lpvData;
		if(!pView)
			return;

		// �ĵ�
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// ͼ�㼯
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		// ���ؼ�
		pTreeCtrl->ModifyStyle(0,TVS_CHECKBOXES);	// ʹ����CheckBox
		pTreeCtrl->ModifyStyle(0,TVS_LINESATROOT);	//

		//pTreeCtrl->EnableMultiSelect();	//�����ѡ
		//pTreeCtrl->EnableColorFont();	//Enable Color_Font

		// ���� ImageList
		m_ImageListLayer.DeleteImageList();

		//m_ImageListLayer.Create(KIDB_BITMAP_LAYER,16,8,RGB(192,192,192));
		//pTreeCtrl->SetImageList(&m_ImageListLayer,TVSIL_NORMAL);

		m_ImageListLayer.Create(16,16,ILC_COLOR|ILC_MASK,16,16);
		CBitmap bitmap;
		bitmap.Attach(KResource::LoadBitmap(KIDB_BITMAP_LAYER));
		m_ImageListLayer.Add(&bitmap,RGB(192,192,192));
		bitmap.DeleteObject();

		pTreeCtrl->SetImageList(&m_ImageListLayer,TVSIL_NORMAL);

		// ״̬ͼƬ
		pTreeCtrl->SetStateImage(KResource::LoadBitmap(KIDB_BITMAP_STATE),RGB(255,0,255));

		//TreeCtrl.SetBkImage(IDB_METEOR);
		//pTreeCtrl->SetBkColor(pDoc->GetGParameter()->GetColortabData()->GetSystemColor(SNYC_CTI_BACKGROUND));
		//pTreeCtrl->SetTextColor(pDoc->GetGParameter()->GetColortabData()->GetSystemColor(SNYC_CTI_FORE));
		//pTreeCtrl->SetStateImage(SFX_IDB_TREE_STATE_EYE,RGB(255,0,255));

		// ����
		m_hTreeRoot = pTreeCtrl->InsertItem(_T("ͼ�����"),0,1,TVI_ROOT);
		pTreeCtrl->SetCheck(m_hTreeRoot,TRUE);
		pTreeCtrl->SetItemData(m_hTreeRoot, (DWORD)(KLayer*)NULL);
		pTreeCtrl->SetItemColor(m_hTreeRoot,255);
		//pTreeCtrl->EnableItemBold(m_hTreeRoot);

		HTREEITEM hSelectItem = NULL;

		// 1.��Ӹ��̶�ͼ��
		int iLayerCount = pLayerPtrCollection->Count();
		for(int i=0;i<iLayerCount;i++)
		{
			// ͼ��
			KLayer* pLayer = pLayerPtrCollection->GetAt(i);
			if(!pLayer)
				continue;

			// ��Ⱦ��
			KRender* pRender = pLayer->GetRender();
			if(!pRender)
				continue;

			// ͼ������
			KLayerProperty* pLayerProperty = pRender->GetLayerProperty();
			if(!pLayerProperty)
				continue;

			// �����ƶ���ͼ���ں������
			//if(pLayerProperty->IsAllowDelete())
			if(pLayerProperty->IsAllowMove())
				continue;

			// �����ṩ��
			KProvider* pProvider = pLayerProperty->GetProvider();
			if(!pProvider)
				continue;

			m_hTreeChild[i] = pTreeCtrl->InsertItem(pProvider->GetComment(),2,3,m_hTreeRoot);
			pTreeCtrl->SetItemData(m_hTreeChild[i], (DWORD)pLayer);
			pTreeCtrl->SetCheck(m_hTreeChild[i],pLayerProperty->IsVisible());
			pTreeCtrl->EnableItemBold(m_hTreeChild[i],FALSE);

			// ����ɾ��ͼ��
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

		// 2.������ӷǹ̶�ͼ��
		for(int i=iLayerCount-1;i>=0;i--)
		{
			// ͼ��
			KLayer* pLayer = pLayerPtrCollection->GetAt(i);
			if(!pLayer)
				continue;

			// ��Ⱦ��
			KRender* pRender = pLayer->GetRender();
			if(!pRender)
				continue;

			// ͼ������
			KLayerProperty* pLayerProperty = pRender->GetLayerProperty();
			if(!pLayerProperty)
				continue;

			// ����ɾ����ͼ���ں������
			//if(!pLayerProperty->IsAllowDelete())
			if(!pLayerProperty->IsAllowMove())
				continue;

			// �����ṩ��
			KProvider* pProvider = pLayerProperty->GetProvider();
			if(!pProvider)
				continue;

			m_hTreeChild[i] = pTreeCtrl->InsertItem(pProvider->GetComment(),2,3,m_hTreeRoot);
			pTreeCtrl->SetItemData(m_hTreeChild[i], (DWORD)pLayer);
			pTreeCtrl->SetCheck(m_hTreeChild[i],pLayerProperty->IsVisible());

			pTreeCtrl->EnableItemBold(m_hTreeChild[i],FALSE);

			//�����޸�ͼ��
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

	// ͼ�����
	sfx_msg void KLayerDockablePane::OnLayerOperate(UINT id)
	{
		switch(id)
		{
		case KID_LAYER_DELETE:			// ɾ��ͼ��
			this->DeleteLayer();
			break;

		case KID_LAYER_MOVE_UP:			// ����ͼ��
			this->MoveLayerDn();
			break;

		case KID_LAYER_MOVE_DN:			// ����ͼ��
			this->MoveLayerUp();
			break;

		case KID_LAYER_MOVE_FIRST:		// ͼ���ö�
			this->MoveLayerLast();
			break;

		case KID_LAYER_MOVE_LAST:		// ͼ���õ�
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
		case KID_LAYER_DELETE:		// ɾ��
			break;
		case KID_LAYER_MOVE_UP:		// ����
			break;
		case KID_LAYER_MOVE_DN:		// ����
			break;
		}

		return;
	}

	// ɾ��ѡ�е�ͼ��
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

		// ������ɾ��
		if(!pLayerProperty->IsAllowDelete())
		{
			AfxMessageBox(_T("��ͼ�㲻����ɾ��!"));
			return;
		}
		else
		{
			// ɾ��ָ��ͼ��
			pDoc->RemoveLayer(pLayer);
			// ˢ�����ؼ�
			Refresh(m_lpvData,TRUE);
		}

		// ���»�����ͼ
		pMapEngine->EnableRedraw(TRUE);
   		pFrame->SetActiveView(pView);
		pView->Invalidate(false);

	}
	// ����ͼ��
	void KLayerDockablePane::MoveLayerUp()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// ��ͼ
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// �ĵ�
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// ���
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// ͼ�㼯
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveUp(pLayer, pLayerPtrCollection))
		{
			// ǿ��ˢ��
			this->Refresh(m_lpvData,TRUE);
			// ѡ��ԭѡ���ͼ��
			this->SelectLayer(pLayer);

			// ���»�����ͼ
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}
	// ����ͼ��
	void KLayerDockablePane::MoveLayerDn()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// ��ͼ
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// �ĵ�
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// ���
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// ͼ�㼯
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveDn(pLayer, pLayerPtrCollection))
		{
			// ǿ��ˢ��
			this->Refresh(m_lpvData,TRUE);
			// ѡ��ԭѡ���ͼ��
			this->SelectLayer(pLayer);

			// ���»�����ͼ
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}
	// ����ͼ��Ϊ���
	void KLayerDockablePane::MoveLayerFirst()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// ��ͼ
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// �ĵ�
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// ���
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// ͼ�㼯
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveFirst(pLayer, pLayerPtrCollection))
		{
			// ǿ��ˢ��
			this->Refresh(m_lpvData,TRUE);
			// ѡ��ԭѡ���ͼ��
			this->SelectLayer(pLayer);

			// ���»�����ͼ
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}
	// ����ͼ��Ϊ��ײ�
	void KLayerDockablePane::MoveLayerLast()
	{
		KLayer* pLayer = this->GetSelectedLayer();
		if(!pLayer)
			return;

		// ��ͼ
		KGisViewBase* pView = (KGisViewBase*)m_lpvData;
		if(!pView)
			return;

		// �ĵ�
		KGisDocumentBase* pDoc = (KGisDocumentBase*)pView->GetDocument();
		if(!pDoc)
			return;

		// ���
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		CView* pActiveView = pFrame->GetActiveView();
		if(!pActiveView->IsKindOf(RUNTIME_CLASS(KGisViewBase)))
			return;

		// ��ͼ�ؼ�
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if(!pMapEngine)
			return;

		// ͼ�㼯
		TPtrCollection<KLayer*>* pLayerPtrCollection = pMapEngine->GetLayerPtrCollection();
		if(!pLayerPtrCollection)
			return;

		if(this->MoveLast(pLayer, pLayerPtrCollection))
		{
			// ǿ��ˢ��
			this->Refresh(m_lpvData,TRUE);
			// ѡ��ԭѡ���ͼ��
			this->SelectLayer(pLayer);

			// ���»�����ͼ
			pMapEngine->EnableRedraw(TRUE);
   			pFrame->SetActiveView(pActiveView);
			pActiveView->Invalidate(false);
		}

		return;
	}

	bool KLayerDockablePane::MoveUp(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection)
	{
		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// ������ɾ��
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// �������ƶ�
		if(!pLayerProperty->IsAllowMove())
			return false;

		// ͼ������
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
		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// ������ɾ��
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// �������ƶ�
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
		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// ������ɾ��
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// �������ƶ�
		if(!pLayerProperty->IsAllowMove())
			return false;

		// ��ͼ������
		int iCount = pLayerPtrCollection->Count();
		// �ƶ���ͼ������
		int index = pLayerPtrCollection->IndexOf(pLayer);

		// ���ҵ�һ���ǹ̶�ͼ��
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
		// ͼ������
		KLayerProperty* pLayerProperty = (KLayerProperty*)pLayer->GetProperty();
		if(!pLayerProperty)
			return false;

		//// ������ɾ��
		//if(!pLayerProperty->IsAllowDelete())
		//	return false;
		// �������ƶ�
		if(!pLayerProperty->IsAllowMove())
			return false;

		// ��ͼ������
		int iCount = pLayerPtrCollection->Count();
		// �ƶ���ͼ������
		int index = pLayerPtrCollection->IndexOf(pLayer);
		// �����ͼ��
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

