#pragma once


// KView 视图
namespace Framework
{

	class KIND_EXT_CLASS KView : public CView
	{
		DECLARE_DYNCREATE(KView)

	protected:
		KView();           // 动态创建所使用的受保护的构造函数
		virtual ~KView();

	public:
		virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		afx_msg BOOL OnEraseBkgnd(CDC* pDC);

		DECLARE_MESSAGE_MAP()

	public:
		// 背景颜色
		void SetBackColor(COLORREF clr) { m_clrBack = clr; }
		COLORREF GetBackColor() { return m_clrBack; }

		// 客户区图象
		Image* GetImage() { return m_pImage; }

	protected:
		Image* m_pImage;
		COLORREF m_clrBack;

	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

