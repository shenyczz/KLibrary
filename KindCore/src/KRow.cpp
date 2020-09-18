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

	// 克隆
	KRow* KRow::Clone()
	{
		return new KRow(*this);
	}

	// 取得记录偏移地址
	LONG KRow::GetOffset()
	{
		LONG lOffset = 0;

		// 记录号
		int iRecNo = this->GetId();
		// 记录长度
		int iRecLength = this->GetLength();
		// 字段数量
		int iFieldCount = this->GetFieldCount();
		// 文件头长度
		int iHeaderLength = sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount;
		iHeaderLength += 1;	// 终止标识 0x0d

		// 偏移
		lOffset = iHeaderLength + iRecLength * (iRecNo - 1);
		return lOffset;
	}

	// 转换为字符串
	char* KRow::ToString()
	{
		_deletea(m_pszBuffer);

		// 记录长度
		int iLength = this->GetLength();
		if(iLength >= 4000)
			return NULL;

		// 分配足够内存
		m_pszBuffer = new char[iLength+1];
		memset(m_pszBuffer,0,sizeof(char)*(iLength+1));
		for(int i=0;i<iLength;i++)
		{
			m_pszBuffer[i] = 0x20;
		}

		// 删除标记 
		// 0x20 - ' '表示正常记录
		// 0x2A - '*'表示删除记录
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

	// 克隆
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

	// 取得记录偏移地址
	LONG KRow::GetOffset()
	{
		LONG lOffset = 0;

		// 记录号
		int iRecNo = this->GetId();
		// 记录长度
		int iRecLength = this->GetLength();
		// 字段数量
		int iFieldCount = this->GetFields()->GetFieldCount();
		// 文件头长度
		int iHeaderLength = sizeof(DBFHeader) + sizeof(DBFieldDescriptor) * iFieldCount;
		iHeaderLength += 1;

		// 偏移
		lOffset = iHeaderLength + iRecLength * (iRecNo - 1);
		return lOffset;
	}

	// 转换为字符串
	int KRow::ToString(char* pszBuff)
	{
		//char szTemp[1024];
		//pszBuff = szTemp;

		int iLength = this->GetLength();
		for(int i=0;i<iLength;i++)
		{
			pszBuff[i] = 0x20;
		}

		// 删除标记 
		// 0x20 - ' '表示正常记录
		// 0x2A - '*'表示删除记录
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
	/// 私有函数 - Beg
	///----------------
	// 字段转换为字符串
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
	// 二进制型数据
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
	// 字符型数据
	// 左对齐,右边以空格补足
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
	// 日期型数据
	// 占8位
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
	// 日期时间型数据
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
	// 双精度型数据
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
	// 浮点型数据
	// 使用
	// 字段长度和小数位数的差等于2时使用科学计数
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

		// 整数部分位数
		int ipart = iLength - iDecimal - 1;

		// 取得字段值
		double value = pFieldValue->GetDoubleValue();

		// 格式
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
	// 逻辑型数据
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
	// 数值型数据以ASCII码存放,右对齐
	// 左边以空格补齐
	// 右边不足的小数为以‘0’补齐
	// 负数前有负号'-'
	// 小数包括小数点'.'
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

		// 取得字段值
		double value = pFieldValue->GetDoubleValue();

		// 格式
		char szFormat[128];
		sprintf(szFormat,"%%% d.%df"
			,iLength
			,iDecimal
			);

		sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	// 整型数据
	char* KRow::getFieldValueString_Integer(KField* pField,KFieldValue* pFieldValue)
	{
		return getFieldValueString_Numeric(pField,pFieldValue);
	}
	///----------------
	/// 私有函数 - End
	///----------------------------------------------------
	*/

//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
