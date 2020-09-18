#include "StdAfx.h"
#include "KAxinDataProvider.h"

KAxinDataProvider::KAxinDataProvider(void)
{
}

KAxinDataProvider::KAxinDataProvider(LPCTSTR lpszConnectionString) throw(KException)
	: KProvider(lpszConnectionString)
{
	SetType(ProviderType_AxinData);
	if(!Open(lpszConnectionString))
	{
		throw KException(_T("Open connection string error."));
	}
}

KAxinDataProvider::~KAxinDataProvider(void)
{
}

const KExtents& KAxinDataProvider::GetExtents()
{
	KDataObject* pDataObject = this->GetDataObject();
	this->SetExtents(pDataObject->GetExtents());
	return KProvider::GetExtents();
}

// 打开
BOOL KAxinDataProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = FALSE;

	if(KAxinData::IsAxinData(lpszConnectionString))
	{
		// 取得文件格式代码
		int iFormatCode = KAxinData::GetFormatCode(lpszConnectionString);
		switch(iFormatCode)
		{
		case AXIN_FMT_CODE_TIN:					// 离散数据
			m_bOpen = this->openStationData(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_GRID_CONTOUR:		// 追踪等值线的格点数据
			m_bOpen = this->openGridData_Contour(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_GRID_IMAGE:			// 生成图像的格点数据
			m_bOpen = openGridData_Image(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_GRID_WIND_VECTOR:	// 格点数据(用于风场矢量)
			m_bOpen = openGridData_WindVector(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_IMAGE_CTRL_INFO:		// 图像控制信息
			m_bOpen = this->openImageCtrlData(lpszConnectionString);
			break;
		}
	}

	return KProvider::Open(lpszConnectionString);
}

// 站点数据
BOOL KAxinDataProvider::openStationData(LPCTSTR lpszConnectionString)
{
	// 回调参数
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// 状态条
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// 数据对象
	KStationData* pDataObject = new KStationData();
	m_pDataObject = pDataObject;
	// 注册进度回调函数和进度参数
	pDataObject->RegiestCallbackFunc(pCallbackFunc,pProgressParam);

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
	int iProductCode = pDataInfo->ProductCode();
	TCHAR szColortableFile[MAX_PATH] = _T("");
	_stprintf(szColortableFile,_T("%s\\System\\Palette\\%d.pal"),((KWinAppEx*)AfxGetApp())->GetAppPath(),iProductCode);
	pDataObject->BuildPalette(szColortableFile);

	// 复位进度显示
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// 格点数据 - 等值线
BOOL KAxinDataProvider::openGridData_Contour(LPCTSTR lpszConnectionString)
{
	// 回调参数
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// 状态条
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// 数据对象
	KGridContourData* pDataObject = new KGridContourData();
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
	int iProductCode = pDataInfo->ProductCode();
	TCHAR szColortableFile[MAX_PATH] = _T("");
	_stprintf(szColortableFile,_T("%s\\System\\Palette\\%d.pal"),((KWinAppEx*)AfxGetApp())->GetAppPath(),iProductCode);
	pDataObject->BuildPalette(szColortableFile);

	// 复位进度显示
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// 格点数据 - 图像
BOOL KAxinDataProvider::openGridData_Image(LPCTSTR lpszConnectionString)
{
	// 回调参数
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// 状态条
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// 数据对象
	KGridImageData* pDataObject = new KGridImageData();
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
	else if(!pDataObject->BuildImage(0))	// 构造图像
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

	// 复位进度显示
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// 格点数据 - 风场矢量
BOOL KAxinDataProvider::openGridData_WindVector(LPCTSTR lpszConnectionString)
{
	// 回调参数
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// 状态条
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// 数据对象
	KWindVectorData* pDataObject = new KWindVectorData();
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

	// 复位进度显示
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// 图像控制数据
BOOL KAxinDataProvider::openImageCtrlData(LPCTSTR lpszConnectionString)
{
	// 回调参数
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// 状态条
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// 数据对象
	KImageCtrlData* pDataObject = new KImageCtrlData();
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

	// 复位进度显示
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
