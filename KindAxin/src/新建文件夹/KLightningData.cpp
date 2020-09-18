#include "StdAfx.h"
#include "KLightningData.h"

KLightningData::KLightningData(void)
{
	m_pLightningCollection = NULL;
}

KLightningData::~KLightningData(void)
{
	Empty();
}

// ������ݲ������ڴ�
void KLightningData::Empty()
{
	_delete(m_pLightningCollection);
}
// �Ƿ�Ϊ��
BOOL KLightningData::IsEmpty()
{
	return (m_pLightningCollection == NULL) ||
		   (m_pLightningCollection != NULL) && (0 == m_pLightningCollection->Count())
		   ;
}


BOOL KLightningData::LoadData_txt(LPCTSTR lpszFile)
{
	int i;
	int iTemp;
	TCHAR szTemp[MAX_PATH] = _T("");

	// �������
	if(!IsEmpty())
		Empty();

	// ������Ϣ
	DataInfo* pDataInfo = this->GetDataInfo();

	// վ����Ϣ��
	if(m_pLightningCollection==NULL)
	{
		m_pLightningCollection = new KLightningCollection();
	}
	KLightningCollection* pLightningCollection = m_pLightningCollection;

	//���ļ�
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("���ļ�����! in KStationData::LoadData_txt"));
		return FALSE;
	}

	// 1.���ļ���ʶ
	int sizeFid = _tcslen(SNYC_FID);
	_ftscanf(fp,_T("%s"),szTemp);		//�ļ�ͷ��ʾ,Ϊ�ַ�����SNYC��,��Сд����
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - SNYC_FID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	//ת��Ϊ��д
	KBase::UpperString(szTemp);

	if( _tcscmp(szTemp,SNYC_FID) != 0 )
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - SNYC_FID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	else
	{
		// �����ļ���ʶ
		_tcscpy(pDataInfo->szFileID,szTemp);
	}

	// 2.����Ʒ���ݸ�ʽ����
	_ftscanf(fp,_T("%d"),&pDataInfo->wFormatCode);
	if(pDataInfo->wFormatCode != SNYC_FMT_CODE_LIGHTNING)	// 130
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wFmtCode"));
		fclose(fp);
		return FALSE;
	}

	// 3.��ע����Ϣ
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (SNYC_COMMENT_LENGTH) )
	{
		szTemp[SNYC_COMMENT_LENGTH] = _T('\0');
	}
	_tcscpy(pDataInfo->szComment,szTemp);

	// 4.������ʱ��
	_ftscanf(fp,_T("%d%d%d %d%d%d")
		,&pDataInfo->wYear
		,&pDataInfo->wMonth
		,&pDataInfo->wDay

		,&pDataInfo->wHour
		,&pDataInfo->wMinute
		,&pDataInfo->wSecond
		);

	// 5.��ʱЧ����Ρ���Ʒ����
	_ftscanf(fp,_T("%d%d%d")
		,&pDataInfo->wTimePeriod
		,&pDataInfo->wLayer
		,&pDataInfo->wProductCode
		);

	// 6.������������
	_ftscanf(fp,_T("%d"),&pDataInfo->wTotalNum);
	if( ((int)pDataInfo->wTotalNum) < 0)
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wTotalNum"));
		fclose(fp);
		return FALSE;
	}

	// 7.��������
	int iLightningNum = pDataInfo->wTotalNum;
	KLightning* pLightning = NULL;
	pLightningCollection->Clear();
	for(i=0;i<iLightningNum;i++)
	{
		pLightning = new KLightning();

		KString kstring(_T(""));
		KStringArray astring;

		// YYYY-MM-DD
		_ftscanf(fp,_T("%s"),szTemp);
		kstring  = szTemp;
		astring = kstring.Split(_T('-'));
		if(astring.Count()!=3)
		{
			_delete(pLightning);
			break;
		}
		pLightning->wYear = _tstoi(astring[0]);
		pLightning->wMonth = _tstoi(astring[1]);
		pLightning->wDay = _tstoi(astring[2]);

		// hh:mm:ss
		_ftscanf(fp,_T("%s"),szTemp);
		kstring  = szTemp;
		astring = kstring.Split(_T(':'));
		if(astring.Count()!=3)
		{
			_delete(pLightning);
			break;
		}
		pLightning->wHour = _tstoi(astring[0]);
		pLightning->wMinute = _tstoi(astring[1]);
		pLightning->wSecond = _tstoi(astring[2]);

		_ftscanf(fp,_T("%u"),&pLightning->dwMillisecond);	// ����

		_ftscanf(fp,_T("%d"),&pLightning->wType);	// ��������

		_ftscanf(fp,_T("%f%f")
			,&pLightning->fLon			// ����
			,&pLightning->fLat			// γ��
			);

		_ftscanf(fp,_T("%f%f%f%f%f")
			,&pLightning->fIntensity	// ����ǿ��(KA)
			,&pLightning->fGradient		// ����(KA/us)
			,&pLightning->fEnergy		// ����(����)
			,&pLightning->fCharge		// ���(C)
			,&pLightning->fLocateError	// ��λ���
			);

		_ftscanf(fp,_T("%d"),&iTemp);// ��λ����
		pLightning->wLocateMethod = (WORD)iTemp;

		if(feof(fp))
		{
			_delete(pLightning);
			break;
		}

		// �����������
		pLightningCollection->Add(pLightning);

		// ��ʾ����
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = iLightningNum;
				pProgressParam->lCurrentPos = i;
			}

			m_pCallbackFunc(m_pCallbackParam);
		}
	}
	
	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	// ������������
	pDataInfo->wTotalNum = pLightningCollection->Count();

	// ����󶨾���
	this->ComputeBoundingBox();

	//pLightningCollection->Clear();

	return TRUE;
}

// ����󶨾���
void KLightningData::ComputeBoundingBox()
{
	this->SetBoundingBox(KBox(0,0,0,0));
}

