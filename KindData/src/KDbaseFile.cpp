#include "StdAfx.h"
#include "KDbaseFile.h"

#include "KRows.h"

namespace XBase
{

	KDbaseFile::KDbaseFile(void)
		: KRows()
	{
	}

	KDbaseFile::KDbaseFile(LPCTSTR lpszFile) throw(KException)
		: KRows()
	{
		if(!this->LoadData(lpszFile))
		{
			throw KException(_T("Open DBF file error."));
		}
	}

	KDbaseFile::~KDbaseFile(void)
	{
	}

	//---------------------------------------------------------
	// Name:     LoadData
	// Function: 装载数据
	// Parameters:
	//	[i] pszFile - DBF 文件名
	//
	// Return: true/false
	// Comment: 
	//---------------------------------------------------------
	BOOL KDbaseFile::LoadData(LPCTSTR pszFile)
	{
		int i;

		this->SetName(pszFile);

		// 打开文件
		FILE* fp = _tfopen(pszFile,_T("rb"));
		if(fp==NULL)
			return FALSE;


		// 读取文件头
		DBFHeader oDBFHeader;					// 文件头
		fseek(fp,0L,SEEK_SET);
		fread(&oDBFHeader,sizeof(DBFHeader),1,fp);

		// 计算字段数量(需要减去0x0d字节)
		int field_counts = (oDBFHeader.iLengthOfHeader - sizeof(DBFHeader) - 1) / sizeof(FieldDescriptor);

		// 读取字段信息构建字段集 KFields
		KRow rowTemp;
		KRow* pRowTemp = NULL;
		for(i=0;i<field_counts;i++)
		{
			FieldDescriptor oDBFieldDescriptor;	// 字段描述器
			fread(&oDBFieldDescriptor,sizeof(FieldDescriptor),1,fp);

			KField* pField = new KField
			(
				KString::ToTChar(oDBFieldDescriptor.szName),
				(FieldType)oDBFieldDescriptor.cType,
				oDBFieldDescriptor.cFieldLength,
				oDBFieldDescriptor.cFieldDecimal,
				_T("")
			);

			rowTemp.Add(pField);
		}

		// 读取头结束标记 0x0d
		char cEndOfHeader = fgetc(fp);
		if(cEndOfHeader != 0x0d )
			return FALSE;

		// 每条记录长度
		int iRecordLength = oDBFHeader.iLengthOfRecord;

		// 记录总数
		long lNumberOfRecord = oDBFHeader.lNumberOfRecord;

		// 为一条记录数据分配足够多的内存
		char *pRecordValue = new char[iRecordLength+1];
		memset((char*)pRecordValue,0,iRecordLength+1);

		for(int rec=0; rec<(int)lNumberOfRecord; rec++)
		{
			// 行对象
			rowTemp.SetId(rec+1);

			// 读入一条记录的内容
			memset((char*)pRecordValue,(char)0x20,iRecordLength+1);
			fread( pRecordValue, sizeof(char) * iRecordLength, 1, fp);

			int pos = 1;
			int iFieldCount = rowTemp.GetFieldCount();
			for(i=0; i<iFieldCount; i++)
			{
				KField* pField = rowTemp.GetField(i);		// 字段对象
				int fldType = pField->GetType();		// 该字段类型
				int fldLen = pField->GetLength();		// 字段长度

				// DBF 中保存的是 char
				char* fldValue = new char[fldLen+1];
				memset(fldValue,0,sizeof(char)*(fldLen+1));
				for ( int t = 0, k = pos; k < pos+fldLen; k++, t++ )
				{
					fldValue[t] = pRecordValue[k];
				}
				fldValue[fldLen] = '\0';

				switch( fldType )
				{
				case FieldType_Character:
					pField->SetValue( KString::TrimString((LPTSTR)KString::ToTChar(fldValue)) );
					break;

				default:
					pField->SetValue( atof(fldValue) );
					break;
				}

				pos += fldLen;
				_deletea(fldValue);
			}// i

			// 添加行记录
			this->Add(rowTemp.Clone());

		}// rec

		_deletea(pRecordValue);

		// 关闭文件
		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	// 取得指定记录序号指定字段的值
	_variant_t KDbaseFile::GetValue(int id, LPCTSTR lpszFieldName)
	{
		_variant_t var(0);
		/*
		int type = GetFieldType(lpszFieldName);

		KRow* pRow = m_pRows->GetRowById(id);
		if(pRow)
		{
			if(type == KField::FieldType::None)
				var = _variant_t(0);
			else if(type == KField::FieldType::Character)
				var = _variant_t(pRow->GetFieldValue(lpszFieldName)->GetStringValue());
			else if(type == KField::FieldType::Float)
				var = _variant_t(pRow->GetFieldValue(lpszFieldName)->GetDoubleValue());
			else if(type == KField::FieldType::Numeric)
				var = _variant_t(pRow->GetFieldValue(lpszFieldName)->GetDoubleValue());
		}
		*/
		return var;
	}

	// 更新
	BOOL KDbaseFile::Update()
	{
		int i;

		// 系统时间
		SYSTEMTIME stm;
		::GetSystemTime(&stm);

		// 文件名
		KString strFile = this->GetName();
		FILE* fp = _tfopen((LPCTSTR)strFile,_T("r+b"));
		if(!fp)
			return FALSE;

		//-------------------------------------------------
		// 记录数量
		int iRowCount = this->GetRowCount();
		if(iRowCount==0)
			return FALSE;

		// 字段数量
		int iFieldCount = this->GetRow(0)->GetFieldCount();
		// 每条记录长度
		int iRecordLength = this->GetRow(0)->GetRecordLength();
		// 每条记录字符数最多为 4000
		if(iRecordLength>=4000)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		DBFHeader header;
		memset(&header,0,sizeof(DBFHeader));

		header.cVersion			= 0x03;
		header.cYear			= (BYTE)(stm.wYear - 1900);
		header.cMonth			= (BYTE)stm.wMonth;
		header.cDay				= (BYTE)stm.wDay;

		// 总记录数
		header.lNumberOfRecord	= iRowCount;
		// 文件头 + 字段结构描述部分的长度 + 终止标识0x0d
		header.iLengthOfHeader	= sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount + 1;
		// 每条记录长度
		header.iLengthOfRecord	= iRecordLength;
		// Language Driver ID
		header.cLdid = 0x4d;

		fseek(fp,0,SEEK_SET);
		fwrite(&header,sizeof(DBFHeader),1,fp);
		//-------------------------------------------------
		// 字段描述块
		for(i=0;i<iFieldCount;i++)
		{
			FieldDescriptor fldd;
			memset(&fldd,0,sizeof(FieldDescriptor));

			KField* pField = this->GetFields()->GetField(i);

			// 字段名
			strncpy(fldd.szName,KString::ToMByte(pField->GetName()),10);
			// 字段类型
			fldd.cType = (BYTE)pField->GetType();
			// 字段数据相对于该记录的偏移量
			fldd.lOffset = 0;
			// 字段宽度
			fldd.cFieldLength = pField->GetLength();
			// 小数点后位数
			fldd.cFieldDecimal = pField->GetDecimal();

			fwrite(&fldd,sizeof(FieldDescriptor),1,fp);
		}
		//-------------------------------------------------
		char cTerminator = 0x0d;	// 终止符号 0x0d
		fwrite(&cTerminator,sizeof(char),1,fp);
		//-------------------------------------------------
		// 记录
		for(i=0;i<iRowCount;i++)
		{
			KRow* pRow = this->GetRow(i);
			if(!pRow)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			char* pszRow = pRow->ToString();
			int lenRow = strlen(pszRow);
			if(lenRow != iRecordLength)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			fwrite(pszRow,sizeof(char),iRecordLength,fp);
		}
		//-------------------------------------------------
		char cEnd = 0x1a;	// 文件结尾
		fwrite(&cEnd,sizeof(char),1,fp);
		//-------------------------------------------------
		fflush(fp);
		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	// 更新记录
	BOOL KDbaseFile::Update(KRow* pRow)
	{
		if(!pRow)
			return FALSE;

		// 系统时间
		SYSTEMTIME stm;
		::GetSystemTime(&stm);

		// 文件名
		LPCTSTR lpszFile = (LPCTSTR)this->GetName();
		FILE* fp = _tfopen(lpszFile,_T("r+b"));
		if(!fp)
			return FALSE;

		//-------------------------------------------------
		// 记录数量
		int iRecordCount = this->GetRowCount();
		if(iRecordCount==0)
			return FALSE;

		// 字段数量
		int iFieldCount = this->GetRow(0)->GetFieldCount();
		// 每条记录长度
		int iRecordLength = this->GetRow(0)->GetRecordLength();
		// 每条记录字符数最多为 4000
		if(iRecordLength>=4000)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// 填充基本文件头
		DBFHeader header;
		memset(&header,0,sizeof(DBFHeader));

		header.cVersion			= 0x03;
		header.cYear			= (BYTE)(stm.wYear - 1900);
		header.cMonth			= (BYTE)stm.wMonth;
		header.cDay				= (BYTE)stm.wDay;

		// 总记录数
		header.lNumberOfRecord	= iRecordCount;
		// 文件头 + 字段结构描述部分的长度 + 终止标识0x0d
		header.iLengthOfHeader	= sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount + 1;
		// 每条记录长度
		header.iLengthOfRecord	= iRecordLength;
		// Language Driver ID
		header.cLdid = 0x4d;

		fseek(fp,0,SEEK_SET);
		fwrite(&header,sizeof(DBFHeader),1,fp);
		//-------------------------------------------------
		char* pszRecord = pRow->ToString();
		int len = strlen(pszRecord);
		if(len != iRecordLength || len<=0)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		long lOffset = pRow->GetOffset();
		fseek(fp,lOffset,SEEK_SET);
		fwrite(pszRecord,sizeof(char),iRecordLength,fp);
		//-------------------------------------------------
		fflush(fp);
		fclose(fp);
		fp = NULL;

		return TRUE;
	}
	// 更新记录
	BOOL KDbaseFile::Update(int iRecNo)
	{
		KRow* pRow = this->GetRowById(iRecNo);
		if(!pRow)	return FALSE;
		else		return this->Update(pRow);
	}

	// 更新字段
	BOOL KDbaseFile::Update(int iRecNo, LPCTSTR lpszFieldName, _variant_t var)
	{
		KRow* pRow = this->GetRowById(iRecNo);
		if(!pRow)
			return FALSE;

		KField* pField = pRow->GetField(lpszFieldName);
		if(!pField)
			return FALSE;

		if(var.vt == VT_EMPTY || var.vt == VT_NULL)
			return FALSE;

		pField->SetValue(var);
		return this->Update(pRow);
	}
	BOOL KDbaseFile::Update(int iRecNo,LPCTSTR lpszFieldName, LPCTSTR lpszValue)
	{
		return this->Update(iRecNo,lpszFieldName,_variant_t(lpszValue));
	}
	BOOL KDbaseFile::Update(int iRecNo,LPCTSTR lpszFieldName, int iValue)
	{
		return this->Update(iRecNo,lpszFieldName,_variant_t(iValue));
	}
	BOOL KDbaseFile::Update(int iRecNo,LPCTSTR lpszFieldName, double dValue)
	{
		return this->Update(iRecNo,lpszFieldName,_variant_t(dValue));
	}

	// 保存
	BOOL KDbaseFile::Save()
	{
		return this->Update();
	}
	// 保存为
	BOOL KDbaseFile::SaveAs(LPCTSTR lpszFile)
	{
		return KDbaseFile::Create(lpszFile,this);
	}

	//static
	BOOL KDbaseFile::Create(LPCTSTR lpszFile,KRows* pRows)
	{
		int i;

		// 系统时间
		SYSTEMTIME stm;
		::GetSystemTime(&stm);

		// 打开文件
		FILE* fp = _tfopen(lpszFile,_T("wb"));
		if(!fp)
			return FALSE;

		//-------------------------------------------------
		// 记录数量
		int iRowCount = pRows->GetRowCount();
		if(iRowCount==0)
			return FALSE;

		// 字段数量
		int iFieldCount = pRows->GetRow(0)->GetFieldCount();
		// 每条记录长度
		int iRecordLength = pRows->GetRow(0)->GetRecordLength();
		// 每条记录字符数最多为 4000
		if(iRecordLength>=4000)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		//-------------------------------------------------
		// 文件头
		DBFHeader header;
		memset(&header,0,sizeof(DBFHeader));

		header.cVersion			= 0x03;
		header.cYear			= (BYTE)(stm.wYear - 1900);
		header.cMonth			= (BYTE)stm.wMonth;
		header.cDay				= (BYTE)stm.wDay;

		// 总记录数
		header.lNumberOfRecord	= iRowCount;
		// 文件头 + 字段结构描述部分的长度 + 终止标识0x0d
		header.iLengthOfHeader	= sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount + 1;
		// 每条记录长度
		header.iLengthOfRecord	= iRecordLength;
		// Language Driver ID
		header.cLdid = 0x4d;

		fseek(fp,0,SEEK_SET);
		fwrite(&header,sizeof(DBFHeader),1,fp);
		//-------------------------------------------------
		// 字段描述块
		for(i=0;i<iFieldCount;i++)
		{
			FieldDescriptor fldd;
			memset(&fldd,0,sizeof(FieldDescriptor));

			KField* pField = pRows->GetFields()->GetField(i);

			// 字段名
			strncpy(fldd.szName,KString::ToMByte(pField->GetName()),10);
			// 字段类型
			fldd.cType = (BYTE)pField->GetType();
			// 字段数据相对于该记录的偏移量
			fldd.lOffset = 0;
			// 字段宽度
			fldd.cFieldLength = pField->GetLength();
			// 小数点后位数
			fldd.cFieldDecimal = pField->GetDecimal();

			fwrite(&fldd,sizeof(FieldDescriptor),1,fp);
		}
		//-------------------------------------------------
		char cTerminator = 0x0d;	// 终止符号 0x0d
		fwrite(&cTerminator,sizeof(char),1,fp);
		//-------------------------------------------------
		// 记录
		for(i=0;i<iRowCount;i++)
		{
			KRow* pRow = pRows->GetRow(i);
			if(!pRow)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			char* pszRow = pRow->ToString();
			int lenRow = strlen(pszRow);
			if(lenRow != iRecordLength)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			fwrite(pszRow,sizeof(char),iRecordLength,fp);
		}
		//-------------------------------------------------
		char cEnd = 0x1a;	// 文件结尾
		fwrite(&cEnd,sizeof(char),1,fp);
		//-------------------------------------------------
		fflush(fp);
		fclose(fp);
		fp = NULL;

		return TRUE;

		/*
		int i;

		SYSTEMTIME stm;
		::GetSystemTime(&stm);

		FILE* fp = _tfopen(lpszFile,_T("wb"));
		if(!fp)
			return FALSE;

		//-------------------------------------------------
		DBFHeader header;
		memset(&header,0,sizeof(DBFHeader));

		// 记录数量
		int iRecordCount = pRows->GetRowCount();
		// 字段数量
		int iFieldCount = pRows->GetFields()->GetFieldCount();
		// 每条记录长度
		int iRecordLength = pRows->GetRecordLength();

		// 每条记录字符数最多为 4000
		if(iRecordLength>=4000)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		header.cVersion			= 0x03;
		header.cYear			= (BYTE)(stm.wYear - 1900);
		header.cMonth			= (BYTE)stm.wMonth;
		header.cDay				= (BYTE)stm.wDay;

		// 总记录数
		header.lNumberOfRecord	= iRecordCount;
		// 文件头 + 字段结构描述部分的长度 + 终止标识0x0d
		header.iLengthOfHFLD	= sizeof(DBFHeader) + sizeof(DBFieldDescriptor) * iFieldCount + 1;
		// 每条记录长度
		header.iLengthOfRecord	= iRecordLength;

		// Language Driver ID
		header.cLdid = 0x4d;

		fwrite(&header,sizeof(DBFHeader),1,fp);
		//-------------------------------------------------
		// 字段描述
		for(i=0;i<iFieldCount;i++)
		{
			KField* pField = pRows->GetFields()->GetField(i);

			DBFieldDescriptor fldd;
			memset(&fldd,0,sizeof(DBFieldDescriptor));

			// 字段名
			strncpy(fldd.szName,KString::ToMByte(pField->GetName()),10);
			// 字段类型
			fldd.cType = (BYTE)pField->GetType();
			// 字段数据相对于该记录的偏移量
			fldd.lOffset = 0;
			// 字段宽度
			fldd.cFieldLength = pField->GetLength();
			// 小数点后位数
			fldd.cFieldDecimal = pField->GetDecimal();

			fwrite(&fldd,sizeof(DBFieldDescriptor),1,fp);
		}
		//-------------------------------------------------
		char cTerminator = 0x0d;
		fwrite(&cTerminator,sizeof(char),1,fp);
		//-------------------------------------------------
		char szRecord[4000];	// 每条记录字符数最多为  4000
		for(i=0;i<iRecordCount;i++)
		{
			memset(szRecord,0,sizeof(char)*4000);
			KRow* pRow = pRows->GetRowById(i+1);

			// 记录转换为字符
			int len = pRow->ToString(szRecord);
			if(len !=iRecordLength || len<=0)
			{
				fclose(fp);
				fp = NULL;
				return FALSE;
			}

			// 
			fwrite(szRecord,sizeof(char),iRecordLength,fp);
		}
		//-------------------------------------------------
		char cEnd = 0x1a;
		fwrite(&cEnd,sizeof(char),1,fp);
		//-------------------------------------------------
		fflush(fp);
		fclose(fp);
		fp = NULL;
		*/
		return TRUE;
	}

	//---------------------------------------------------------
}// namespace XBase - end
//---------------------------------------------------------
