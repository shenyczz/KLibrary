// KGMDIFrameWndEx.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "KGisMdiFrameWnd.h"


// KGMDIFrameWndEx

namespace Framework
{

	IMPLEMENT_DYNCREATE(KGisMdiFrameWnd, KMDIFrameWndEx)

	KGisMdiFrameWnd::KGisMdiFrameWnd()
	{
		
	}

	KGisMdiFrameWnd::~KGisMdiFrameWnd()
	{
	}


	BEGIN_MESSAGE_MAP(KGisMdiFrameWnd, KMDIFrameWndEx)
	END_MESSAGE_MAP()


	// KGisMdiFrameWnd ��Ϣ�������

	// �����û�������(��Ҫʱ������������)
	bool KGisMdiFrameWnd::CreateUserToolbars()
	{ 
		return true;
	}
	// ����ͣ������(��Ҫʱ������������)
	bool KGisMdiFrameWnd::CreateDockingWindows()
	{
		return true;
	}

//---------------------------------------------------------
}// namespace Framework - end
//---------------------------------------------------------
