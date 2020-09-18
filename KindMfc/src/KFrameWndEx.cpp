// KFrameWndEx.cpp : 实现文件
//

#include "stdafx.h"
#include "KFrameWndEx.h"

namespace Framework
{

	// KFrameWndEx

	IMPLEMENT_DYNCREATE(KFrameWndEx, CFrameWndEx)

	KFrameWndEx::KFrameWndEx()
	{
		m_pLayerWnd = NULL;
		m_pOutputWnd = NULL;
		m_pPropertyWnd = NULL;
	}

	KFrameWndEx::~KFrameWndEx()
	{
		_delete(m_pLayerWnd);
		_delete(m_pOutputWnd);
		_delete(m_pPropertyWnd);
	}


	BEGIN_MESSAGE_MAP(KFrameWndEx, CFrameWndEx)
	END_MESSAGE_MAP()


	// KFrameWndEx 消息处理程序

	BOOL KFrameWndEx::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CFrameWndEx::PreCreateWindow(cs) )
			return FALSE;
		// TODO: 在此处通过修改
		//  CREATESTRUCT cs 来修改窗口类或样式
		cs.style &= ~FWS_ADDTOTITLE;

		return TRUE;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
