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

// ��
BOOL KProvider::Open(LPCTSTR lpszConnectionString)
{
	if(lpszConnectionString)
		this->SetConnectionString(lpszConnectionString);

	return m_bOpen;
}
// �ر�
void KProvider::Close()
{
	_delete(m_pDataObject);
	m_bOpen = false;
}

// ע��
// ������Ϣ��ע��->���ݶ�������->�ļ���->_T("δ֪ͼ��")
KString KProvider::GetComment()
{
	KString strComment(_T(""));
	if(m_pDataObject)
	{
		// ���ݶ���ע��
		strComment = m_pDataObject->GetDataInfo()->GetComment();
		if(strComment.IsEmpty())
		{
			// ���ݶ�������
			strComment = m_pDataObject->GetName();
			if(strComment.IsEmpty())
			{
				// ���ݶ����ļ���
				KFile file(m_pDataObject->GetFileName());
				strComment = file.GetFileName(false);
				if(strComment.IsEmpty())
				{
					strComment = _T("δ֪ͼ��");
				}
			}
		}
	}
	return strComment;
}
