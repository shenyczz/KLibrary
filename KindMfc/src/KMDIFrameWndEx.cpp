// KMDIFrameWndEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KMDIFrameWndEx.h"


namespace Framework
{

	// KMDIFrameWndEx

	IMPLEMENT_DYNCREATE(KMDIFrameWndEx, CMDIFrameWndEx)

	KMDIFrameWndEx::KMDIFrameWndEx()
	{
		m_pLayerWnd = NULL;
		m_pOutputWnd = NULL;
		m_pPropertyWnd = NULL;
	}

	KMDIFrameWndEx::~KMDIFrameWndEx()
	{
		_delete(m_pLayerWnd);
		_delete(m_pOutputWnd);
		_delete(m_pPropertyWnd);
	}


	BEGIN_MESSAGE_MAP(KMDIFrameWndEx, CMDIFrameWndEx)
	END_MESSAGE_MAP()


	// KMDIFrameWndEx ��Ϣ�������

	BOOL KMDIFrameWndEx::PreCreateWindow(CREATESTRUCT& cs)
	{
		if( !CMDIFrameWndEx::PreCreateWindow(cs) )
			return FALSE;
		// TODO: �ڴ˴�ͨ���޸�
		//  CREATESTRUCT cs ���޸Ĵ��������ʽ
		cs.style &= ~FWS_ADDTOTITLE;

		return TRUE;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
