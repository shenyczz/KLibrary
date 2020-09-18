#include "StdAfx.h"
#include "KFields.h"

namespace XBase
{

	KFields::KFields(void)
		: TPtrCollection<KField*>()
	{
	}

	KFields::KFields(const KFields& fields)
		: TPtrCollection<KField*>(fields)
	{
		this->clear();
		int fldCount = fields.Count();
		for(int i=0;i<fldCount;i++)
		{
			this->Add(fields.GetField(i)->Clone());
		}
	}

	KFields::~KFields(void)
	{
	}

	// ��������ȡ���ֶ�
	KField* KFields::GetField(int index) const
	{
		return this->GetAt(index);
	}

	// ��������ȡ���ֶ�
	KField* KFields::GetField(LPCTSTR pszFieldName) const
	{
		KField* pField = NULL;
		//-------------------------------------------------
		KString strFildName = pszFieldName;
		strFildName.Trim().Upper();

		int fldCount = this->Count();
		for(int i = 0; i < fldCount; i++)
		{
			KField* pFieldTemp = this->GetAt(i);
			if(pFieldTemp)
			{
				KString strTemp(pFieldTemp->GetName());
				strTemp.Trim().Upper();
				if(strFildName == strTemp)
				{
					pField = pFieldTemp;
					break;
				}
			}
		}
		//-------------------------------------------------
		return pField;
	}

	// ȡ���ֶε�����ֵ
	//int KFields::GetFieldIndex(LPCTSTR pszFieldName)
	//{
	//	KString strFildName = pszFieldName;
	//	strFildName.Trim().Upper();

	//	int fldCount = this->Count();
	//	for(int i = 0; i < fldCount; i++)
	//	{
	//		KField* pField = this->GetAt(i);
	//		if(!pField) return -1;

	//		KString strTemp(pField->GetName());
	//		strTemp.Trim().Upper();
	//		if(strFildName == strTemp) return i;
	//	}

	//	return -1;
	//}

	// ȡ�ü�¼����
	int KFields::GetRecordLength()
	{
		// ��¼����
		int recordLength = 0;

		// �ֶ�����
		int fldCount = this->Count();
		if(fldCount>0)
		{
			// ÿ����¼�ĵ�һ�ֽ�Ϊɾ�����
			recordLength += 1;
			for(int i=0;i<fldCount;i++)
			{
				recordLength += this->GetField(i)->GetLength();
			}
		}

		return recordLength;
	}

	// ��¡
	KFields* KFields::Clone()
	{
		return new KFields(*this);
	}

	/*
	KFields::KFields(void)
		:m_pFieldMap(0)
		,m_pFieldVector(0)
	{
	}

	KFields::~KFields(void)
	{
		_delete(m_pFieldMap);
		_delete(m_pFieldVector);
	}

	// ��¡
	KFields* KFields::Clone()
	{
		KFields* pFields = new KFields();

		int iFieldCount = pFields->GetFieldCount();
		for(int i=0;i<iFieldCount;i++)
		{
			KField* pField = pFields->GetField(i)->Clone();
			pFields->AddField(pField);
		}

		return pFields;
	}

	// ȡ���ֶε�����ֵ
	int KFields::GetFieldIndex(LPCTSTR pszFieldName)
	{
		TCHAR szValue1[MAX_PATH] = _T("");
		_stprintf(szValue1,_T("%s"),pszFieldName);
		KString::TrimString(szValue1);
		KString::UpperString(szValue1);

		TCHAR szValue2[MAX_PATH] = _T("");

		int size = m_pFieldVector->size();
		for(int i=0;i<size;i++)
		{
			_stprintf(szValue2,_T("%s"),m_pFieldVector->at(i)->GetName());
			KString::TrimString(szValue2);
			KString::UpperString(szValue2);
			if( _tcscmp(szValue1,szValue2) == 0 )
			{
				return i;
			}
		}

		return -1;
	}

	// ȡ���ֶ�
	KField* KFields::GetField(int index)
	{
		KField* pField = (*m_pFieldVector)[index];
		return pField;
	}
	KField* KFields::GetField(LPCTSTR pszFieldName)
	{
		KField* pField = (*m_pFieldMap)[_tstring(pszFieldName)];
		return pField;
	}

	// ����һ���ֶ�
	void KFields::AddField(KField* pField)
	{
		if(!pField)
		{
			return;
		}

		if(!m_pFieldMap)
		{
			m_pFieldMap = new KFieldMap;
		}

		LPCTSTR pszName = pField->GetName();
		m_pFieldMap->insert( KFieldMap::value_type(pszName,pField) ); 

		if(!m_pFieldVector)
		{
			m_pFieldVector = new KFieldVector;
		}

		//m_pFieldVector->push_back(pField->Clone());
		m_pFieldVector->push_back(pField);
	}

	// ȡ�ü�¼����
	int KFields::GetRecordLength()
	{
		// �ֶ�����
		int iFieldCount = this->GetFieldCount();
		if(iFieldCount<=0)
			return 0;

		// ÿ����¼����
		int iRecordLength = 0;
		for(int i=0;i<iFieldCount;i++)
		{
			iRecordLength += this->GetField(i)->GetLength();
		}

		// ÿ����¼�ĵ�һ�ֽ�Ϊɾ��
		iRecordLength += 1;
		return iRecordLength;
	}
	*/

//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
