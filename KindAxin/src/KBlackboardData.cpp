#include "StdAfx.h"
#include "KBlackboardData.h"

KBlackboardData::KBlackboardData(void)
{
	m_pObjectPtrCollection = NULL;
}

KBlackboardData::~KBlackboardData(void)
{
	_delete(m_pObjectPtrCollection);
}

// �Ƿ��
BOOL KBlackboardData::IsEmpty()
{
	return m_pObjectPtrCollection == NULL
		|| m_pObjectPtrCollection->Count() == 0
		;
}

// ȡ�÷�Χ
const KExtents& KBlackboardData::GetExtents()
{
	if(!m_pObjectPtrCollection)
	{
		SetExtents(KExtents(0,0,0,0));
		return KObject::GetExtents(); 
	}

	int iCount = m_pObjectPtrCollection->Count();
	if(iCount==0)
	{
		SetExtents(KExtents(0,0,0,0));
		return KObject::GetExtents(); 
	}

	KExtents extents = KObject::GetExtents();
	if(extents.IsEmpty())
	{
		extents = KExtents(181,91,-181,-91);
	}
	for(int i=0;i<iCount;i++)
	{
		KObject* pObject = m_pObjectPtrCollection->GetAt(i);
		if(pObject)
		{
			extents.Join(pObject->GetExtents());
		}
	}

	SetExtents(extents);
	return KObject::GetExtents();
}

// ��Ӷ���
void KBlackboardData::AddObject(KObject* pObject)
{
	if(!m_pObjectPtrCollection)
		m_pObjectPtrCollection = new KObjectPtrCollection();

	m_pObjectPtrCollection->Add(pObject);
}
// �Ƴ�����
void KBlackboardData::RemoveObject(KObject* pObject)
{
	if(!m_pObjectPtrCollection)
		return;

	m_pObjectPtrCollection->Remove(pObject);
}
// ѡ������
void KBlackboardData::SelectAll(bool bSelect)
{
	KObjectPtrCollection* pObjectPtrCollection = m_pObjectPtrCollection;
	if(!pObjectPtrCollection)
		return;

	int count = pObjectPtrCollection->Count();
	for(int i=0;i<count;i++)
	{
		KObject* pObject = pObjectPtrCollection->GetAt(i);
		pObject->Select(bSelect);
	}
	// FUN_END
}
// ѡ�����
void KBlackboardData::Select(KObject* pObject, bool bSelect)
{
	KObjectPtrCollection* pObjectPtrCollection = m_pObjectPtrCollection;
	if(!pObjectPtrCollection)
		return;

	if(!pObjectPtrCollection->Contains(pObject))
		return;

	pObject->Select(bSelect);
	// FUN_END
}

// ȡ�����λ���·��Ķ���(point ����������)
KObject* KBlackboardData::GetObject(KPoint point)
{
	KObject* pObject = NULL;

	KObjectPtrCollection* pObjectPtrCollection = m_pObjectPtrCollection;
	if(!pObjectPtrCollection)
		return NULL;

	int iCount = pObjectPtrCollection->Count();
	for(int i=0;i<iCount;i++)
	{
		KObject* pObjectTemp = pObjectPtrCollection->GetAt(iCount-1-i);
		KExtents extents = pObjectTemp->GetExtents();
		if(extents.PtInside(point.X,point.Y))
		{
			pObject = pObjectTemp;
			break;
		}
	}

	return pObject;
}
// ȡ�����λ���·��Ķ���(��Ļ����)
KObject* KBlackboardData::GetObject(POINT point)
{
	KObject* pObject = NULL;

	KObjectPtrCollection* pObjectPtrCollection = m_pObjectPtrCollection;
	if(!pObjectPtrCollection)
		return NULL;

	int iCount = pObjectPtrCollection->Count();
	for(int i=0;i<iCount;i++)
	{
		KObject* pObjectTemp = pObjectPtrCollection->GetAt(iCount-1-i);
		KExtents box = pObjectTemp->GetExtentsClient();
		//RectF rc((REAL)box.Xmin,(REAL)box.Ymin,(REAL)box.Width(),(REAL)box.Height());
		RectF rc((REAL)box.MinX(),(REAL)box.MinY(),(REAL)box.Width(),(REAL)box.Height());
		if(rc.Contains((REAL)point.x,(REAL)point.y))
		{
			pObject = pObjectTemp;
			break;
		}
	}

	return pObject;
}

// ����ָ����ǵ�ͼ��
KObject* KBlackboardData::FindObject(DWORD dwFlag)
{
	KObject* pObject = NULL;
	KObjectPtrCollection* pObjectPtrCollection = m_pObjectPtrCollection;

	if(!pObjectPtrCollection)
		return NULL;

	int iCount = pObjectPtrCollection->Count();
	for(int i=0;i<iCount;i++)
	{
		KObject* pObjectTemp = pObjectPtrCollection->GetAt(iCount-1-i);
		if(pObjectTemp->GetFlag() == dwFlag)
		{
			pObject = pObjectTemp;
			break;
		}
	}

	return pObject;

}

// ����װ������
BOOL KBlackboardData::LoadData_txt(LPCTSTR lpszFile)
{
	/*
	//int i,j;
	TCHAR szTemp[MAX_PATH] = _T("");

	// �������
	if(!IsEmpty())
		Empty();

	// ������Ϣ
	DataInfo* pDataInfo = this->GetDataInfo();

	// ���ļ�
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("���ļ�����! in KBlackboardData::LoadData_txt"));
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

	// ת��Ϊ��д
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
	if(pDataInfo->wFormatCode != SNYC_FMT_CODE_BLACKBOARD)	// 114
	{
		SetMessage(_T("SNYC �����ļ���ʽ����Ҫ��! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}

	// 3.��ע����Ϣ
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (SNYC_COMMENT_LENGTH) )	// SNYC_COMMENT_LENGTH = 64
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

	//--------------
	// 6.��ȡ���ֶ���
	//-----------------------------------

	while(1)
	{
		_tcscpy(szTemp,_T(""));
		_ftscanf(fp,_T("%s"),szTemp);
		if(feof(fp))
			break;

		KBase::UpperString(szTemp);

		// 6.1 ��ע
		if( _tcscmp(szTemp,_T("LABELS_BEG")) == 0 )
		{
			if(!m_pGeometryCollection)
				m_pGeometryCollection = new KGeometryCollection();

			if(!this->readLabels(fp,m_pGeometryCollection))
			{
				SetMessage(_T("��ȡ��ע����!"));
				break;
			}
		}
		else if( _tcscmp(szTemp,_T("LEGENDS_BEG")) == 0 )
		{
			if(!m_pGeometryCollection)
				m_pGeometryCollection = new KGeometryCollection();

			if(!this->readLegends(fp,m_pGeometryCollection))
			{
				SetMessage(_T("��ȡͼ������!"));
				break;
			}
		}
		else
		{
			break;
		}

	}// while()

	// �ر��ļ�
	fclose(fp);
	fp = NULL;

	return TRUE;
	*/

	return true;
}

// ȡ��ѡ�еĶ���
KObject* KBlackboardData::GetSelectedObject()
{
	if(!m_pObjectPtrCollection)
		return NULL;

	KObject* pObject = NULL;
	int iCount = m_pObjectPtrCollection->Count();
	for(int i=0;i<iCount;i++)
	{
		KObject* pObjectTemp = m_pObjectPtrCollection->GetAt(i);
		if(pObjectTemp && pObjectTemp->IsSelected())
		{
			pObject = pObjectTemp;
			break;
		}
	}

	return pObject;
}

