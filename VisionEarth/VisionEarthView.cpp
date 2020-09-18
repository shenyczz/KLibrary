// ��� MFC ʾ��Դ������ʾ���ʹ�� MFC Microsoft Office Fluent �û����� 
// (��Fluent UI��)����ʾ�������ο���
// ���Բ��䡶Microsoft ������ο����� 
// MFC C++ ������渽����ص����ĵ���
// ���ơ�ʹ�û�ַ� Fluent UI ����������ǵ����ṩ�ġ�
// ��Ҫ�˽��й� Fluent UI ��ɼƻ�����ϸ��Ϣ�������  
// http://msdn.microsoft.com/officeui��
//
// ��Ȩ����(C) Microsoft Corporation
// ��������Ȩ����

// VisionEarthView.cpp : CVisionEarthView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "VisionEarth.h"
#endif

#include "VisionEarthDoc.h"
#include "VisionEarthView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CVisionEarthView

IMPLEMENT_DYNCREATE(CVisionEarthView, CView)

BEGIN_MESSAGE_MAP(CVisionEarthView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CVisionEarthView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_MESSAGE_VOID(WM_USER_APPIDLE,CVisionEarthView::OnAppIdle)

	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CVisionEarthView ����/����

CVisionEarthView::CVisionEarthView()
{
	// TODO: �ڴ˴���ӹ������

}

CVisionEarthView::~CVisionEarthView()
{
}

BOOL CVisionEarthView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CVisionEarthView ����

void CVisionEarthView::OnDraw(CDC* /*pDC*/)
{
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ��Ⱦ����
	KRenderEngine* pRenderEngine = pDoc->GetRenderEngine();
	if ( pRenderEngine == NULL )
		return;

	pRenderEngine->Render();
	return;
}


// CVisionEarthView ��ӡ


void CVisionEarthView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CVisionEarthView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CVisionEarthView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CVisionEarthView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CVisionEarthView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CVisionEarthView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CVisionEarthView ���

#ifdef _DEBUG
void CVisionEarthView::AssertValid() const
{
	CView::AssertValid();
}

void CVisionEarthView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVisionEarthDoc* CVisionEarthView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisionEarthDoc)));
	return (CVisionEarthDoc*)m_pDocument;
}
#endif //_DEBUG


// CVisionEarthView ��Ϣ�������

void CVisionEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ��Ⱦ����
	KRenderEngine* pRenderEngine = pDoc->GetRenderEngine();
	if ( pRenderEngine == NULL )
		return;

	// ����ʹ�õ�ͼ�ռ����ͼ���ھ��
	pRenderEngine->SetHwnd(this->GetSafeHwnd());

	// ��ʼ��
	pRenderEngine->InitD3D();

	return;
}

sfx_msg void CVisionEarthView::OnAppIdle(WPARAM wParam, LPARAM lParam)
{
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ��Ⱦ����
	KRenderEngine* pRenderEngine = pDoc->GetRenderEngine();
	if ( pRenderEngine == NULL )
		return;

	try
	{
		pRenderEngine->Render();
	}
	catch (CException*)
	{
	}

	// Flip
	pRenderEngine->Present();
	return;

	/*
	if (Device3d == null)
		return;

	// Sleep will always overshoot by a bit so under-sleep by
	// 2ms in the hopes of never oversleeping.
	// ����˯������ɵ�ϵͳ��Դ����ʱ��
	const float sleepOverHeadSeconds = 2e-3f;

	// Overhead associated with displaying the frame
	// ������ʾ��Ⱦ����ɵ�ϵͳ����ʱ��
	const float presentOverheadSeconds = 0;//3e-4f;

	try
	{
		if (!this.DrawContent.Focused)
			this.DrawContent.Focus();

		while (InteropHelper.IsAppStillIdle)
		{
			if (!World.Settings.AlwaysRenderWindow && IsRenderDisabled && !World.Settings.CameraHasMomentum)
				return;

			Render();

			if (World.Settings.ThrottleFpsHz > 0)
			{
				// optionally throttle the frame rate (to get consistent frame
				// rates or reduce CPU usage.
				float frameSeconds = 1.0f / World.Settings.ThrottleFpsHz - presentOverheadSeconds;

				// Sleep for remaining period of time until next render
				float sleepSeconds = frameSeconds - sleepOverHeadSeconds - this.SecondsSinceLastFrame;
				if (sleepSeconds > 0)
				{
					// Don't sleep too long. We don't know the accuracy of Thread.Sleep
					Thread.Sleep((int)(1000 * sleepSeconds));

					// Burn off what little time still remains at 100% CPU load
					while (this.SecondsSinceLastFrame < frameSeconds)
					{
						// Patience
					}
				}
			}
			// Flip
			this.Present();
		}
	}
	catch (Exception)
	{

	}
	*/
}


void CVisionEarthView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: �ڴ˴������Ϣ����������
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// ��Ⱦ����
	KRenderEngine* pRenderEngine = pDoc->GetRenderEngine();
	if ( pRenderEngine == NULL )
		return;

	pRenderEngine->Render();
	pRenderEngine->Present();
	return;
}


BOOL CVisionEarthView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
