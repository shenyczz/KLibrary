#pragma once


// KView ��ͼ
namespace Framework
{

	class KIND_EXT_CLASS KView : public CView
	{
		DECLARE_DYNCREATE(KView)

	protected:
		KView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
		virtual ~KView();

	public:
		virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
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
		// ������ɫ
		void SetBackColor(COLORREF clr) { m_clrBack = clr; }
		COLORREF GetBackColor() { return m_clrBack; }

		// �ͻ���ͼ��
		Image* GetImage() { return m_pImage; }

	protected:
		Image* m_pImage;
		COLORREF m_clrBack;

	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------

