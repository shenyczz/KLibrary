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
 **   Author: ������.֣�� (shenyczz@163.com)
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
		// ��¡
		KField* Clone();

		// ����/��ȡ�ֶγ���
		void SetLength(int iLength) { m_iLength = iLength; }
		int GetLength() { return m_iLength; }

		// ����/��ȡС��λ��
		void SetDecimal(int iDecimal) { m_iDecimal = iDecimal; }
		int GetDecimal() { return m_iDecimal; }

		// ����/��ȡ�ֶα���
		void SetCaption(LPCTSTR pszCaption) { KString::SetString(m_pszCaption,pszCaption); }
		LPCTSTR GetCaption() { return m_pszCaption; }

		// ת��Ϊ�ַ���
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
		// �ֶγ���
		int m_iLength;
		// �ֶ�С��λ��
		int m_iDecimal;
		// �ֶα���
		LPTSTR m_pszCaption;

		// ��ʱ�ַ���
		char m_szBuffer[1024];
	};

///--------------------------------------------------------
}// namespace XBase - end
///--------------------------------------------------------
