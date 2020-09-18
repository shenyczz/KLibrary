// KEditCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "KEditCtrl.h"

//const DWORD KEditCtrl::dwStyleDefault =
//	AFX_WS_DEFAULT_VIEW |
//	WS_HSCROLL | WS_VSCROLL |
//	ES_AUTOHSCROLL | ES_AUTOVSCROLL |
//	ES_MULTILINE | ES_NOHIDESEL;

// KEditCtrl
namespace Gui
{

	IMPLEMENT_DYNAMIC(KEditCtrl, CEdit)

	KEditCtrl::KEditCtrl()
	{
		m_clrBack = RGB(255,255,255);
		m_clrFore = RGB(0,0,0);
	}

	KEditCtrl::~KEditCtrl()
	{
	}


	BEGIN_MESSAGE_MAP(KEditCtrl, CEdit)
		ON_WM_CTLCOLOR_REFLECT()
	END_MESSAGE_MAP()



	// KEditCtrl 消息处理程序

	HBRUSH KEditCtrl::CtlColor(CDC* pDC, UINT nCtlColor)
	{
		CBrush brush;
		//-----------------------------
		if(nCtlColor=CTLCOLOR_EDIT)
		{
			pDC->SetBkMode(OPAQUE);				//不透明
			pDC->SetTextColor(m_clrFore);		//设置前景颜色
			pDC->SetBkColor(m_clrBack);			//设置背景颜色
			brush.CreateSolidBrush(m_clrBack);	//创建实心画笔
		}
		//-----------------------------
		return (HBRUSH)brush.Detach();			//返回非空画笔
	}

	void KEditCtrl::AppendText(LPCTSTR lpszText,BOOL bLineScroll)
	{
		CString strInfo("");
		this->GetWindowText(strInfo);

		strInfo += lpszText;
		strInfo += "\r\n";
		this->SetWindowText(strInfo);

		if(bLineScroll)
		{
			this->LineScroll(this->GetLineCount());
		}
	}

}// namespace Gui - end

/*
ASSERT(cs.lpszClass == NULL);
cs.lpszClass = m_strClass;

// initialize common controls
VERIFY(AfxDeferRegisterClass(AFX_WNDCOMMCTLS_REG));
AfxDeferRegisterClass(AFX_WNDCOMMCTLSNEW_REG);

// map default CView style to default style
// WS_BORDER is insignificant
if ((cs.style | WS_BORDER) == AFX_WS_DEFAULT_VIEW)
	cs.style = m_dwDefaultStyle & (cs.style | ~WS_BORDER);

return CView::PreCreateWindow(cs);
*/