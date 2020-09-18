#ifndef __INCLUDED_BASE_INL__
#define __INCLUDED_BASE_INL__

// 回调函数
typedef void (CALLBACK *CALLBACK_FUNC)(void*);

#pragma pack(1)

// 进度参数
struct ProgressParam
{
	int		iFlag;				// 标记
								// 0: 以 CMFCStatusBar 的某个窗格显示进度条
								// 1: 以 CProgressCtrl 显示进度条

	int		indexPaneText;		// 状态栏进度条说明文本窗格
	int		indexPaneProgress;	// 状态栏进度条窗格

	long	lTotalNums;			// 总数
	long	lCurrentPos;		// 当前位置

	TCHAR	szInfo[MAX_PATH];	// 进度信息

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
