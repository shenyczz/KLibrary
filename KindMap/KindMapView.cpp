
// KindMapView.cpp : CKindMapView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "KindMap.h"
#endif

#include "MainFrm.h"
#include "KindMapDoc.h"
#include "KindMapView.h"

#include "KTest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CKindMapView

IMPLEMENT_DYNCREATE(CKindMapView, KGisView)

BEGIN_MESSAGE_MAP(CKindMapView, KGisView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &KGisView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &KGisView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKindMapView::OnFilePrintPreview)
	ON_WM_RBUTTONUP()

	//ON_WM_CONTEXTMENU()	// ����

	ON_COMMAND(ID_KINDMAP_TEST, &CKindMapView::OnKindmapTest)
	ON_UPDATE_COMMAND_UI(ID_KINDMAP_TEST,&CKindMapView::OnUpdateKindmapTest)

END_MESSAGE_MAP()

// CKindMapView ����/����

CKindMapView::CKindMapView()
{
	// TODO: �ڴ˴���ӹ������
}

CKindMapView::~CKindMapView()
{
}

BOOL CKindMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return KGisView::PreCreateWindow(cs);
}

// CKindMapView ����

void CKindMapView::OnDraw(CDC* pDC)
{
	CKindMapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	KGisView::OnDraw(pDC);
	//-----------------------------------------------------
	HDC hDC  = pDC->GetSafeHdc();
	RECT rect;
	::GetClientRect(this->GetSafeHwnd(),&rect);

	// ����϶�ʱ����ʾ����
	if(!this->IsMouseDraging())
	{
		//-----------------------------
		// ���������ʾ������ͼ�����Ϣ
		//pDC->TextOut(0,0,_T("���������ʾ������ͼ�����Ϣ"));
		//-----------------------------
	}

	KDib* pDib = pDoc->GetDib();
	pDib->AttachHBITMAP(pDoc->GetMapEngine()->GetHBitmap());

	if(0)
	{
		Graphics* pGraphics = Graphics::FromHDC(hDC);

		HDC hBufDC = ::CreateCompatibleDC(hDC);
		HBITMAP hBitmap = ::CreateCompatibleBitmap(hDC, rect.right, rect.bottom);
		HBITMAP hOldBitmap = (HBITMAP)::SelectObject(hBufDC,hBitmap);

		::BitBlt(hBufDC,0,0,rect.right, rect.bottom,hDC,0,0,SRCCOPY);

		pDib->AttachHBITMAP(hBitmap);

		KImage* pImage = (KImage*)Bitmap::FromHBITMAP(hBitmap,NULL);
		//pImage->Save(_T("e:\\temp\\321.png"),KImage::ImageType::png);	// OK

		::SelectObject(hBufDC,hOldBitmap);
		::DeleteObject(hBitmap);
		::DeleteDC(hBufDC);

		_delete(pImage);
		_delete(pGraphics);
	}
	//-----------------------------------------------------
}


// CKindMapView ��ӡ


void CKindMapView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	//AFXPrintPreview(this);
#endif
	//AFXPrintPreview(this);
	KGisView::OnFilePrintPreview();
}

BOOL CKindMapView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	//return DoPreparePrinting(pInfo);
	return KGisView::OnPreparePrinting(pInfo);
}

void CKindMapView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
	KGisView::OnBeginPrinting(pDC, pInfo);
}

void CKindMapView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: ��Ӵ�ӡ����е��������
	KGisView::OnEndPrinting(pDC, pInfo);
}

void CKindMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	KGisView::OnRButtonUp(nFlags, point);
	//ClientToScreen(&point);
	//OnContextMenu(this, point);
}

void CKindMapView::OnContextMenu(CWnd* pWnd, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
	//KGisView::OnContextMenu(pWnd, point);
}


// CKindMapView ���

#ifdef _DEBUG
void CKindMapView::AssertValid() const
{
	KGisView::AssertValid();
}

void CKindMapView::Dump(CDumpContext& dc) const
{
	KGisView::Dump(dc);
}

CKindMapDoc* CKindMapView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKindMapDoc)));
	return (CKindMapDoc*)m_pDocument;
}
#endif //_DEBUG

// CKindMapView ��Ϣ�������

