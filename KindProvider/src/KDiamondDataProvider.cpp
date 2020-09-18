#include "StdAfx.h"
#include "KDiamondDataProvider.h"


KDiamondDataProvider::KDiamondDataProvider(void)
{
}

KDiamondDataProvider::KDiamondDataProvider(LPCTSTR lpszConnectionString) throw(KException)
{

}

KDiamondDataProvider::~KDiamondDataProvider(void)
{
}

// 打开
BOOL KDiamondDataProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = FALSE;

	if(KDiamondData::IsDiamondData(lpszConnectionString))
	{
		// 取得文件格式代码
		int iFormatCode = KDiamondData::GetFormatCode(lpszConnectionString);
		switch(iFormatCode)
		{
		case DIAM_FMT_CODE_TIN:					// 3
			m_bOpen = this->openStationData(lpszConnectionString);
			break;
		case DIAM_FMT_CODE_GRID:					// 4
			m_bOpen = this->openGridData_Contour(lpszConnectionString);
			break;
		}
	}

	return KProvider::Open(lpszConnectionString);
}

BOOL KDiamondDataProvider::openStationData(LPCTSTR lpszConnectionString)
{
	return FALSE;
}
// 格点数据 - 等值线
BOOL KDiamondDataProvider::openGridData_Contour(LPCTSTR lpszConnectionString)
{
	// 回调参数
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// 状态条
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// 数据对象
	KDiamondGridData* pDataObject = new KDiamondGridData();
	m_pDataObject = pDataObject;
	// 注册进度回调函数和进度参数
	pDataObject->RegiestCallbackFunc(pCallbackFunc,pProgressParam);

	// 装载数据
	if(!pDataObject->LoadData(lpszConnectionString))
	{
		// 删除数据对象
		_delete(pDataObject);

		// 复位进度显示
		if(pMFCStatusBar && pProgressParam)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}

		return FALSE;
	}

	// 构造调色板
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	//int iProductCode = pDataInfo->ProductCode();
	int iProductCode = pDataInfo->Layer();
	if(iProductCode>100)
	{
		TCHAR szColortableFile[MAX_PATH] = _T("");
		_stprintf(szColortableFile,_T("%s\\System\\Palette\\%d.pal"),((KWinAppEx*)AfxGetApp())->GetAppPath(),iProductCode);
		pDataObject->BuildPalette(szColortableFile);
	}

	// 复位进度显示
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
