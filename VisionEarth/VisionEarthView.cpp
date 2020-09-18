// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面 
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和 
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问  
// http://msdn.microsoft.com/officeui。
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// VisionEarthView.cpp : CVisionEarthView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CVisionEarthView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_MESSAGE_VOID(WM_USER_APPIDLE,CVisionEarthView::OnAppIdle)

	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// CVisionEarthView 构造/析构

CVisionEarthView::CVisionEarthView()
{
	// TODO: 在此处添加构造代码

}

CVisionEarthView::~CVisionEarthView()
{
}

BOOL CVisionEarthView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CVisionEarthView 绘制

void CVisionEarthView::OnDraw(CDC* /*pDC*/)
{
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 渲染引擎
	KRenderEngine* pRenderEngine = pDoc->GetRenderEngine();
	if ( pRenderEngine == NULL )
		return;

	pRenderEngine->Render();
	return;
}


// CVisionEarthView 打印


void CVisionEarthView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CVisionEarthView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CVisionEarthView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CVisionEarthView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CVisionEarthView 诊断

#ifdef _DEBUG
void CVisionEarthView::AssertValid() const
{
	CView::AssertValid();
}

void CVisionEarthView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CVisionEarthDoc* CVisionEarthView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CVisionEarthDoc)));
	return (CVisionEarthDoc*)m_pDocument;
}
#endif //_DEBUG


// CVisionEarthView 消息处理程序

void CVisionEarthView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 渲染引擎
	KRenderEngine* pRenderEngine = pDoc->GetRenderEngine();
	if ( pRenderEngine == NULL )
		return;

	// 设置使用地图空间的视图窗口句柄
	pRenderEngine->SetHwnd(this->GetSafeHwnd());

	// 初始化
	pRenderEngine->InitD3D();

	return;
}

sfx_msg void CVisionEarthView::OnAppIdle(WPARAM wParam, LPARAM lParam)
{
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 渲染引擎
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
	// 由于睡眠所造成的系统资源消耗时间
	const float sleepOverHeadSeconds = 2e-3f;

	// Overhead associated with displaying the frame
	// 由于显示渲染所造成的系统消耗时间
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

	// TODO: 在此处添加消息处理程序代码
	CVisionEarthDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// 渲染引擎
	KRenderEngine* pRenderEngine = pDoc->GetRenderEngine();
	if ( pRenderEngine == NULL )
		return;

	pRenderEngine->Render();
	pRenderEngine->Present();
	return;
}


BOOL CVisionEarthView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//return CView::OnEraseBkgnd(pDC);
	return TRUE;
}
