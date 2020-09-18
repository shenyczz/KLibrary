#include "StdAfx.h"
#include "KRows.h"

#include "KRow.h"

namespace XBase
{

	KRows::KRows(void)
		: TPtrCollection<KRow*>()
	{
	}

	KRows::KRows(const KRows& rows)
		: TPtrCollection<KRow*>(rows)
	{
	}

	KRows::KRows(LPCTSTR lpszName)
		: TPtrCollection<KRow*>()
	{
		SetName(lpszName);
	}

	KRows::~KRows(void)
	{
	}

	// ��¡
	KRows* KRows::Clone()
	{
		return new KRows(*this);
	}

	// ȡ���ֶμ���
	KFields* KRows::GetFields()
	{
		int rowCount = this->Count();
		return rowCount > 0 ? this->GetAt(0) : NULL;
	}

	// �����ֶ����ƺ��ֶ�ֵȡ�ü�¼���
	int KRows::RecordOf(LPCTSTR lpszFieldName,double dValue)
	{
		int iRowCount = this->Count();
		for(int i=0;i<iRowCount;i++)
		{
			double dTemp = this->GetAt(i)->GetField(lpszFieldName)->GetFloatValue();
			if(fabs(dTemp-dValue)<EPS)
				return i+1;
		}

		return -1;
	}
	int KRows::RecordOf(LPCTSTR lpszFieldName,LPCTSTR lpszValue)
	{
		KString strValue(lpszValue);
		strValue.Upper();

		int iRowCount = this->Count();
		for(int i=0;i<iRowCount;i++)
		{
			KString strTemp = this->GetAt(i)->GetField(lpszFieldName)->GetStringValue();
			if(strValue == strTemp)
				return i+1;
		}

		return -1;
	}




	/*
	// ��̬����
	static KRowMap::iterator s_curRow;

	KRows::KRows(LPCTSTR lpszName,const KFields* pFields)
		:m_pFields(0)
		,m_pRowMap(0)
	{
		m_pFields = (KFields*)pFields;
		this->SetName(lpszName);
	}

	KRows::~KRows(void)
	{
		_delete(m_pFields);
		_delete(m_pRowMap);
	}

	// ��¡
	KRows* KRows::Clone()
	{
		KRows* pRows = new KRows(this->GetName(),this->GetFields()->Clone());

		int iRowCount = this->GetRowCount();
		for(int i=0;i<iRowCount;i++)
		{
			KRow* pRow = this->GetRowById(i+1)->Clone();
			pRows->AddRow(pRow);
		}

		return pRows;
	}

	// ����һ����¼
	void KRows::AddRow(KRow* pRow)
	{
		if(!m_pRowMap)
		{
			m_pRowMap = new KRowMap;
		}

		m_pRowMap->insert( KRowMap::value_type(pRow->GetId(), pRow) );
	}

	// ȡ�ü�¼����
	int KRows::GetRowCount()
	{
		return m_pRowMap ? m_pRowMap->size() : 0; 
	}

	// ȡ��һ����¼
	// id �Ǽ�¼��Ŵ�1��ʼ
	KRow* KRows::GetRowById(int id)
	{
		return m_pRowMap ? (*m_pRowMap)[id] : NULL;
	}

	// ������¼
	KRow* KRows::GetFirst()
	{
		s_curRow = m_pRowMap->begin();
		if(s_curRow == m_pRowMap->end())
		{
			return NULL;
		}
		else
		{
			return (*s_curRow).second;
		}
	}
	KRow* KRows::GetNext()
	{
		s_curRow++;

		if(s_curRow == m_pRowMap->end())
		{
			s_curRow--;
			return NULL;
		}
		else
		{
			return (*s_curRow).second;
		}
	}

	// ������ֶ�
	void KRows::NewField(KField* pField)
	{
		//// ����ԭ�����м�
		//KRows* pOldRows = this->Clone();

		//// ȡ���ֶμ���
		//KFields* pFields = this->GetFields();

		//// �������ͬ���ֶ�
		//if( pFields->GetFieldIndex(pField->GetName()) >= 0 )
		//{
		//	_delete(pField);	// �����ڴ�
		//	return;
		//}

		//// ����ֶ�
		//pFields->Add(pField);

		//m_pRowMap->Clear();
	}
	*/
//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
