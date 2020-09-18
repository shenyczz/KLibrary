#include "StdAfx.h"
#include "KRow.h"

namespace XBase
{

	KRow::KRow(void)
		: KFields()
		, m_pszBuffer(NULL)
	{
	}
	KRow::KRow(const KRow& row)
		: KFields(row)
		, m_pszBuffer(NULL)
	{
	}
	KRow::KRow(int id)
		: KFields()
		, m_pszBuffer(NULL)
	{
		SetId(id);
	}

	KRow::~KRow(void)
	{
		_deletea(m_pszBuffer);
	}

	// ��¡
	KRow* KRow::Clone()
	{
		return new KRow(*this);
	}

	// ȡ�ü�¼ƫ�Ƶ�ַ
	LONG KRow::GetOffset()
	{
		LONG lOffset = 0;

		// ��¼��
		int iRecNo = this->GetId();
		// ��¼����
		int iRecLength = this->GetLength();
		// �ֶ�����
		int iFieldCount = this->GetFieldCount();
		// �ļ�ͷ����
		int iHeaderLength = sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount;
		iHeaderLength += 1;	// ��ֹ��ʶ 0x0d

		// ƫ��
		lOffset = iHeaderLength + iRecLength * (iRecNo - 1);
		return lOffset;
	}

	// ת��Ϊ�ַ���
	char* KRow::ToString()
	{
		_deletea(m_pszBuffer);

		// ��¼����
		int iLength = this->GetLength();
		if(iLength >= 4000)
			return NULL;

		// �����㹻�ڴ�
		m_pszBuffer = new char[iLength+1];
		memset(m_pszBuffer,0,sizeof(char)*(iLength+1));
		for(int i=0;i<iLength;i++)
		{
			m_pszBuffer[i] = 0x20;
		}

		// ɾ����� 
		// 0x20 - ' '��ʾ������¼
		// 0x2A - '*'��ʾɾ����¼
		m_pszBuffer[0] = 0x20;

		int pos = 1;
		int iFieldCount = this->GetFieldCount();
		for(int i=0;i<iFieldCount;i++)
		{
			KField* pField = this->GetField(i);
			char* strFieldValue = pField->ToString();
			int iFieldLength = pField->GetLength();
			if(iFieldLength==0)
				return NULL;

			memcpy(&m_pszBuffer[pos],strFieldValue,iFieldLength);
			pos += iFieldLength;
		}
		m_pszBuffer[iLength] = '\0';

		return m_pszBuffer;
	}

