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

	// 克隆
	KField* KField::Clone()
	{
		return ( new KField(*this) );
	}

	// 转换为字符串
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
	// 私有辅助函数
	//-----------------------------------------------------
	// 二进制型数据
	char* KField::toString_Binary()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// 字符型数据
	// 左对齐,右边以空格补足
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
	// 日期型数据
	// 占8位
	char* KField::toString_Date()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// 日期时间型数据
	// waiting...
	char* KField::toString_DateTime()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// 双精度型数据
	// waiting...
	char* KField::toString_Double()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// 浮点型数据
	// 使用
	// 字段长度和小数位数的差等于2时使用科学计数
	char* KField::toString_Float()
	{
		memset(m_szBuffer,0,1024);

		int iLength = this->GetLength();	// 字段长度
		int iDecimal = this->GetDecimal();	// 小数位数

		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		// 整数部分位数
		int ipart = iLength - iDecimal - 1;	// 1是小数点
		// 取得字段值
		double value = this->GetFloatValue();

		// 格式
		char szFormat[128];
		sprintf(szFormat,"%%% d.%df",iLength,iDecimal);

		if(ipart==1)
		{
			sprintf(szFormat,"%% .5e");	// 使用科学计数法
		}

		sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	// 逻辑型数据
	// waiting...
	char* KField::toString_Logical()
	{
		memset(m_szBuffer,0,1024);
		return m_szBuffer;
	}
	// 整型数据
	// waiting...
	char* KField::toString_Interger()
	{
		memset(m_szBuffer,0,1024);

		int iLength = this->GetLength();	// 字段长度
		int iDecimal = this->GetDecimal();	// 小数位数

		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		//// 取得字段值
		//int value = this->GetIntergerValue();

		//// 格式
		//char szFormat[128];
		//sprintf(szFormat,"%%% dd",iLength);
		//sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	// 数值型数据以ASCII码存放,右对齐
	// 左边以空格补齐
	// 右边不足的小数为以‘0’补齐
	// 负数前有负号'-'
	// 小数包括小数点'.'
	char* KField::toString_Numeric()
	{
		memset(m_szBuffer,0,1024);

		int iLength = this->GetLength();	// 字段长度
		int iDecimal = this->GetDecimal();	// 小数位数

		if(iLength<1024)
		{
			for(int i=0;i<iLength;i++)
			{
				m_szBuffer[i] = 0x20;
			}
		}

		// 取得字段值
		double value = this->GetFloatValue();

		// 格式
		char szFormat[128];
		sprintf(szFormat,"%%% d.%df",iLength,iDecimal);
		sprintf(m_szBuffer,szFormat,value);

		return m_szBuffer;
	}
	//-----------------------------------------------------

///--------------------------------------------------------
}// namespace XBase - end
///--------------------------------------------------------
