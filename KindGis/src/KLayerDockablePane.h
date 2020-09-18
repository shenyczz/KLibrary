/******************************************************************************
 ** KLayerDockablePane.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLayerDockablePane - 可停靠图层窗口
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

#include "KLayer.h"

namespace DockablePanes
{
	// 子树最大数量
	//#define KIND_TREE_CHILD_MAX		50

	// KLayerDockablePane

	class KIND_EXT_CLASS KLayerDockablePane : public KDockablePane
	{
		DECLARE_DYNAMIC(KLayerDockablePane)

	public:
		KLayerDockablePane();
		virtual ~KLayerDockablePane();

	protected:
		// 来自 KTreeCtrl WM_KIND_TREE_CTRL_CHECKBOX 消息
		sfx_msg LRESULT OnTreeCtrlCheckBox(WPARAM wParam,LPARAM lParam);
		// 来自 KTreeCtrl WM_KIND_TREE_CTRL_LABEL 消息
		sfx_msg LRESULT OnTreeCtrlLabel(WPARAM wParam,LPARAM lParam);

		// 用户向图层窗口发送 WM_USER_REFRESH_LAYER_WND 来刷新图层窗口
		sfx_msg LRESULT OnRefreshLayerWnd(WPARAM wParam,LPARAM lParam);

		// 图层操作
		sfx_msg void OnLayerOperate(UINT id);
		sfx_msg void OnUpdateLayerOperate(CCmdUI* pCmdUI);

		DECLARE_MESSAGE_MAP()

	public:
		//=================================================
		// 刷新停靠窗口(必要时在派生类重载)
		virtual void Refresh(LPVOID lpvData, BOOL bForceRefresh = FALSE);
		protected:
			// 建立停靠窗口内部控件(必要时在派生类重载)
			virtual BOOL CreateCtrl();
			// 设置控件字体(必要时在派生类重载)
			virtual void SetCtrlFont();
			// 调整停靠窗口布局(必要时在派生类重载)
			virtual void AdjustLayout();
		//=================================================


	public:
		// 设置快捷菜单
		void SetMenuID(UINT uMenuID);
		// 取得选中的图层
		KLayer* GetSelectedLayer();
		// 选中图层
		void SelectLayer(KLayer* pLayer);

	protected:
		// 删除图层
		virtual void DeleteLayer();
		// 上移图层
		virtual void MoveLayerUp();
		// 下移图层
		virtual void MoveLayerDn();
		// 上移图层为最顶层
		virtual void MoveLayerFirst();
		// 下移图层为最底层
		virtual void MoveLayerLast();

	private:
		bool MoveUp(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);
		bool MoveDn(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);
		bool MoveFirst(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);
		bool MoveLast(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);

	protected:
		// 树控件
		KTreeCtrl* m_pTreeCtrl;
		// 树根
		HTREEITEM m_hTreeRoot;
		// 子树
		HTREEITEM m_hTreeChild[50];

		CImageList m_ImageListLayer;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