	/*
	KRow::KRow(int id, KFields* pFields) : m_pFields(0),m_pFieldValueVector(0)
	{
		SetId(id);
		SetFields(pFields);
	}

	KRow::~KRow(void)
	{
		_deletea(m_pFieldValueVector);
	}

	void KRow::SetFields(KFields* pFields)
	{
		if(pFields==NULL)
			return;

		m_pFields = pFields;

		int iFieldCount = pFields->GetFieldCount();	// max = 128
		if(iFieldCount>0)
		{
			_delete(m_pFieldValueVector);
			m_pFieldValueVector = new KFieldValueVector();
			for(int i=0;i<iFieldCount;i++)
			{
				m_pFieldValueVector->push_back(new KFieldValue());
			}
		}
	}

	KField* KRow::GetField(int index)
	{
		return m_pFields->GetField(index); 
	}
	KField* KRow::GetField(LPCTSTR pszName)
	{ 
		return m_pFields->GetField(pszName);
	}

	//KFieldValue* KRow::GetFieldValue(LPCTSTR pszFieldName)
	//{
	//	int index = m_pFields->GetFieldIndex( pszFieldName );
	//	return this->GetFieldValue(index);
	//}
	//KFieldValue* KRow::GetFieldValue(int index)
	//{
	//	int iFieldCount = this->GetFields()->GetFieldCount();
	//	if(index<0 || index>=iFieldCount)
	//		return NULL;

	//	return m_pFieldValueVector->at(index);
	//}

	// ��¡
	KRow* KRow::Clone()
	{
		//KRow* pRow = new KRow(this->GetId(),this->GetFields());
		//pRow->m_pFieldValueVector->Cleanup();

		//int iFieldCount = this->GetFields()->GetFieldCount();
		//for(int i=0;i<iFieldCount;i++)
		//{
		//	pRow->m_pFieldValueVector->Add(this->GetFieldValue(i)->Clone());
		//}

		//return pRow;

		return NULL;
	}

	// ȡ�ü�¼ƫ�Ƶ�ַ
	LONG KRow::GetOffset()
	{
		LONG lOffset = 0;

		// ��¼��
		int iRecNo = this->GetId();
		// ��¼����
		int iRecLength = this->GetLength();
		// �ֶ�����
		int iFieldCount = this->GetFields()->GetFieldCount();
		// �ļ�ͷ����
		int iHeaderLength = sizeof(DBFHeader) + sizeof(DBFieldDescriptor) * iFieldCount;
		iHeaderLength += 1;

		// ƫ��
		lOffset = iHeaderLength + iRecLength * (iRecNo - 1);
		return lOffset;
	}

	// ת��Ϊ�ַ���
	int KRow::ToString(char* pszBuff)
	{
		//char szTemp[1024];
		//pszBuff = szTemp;

		int iLength = this->GetLength();
		for(int i=0;i<iLength;i++)
		{
			pszBuff[i] = 0x20;
		}

		// ɾ����� 
		// 0x20 - ' '��ʾ������¼
		// 0x2A - '*'��ʾɾ����¼
		pszBuff[0] = 0x20;

		int pos = 1;
		int iFieldCount = this->GetFields()->GetFieldCount();
		for(int i=0;i<iFieldCount;i++)
		{
			KField* pField				= this->GetField(i);
			//KFieldValue* pFieldValue	= this->GetFieldValue(i);
			//char* strFieldValue = getFieldValueString(pField,pFieldValue);
			//pField->GetStringValue();

			char* strFieldValue = (char*)KString::ToMByte(pField->GetStringValue());

			int iFieldLength = pField->GetLength();
			memcpy(&pszBuff[pos],strFieldValue,iFieldLength);
			pos += iFieldLength;
		}

		pszBuff[iLength] = '\0';
		return strlen(pszBuff);
	}

	///----------------------------------------------------
	/// ˽�к��� - Beg
	///----------------
	// �ֶ�ת��Ϊ�ַ���
	char* KRow::getFieldValueString(KField* pField,KFieldValue* pFieldValue)
	{
		char* pret = 0;

		KField::FieldType eFieldType = (KField::FieldType)pField->GetType();
		switch(eFieldType)
		{
		case KField::FieldType::Character:
			pret = getFieldValueString_Character(pField,pFieldValue);
			break;

		case KField::FieldType::Float:
			pret = getFieldValueString_Float(pField,pFieldValue);
			break;

		case KField::FieldType::Numeric:
			pret = getFieldValueString_Numeric(pField,pFieldValue);
			break;

		//case KField::FieldType::Integer:
		//	pret = getFieldValueString_Integer(pField,pFieldValue);
		//	break;
		}

		return pret;
	}
	// ������������
	char* KRow::getFieldValueString_Binary(KField* pField,KFieldValue* pFieldValue)
	{
		memset(m_szBuffer,0,1024);
		int iLength = pField->GetLength();
		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		return m_szBuffer;
	}
	// �ַ�������
	// �����,�ұ��Կո���
	char* KRow::getFieldValueString_Character(KField* pField,KFieldValue* pFieldValue)
	{
		int i;
		memset(m_szBuffer,0,1024);
		int iLength = pField->GetLength();
		if(iLength<1024)
		{
			for(i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}

			m_szBuffer[iLength] = '\0';
		}

		const char* str = KString::ToMByte(pFieldValue->GetStringValue());
		int len = strlen(str);
		//int m = len%2;
		if(len <=iLength)
		{
			strncpy(m_szBuffer,str,len);
		}
		else
		{
			strncpy(m_szBuffer,str,iLength);
		}

		return m_szBuffer;
	}
	// ����������
	// ռ8λ
	char* KRow::getFieldValueString_Date(KField* pField,KFieldValue* pFieldValue)
	{
		memset(m_szBuffer,0,1024);
		int iLength = pField->GetLength();
		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		const char* str = KString::ToMByte(pFieldValue->GetStringValue());
		int len = strlen(str);
		if(len <=iLength)
		{
			strncpy(m_szBuffer,str,len);
		}

		return m_szBuffer;
	}
	// ����ʱ��������
	// waiting...
	char* KRow::getFieldValueString_DateTime(KField* pField,KFieldValue* pFieldValue)
	{
		memset(m_szBuffer,0,1024);
		int iLength = pField->GetLength();
		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		return m_szBuffer;
	}
	// ˫����������
	// waiting...
	char* KRow::getFieldValueString_Double(KField* pField,KFieldValue* pFieldValue)
	{
		//memset(m_szBuffer,0,1024);
		//int iLength = pField->GetLength();
		//if(iLength<1024)
		//{
		//	for(int i=0;i<iLength;i++)
		//	{
		//		m_szBuffer[i] = 0x20;
		//	}
		//}

		//return m_szBuffer;
		return getFieldValueString_Float(pField,pFieldValue);
	}
	// ����������
	// ʹ��
	// �ֶγ��Ⱥ�С��λ���Ĳ����2ʱʹ�ÿ�ѧ����
	char* KRow::getFieldValueString_Float(KField* pField,KFieldValue* pFieldValue)
	{
		memset(m_szBuffer,0,1024);
		int iLength = pField->GetLength();
		int iDecimal = pField->GetDecimal();
		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		// ��������λ��
		int ipart = iLength - iDecimal - 1;

		// ȡ���ֶ�ֵ
		double value = pFieldValue->GetDoubleValue();

		// ��ʽ
		char szFormat[128];
		sprintf(szFormat,"%%% d.%df"
			,iLength
			,iDecimal
			);

		//
		if(ipart==1)
		{
			sprintf(szFormat,"%% .5e");
		}

		sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	// �߼�������
	char* KRow::getFieldValueString_Logical(KField* pField,KFieldValue* pFieldValue)
	{
		memset(m_szBuffer,0,1024);
		int iLength = pField->GetLength();
		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		return m_szBuffer;
	}
	// ��ֵ��������ASCII����,�Ҷ���
	// ����Կո���
	// �ұ߲����С��Ϊ�ԡ�0������
	// ����ǰ�и���'-'
	// С������С����'.'
	char* KRow::getFieldValueString_Numeric(KField* pField,KFieldValue* pFieldValue)
	{
		memset(m_szBuffer,0,1024);
		int iLength = pField->GetLength();
		int iDecimal = pField->GetDecimal();
		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		// ȡ���ֶ�ֵ
		double value = pFieldValue->GetDoubleValue();

		// ��ʽ
		char szFormat[128];
		sprintf(szFormat,"%%% d.%df"
			,iLength
			,iDecimal
			);

		sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	// ��������
	char* KRow::getFieldValueString_Integer(KField* pField,KFieldValue* pFieldValue)
	{
		return getFieldValueString_Numeric(pField,pFieldValue);
	}
	///----------------
	/// ˽�к��� - End
	///----------------------------------------------------
	*/

//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
