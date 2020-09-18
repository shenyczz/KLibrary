//////////////////////////////////////////////////////////////////////
// GView.h : header file
//
// �������ƣ�ͼ����ͼ��
// �汾˵����
// �� �� �ţ�
// ��    �ܣ�
// �� �� �ˣ�������(shenyc@371.net)
// ����ʱ�䣺2000-2006
// �� �� �ߣ�
// �޸�ʱ�䣺
// �޸�˵����
// ��    ������CGDocument���ʹ��
//--------------------------------------------------------------------
// �÷�:
//
// һ��Ҫ����������ͼ���а�
//	ON_COMMAND(ID_FILE_PRINT, KView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, KView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, KView::OnFilePrintPreview)
//
//	�ĳ�
//
//	ON_COMMAND(ID_FILE_PRINT, CGView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_DIRECT, CGView::OnFilePrint)
//	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CGView::OnFilePrintPreview)
//
// �����޸�������ͼ��һ�º���
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
// �����ڹ�����ͼ�е� OnDraw(CDC* pDC)
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

// KPrintView ��ͼ

namespace Framework
{

	class KIND_EXT_CLASS KPrintView : public KView
	{
		DECLARE_DYNCREATE(KPrintView)

	protected:
		KPrintView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KPrintView();

	public:
		virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		//��ӡʱ����˳��
		//	1. OnPreparePrinting	--> �ڴ�ӡ�Ի�����ʾ֮ǰ����
		//	2. OnBeginPrinting		--> �ڴ�ӡ�Ի����˳�֮��,��ӡ֮ǰ����
		//	3. OnPrepareDC			--> Ϊ��ӡ����ӳ��ģʽ�Ȼ�ͼ����
		//	4. OnPrint				--> ����OnDraw����
		//	5. OnEndPrinting		--> �ڴ�ӡ���֮�����
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
*/
