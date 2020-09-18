/******************************************************************************
 ** KField.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KField
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KFieldValue.h"

namespace XBase
{
///--------------------------------------------------------
}// namespace XBase - end
///--------------------------------------------------------

namespace XBase
{

	class KIND_EXT_CLASS KField : public KFieldValue
	{
	public:
		KField(void);
		KField(const KField& field);
		KField(LPCTSTR pszName,FieldType eType,int iLength);
		KField(LPCTSTR pszName,FieldType eType,int iLength,int iDecimal);
		KField(LPCTSTR pszName,FieldType eType,int iLength,LPCTSTR pszCaption);
		KField(LPCTSTR pszName,FieldType eType,int iLength,int iDecimal,LPCTSTR pszCaption);
		virtual ~KField(void);

	public:
		// 克隆
		KField* Clone();

		// 设置/获取字段长度
		void SetLength(int iLength) { m_iLength = iLength; }
		int GetLength() { return m_iLength; }

		// 设置/获取小数位数
		void SetDecimal(int iDecimal) { m_iDecimal = iDecimal; }
		int GetDecimal() { return m_iDecimal; }

		// 设置/获取字段标题
		void SetCaption(LPCTSTR pszCaption) { KString::SetString(m_pszCaption,pszCaption); }
		LPCTSTR GetCaption() { return m_pszCaption; }

		// 转换为字符串
		char* ToString();

	private:
		char* toString_Binary();
		char* toString_Character();
		char* toString_Date();
		char* toString_DateTime();
		char* toString_Double();
		char* toString_Float();
		char* toString_Logical();
		char* toString_Interger();
		char* toString_Numeric();

	private:
		// 字段长度
		int m_iLength;
		// 字段小数位数
		int m_iDecimal;
		// 字段标题
		LPTSTR m_pszCaption;

		// 临时字符串
		char m_szBuffer[1024];
	};

///--------------------------------------------------------
}// namespace XBase - end
///--------------------------------------------------------
