#include "StdAfx.h"
#include "KAgmrsDataProvider.h"


KAgmrsDataProvider::KAgmrsDataProvider(void)
{
	SetType(ProviderType_AgmrsData);
}

KAgmrsDataProvider::KAgmrsDataProvider(LPCTSTR lpszConnectionString) throw(KException)
	: KProvider(lpszConnectionString)
{
	SetType(ProviderType_AgmrsData);
	if(!Open(lpszConnectionString))
	{
		throw KException(_T("Open connection string error."));
	}
}

KAgmrsDataProvider::~KAgmrsDataProvider(void)
{
}

const KExtents& KAgmrsDataProvider::GetExtents()
{
	KAgmrsData* pAgmrsData = (KAgmrsData*)this->GetDataObject();
	this->SetExtents(pAgmrsData->GetExtents());
	return KObject::GetExtents();
}

// 打开
BOOL KAgmrsDataProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = false;
	//-------------------------------------------------
	// 回调
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

	// 状态条
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// 遥感数据
	KAgmrsData* pAgmrsData = NULL;
	//-------------------------------------------------
	if(KAgmrsData::IsAvhrrData(lpszConnectionString))		// AVHRR
	{
		pAgmrsData = new KAvhrrData();
		_delete(m_pDataObject);
		m_pDataObject = pAgmrsData;
	}
	else if(KAgmrsData::IsModisData(lpszConnectionString))	// MODIS
	{
		pAgmrsData = new KModisData();
		_delete(m_pDataObject);
		m_pDataObject = pAgmrsData;
	}
	else if(KAgmrsData::IsMersiData(lpszConnectionString))	// MERSI
	{
		pAgmrsData = new KMersiData();
		_delete(m_pDataObject);
		m_pDataObject = pAgmrsData;
	}
	else if(KAgmrsData::IsVirrData(lpszConnectionString))	// VIRR
	{
		pAgmrsData = new KVirrData();
		_delete(m_pDataObject);
		m_pDataObject = pAgmrsData;
	}
	//-------------------------------------------------
	if(!pAgmrsData)
		return FALSE;
	//-------------------------------------------------
	// 注册进度显示
	pAgmrsData->RegiestCallbackFunc(this->CallbackFunc(),this->CallbackParam(),this->IsDeleteCallbackParam());

	// 打开文件
	if(!pAgmrsData->LoadData(lpszConnectionString))
	{
		_delete(pAgmrsData);
		// 复位进度显示
		if(pMFCStatusBar && pProgressParam)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}
		m_bOpen = false;
	}
	else
	{
		m_bOpen = true;

		// 复位进度显示
		if(pMFCStatusBar && pProgressParam)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}

		if(!pAgmrsData->DataToDIB())
		{
			_delete(pAgmrsData);

			// 复位进度显示
			if(pMFCStatusBar && pProgressParam)
			{
				pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
				pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
			}

			m_bOpen = false;
		}
		// 复位进度显示
		if(pMFCStatusBar && pProgressParam)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}

	}
	//-------------------------------------------------
	return KProvider::Open(lpszConnectionString);
}
