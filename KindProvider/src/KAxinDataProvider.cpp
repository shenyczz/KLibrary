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

// ��
BOOL KAxinDataProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = FALSE;

	if(KAxinData::IsAxinData(lpszConnectionString))
	{
		// ȡ���ļ���ʽ����
		int iFormatCode = KAxinData::GetFormatCode(lpszConnectionString);
		switch(iFormatCode)
		{
		case AXIN_FMT_CODE_TIN:					// ��ɢ����
			m_bOpen = this->openStationData(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_GRID_CONTOUR:		// ׷�ٵ�ֵ�ߵĸ������
			m_bOpen = this->openGridData_Contour(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_GRID_IMAGE:			// ����ͼ��ĸ������
			m_bOpen = openGridData_Image(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_GRID_WIND_VECTOR:	// �������(���ڷ糡ʸ��)
			m_bOpen = openGridData_WindVector(lpszConnectionString);
			break;

		case AXIN_FMT_CODE_IMAGE_CTRL_INFO:		// ͼ�������Ϣ
			m_bOpen = this->openImageCtrlData(lpszConnectionString);
			break;
		}
	}

	return KProvider::Open(lpszConnectionString);
}

// վ������
BOOL KAxinDataProvider::openStationData(LPCTSTR lpszConnectionString)
{
	// �ص�����
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// ״̬��
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// ���ݶ���
	KStationData* pDataObject = new KStationData();
	m_pDataObject = pDataObject;
	// ע����Ȼص������ͽ��Ȳ���
	pDataObject->RegiestCallbackFunc(pCallbackFunc,pProgressParam);

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
	int iProductCode = pDataInfo->ProductCode();
	TCHAR szColortableFile[MAX_PATH] = _T("");
	_stprintf(szColortableFile,_T("%s\\System\\Palette\\%d.pal"),((KWinAppEx*)AfxGetApp())->GetAppPath(),iProductCode);
	pDataObject->BuildPalette(szColortableFile);

	// ��λ������ʾ
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// ������� - ��ֵ��
BOOL KAxinDataProvider::openGridData_Contour(LPCTSTR lpszConnectionString)
{
	// �ص�����
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// ״̬��
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// ���ݶ���
	KGridContourData* pDataObject = new KGridContourData();
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
	int iProductCode = pDataInfo->ProductCode();
	TCHAR szColortableFile[MAX_PATH] = _T("");
	_stprintf(szColortableFile,_T("%s\\System\\Palette\\%d.pal"),((KWinAppEx*)AfxGetApp())->GetAppPath(),iProductCode);
	pDataObject->BuildPalette(szColortableFile);

	// ��λ������ʾ
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// ������� - ͼ��
BOOL KAxinDataProvider::openGridData_Image(LPCTSTR lpszConnectionString)
{
	// �ص�����
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// ״̬��
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// ���ݶ���
	KGridImageData* pDataObject = new KGridImageData();
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
	else if(!pDataObject->BuildImage(0))	// ����ͼ��
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

	// ��λ������ʾ
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// ������� - �糡ʸ��
BOOL KAxinDataProvider::openGridData_WindVector(LPCTSTR lpszConnectionString)
{
	// �ص�����
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// ״̬��
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// ���ݶ���
	KWindVectorData* pDataObject = new KWindVectorData();
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

	// ��λ������ʾ
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
// ͼ���������
BOOL KAxinDataProvider::openImageCtrlData(LPCTSTR lpszConnectionString)
{
	// �ص�����
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();
	// ״̬��
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// ���ݶ���
	KImageCtrlData* pDataObject = new KImageCtrlData();
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

	// ��λ������ʾ
	if(pMFCStatusBar && pProgressParam)
	{
		pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
		pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
	}

	return TRUE;
}
