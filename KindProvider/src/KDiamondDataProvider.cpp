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

// ��
BOOL KDiamondDataProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = FALSE;

	if(KDiamondData::IsDiamondData(lpszConnectionString))
	{
		// ȡ���ļ���ʽ����
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
// ������� - ��ֵ��
BOOL KDiamondDataProvider::openGridData_Contour(LPCTSTR lpszConnectionString)
{
	// �ص�����
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// ״̬��
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// ���ݶ���
	KDiamondGridData* pDataObject = new KDiamondGridData();
	m_pDataObject = pDataObject;
	// ע����Ȼص������ͽ��Ȳ���
	pDataObject->RegiestCallbackFunc(pCallbackFunc,pProgressParam);

	// װ������
	if(!pDataObject->LoadData(lpszConnectionString))
	{
		// ɾ�����ݶ���
		_delete(pDataObject);

		// ��λ������ʾ
		if(pMFCStatusBar && pProgressParam)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}

		return FALSE;
	}

	// �����ɫ��
	KDataInfo* pDataInfo = pDataObject->GetDataInfo();
	//int iProductCode = pDataInfo->ProductCode();
	int iProductCode = pDataInfo->Layer();
	if(iProductCode>100)
	{
		TCHAR szColortableFile[MAX_PATH] = _T("");
		_stprintf(szColortableFile,_T("%s\\System\\Palette\\%d.pal"),((KWinAppEx*)AfxGetApp())->GetAppPath(),iProductCode);
		pDataObject->BuildPalette(szColortableFile);
	}

	// ��λ������ʾ
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
