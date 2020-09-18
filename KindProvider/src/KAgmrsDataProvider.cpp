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

// ��
BOOL KAgmrsDataProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = false;
	//-------------------------------------------------
	// �ص�
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	ProgressParam* pProgressParam = (ProgressParam*)this->CallbackParam();

	// ״̬��
	CMFCStatusBar* pMFCStatusBar = (CMFCStatusBar*)((KWinAppEx*)AfxGetApp())->GetStatusBar();

	// ң������
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
	// ע�������ʾ
	pAgmrsData->RegiestCallbackFunc(this->CallbackFunc(),this->CallbackParam(),this->IsDeleteCallbackParam());

	// ���ļ�
	if(!pAgmrsData->LoadData(lpszConnectionString))
	{
		_delete(pAgmrsData);
		// ��λ������ʾ
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

		// ��λ������ʾ
		if(pMFCStatusBar && pProgressParam)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}

		if(!pAgmrsData->DataToDIB())
		{
			_delete(pAgmrsData);

			// ��λ������ʾ
			if(pMFCStatusBar && pProgressParam)
			{
				pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
				pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
			}

			m_bOpen = false;
		}
		// ��λ������ʾ
		if(pMFCStatusBar && pProgressParam)
		{
			pMFCStatusBar->SetPaneText(pProgressParam->indexPaneText,_T(""));
			pMFCStatusBar->EnablePaneProgressBar(pProgressParam->indexPaneProgress,-1);
		}

	}
	//-------------------------------------------------
	return KProvider::Open(lpszConnectionString);
}
