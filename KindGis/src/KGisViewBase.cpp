// KGisViewBase.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KGisViewBase.h"

#include "KGisFrameWnd.h"
#include "KGisDocument.h"
#include "KLayerDockablePane.h"

namespace Framework
{

	// KGisViewBase

	IMPLEMENT_DYNCREATE(KGisViewBase, KPrintView)

	KGisViewBase::KGisViewBase()
	{
		// ������Ϣ����
		m_iGisTool = GisTool_None;
		// ��ʾ������Ϣ
		m_bShowGisInfo = true;
		// ����϶�
		m_bMouseDraging = false;

		//���
		m_hCurDefault   = ::LoadCursor(NULL,IDC_ARROW);	//ȱʡ���
		m_hCurNew       = m_hCurDefault;			//�¹��
		m_hCurOld       = m_hCurDefault;  			//ԭ�����

		//�������
		m_hCurAnimate = NULL;
		//m_hCurAnimate = KResource::LoadAnimateCursor(KIND_IDR_ANI_HAND_APP_START,_T("ANI"));
	}

	KGisViewBase::~KGisViewBase()
	{
		DestroyCursor(m_hCurDefault);
		DestroyCursor(m_hCurOld);
		DestroyCursor(m_hCurNew);
		DestroyCursor(m_hCurAnimate);
	}

	BEGIN_MESSAGE_MAP(KGisViewBase, KPrintView)
		ON_WM_SETCURSOR()
		ON_WM_KEYDOWN()
		ON_WM_KEYUP()

		// ���β���
		ON_COMMAND_RANGE(KID_ROAM_BEG,KID_ROAM_END,OnRoamOperate)
		ON_UPDATE_COMMAND_UI_RANGE(KID_ROAM_BEG,KID_ROAM_END,OnRoamOperateUpdate)

		// ���ͼ�� OnAddLayer
		ON_MESSAGE(WM_USER_LAYER_ADD,OnAddLayer)
		// �Ƴ�ͼ�� OnRemoveLayer
		ON_MESSAGE(WM_USER_LAYER_REMOVE,OnRemoveLayer)

		// WM_USER_LANDMARK_CALIBRATE : WPARAM = bDraging, LPARAM = &m_sizeDraging
		ON_MESSAGE(WM_USER_LANDMARK_CALIBRATE,OnLandmarkCalibrate)

		// WM_USER_LANDMARK_CALIBRATE_STEP : WPARAM = 0, LPARAM = &m_sizeDraging
		ON_MESSAGE(WM_USER_LANDMARK_CALIBRATE_STEP,OnLandmarkCalibrateStep)
	END_MESSAGE_MAP()


	// KGisViewBase ��ͼ

	void KGisViewBase::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KGisViewBase ���

