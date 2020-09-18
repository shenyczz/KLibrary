
// KindMapView.h : CKindMapView ��Ľӿ�
//

#pragma once


class CKindMapView : public KGisView
{
protected: // �������л�����
	CKindMapView();
	DECLARE_DYNCREATE(CKindMapView)

// ����
public:
	CKindMapDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CKindMapView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);

	afx_msg void OnKindmapTest();
	sfx_msg void OnUpdateKindmapTest(CCmdUI* pCmdUI);

	DECLARE_MESSAGE_MAP()

	// ���Գ���
protected:
	//void TestAgmrsProduct();
	//void Asc2Axd();
	//void TestMisceCalibrate();	// �ر�У׼

	// LST
	void agmrsProductGenerate_Lst(int iMethod);

	// VIX
	void agmrsProductGenerate_Vix(int iProductID);

	// ���ɻ��
	void agmrsProductGenerate_Misc(int iProductID);

};

#ifndef _DEBUG  // KindMapView.cpp �еĵ��԰汾
inline CKindMapDoc* CKindMapView::GetDocument() const
   { return reinterpret_cast<CKindMapDoc*>(m_pDocument); }
#endif

