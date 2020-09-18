#include "StdAfx.h"
#include "KField.h"

namespace XBase
{
	KField::KField(void)
		:KFieldValue()
		,m_iLength(0)
		,m_iDecimal(0)
		,m_pszCaption(NULL)
	{
	}

	KField::KField(const KField& field)
		:KFieldValue(field)
		,m_iLength(field.m_iLength)
		,m_iDecimal(field.m_iDecimal)
		,m_pszCaption(NULL)
	{
		SetCaption(field.m_pszCaption);
	}

	KField::KField(LPCTSTR pszName,FieldType eType,int iLength)
		:KFieldValue()
		,m_iLength(iLength)
		,m_iDecimal(0)
		,m_pszCaption(NULL)
	{
		this->SetName(pszName);
		this->SetType(eType);
	}

	KField::KField(LPCTSTR pszName,FieldType eType,int iLength,int iDecimal)
		:KFieldValue()
		,m_iLength(iLength)
		,m_iDecimal(iDecimal)
		,m_pszCaption(NULL)
	{
		this->SetName(pszName);
		this->SetType(eType);
	}

	KField::KField(LPCTSTR pszName,FieldType eType,int iLength,LPCTSTR pszCaption)
		:KFieldValue()
		,m_iLength(iLength)
		,m_iDecimal(0)
		,m_pszCaption(NULL)
	{
		this->SetName(pszName);
		this->SetType(eType);
		this->SetCaption(pszCaption);
	}

	KField::KField(LPCTSTR pszName,FieldType eType,int iLength,int iDecimal,LPCTSTR pszCaption)
		:KFieldValue()
		,m_iLength(iLength)
		,m_iDecimal(iDecimal)
		,m_pszCaption(NULL)
	{
		this->SetName(pszName);
		this->SetType(eType);
		this->SetCaption(pszCaption);
	}

	KField::~KField(void)
	{
		_deletea(m_pszCaption);
	}

	// ��¡
	KField* KField::Clone()
	{
		return ( new KField(*this) );
	}

	// ת��Ϊ�ַ���
	char* KField::ToString()
	{
		char* pret = 0;

		FieldType eFieldType = (FieldType)this->GetType();
		switch(eFieldType)
		{
		case FieldType_Character:
			pret = toString_Character();
			break;

		case FieldType_Numeric:
			pret = toString_Numeric();
			break;

		//case FieldType_Interger:
		//	pret = toString_Interger();
		//	break;

		case FieldType_Float:
			pret = toString_Float();
			break;
		}

		return pret;
	}

	//-----------------------------------------------------
	// ˽�и�������
	//-----------------------------------------------------
	// ������������
	char* KField::toString_Binary()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// �ַ�������
	// �����,�ұ��Կո���
	char* KField::toString_Character()
	{
		int i;
		memset(m_szBuffer,0,1024);
		int iLength = this->GetLength();
		if(iLength<1024)
		{
			for(i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
			m_szBuffer[iLength] = '\0';
		}

		const char* str = KString::ToMByte(this->GetStringValue());
		int len = strlen(str);
		if(len <= iLength)
			strncpy(m_szBuffer,str,len);
		else
			strncpy(m_szBuffer,str,iLength);

		return m_szBuffer;
	}
	// ����������
	// ռ8λ
	char* KField::toString_Date()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// ����ʱ��������
	// waiting...
	char* KField::toString_DateTime()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// ˫����������
	// waiting...
	char* KField::toString_Double()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// ����������
	// ʹ��
	// �ֶγ��Ⱥ�С��λ���Ĳ����2ʱʹ�ÿ�ѧ����
	char* KField::toString_Float()
	{
		memset(m_szBuffer,0,1024);

		int iLength = this->GetLength();	// �ֶγ���
		int iDecimal = this->GetDecimal();	// С��λ��

		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		// ��������λ��
		int ipart = iLength - iDecimal - 1;	// 1��С����
		// ȡ���ֶ�ֵ
		double value = this->GetFloatValue();

		// ��ʽ
		char szFormat[128];
		sprintf(szFormat,"%%% d.%df",iLength,iDecimal);

		if(ipart==1)
		{
			sprintf(szFormat,"%% .5e");	// ʹ�ÿ�ѧ������
		}

		sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	// �߼�������
	// waiting...
	char* KField::toString_Logical()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// ��������
	// waiting...
	char* KField::toString_Interger()
	{
		memset(m_szBuffer,0,1024);

		int iLength = this->GetLength();	// �ֶγ���
		int iDecimal = this->GetDecimal();	// С��λ��

		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		//// ȡ���ֶ�ֵ
		//int value = this->GetIntergerValue();

		//// ��ʽ
		//char szFormat[128];
		//sprintf(szFormat,"%%% dd",iLength);
		//sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	// ��ֵ��������ASCII����,�Ҷ���
	// ����Կո���
	// �ұ߲����С��Ϊ�ԡ�0������
	// ����ǰ�и���'-'
	// С������С����'.'
	char* KField::toString_Numeric()
	{
		memset(m_szBuffer,0,1024);

		int iLength = this->GetLength();	// �ֶγ���
		int iDecimal = this->GetDecimal();	// С��λ��

		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		// ȡ���ֶ�ֵ
		double value = this->GetFloatValue();

		// ��ʽ
		char szFormat[128];
		sprintf(szFormat,"%%% d.%df",iLength,iDecimal);
		sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	//-----------------------------------------------------

///--------------------------------------------------------
}// namespace XBase - end
///--------------------------------------------------------
