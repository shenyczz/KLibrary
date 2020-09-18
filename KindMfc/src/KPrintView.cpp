// KPrintView.cpp : ʵ���ļ�
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


	// KPrintView ��ͼ

	void KPrintView::OnDraw(CDC* pDC)
	{
		CDocument* pDoc = GetDocument();
		// TODO: �ڴ���ӻ��ƴ���
	}


	// KPrintView ���

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


	// KPrintView ��Ϣ�������

	void KPrintView::OnFilePrint()
	{
		// �����յ���ͼ��ȡ�� m_Dib
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
		// �����յ���ͼ��ȡ�� m_Dib
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
	��ӡʱ����˳��
		1. OnPreparePrinting	--> �ڴ�ӡ�Ի�����ʾ֮ǰ����
		2. OnBeginPrinting		--> �ڴ�ӡ�Ի����˳�֮��,��ӡ֮ǰ����
		3. OnPrepareDC			--> Ϊ��ӡ����ӳ��ģʽ�Ȼ�ͼ����
		4. OnPrint				--> ����OnDraw����
		5. OnEndPrinting		--> �ڴ�ӡ���֮�����

	  ��ʾʱOnPaint��������OnDraw, DC Ϊ��ʾ�豸����

	  ��ӡʱOnPrint��������OnDraw, DC Ϊ��ӡ�豸����
	  ��ӡԤ��ʱOnPrint��������OnDraw, DC ΪCPreviewDC ,��Щ�ɿ���Զ�����

	  ������ʱ,�Ѿ��ṩ��ӡ����ӡԤ������ӡ���õ�ȱʡ����
		ON_COMMAND(ID_FILE_PRINT, KView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_DIRECT, KView::OnFilePrint)
		ON_COMMAND(ID_FILE_PRINT_PREVIEW, KView::OnFilePrintPreview)
  
		�û������������Щ����,��������Ҫ.
	**********************************************************/


	//1. OnPreparePrinting	--> �ڴ�ӡ�Ի�����ʾ֮ǰ����
	BOOL KPrintView::OnPreparePrinting(CPrintInfo* pInfo)
	{
		// ���ô�ӡҳΪ1ҳ
		pInfo->SetMaxPage(1);

		// ��ӡ�Ի���ṹ
		PRINTDLG pd;
		::ZeroMemory(&pd, sizeof(PRINTDLG));
		// �õ���ӡ�豸
		AfxGetApp()->GetPrinterDeviceDefaults(&pd);

		// �豸ģʽ�ṹ
		DEVMODE* pDV = (LPDEVMODE) ::GlobalLock(pd.hDevMode);
		//pDV->dmOrientation=DMORIENT_PORTRAIT;		//DMORIENT_PORTRAIT
		//pDV->dmOrientation=DMORIENT_LANDSCAPE;		//����DMORIENT_LANDSCAPE
		pInfo->m_pPD->m_pd.hDevMode = pDV;
		GlobalUnlock(pd.hDevMode);

		//��ʾ��׼��ӡ�Ի���
		return DoPreparePrinting(pInfo);
		//return KView::OnPreparePrinting(pInfo);
	}

	//2. OnBeginPrinting		--> �ڴ�ӡ�Ի����˳�֮��,��ӡ֮ǰ����
	void KPrintView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
	{
		KView::OnBeginPrinting(pDC, pInfo);
	}

	//3. OnPrepareDC			--> Ϊ��ӡ����ӳ��ģʽ�Ȼ�ͼ����
	void KPrintView::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo)
	{
		KView::OnPrepareDC(pDC, pInfo);

		// TODO: �ڴ����ר�ô����/����û���
		if(pDC->IsPrinting())
		{
			pDC->SetMapMode(MM_LOENGLISH);		//ӳ��ģʽ
		}

		return;
	}

	//4. OnPrint				--> ����OnDraw����
	void KPrintView::OnPrint(CDC* pDC, CPrintInfo* pInfo)
	{
		//KView::OnPrint(pDC, pInfo);

		//m_dib ������ OnFilePrint() ���� OnFilePrintPreview() ץ��
		//m_Dib.Print(pDC->GetSafeHdc());		//��Ļ��ӡ

		KDocument* pDoc = (KDocument*)this->GetDocument();
		if(!pDoc)
			return;

		KDib* pDib = pDoc->GetDib();
		if(pDib->IsEmpty())
			return;


		DWORD dwWidth = pDib->Width();		// DIB ���
		DWORD dwHeight = pDib->Height();	// DIB �߶�

		// DIBͷ�ߴ�
		DWORD dwDIBHeadSize = sizeof(BITMAPINFOHEADER) + sizeof(RGBQUAD) * pDib->ColorTableEntries();

		LPBITMAPINFOHEADER lpBitmapInfoHeader = pDib->BitmapInfoHeader();

		//����ָ��
		LPSTR lpDIBBits = (LPSTR)pDib->ImageData();	//only image data

		//����ɴ�ӡ����,Ҫ�������1/8Ӣ���϶ 1 inch=25.4mm
		//��MM_LOENGLISH��
		int cxBorder = pDC->GetDeviceCaps(LOGPIXELSX) / 8;
		int cyBorder = pDC->GetDeviceCaps(LOGPIXELSY) / 8;
		TRACE("cxBorder=%d cyborder=%d\n",cxBorder,cyBorder);

		CSize size;
		//ȡ�ô�ӡ����Ŀ�Ⱥ͸߶�(����)	A4=210*297mm
		size.cx = pDC->GetDeviceCaps(HORZRES);
		size.cy = pDC->GetDeviceCaps(VERTRES);
		//TRACE("Pixel cxPage=%d cyPage=%d\n",size.cx,size.cy);	//1488*2104
		pDC->DPtoLP(&size);
		//TRACE("LP cxPage=%d cyPage=%d\n",size.cx,size.cy);		//827*1169

		int cxPage = size.cx;	// ҳ��
		int cyPage = size.cy;	// ҳ��

		if(cxPage < cyPage)
		{
			cyPage=(int)(((double)cxPage / (double)dwWidth) * (double)dwHeight);
		}

		//��չλͼ�Գ�����ӡֽ
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

	//5. OnEndPrinting		--> �ڴ�ӡ���֮�����
	void KPrintView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
	{
		KView::OnEndPrinting(pDC, pInfo);
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
