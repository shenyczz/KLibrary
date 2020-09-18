
// KindMapView.h : CKindMapView 类的接口
//

#pragma once


class CKindMapView : public KGisView
{
protected: // 仅从序列化创建
	CKindMapView();
	DECLARE_DYNCREATE(CKindMapView)

// 特性
public:
	CKindMapDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CKindMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnKindmapTest();
	sfx_msg void OnUpdateKindmapTest(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	// 测试程序
protected:
	//void TestAgmrsProduct();
	//void Asc2Axd();
	//void TestMisceCalibrate();	// 地标校准

	// LST
	void agmrsProductGenerate_Lst(int iMethod);

	// VIX
	void agmrsProductGenerate_Vix(int iProductID);

	// 生成火点
	void agmrsProductGenerate_Misc(int iProductID);

};

#ifndef _DEBUG  // KindMapView.cpp 中的调试版本
inline CKindMapDoc* CKindMapView::GetDocument() const
   { return reinterpret_cast<CKindMapDoc*>(m_pDocument); }
#endif

