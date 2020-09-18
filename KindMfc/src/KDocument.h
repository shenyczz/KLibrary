#pragma once

// KDocument �ĵ�

namespace Framework
{

	class KIND_EXT_CLASS KDocument : public CDocument
	{
		DECLARE_DYNCREATE(KDocument)

	public:
		KDocument();
		virtual ~KDocument();
	#ifndef _WIN32_WCE
		virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
	#endif
	#ifdef _DEBUG
		virtual void AssertValid() const;
	#ifndef _WIN32_WCE
		virtual void Dump(CDumpContext& dc) const;
	#endif
	#endif

	protected:
		virtual BOOL OnNewDocument();

		DECLARE_MESSAGE_MAP()

	public:
		KDib* GetDib() { return m_pDib; }

	protected:
		KDib* m_pDib;
	};

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
