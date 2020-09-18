#include "StdAfx.h"
#include "KProvider.h"

KProvider::KProvider(void)
	: KProgressObject()
	, m_bOpen(false)
	, m_ConnectionString(_T(""))
	, m_pDataObject(NULL)
{
}

KProvider::KProvider(const KProvider& provider)
	: KProgressObject(provider)
	, m_bOpen(provider.m_bOpen)
	, m_ConnectionString(provider.m_ConnectionString)
	, m_pDataObject(NULL)
{
}

KProvider::KProvider(LPCTSTR lpszConnectionString)
	: KProgressObject(lpszConnectionString)
	, m_bOpen(false)
	, m_ConnectionString(lpszConnectionString)
	, m_pDataObject(NULL)
{
}

KProvider::~KProvider(void)
{
	if(IsOpen())
	{
		this->Close();
	}
}

// 打开
BOOL KProvider::Open(LPCTSTR lpszConnectionString)
{
	if(lpszConnectionString)
		this->SetConnectionString(lpszConnectionString);

	return m_bOpen;
}
// 关闭
void KProvider::Close()
{
	_delete(m_pDataObject);
	m_bOpen = false;
}

// 注释
// 数据信息的注释->数据对象名称->文件名->_T("未知图层")
KString KProvider::GetComment()
{
	KString strComment(_T(""));
	if(m_pDataObject)
	{
		// 数据对象注释
		strComment = m_pDataObject->GetDataInfo()->GetComment();
		if(strComment.IsEmpty())
		{
			// 数据对象名称
			strComment = m_pDataObject->GetName();
			if(strComment.IsEmpty())
			{
				// 数据对象文件名
				KFile file(m_pDataObject->GetFileName());
				strComment = file.GetFileName(false);
				if(strComment.IsEmpty())
				{
					strComment = _T("未知图层");
				}
			}
		}
	}
	return strComment;
}
