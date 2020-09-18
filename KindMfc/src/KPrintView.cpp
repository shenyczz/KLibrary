// KPrintView.cpp : 实现文件
//

#include "stdafx.h"
#include "KDocument.h"
#include "KPrintView.h"

// KPrintView

namespace Framework
{

	IMPLEMENT_DYNCREATE(KPrintView, KView)

	KPrintView::KPrintView()
	{
	}

	KPrintView::~KPrintView()
	{
	}

	BEGIN_MESSAGE_MAP(KPrintView, KView)
	END_MESSAGE_MAP()


	// KPrintView 绘图

	void KPrintView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: 在此添加绘制代码
	}


	// KPrintView 诊断

	#ifdef _DEBUG
	void KPrintView::AssertValid() const
	{
		KView::AssertValid();
	}

	#ifndef _WIN32_WCE
	void KPrintView::Dump(CDumpContext& dc) const
	{
		KView::Dump(dc);
	}
	#endif
	#endif //_DEBUG


	// KPrintView 消息处理程序

	void KPrintView::OnFilePrint()
	{
		// 在最终的视图中取得 m_Dib
		//CDC* pDC = GetDC();
		//CRect rcSave(0,0,0,0);
		//this->GetClientRect(&rcSave);
		//m_Dib.Capture(pDC->GetSafeHdc(),rcSave);
		//ReleaseDC(pDC);

		//must be called -- comment by shenyc
		KView::OnFilePrint();
	}

	void KPrintView::OnFilePrintPreview()
	{
		// 在最终的视图中取得 m_Dib
		//CDC* pDC = GetDC();
		//CRect rcSave;
		//this->GetClientRect(&rcSave);
		//m_Dib.Capture(pDC->GetSafeHdc(),rcSave);
		//ReleaseDC(pDC);

		//must be called -- comment by shenyc
		//KView::OnFilePrintPreview();
		AFXPrintPreview(this);
	}

	/**********************************************************
	打印时调用顺序
		1. OnPreparePrinting	--> 在打印对话框显示之前调用
		2. OnBeginPrinting		--> 在打印对话框退出之后,打印之前调用
		3. OnPrepareDC			--> 为打印设置映射模式等绘图属性
		4. OnPrint				--> 调用OnDraw函数
		5. OnEndPrinting		--> 在打印完毕之后调用

	  显示时OnPaint函数调用OnDraw, DC 为显示设备环境

	  打印时OnPrint函数调用OnDraw, DC 为打印设备环境
	  打印预览时OnPrint函数调用OnDraw, DC 为CPreviewDC ,这些由框架自动设置

	  向导生成时,已经提供打印、打印预览、打印设置的缺省功能
		ON_COMMAND(ID_FILE_PRINT, KView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, KView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, KView::OnFilePrintPreview)
  
		用户可重新设计这些函数,以满足需要.
	**********************************************************/


	//1. OnPreparePrinting	--> 在打印对话框显示之前调用
	BOOL KPrintView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// 设置打印页为1页
		pInfo->SetMaxPage(1);

		// 打印对话框结构
		PRINTDLG pd;
		::ZeroMemory(&pd, sizeof(PRINTDLG));
		// 得到打印设备
		AfxGetApp()->GetPrinterDeviceDefaults(&pd);

		// 设备模式结构
		DEVMODE* pDV = (LPDEVMODE) ::GlobalLock(pd.hDevMode);
		//pDV->dmOrientation=DMORIENT_PORTRAIT;		//DMORIENT_PORTRAIT
		//pDV->dmOrientation=DMORIENT_LANDSCAPE;		//横向DMORIENT_LANDSCAPE
		pInfo->m_pPD->m_pd.hDevMode = pDV;
		GlobalUnlock(pd.hDevMode);

		//显示标准打印对话框
		return DoPreparePrinting(pInfo);
		//return KView::OnPreparePrinting(pInfo);
	}

	//2. OnBeginPrinting		--> 在打印对话框退出之后,打印之前调用
	void KPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
	{
		KView::OnBeginPrinting(pDC, pInfo);
	}

	//3. OnPrepareDC			--> 为打印设置映射模式等绘图属性
	void KPrintView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
	{
		KView::OnPrepareDC(pDC, pInfo);

		// TODO: 在此添加专用代码和/或调用基类
		if(pDC->IsPrinting())
		{
			pDC->SetMapMode(MM_LOENGLISH);		//映射模式
		}

		return;
	}

	//4. OnPrint				--> 调用OnDraw函数
	void KPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
	{
		//KView::OnPrint(pDC, pInfo);

		//m_dib 必须在 OnFilePrint() 或者 OnFilePrintPreview() 抓屏
		//m_Dib.Print(pDC->GetSafeHdc());		//屏幕打印

		KDocument* pDoc = (KDocument*)this->GetDocument();
		if(!pDoc)
			return;

		KDib* pDib = pDoc->GetDib();
		if(pDib->IsEmpty())
			return;


		DWORD dwWidth = pDib->Width();		// DIB 宽度
		DWORD dwHeight = pDib->Height();	// DIB 高度

		// DIB头尺寸
		DWORD dwDIBHeadSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * pDib->ColorTableEntries();

		LPBITMAPINFOHEADER lpBitmapInfoHeader = pDib->BitmapInfoHeader();

		//数据指针
		LPSTR lpDIBBits = (LPSTR)pDib->ImageData();	//only image data

		//计算可打印区域,要求各边有1/8英寸空隙 1 inch=25.4mm
		//在MM_LOENGLISH下
		int cxBorder = pDC->GetDeviceCaps(LOGPIXELSX) / 8;
		int cyBorder = pDC->GetDeviceCaps(LOGPIXELSY) / 8;
		TRACE("cxBorder=%d cyborder=%d\n",cxBorder,cyBorder);

		CSize size;
		//取得打印区域的宽度和高度(象素)	A4=210*297mm
		size.cx = pDC->GetDeviceCaps(HORZRES);
		size.cy = pDC->GetDeviceCaps(VERTRES);
		//TRACE("Pixel cxPage=%d cyPage=%d\n",size.cx,size.cy);	//1488*2104
		pDC->DPtoLP(&size);
		//TRACE("LP cxPage=%d cyPage=%d\n",size.cx,size.cy);		//827*1169

		int cxPage = size.cx;	// 页宽
		int cyPage = size.cy;	// 页高

		if(cxPage < cyPage)
		{
			cyPage=(int)(((double)cxPage / (double)dwWidth) * (double)dwHeight);
		}

		//伸展位图以充满打印纸
		pDC->SetStretchBltMode(COLORONCOLOR);
		::StretchDIBits(pDC->GetSafeHdc(),
						cxBorder,
						-cyBorder,
						cxPage-cxBorder*2,
						-(cyPage-cyBorder*2),
						0,0,
						dwWidth,
						dwHeight,
						lpDIBBits,
						(LPBITMAPINFO)lpBitmapInfoHeader,
						DIB_RGB_COLORS,
						SRCCOPY
						);

	}

	//5. OnEndPrinting		--> 在打印完毕之后调用
	void KPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
	{
		KView::OnEndPrinting(pDC, pInfo);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
