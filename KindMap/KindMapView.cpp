
// KindMapView.cpp : CKindMapView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &KGisView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &KGisView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CKindMapView::OnFilePrintPreview)
	ON_WM_RBUTTONUP()

	//ON_WM_CONTEXTMENU()	// 屏蔽

	ON_COMMAND(ID_KINDMAP_TEST, &CKindMapView::OnKindmapTest)
	ON_UPDATE_COMMAND_UI(ID_KINDMAP_TEST,&CKindMapView::OnUpdateKindmapTest)

END_MESSAGE_MAP()

// CKindMapView 构造/析构

CKindMapView::CKindMapView()
{
	// TODO: 在此处添加构造代码
}

CKindMapView::~CKindMapView()
{
}

BOOL CKindMapView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return KGisView::PreCreateWindow(cs);
}

// CKindMapView 绘制

void CKindMapView::OnDraw(CDC* pDC)
{
	CKindMapDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	KGisView::OnDraw(pDC);
	//-----------------------------------------------------
	HDC hDC  = pDC->GetSafeHdc();
	RECT rect;
	::GetClientRect(this->GetSafeHwnd(),&rect);

	// 解决拖动时的显示混乱
	if(!this->IsMouseDraging())
	{
		//-----------------------------
		// 这里可以显示不属于图层的信息
		//pDC->TextOut(0,0,_T("这里可以显示不属于图层的信息"));
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


// CKindMapView 打印


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
	// 默认准备
	//return DoPreparePrinting(pInfo);
	return KGisView::OnPreparePrinting(pInfo);
}

void CKindMapView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 添加额外的打印前进行的初始化过程
	KGisView::OnBeginPrinting(pDC, pInfo);
}

void CKindMapView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// TODO: 添加打印后进行的清理过程
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


// CKindMapView 诊断

#ifdef _DEBUG
void CKindMapView::AssertValid() const
{
	KGisView::AssertValid();
}

void CKindMapView::Dump(CDumpContext& dc) const
{
	KGisView::Dump(dc);
}

CKindMapDoc* CKindMapView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CKindMapDoc)));
	return (CKindMapDoc*)m_pDocument;
}
#endif //_DEBUG

// CKindMapView 消息处理程序

