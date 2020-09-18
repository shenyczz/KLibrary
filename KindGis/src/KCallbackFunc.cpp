#include "StdAfx.h"
#include "KCallbackFunc.h"

#include "KWinAppEx.h"
using namespace Framework;

void CALLBACK KCallbackFunc::ShowProgressCallback(LPVOID lpCallbackParam)
{
	// FUN_BEG

	ProgressParam* pProgressParam = (ProgressParam*)lpCallbackParam;
	if(!pProgressParam)
		return;

	if(pProgressParam->iFlag==0)
		ShowProgressUseMFCStatusBar(lpCallbackParam);
	else
		ShowProgressUserProgressCtrl(lpCallbackParam);

	// FUN_END
}

//static 
void KCallbackFunc::ShowProgressUseMFCStatusBar(LPVOID lpCallbackParam)
{
	// FUN_BEG

	// 进度参数
	ProgressParam* pProgressParam = (ProgressParam*)lpCallbackParam;
	if(!pProgressParam)
		return;

	// 状态栏
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();
	if(NULL == pMFCStatusBar)
		return;

	int indexPaneText = pProgressParam->indexPaneText;			// 状态栏进度条说明文本窗格
	int indexPaneProgress = pProgressParam->indexPaneProgress;	// 状态栏进度条窗格
	if(indexPaneText<0 || indexPaneProgress<0)
		return;

	if(pProgressParam->bReset)
	{
		pProgressParam->bReset = FALSE;
		pMFCStatusBar->EnablePaneProgressBar(indexPaneProgress,pProgressParam->lTotalNums,FALSE);

		// 显示进度信息
		pMFCStatusBar->SetPaneText(indexPaneText,pProgressParam->szInfo);
	}

	if(pProgressParam->lCurrentPos+1 <= pProgressParam->lTotalNums)
	{
		pMFCStatusBar->SetPaneProgress(indexPaneProgress,pProgressParam->lCurrentPos+1);
	}

	// FUN_END
}

//static 
void KCallbackFunc::ShowProgressUserProgressCtrl(LPVOID lpCallbackParam)
{
	// FUN_BEG

	// 进度参数
	ProgressParam* pProgressParam = (ProgressParam*)lpCallbackParam;
	if(!pProgressParam)
		return;

	// 进度控件
	CProgressCtrl* pProgressCtrl = (CProgressCtrl*)pProgressParam->lpProgressCtrl;
	if(!pProgressCtrl)
		return;

	if(pProgressParam->bReset)
	{
		pProgressParam->bReset = FALSE;
		pProgressCtrl->SetStep(1);
		pProgressCtrl->SetRange32(0,pProgressParam->lTotalNums);
		pProgressCtrl->SetPos(0);

		// 显示进度信息
		CStatic* pStatic = (CStatic*)pProgressParam->lpProgressInfoCtrl;
		if(pStatic) pStatic->SetWindowText(pProgressParam->szInfo);
	}

	pProgressCtrl->StepIt();

	// FUN_END
}