void CKindMapView::OnKindmapTest()
{
	//-----------------------------------------------------
	//ftp://172.18.152.243/Yaogan/���м�ҵ�����/
	//ftp://172.18.152.243/Yaogan/%B5%D8%CA%D0%BC%B6%D2%B5%CE%F1%C8%ED%BC%FE/
	//-----------------------------------------------------

	//AGMRS_PRODUCT_CODE_VIX_NDVI
	agmrsProductGenerate_Vix(AGMRS_PRODUCT_CODE_VIX_RVI);
	//agmrsProductGenerate_Vix(AGMRS_PRODUCT_CODE_VIX_DVI);
	//agmrsProductGenerate_Vix(AGMRS_PRODUCT_CODE_VIX_LAI);
	//agmrsProductGenerate_Lst(0);

	//MessageBox(_T("OnKindmapTest"));

	return;
}

void CKindMapView::OnUpdateKindmapTest(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_iGisTool == GisTool_Misce_Landmark_Calibrate);
}


void CKindMapView::agmrsProductGenerate_Lst(int iMethod)
{
	//-----------------------------------------------------
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();
	KLayer* pLayer = pLayerWnd->GetSelectedLayer();
	if(!pLayer)
	{
		AfxMessageBox(_T("û��ѡ������ͼ��"));
		return;
	}

	// �����ṩ��
	KProvider* pProvider = pLayer->GetProvider();
	if(!pProvider)
		return;

	// ���ݶ���
	KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
	if(!pAgmrsData)
		return;
	//-----------------------------------------------------
	// ���Ȳ���
	ProgressParam* pProgressParam = new ProgressParam();
	pProgressParam->iFlag = 0;
	pProgressParam->indexPaneProgress = 1;
	pProgressParam->indexPaneText = 2;
	pProgressParam->SetInfo(_T("�ر��¶ȷ���..."));

	// ע�������ʾ
	pAgmrsData->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam,TRUE);
	//-----------------------------------------------------
	// �ر��¶ȷ��ݴ���
	if(!pAgmrsData->Lst(iMethod))
	{
		// ��λ������ʾ
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}
		return;
	}

	// ��λ������ʾ
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
	if(pMFCStatusBar)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	// ��Ʒ�������Ʒ������Ϣ
	KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
	PFLOAT* ppfProduct = pAgmrsDataProcessor->GetProductData();
	KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

	// �����ļ���
	TCHAR szFile[MAX_PATH] = _T("");
	_stprintf(szFile,_T("C:\\TopPath\\Lst\\%s"),pAgmrsData->NameProduct(AGMRS_PRODUCT_CODE_MISC_LST));

	// ��ɫ��
	KPalette* pPalette = NULL;
	if(pPalette)
	{
		// ���õ�ɫ��͸��ɫ
		pPalette->HasTransparentColor() = FALSE;
		pPalette->TransparentColor() = RGB(0,0,0);
	}

	// �����ļ�
	if(!KAxinData::CreateFile(szFile,*ppfProduct,pProductDataInfo,pPalette))
	{
		CString strFile;
		strFile.Format(_T("�����ļ����� - %s"),szFile);
		AfxMessageBox(strFile);
	}
	else
	{
		// ���� ENVI �ļ�ͷ
		CString sf=szFile;
		sf.Replace(_T("axd"),_T("hdr"));
		KGridImageData gid;
		gid.LoadData(szFile);
		gid.ExportEnviHdr(sf);

		// ���ļ�
		theApp.OpenDocumentFile(szFile);
	}

	return;
}

