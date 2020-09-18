// KFrameWndEx.cpp : ʵ���ļ�
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


	// KFrameWndEx ��Ϣ�������

	BOOL KFrameWndEx::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CFrameWndEx::PreCreateWindow(cs) )
			return FALSE;
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ
		cs.style &= ~FWS_ADDTOTITLE;

		return TRUE;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