void CKindMapView::OnKindmapTest()
{
	//-----------------------------------------------------
	//ftp://172.18.152.243/Yaogan/地市级业务软件/
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
		AfxMessageBox(_T("没有选择数据图层"));
		return;
	}

	// 数据提供者
	KProvider* pProvider = pLayer->GetProvider();
	if(!pProvider)
		return;

	// 数据对象
	KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
	if(!pAgmrsData)
		return;
	//-----------------------------------------------------
	// 进度参数
	ProgressParam* pProgressParam = new ProgressParam();
	pProgressParam->iFlag = 0;
	pProgressParam->indexPaneProgress = 1;
	pProgressParam->indexPaneText = 2;
	pProgressParam->SetInfo(_T("地表温度反演..."));

	// 注册进度显示
	pAgmrsData->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam,TRUE);
	//-----------------------------------------------------
	// 地表温度反演处理
	if(!pAgmrsData->Lst(iMethod))
	{
		// 复位进度显示
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}
		return;
	}

	// 复位进度显示
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
	if(pMFCStatusBar)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	// 产品数据与产品数据信息
	KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
	PFLOAT* ppfProduct = pAgmrsDataProcessor->GetProductData();
	KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

	// 构造文件名
	TCHAR szFile[MAX_PATH] = _T("");
	_stprintf(szFile,_T("C:\\TopPath\\Lst\\%s"),pAgmrsData->NameProduct(AGMRS_PRODUCT_CODE_MISC_LST));

	// 调色板
	KPalette* pPalette = NULL;
	if(pPalette)
	{
		// 设置调色板透明色
		pPalette->HasTransparentColor() = FALSE;
		pPalette->TransparentColor() = RGB(0,0,0);
	}

	// 保存文件
	if(!KAxinData::CreateFile(szFile,*ppfProduct,pProductDataInfo,pPalette))
	{
		CString strFile;
		strFile.Format(_T("创建文件错误 - %s"),szFile);
		AfxMessageBox(strFile);
	}
	else
	{
		// 导出 ENVI 文件头
		CString sf=szFile;
		sf.Replace(_T("axd"),_T("hdr"));
		KGridImageData gid;
		gid.LoadData(szFile);
		gid.ExportEnviHdr(sf);

		// 打开文件
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
		AfxMessageBox(_T("没有选择数据图层"));
		return;
	}

	// 图层类型
	LayerType eLayerType = (LayerType)pLayer->GetType();

	// 是否是农业遥感数据
	BOOL bAgmrsDataLayer = FALSE
		|| eLayerType == LayerType_Avhrr
		|| eLayerType == LayerType_Modis
		|| eLayerType == LayerType_Mersi
		|| eLayerType == LayerType_Virr
		;

	// 图层类型
	if(!bAgmrsDataLayer)
	{
		AfxMessageBox(_T("没有选择正确的图层。\n\n可能需要选择干数据图层..."));
		return;
	}

	// 数据提供者
	KProvider* pProvider = pLayer->GetProvider();
	if(!pProvider)
		return;

	// 数据对象
	KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
	if(!pAgmrsData)
		return;
	//-----------------------------------------------------
	// 进度参数
	ProgressParam* pProgressParam = new ProgressParam();
	pProgressParam->iFlag = 0;
	pProgressParam->indexPaneProgress = 1;
	pProgressParam->indexPaneText = 2;
	pProgressParam->SetInfo(_T("植被指数反演..."));

	// 注册进度显示
	pAgmrsData->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam,TRUE);
	//-----------------------------------------------------
	// 干旱指数反演处理
	KStringArray sa;
	sa.Add(_T("0501"));
	KAgmrsAlgorithm* pAgmrsAlgorithm = (KAgmrsAlgorithm*)pAgmrsData->GetDataProcessor()->GetDataAlgorithm();
	pAgmrsAlgorithm->SetLaiConfig(sa);

	if(!pAgmrsData->Vix(iProductID))
	{
		// 复位进度显示
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}
		return;
	}

	// 复位进度显示
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
	if(pMFCStatusBar)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	// 取得产品数据与产品数据信息
	KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
	PFLOAT* ppfProduct = pAgmrsDataProcessor->GetProductData();
	KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();

	// 构造文件名
	TCHAR szFile[MAX_PATH] = _T("");
	_stprintf(szFile,_T("C:\\TopPath\\Vix\\%s"),pAgmrsData->NameProduct(iProductID));

	// 调色板
	KPalette* pPalette = NULL;
	pPalette = KAgmrsData::GetAgmrsProductPalette(iProductID);
	if(pPalette)
	{
		// 设置调色板透明色
		pPalette->HasTransparentColor() = FALSE;
		pPalette->TransparentColor() = RGB(0,0,0);
	}

	// 保存文件
	if(!KAxinData::CreateFile(szFile,*ppfProduct,pProductDataInfo,pPalette))
	{
		CString strFile;
		strFile.Format(_T("创建文件错误 - %s"),szFile);
		AfxMessageBox(strFile);
	}
	else
	{
		// 打开文件
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
		AfxMessageBox(_T("没有选择数据图层"));
		return;
	}

	// 图层类型
	const int iLayerType = pLayer->GetType();
	if( TRUE
		&& iLayerType != LayerType_Avhrr
		&& iLayerType != LayerType_Modis
		&& iLayerType != LayerType_Mersi
		&& iLayerType != LayerType_Virr
		)
	{
		AfxMessageBox(_T("没有选择正确的图层。\n\n可能需要选择遥感数据图层..."));
		return;
	}

	// 数据提供者
	KProvider* pProvider = pLayer->GetProvider();
	if(!pProvider)
		return;

	// 数据对象
	KAgmrsData* pAgmrsData = (KAgmrsData*)pProvider->GetDataObject();
	if(!pAgmrsData)
		return;

	pAgmrsData->GetDataProcessor()->SetFlag(1);
	//-----------------------------------------------------
	// 进度参数
	ProgressParam* pProgressParam = new ProgressParam();
	pProgressParam->iFlag = 0;
	pProgressParam->indexPaneProgress = 1;
	pProgressParam->indexPaneText = 2;
	pProgressParam->SetInfo(_T("其他检测项目..."));

	// 注册进度显示
	pAgmrsData->RegiestCallbackFunc(KCallbackFunc::ShowProgressCallback,pProgressParam,TRUE);
	//-----------------------------------------------------
	// 杂项检测处理
	if(!pAgmrsData->Sim(iProductID))
	{
		// 复位进度显示
		CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
		if(pMFCStatusBar)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}
		return;
	}
	//-----------------------------------------------------
	// 复位进度显示
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
	if(pMFCStatusBar)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	// 产品数据与产品数据信息
	KAgmrsDataProcessor* pAgmrsDataProcessor = (KAgmrsDataProcessor*)pAgmrsData->GetDataProcessor();
	PFLOAT* ppfProduct = pAgmrsDataProcessor->GetProductData();
	KDataInfo* pProductDataInfo = pAgmrsData->GetProductDataInfo();			// 产品信息

	FirePointVector* pFirePointVector = pAgmrsDataProcessor->GetFirePointVector();	// 火点集合
	//-----------------------------------------------------
	CString strFireCount;
	strFireCount.Format(_T("火点数量 - %d"),pFirePointVector->size());
	AfxMessageBox(strFireCount);
	//-----------------------------------------------------
	return;
}
