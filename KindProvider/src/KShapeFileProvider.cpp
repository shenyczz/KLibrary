#include "StdAfx.h"
#include "KShapeFileProvider.h"

#include "KShapeFile.h"

KShapeFileProvider::KShapeFileProvider(void)
	: KProvider()
{
	SetType(ProviderType_ShapeFile);
}

KShapeFileProvider::KShapeFileProvider(const KShapeFileProvider& shapeFileProvider)
	: KProvider(shapeFileProvider)
{
	SetType(ProviderType_ShapeFile);
}

KShapeFileProvider::KShapeFileProvider(LPCTSTR lpszConnectionString) throw(KException)
	: KProvider(lpszConnectionString)
{
	SetType(ProviderType_ShapeFile);
	if(!Open(lpszConnectionString))
	{
		throw KException(_T("Open connection string error."));
	}
}

KShapeFileProvider::~KShapeFileProvider(void)
{
}

// 打开
BOOL KShapeFileProvider::Open(LPCTSTR lpszConnectionString)
{
	m_bOpen = true;

	m_pDataObject = new KShapeFile();
	KShapeFile* pShapeFile = (KShapeFile*)m_pDataObject;

	// 注释
	KFile file(lpszConnectionString);
	m_pDataObject->GetDataInfo()->SetComment(file.GetFileName(false));

	try
	{
		pShapeFile->LoadData(this->GetConnectionString());
	}
	catch(...)
	{
		_delete(m_pDataObject);
		m_bOpen = false;
	}

	return KProvider::Open(lpszConnectionString);
}

// 取得数据范围
const KExtents& KShapeFileProvider::GetExtents()
{
	KShapeFile* pShapeFile = (KShapeFile*)this->GetDataObject();
	this->SetExtents(pShapeFile->GetExtents());
	return KObject::GetExtents();
}
