#ifndef __INCLUDED_BASE_INL__
#define __INCLUDED_BASE_INL__

// �ص�����
typedef void (CALLBACK *CALLBACK_FUNC)(void*);

#pragma pack(1)

// ���Ȳ���
struct ProgressParam
{
	int		iFlag;				// ���
								// 0: �� CMFCStatusBar ��ĳ��������ʾ������
								// 1: �� CProgressCtrl ��ʾ������

	int		indexPaneText;		// ״̬��������˵���ı�����
	int		indexPaneProgress;	// ״̬������������

	long	lTotalNums;			// ����
	long	lCurrentPos;		// ��ǰλ��

	TCHAR	szInfo[MAX_PATH];	// ������Ϣ

	BOOL	bReset;				// 

	LPVOID lpProgressCtrl;		// 
	LPVOID lpProgressInfoCtrl;	// 

public:
	ProgressParam(void)
		:iFlag(0)
		,indexPaneText(0)
		,indexPaneProgress(0)
		,lTotalNums(0)
		,lCurrentPos(0)
		,bReset(TRUE)
		,lpProgressCtrl(NULL)
		,lpProgressInfoCtrl(NULL)
	{
		_tcscpy(szInfo,_T(""));
	}
	virtual ~ProgressParam(void)	{	}

	void SetInfo(LPCTSTR lpszInfo)
	{
		bReset = TRUE;
		_tcscpy(szInfo,lpszInfo);
	}
};

#pragma pack()

#endif	//#ifndef __INCLUDED_BASE_INL__