	#ifdef _DEBUG
	void KGisViewBase::AssertValid() const
	{
		KPrintView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KGisViewBase::Dump(CDumpContext& dc) const
	{
		KPrintView::Dump(dc);
	}
	#endif
	#endif //_DEBUG

	// KGisViewBase ��Ϣ�������

	void KGisViewBase::OnInitialUpdate()
	{
		KPrintView::OnInitialUpdate();

		// TODO: �ڴ����ר�ô����/����û���

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		ASSERT_VALID(pDoc);
		if (!pDoc)
			return;

		// ��ͼ����
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == NULL )
			return;

		// ����ʹ�õ�ͼ�ռ����ͼ���ھ��
		pMapEngine->SetHwnd(this->GetSafeHwnd());

		// ͬ����ͼ�������ͼ�ı�����ɫ
		pMapEngine->SetBackColor(this->GetBackColor());
	}
	void KGisViewBase::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView)
	{
		KPrintView::OnActivateView(bActivate, pActivateView, pDeactiveView);

		// TODO: �ڴ����ר�ô����/����û���
		CWnd* pWndLayer = NULL;
		BOOL bKindOfMDIFrameWnd = FALSE;
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndLayer = ((KFrameWndEx*)pFrame)->GetLayerWnd();
		}
		else if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			bKindOfMDIFrameWnd = TRUE;
			pWndLayer = ((KMDIFrameWndEx*)pFrame)->GetLayerWnd();
		}

		if(pWndLayer && pWndLayer->GetSafeHwnd())
		{
			if(bActivate)
			{
				// TRUE - �Ƿ�ǿ��ˢ��
				pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)bKindOfMDIFrameWnd,(LPARAM)this);
			}
			else
			{
				pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)FALSE,(LPARAM)this);
			}
		}
		TRACE(_T("OnActivateView - %s\n"),bActivate ? _T("bActivate") : _T("Deactive"));
	}

	BOOL KGisViewBase::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
	{
		::SetCursor(m_hCurNew);
		return TRUE;
	}

	void KGisViewBase::Refresh(BOOL bErase)
	{
		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		pMapEngine->EnableRedraw();
		Invalidate(bErase);

		return;
	}

	// ���ͼ�� WM_USER_LAYER_ADD
	sfx_msg LRESULT KGisViewBase::OnAddLayer(WPARAM wParam, LPARAM lParam)
	{
		// ���ͼ���¼�����
		AddLayerHappened();
		return 0L;
	}
	// �Ƴ�ͼ�� WM_USER_LAYER_REMOVE
	sfx_msg LRESULT KGisViewBase::OnRemoveLayer(WPARAM wParam, LPARAM lParam)
	{
		// �Ƴ�ͼ���¼�����
		RemoveLayerHappened();
		return 0L;
	}
	// ���ͼ�㷢��
	void KGisViewBase::AddLayerHappened()
	{
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(!pFrame)
			return;

		CWnd* pWndLayer = NULL;

		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndLayer = ((KFrameWndEx*)pFrame)->GetLayerWnd();
		}
		else if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			pWndLayer = ((KMDIFrameWndEx*)pFrame)->GetLayerWnd();
		}

		if(!pWndLayer)
			return;

		// �����ͼ�㴰�ڷ�����Ϣ
		if(pWndLayer->GetSafeHwnd())
		{
			// ˢ��ͼ�������
			pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)TRUE,(LPARAM)this);
		}

		return;
	}
	// �Ƴ�ͼ�㷢��
	void KGisViewBase::RemoveLayerHappened()
	{
		CFrameWnd* pFrame = (CFrameWnd*)AfxGetMainWnd();
		if(!pFrame)
			return;

		CWnd* pWndLayer = NULL;

		if(pFrame->IsKindOf(RUNTIME_CLASS(KFrameWndEx)))
		{
			pWndLayer = ((KFrameWndEx*)pFrame)->GetLayerWnd();
		}
		else if(pFrame->IsKindOf(RUNTIME_CLASS(KMDIFrameWndEx)))
		{
			pWndLayer = ((KMDIFrameWndEx*)pFrame)->GetLayerWnd();
		}

		if(!pWndLayer)
			return;

		// �����ͼ�㴰�ڷ�����Ϣ
		if(pWndLayer->GetSafeHwnd())
		{
			// ˢ��ͼ�������
			pWndLayer->PostMessage(WM_USER_REFRESH_LAYER_WND,(WPARAM)TRUE,(LPARAM)this);
		}

		return;
	}

	///========================================================
	///���β��� - Beg
	///---------------
	sfx_msg void KGisViewBase::OnRoamOperate(UINT id)
	{
		// �ĵ�
		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		// ��ͼ����
		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == NULL )
			return;

		// ������������
		KMouseListenerCollection* pMouseListenerCollection = pMapEngine->GetMouseListenerCollection();
		if(!pMouseListenerCollection)
			return;

		// ���õ�ǰ����������ֵ = MapController
		//pMouseListenerCollection->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		switch(id)
		{
		case KID_ROAM_CANCEL:		// ȡ��
			ZoomCancel();
			break;

		case KID_ROAM_RESET:		// ��λ
			ZoomReset();
			break;

		case KID_ROAM_REFRESH:		// ˢ��
			ZoomRefresh();
			break;

		case KID_ROAM_ZOOM_IN:		// �Ŵ�
			ZoomIn();
			break;

		case KID_ROAM_ZOOM_OUT:		// ��С
			ZoomOut();
			break;

		case KID_ROAM_ZOOM_PAN:		// ����
			ZoomPan();
			break;
		}

		return;
	}
	sfx_msg void KGisViewBase::OnRoamOperateUpdate(CCmdUI* pCmdUI)
	{
		UINT id = pCmdUI->m_nID;

		switch(id)
		{
		case KID_ROAM_CANCEL:		// ȡ��
			pCmdUI->SetCheck(m_iGisTool == GisTool_Cancel);
			break;

		case KID_ROAM_RESET:		// ��λ
			pCmdUI->SetCheck(m_iGisTool == GisTool_Reset);
			break;

		case KID_ROAM_REFRESH:	// ˢ��
			pCmdUI->SetCheck(m_iGisTool == GisTool_Refresh);
			break;

		case KID_ROAM_ZOOM_IN:		// �Ŵ�
			pCmdUI->SetCheck(m_iGisTool == GisTool_ZoomIn);
			break;

		case KID_ROAM_ZOOM_OUT:	// ��С
			pCmdUI->SetCheck(m_iGisTool == GisTool_ZoomOut);
			break;

		case KID_ROAM_ZOOM_PAN:	// ����
			pCmdUI->SetCheck(m_iGisTool == GisTool_ZoomPan);
			break;
		}

		return;
	}
	// �Ŵ�
	void KGisViewBase::ZoomIn()
	{
		m_iGisTool = GisTool_ZoomIn;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_ZOOM_IN);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		return;
	}
	// ��С
	void KGisViewBase::ZoomOut()
	{
		m_iGisTool = GisTool_ZoomOut;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_ZOOM_OUT);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		return;
	}
	// ����
	void KGisViewBase::ZoomPan()
	{
		m_iGisTool = GisTool_ZoomPan;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = KResource::LoadCursor(KIDC_CURSOR_HAND_ARROW);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,m_iGisTool);

		return;
	}
	// ��λ
	void KGisViewBase::ZoomReset()
	{
		// ���ı� m_iGisTool

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		pMapEngine->Reset();
		CRect clientRect;
		GetClientRect(&clientRect);
		pMapEngine->ReDraw(clientRect.left,clientRect.top,clientRect.Width(),clientRect.Height());
		Invalidate(false);

		return;
	}
	// ˢ��(������Ҫ�����������޸�)
	void KGisViewBase::ZoomRefresh()
	{
		// ���ı� m_iGisTool

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		pMapEngine->EnableRedraw();
		Invalidate(false);

		return;
	}
	//	ȡ��
	void KGisViewBase::ZoomCancel()
	{
		m_iGisTool = GisTool_None;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = m_hCurDefault;
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_MapController,GisTool_ZoomPan);

		return;
	}
	///----------------
	///���β��� - End
	///========================================================

	///========================================================
	///���̲��� - Beg
	///---------------
	void KGisViewBase::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		//-------------------------------------------------
		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;
		//-------------------------------------------------
		// �ر�У׼
		if(m_iGisTool == GisTool_Misce_Landmark_Calibrate)
		{
			int dx = 0;
			int dy = 0;
			switch(nChar)
			{
			case VK_LEFT:
				dx = -1; dy = 0;
				break;

			case VK_RIGHT:
				dx = +1; dy = 0;
				break;

			case VK_UP:
				dx = 0; dy = -1;
				break;

			case VK_DOWN:
				dx = 0; dy = +1;
				break;

			default:
				break;
			}

			pMapEngine->LandmarkCalibrate(dx,dy);
		}
		//-------------------------------------------------
		KPrintView::OnKeyDown(nChar, nRepCnt, nFlags);
	}

	void KGisViewBase::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		//-------------------------------------------------
		//-------------------------------------------------
		KPrintView::OnKeyUp(nChar, nRepCnt, nFlags);
	}
	///----------------
	///���̲��� - End
	///========================================================

	///========================================================
	// �ر�У׼
	// ��GisTool = GisTool_Misce_Landmark_Calibrate
	void KGisViewBase::LandmarkCalibrate()
	{
		if(m_iGisTool == GisTool_Misce_Landmark_Calibrate)
		{
			this->ZoomCancel();
			return;
		}

		m_iGisTool = GisTool_Misce_Landmark_Calibrate;

		if(m_hCurNew)
		{
			::DestroyCursor(m_hCurNew);
			m_hCurNew = ::LoadCursor(NULL,IDC_SIZEALL);
		}

		KGisDocumentBase* pDoc = (KGisDocumentBase*)GetDocument();
		if (!pDoc)
			return;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if ( pMapEngine == 0 )
			return ;

		// ������������
		KMouseListenerCollection* pMouseListeners = pMapEngine->GetMouseListenerCollection();
		pMouseListeners->SetCurrentMouseListenerKey(GisTool_BlackboardController,m_iGisTool);

		return;
	}

	// ��Ӧ WM_USER_LANDMARK_CALIBRATE
	sfx_msg LRESULT KGisViewBase::OnLandmarkCalibrate(WPARAM wParam,LPARAM lParam)
	{
		//-----------------------------------------------------
		KGisFrameWnd* pMainFrame = (KGisFrameWnd*)AfxGetMainWnd();
		KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();
		KLayer* pLayer = pLayerWnd->GetSelectedLayer();
		if(!pLayer)
		{
			AfxMessageBox(_T("û��ѡ������ͼ��"));
			return -1;
		}

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return -1;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return -1;
		//-----------------------------------------------------
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return -1;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if (!pMapEngine)
			return -1;
		//-----------------------------------------------------
		KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;
		if(!pAgmrsData->IsAgmrsData())
		{
			AfxMessageBox(_T("ѡ������ͼ�㲻��ң������!"));
			return -1;
		}
		//-----------------------------------------------------
		BOOL bDraging = (BOOL)wParam;
		SIZE size = *(SIZE*)lParam;
		int dx = (int)size.cx;
		int dy = (int)size.cy;
		// ���������ң������,ƽ��
		// dx<0 -> ����, dx>0 -> ����
		// dy<0 -> ����, dy>0 -> ����
		//-----------------------------------------------------
		static int flag = 0;
		static float fLonMin0 = 0;	// 
		static float fLonMax0 = 0;	// 
		static float fLatMin0 = 0;	// 
		static float fLatMax0 = 0;	// 
		//-----------------------------------------------------
		LDFHeaderCommon* pFileHeader = NULL;

		if(flag==0 && bDraging)		// ��һ��
		{
			flag = 1;

			// ����ԭ������
			pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();
			fLonMin0 = pFileHeader->fLonMin;
			fLonMax0 = pFileHeader->fLonMax;
			fLatMin0 = pFileHeader->fLatMin;
			fLatMax0 = pFileHeader->fLatMax;
		}
		else if(flag==1 && bDraging)
		{
			pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();
			pFileHeader->fLonMin = fLonMin0;
			pFileHeader->fLonMax = fLonMax0;
			pFileHeader->fLatMin = fLatMin0;
			pFileHeader->fLatMax = fLatMax0;
		}
		else if(!bDraging)
		{
			flag = 0;
			pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();
			return 0;
			//pFileHeader->fLonMin = fLonMin0;
			//pFileHeader->fLonMax = fLonMax0;
			//pFileHeader->fLatMin = fLatMin0;
			//pFileHeader->fLatMax = fLatMax0;
		}

		// �޸�
		pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();

		// �޸ľ���
		pFileHeader->fLonMin += pFileHeader->fLonResolution * dx;
		pFileHeader->fLonMax += pFileHeader->fLonResolution * dx;
		// �޸�γ��
		pFileHeader->fLatMin += pFileHeader->fLatResolution * -dy;
		pFileHeader->fLatMax += pFileHeader->fLatResolution * -dy;

		// �޸�������Ϣ
		pAgmrsData->FillDataInfo(pAgmrsData->FileHeader());
		// �޸İ�Χ��
		pAgmrsData->SetExtents(KExtents(pFileHeader->fLonMin,pFileHeader->fLatMin,pFileHeader->fLonMax,pFileHeader->fLatMax));
		//-----------------------------------------------------
		pMapEngine->Refresh(TRUE);
		//-----------------------------------------------------
		return 0;
	}

	sfx_msg LRESULT KGisViewBase::OnLandmarkCalibrateStep(WPARAM wParam,LPARAM lParam)
	{
		//-----------------------------------------------------
		KGisFrameWnd* pMainFrame = (KGisFrameWnd*)AfxGetMainWnd();
		KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();
		KLayer* pLayer = pLayerWnd->GetSelectedLayer();
		if(!pLayer)
		{
			AfxMessageBox(_T("û��ѡ������ͼ��"));
			return -1;
		}

		// �����ṩ��
		KProvider* pProvider = pLayer->GetProvider();
		if(!pProvider)
			return -1;

		// ���ݶ���
		KDataObject* pDataObject = pProvider->GetDataObject();
		if(!pDataObject)
			return -1;
		//-----------------------------------------------------
		KGisDocument* pDoc = (KGisDocument*)GetDocument();
		if (!pDoc)
			return -1;

		KMapEngine* pMapEngine = pDoc->GetMapEngine();
		if (!pMapEngine)
			return -1;
		//-----------------------------------------------------
		KAgmrsData* pAgmrsData = (KAgmrsData*)pDataObject;
		if(!pAgmrsData->IsAgmrsData())
		{
			AfxMessageBox(_T("ѡ������ͼ�㲻��ң������!"));
			return -1;
		}
		//-----------------------------------------------------
		BOOL bDraging = (BOOL)wParam;
		SIZE size = *(SIZE*)lParam;
		//-----------------------------------------------------
		LDFHeaderCommon* pFileHeader = NULL;
		int dx = (int)size.cx;
		int dy = (int)size.cy;
		// ���������ң������,ƽ��
		// dx<0 -> ����, dx>0 -> ����
		// dy<0 -> ����, dy>0 -> ����
		//-----------------------------------------------------
		// �޸�
		pFileHeader = (LDFHeaderCommon*)pAgmrsData->FileHeader();

		// �޸ľ���
		pFileHeader->fLonMin += pFileHeader->fLonResolution * dx;
		pFileHeader->fLonMax += pFileHeader->fLonResolution * dx;
		// �޸�γ��
		pFileHeader->fLatMin += pFileHeader->fLatResolution * -dy;
		pFileHeader->fLatMax += pFileHeader->fLatResolution * -dy;

		// �޸�������Ϣ
		pAgmrsData->FillDataInfo(pAgmrsData->FileHeader());
		// �޸İ�Χ��
		pAgmrsData->SetExtents(KExtents(pFileHeader->fLonMin,pFileHeader->fLatMin,pFileHeader->fLonMax,pFileHeader->fLatMax));
		//-----------------------------------------------------
		pMapEngine->Refresh(TRUE);
		//-----------------------------------------------------
		return 0;
	}
	///========================================================

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