void CKindMapView::agmrsProductGenerate_Vix(int iProductID)
{
	//-----------------------------------------------------
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();
	KLayer* pLayer = pLayerWnd->GetSelectedLayer();
	if(!pLayer)
	{
		AfxMessageBox(_T("û��ѡ������ͼ��"));
		return;
	}

	// ͼ������
	LayerType eLayerType = (LayerType)pLayer->GetType();

	// �Ƿ���ũҵң������
	BOOL bAgmrsDataLayer = FALSE
		|| eLayerType == LayerType_Avhrr
		|| eLayerType == LayerType_Modis
		|| eLayerType == LayerType_Mersi
		|| eLayerType == LayerType_Virr
		;

	// ͼ������
	if(!bAgmrsDataLayer)
	{
		AfxMessageBox(_T("û��ѡ����ȷ��ͼ�㡣\n\n������Ҫѡ�������ͼ��..."));
		return;
	}

	// �����ṩ��
	KProvider* pProvider = pLayer->GetProvider();
	if(!pProvider)
		return;

	// ���ݶ���
	KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
	if(!pAgmrsData)
		return;
	//-----------------------------------------------------
	// ���Ȳ���
	ProgressParam* pProgressParam = new ProgressParam();
	pProgressParam->iFlag = 0;
	pProgressParam->indexPaneProgress = 1;
	pProgressParam->indexPaneText = 2;
	pProgressParam->SetInfo(_T("ֲ��ָ������..."));

	// ע�������ʾ
	pAgmrsData->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam,TRUE);
	//-----------------------------------------------------
	// �ɺ�ָ�����ݴ���
	KStringArray sa;
	sa.Add(_T("0501"));
	KAgmrsAlgorithm* pAgmrsAlgorithm = (KAgmrsAlgorithm*)pAgmrsData->GetDataProcessor()->GetDataAlgorithm();
	pAgmrsAlgorithm->SetLaiConfig(sa);

	if(!pAgmrsData->Vix(iProductID))
	{
		// ��λ������ʾ
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}
		return;
	}

	// ��λ������ʾ
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
	if(pMFCStatusBar)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	// ȡ�ò�Ʒ�������Ʒ������Ϣ
	KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
	PFLOAT* ppfProduct = pAgmrsDataProcessor->GetProductData();
	KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

	// �����ļ���
	TCHAR szFile[MAX_PATH] = _T("");
	_stprintf(szFile,_T("C:\\TopPath\\Vix\\%s"),pAgmrsData->NameProduct(iProductID));

	// ��ɫ��
	KPalette* pPalette = NULL;
	pPalette = KAgmrsData::GetAgmrsProductPalette(iProductID);
	if(pPalette)
	{
		// ���õ�ɫ��͸��ɫ
		pPalette->HasTransparentColor() = FALSE;
		pPalette->TransparentColor() = RGB(0,0,0);
	}

	// �����ļ�
	if(!KAxinData::CreateFile(szFile,*ppfProduct,pProductDataInfo,pPalette))
	{
		CString strFile;
		strFile.Format(_T("�����ļ����� - %s"),szFile);
		AfxMessageBox(strFile);
	}
	else
	{
		// ���ļ�
		theApp.OpenDocumentFile(szFile);
	}

	return;
}

void CKindMapView::agmrsProductGenerate_Misc(int iProductID)
{
	//-----------------------------------------------------
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	KLayerDockablePane* pLayerWnd = (KLayerDockablePane*)pMainFrame->GetLayerWnd();
	KLayer* pLayer = pLayerWnd->GetSelectedLayer();
	if(!pLayer)
	{
		AfxMessageBox(_T("û��ѡ������ͼ��"));
		return;
	}

	// ͼ������
	const int iLayerType = pLayer->GetType();
	if( TRUE
		&& iLayerType != LayerType_Avhrr
		&& iLayerType != LayerType_Modis
		&& iLayerType != LayerType_Mersi
		&& iLayerType != LayerType_Virr
		)
	{
		AfxMessageBox(_T("û��ѡ����ȷ��ͼ�㡣\n\n������Ҫѡ��ң������ͼ��..."));
		return;
	}

	// �����ṩ��
	KProvider* pProvider = pLayer->GetProvider();
	if(!pProvider)
		return;

	// ���ݶ���
	KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
	if(!pAgmrsData)
		return;

	pAgmrsData->GetDataProcessor()->SetFlag(1);
	//-----------------------------------------------------
	// ���Ȳ���
	ProgressParam* pProgressParam = new ProgressParam();
	pProgressParam->iFlag = 0;
	pProgressParam->indexPaneProgress = 1;
	pProgressParam->indexPaneText = 2;
	pProgressParam->SetInfo(_T("���������Ŀ..."));

	// ע�������ʾ
	pAgmrsData->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam,TRUE);
	//-----------------------------------------------------
	// �����⴦��
	if(!pAgmrsData->Sim(iProductID))
	{
		// ��λ������ʾ
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}
		return;
	}
	//-----------------------------------------------------
	// ��λ������ʾ
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
	if(pMFCStatusBar)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	// ��Ʒ�������Ʒ������Ϣ
	KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
	PFLOAT* ppfProduct = pAgmrsDataProcessor->GetProductData();
	KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();			// ��Ʒ��Ϣ

	FirePointVector* pFirePointVector = pAgmrsDataProcessor->GetFirePointVector();	// ��㼯��
	//-----------------------------------------------------
	CString strFireCount;
	strFireCount.Format(_T("������� - %d"),pFirePointVector->size());
	AfxMessageBox(strFireCount);
	//-----------------------------------------------------
	return;
}
