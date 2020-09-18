/******************************************************************************
 ** KLayerDockablePane.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLayerDockablePane - ��ͣ��ͼ�㴰��
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

#include "KLayer.h"

namespace DockablePanes
{
	// �����������
	//#define KIND_TREE_CHILD_MAX		50

	// KLayerDockablePane

	class KIND_EXT_CLASS KLayerDockablePane : public KDockablePane
	{
		DECLARE_DYNAMIC(KLayerDockablePane)

	public:
		KLayerDockablePane();
		virtual ~KLayerDockablePane();

	protected:
		// ���� KTreeCtrl WM_KIND_TREE_CTRL_CHECKBOX ��Ϣ
		sfx_msg LRESULT OnTreeCtrlCheckBox(WPARAM wParam,LPARAM lParam);
		// ���� KTreeCtrl WM_KIND_TREE_CTRL_LABEL ��Ϣ
		sfx_msg LRESULT OnTreeCtrlLabel(WPARAM wParam,LPARAM lParam);

		// �û���ͼ�㴰�ڷ��� WM_USER_REFRESH_LAYER_WND ��ˢ��ͼ�㴰��
		sfx_msg LRESULT OnRefreshLayerWnd(WPARAM wParam,LPARAM lParam);

		// ͼ�����
		sfx_msg void OnLayerOperate(UINT id);
		sfx_msg void OnUpdateLayerOperate(CCmdUI* pCmdUI);

		DECLARE_MESSAGE_MAP()

	public:
		//=================================================
		// ˢ��ͣ������(��Ҫʱ������������)
		virtual void Refresh(LPVOID lpvData, BOOL bForceRefresh = FALSE);
		protected:
			// ����ͣ�������ڲ��ؼ�(��Ҫʱ������������)
			virtual BOOL CreateCtrl();
			// ���ÿؼ�����(��Ҫʱ������������)
			virtual void SetCtrlFont();
			// ����ͣ�����ڲ���(��Ҫʱ������������)
			virtual void AdjustLayout();
		//=================================================


	public:
		// ���ÿ�ݲ˵�
		void SetMenuID(UINT uMenuID);
		// ȡ��ѡ�е�ͼ��
		KLayer* GetSelectedLayer();
		// ѡ��ͼ��
		void SelectLayer(KLayer* pLayer);

	protected:
		// ɾ��ͼ��
		virtual void DeleteLayer();
		// ����ͼ��
		virtual void MoveLayerUp();
		// ����ͼ��
		virtual void MoveLayerDn();
		// ����ͼ��Ϊ���
		virtual void MoveLayerFirst();
		// ����ͼ��Ϊ��ײ�
		virtual void MoveLayerLast();

	private:
		bool MoveUp(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);
		bool MoveDn(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);
		bool MoveFirst(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);
		bool MoveLast(KLayer* pLayer, TPtrCollection<KLayer*>* pLayerPtrCollection);

	protected:
		// ���ؼ�
		KTreeCtrl* m_pTreeCtrl;
		// ����
		HTREEITEM m_hTreeRoot;
		// ����
		HTREEITEM m_hTreeChild[50];

		CImageList m_ImageListLayer;
	};

//---------------------------------------------------------
}// namespace DockablePanes - end
//---------------------------------------------------------

