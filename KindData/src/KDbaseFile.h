/******************************************************************************
 ** Header File: KDbf.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KDbf
 ** Version: 
 ** Function: 操作 DBF 文件
 ** Explain: 

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site: 

 ** Mender: 
 ** DateTime: 
 ** Explain: 

 ** Usage:

#include <locale.h>
#include "Misce.h"

int _tmain(int argc, _TCHAR* argv[])
{
	int i(0),k(0);

	// 注释掉下面这一行以后,程序无法正确显示
	TCHAR* szLocale = _tsetlocale( LC_ALL, _T("CHS") );

	KDbf dbf;
	dbf.LoadData(_T("D:\\temp\\市界面.dbf"));

	KRows* pRows = dbf.GetRows();

	int iCount = pRows->Count();

	int iv(0);
	double dv(0);
	TCHAR* sv(_T(""));

	KRow* pRow = pRows->GetFirst();
	int size = pRow->GetFields()->Size();
	for(int k=0;k<size;k++)
	{
		sv = pRow->GetFields()->GetField(k)->GetName();
		_tprintf(_T("%s   "),sv);
	}
	_tprintf(_T("\n"));

	//for(i=0; i<iCount; i++)
	while(pRow)
	{
		//pRow = pRows->GetRow(i+1);

		for(k=0; k<size ;k++)
		{
			KField* pField = pRow->GetFields()->GetField(k);
			KFieldValue* pFieldValue = pRow->GetFieldValue(k);

			switch(pField->GetType())
			{
			case KField::eFieldTypeInteger:
			iv = pFieldValue->GetInt();
				_tprintf(_T("%d "),iv);
				break;

			case KField::eFieldTypeDouble:
				dv = pFieldValue->GetDouble();
				_tprintf(_T("%f "),dv);
			break;

			case KField::eFieldTypeString:
				sv = pFieldValue->GetString();
			_tprintf(_T("%s "),sv);
				break;
			}
		}
		_tprintf(_T("\n"));

		pRow = pRows->GetNext();
	}

	getchar();

	return 0;
}

******************************************************************************/

#pragma once

#include "Dbf.inl"
#include "KRows.h"
#include "KDataObject.h"


namespace XBase
{

	class KIND_EXT_CLASS KDbaseFile : public KRows
	{
	public:
		KDbaseFile(void);
		KDbaseFile(LPCTSTR lpszFile) throw(KException);
		virtual ~KDbaseFile(void);

	public:
		BOOL LoadData(LPCTSTR pszFile);

	public:

		// 取得指定记录序号指定字段的值
		_variant_t GetValue(int iRecNo, LPCTSTR lpszFieldName);

		// 更新
		BOOL Update();
		// 更新记录
		BOOL Update(KRow* pRow);
		// 更新记录
		BOOL Update(int iRecNo);
		// 更新字段值
		BOOL Update(int iRecNo,LPCTSTR lpszFieldName, _variant_t var);
		BOOL Update(int iRecNo,LPCTSTR lpszFieldName, LPCTSTR lpszValue);
		BOOL Update(int iRecNo,LPCTSTR lpszFieldName, int iValue);
		BOOL Update(int iRecNo,LPCTSTR lpszFieldName, double dValue);

		// 保存
		BOOL Save();
		BOOL SaveAs(LPCTSTR lpszFile);

	public:
		static BOOL Create(LPCTSTR lpszFile,KRows* pRows);
	};

//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
