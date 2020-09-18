//////////////////////////////////////////////////////////////////////
// GView.h : header file
//
// 程序名称：图形视图类
// 版本说明：
// 版 本 号：
// 功    能：
// 开 发 人：申永辰(shenyc@371.net)
// 开发时间：2000-2006
// 修 改 者：
// 修改时间：
// 修改说明：
// 其    他：和CGDocument配合使用
//--------------------------------------------------------------------
// 用法:
//
// 一、要在派生的视图类中把
//	ON_COMMAND(ID_FILE_PRINT, KView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, KView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, KView::OnFilePrintPreview)
//
//	改成
//
//	ON_COMMAND(ID_FILE_PRINT, CGView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CGView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CGView::OnFilePrintPreview)
//
// 二、修改派生视图的一下函数
//	  1.OnPreparePrinting()
//		{
//			return CGView::OnPreparePrinting(pInfo);
//		}
//
//    2.OnBeginPrinting()
//      {
//			CGView::OnBeginPrinting(pDC, pInfo);
//      }
//
//    3.OnEndPrinting()
//      {
//			CGView::OnEndPrinting(pDC, pInfo);
//      }
//
// 三、在工程视图中的 OnDraw(CDC* pDC)
//
//		void CMeteorView::OnDraw(CDC* pDC)
//		{
//			CMeteorDoc* pDoc = GetDocument();
//			ASSERT_VALID(pDoc);
//			// TODO: add draw code for native data here
//			//-----------------------------------------
//			CGView::OnDraw(pDC);
//			//-----------------------------------------
//		}
//
////////////////////////////////////////////////////////////////////////
#pragma once

#include "KView.h"

// KPrintView 视图

namespace Framework
{

	class KIND_EXT_CLASS KPrintView : public KView
	{
		DECLARE_DYNCREATE(KPrintView)

	protected:
		KPrintView();           // 动态创建所使用的受保护的构造函数
		virtual ~KPrintView();

	public:
		virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		//打印时调用顺序
		//	1. OnPreparePrinting	--> 在打印对话框显示之前调用
		//	2. OnBeginPrinting		--> 在打印对话框退出之后,打印之前调用
		//	3. OnPrepareDC			--> 为打印设置映射模式等绘图属性
		//	4. OnPrint				--> 调用OnDraw函数
		//	5. OnEndPrinting		--> 在打印完毕之后调用
		virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
		virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
		virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
		virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
		virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	protected:

		// not mapped commands - must be mapped in derived class
		afx_msg void OnFilePrint();
		afx_msg void OnFilePrintPreview();

		DECLARE_MESSAGE_MAP()
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------


/*
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
*/
