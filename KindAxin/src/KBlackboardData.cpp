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

// 是否空
BOOL KBlackboardData::IsEmpty()
{
	return m_pObjectPtrCollection == NULL
		|| m_pObjectPtrCollection->Count() == 0
		;
}

// 取得范围
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

// 添加对象
void KBlackboardData::AddObject(KObject* pObject)
{
	if(!m_pObjectPtrCollection)
		m_pObjectPtrCollection = new KObjectPtrCollection();

	m_pObjectPtrCollection->Add(pObject);
}
// 移除对象
void KBlackboardData::RemoveObject(KObject* pObject)
{
	if(!m_pObjectPtrCollection)
		return;

	m_pObjectPtrCollection->Remove(pObject);
}
// 选择所有
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
// 选择对象
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

// 取得鼠标位置下方的对象(point 是世界坐标)
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
// 取得鼠标位置下方的对象(屏幕坐标)
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

// 查找指定标记的图形
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

// 重载装载数据
BOOL KBlackboardData::LoadData_txt(LPCTSTR lpszFile)
{
	/*
	//int i,j;
	TCHAR szTemp[MAX_PATH] = _T("");

	// 清空数据
	if(!IsEmpty())
		Empty();

	// 数据信息
	DataInfo* pDataInfo = this->GetDataInfo();

	// 打开文件
	FILE* fp = _tfopen(lpszFile,_T("rt"));
	if(fp==NULL)
	{
		SetMessage(_T("打开文件错误! in KBlackboardData::LoadData_txt"));
		return FALSE;
	}

	// 1.读文件标识
	int sizeFid = _tcslen(SNYC_FID);
	_ftscanf(fp,_T("%s"),szTemp);		//文件头标示,为字符串“SNYC”,大小写均可
	if( szTemp[sizeFid] != _T('\0') )
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - SNYC_FID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}

	// 转换为大写
	KBase::UpperString(szTemp);

	if( _tcscmp(szTemp,SNYC_FID) != 0 )
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - SNYC_FID"));
		fclose(fp);
		fp = NULL;
		return FALSE;
	}
	else
	{
		// 保存文件标识
		_tcscpy(pDataInfo->szFileID,szTemp);
	}

	// 2.读产品数据格式代码
	_ftscanf(fp,_T("%d"),&pDataInfo->wFormatCode);
	if(pDataInfo->wFormatCode != SNYC_FMT_CODE_BLACKBOARD)	// 114
	{
		SetMessage(_T("SNYC 数据文件格式不合要求! - wFormatCode"));
		fclose(fp);
		return FALSE;
	}

	// 3.读注释信息
	_ftscanf(fp,_T("%s"),szTemp);
	if( _tcslen(szTemp) > (SNYC_COMMENT_LENGTH) )	// SNYC_COMMENT_LENGTH = 64
	{
		szTemp[SNYC_COMMENT_LENGTH] = _T('\0');
	}
	_tcscpy(pDataInfo->szComment,szTemp);

	// 4.读日期时间
	_ftscanf(fp,_T("%d%d%d %d%d%d")
		,&pDataInfo->wYear
		,&pDataInfo->wMonth
		,&pDataInfo->wDay

		,&pDataInfo->wHour
		,&pDataInfo->wMinute
		,&pDataInfo->wSecond
		);

	// 5.读时效、层次、产品代码
	_ftscanf(fp,_T("%d%d%d")
		,&pDataInfo->wTimePeriod
		,&pDataInfo->wLayer
		,&pDataInfo->wProductCode
		);

	//--------------
	// 6.读取各种对象
	//-----------------------------------

	while(1)
	{
		_tcscpy(szTemp,_T(""));
		_ftscanf(fp,_T("%s"),szTemp);
		if(feof(fp))
			break;

		KBase::UpperString(szTemp);

		// 6.1 标注
		if( _tcscmp(szTemp,_T("LABELS_BEG")) == 0 )
		{
			if(!m_pGeometryCollection)
				m_pGeometryCollection = new KGeometryCollection();

			if(!this->readLabels(fp,m_pGeometryCollection))
			{
				SetMessage(_T("读取标注错误!"));
				break;
			}
		}
		else if( _tcscmp(szTemp,_T("LEGENDS_BEG")) == 0 )
		{
			if(!m_pGeometryCollection)
				m_pGeometryCollection = new KGeometryCollection();

			if(!this->readLegends(fp,m_pGeometryCollection))
			{
				SetMessage(_T("读取图例错误!"));
				break;
			}
		}
		else
		{
			break;
		}

	}// while()

	// 关闭文件
	fclose(fp);
	fp = NULL;

	return TRUE;
	*/

	return true;
}

// 取得选中的对象
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

