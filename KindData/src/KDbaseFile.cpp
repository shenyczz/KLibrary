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
	// Function: װ������
	// Parameters:
	//	[i] pszFile - DBF �ļ���
	//
	// Return: true/false
	// Comment: 
	//---------------------------------------------------------
	BOOL KDbaseFile::LoadData(LPCTSTR pszFile)
	{
		int i;

		this->SetName(pszFile);

		// ���ļ�
		FILE* fp = _tfopen(pszFile,_T("rb"));
		if(fp==NULL)
			return FALSE;


		// ��ȡ�ļ�ͷ
		DBFHeader oDBFHeader;					// �ļ�ͷ
		fseek(fp,0L,SEEK_SET);
		fread(&oDBFHeader,sizeof(DBFHeader),1,fp);

		// �����ֶ�����(��Ҫ��ȥ0x0d�ֽ�)
		int field_counts = (oDBFHeader.iLengthOfHeader - sizeof(DBFHeader) - 1) / sizeof(FieldDescriptor);

		// ��ȡ�ֶ���Ϣ�����ֶμ� KFields
		KRow rowTemp;
		KRow* pRowTemp = NULL;
		for(i=0;i<field_counts;i++)
		{
			FieldDescriptor oDBFieldDescriptor;	// �ֶ�������
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

		// ��ȡͷ������� 0x0d
		char cEndOfHeader = fgetc(fp);
		if(cEndOfHeader != 0x0d )
			return FALSE;

		// ÿ����¼����
		int iRecordLength = oDBFHeader.iLengthOfRecord;

		// ��¼����
		long lNumberOfRecord = oDBFHeader.lNumberOfRecord;

		// Ϊһ����¼���ݷ����㹻����ڴ�
		char *pRecordValue = new char[iRecordLength+1];
		memset((char*)pRecordValue,0,iRecordLength+1);

		for(int rec=0; rec<(int)lNumberOfRecord; rec++)
		{
			// �ж���
			rowTemp.SetId(rec+1);

			// ����һ����¼������
			memset((char*)pRecordValue,(char)0x20,iRecordLength+1);
			fread( pRecordValue, sizeof(char) * iRecordLength, 1, fp);

			int pos = 1;
			int iFieldCount = rowTemp.GetFieldCount();
			for(i=0; i<iFieldCount; i++)
			{
				KField* pField = rowTemp.GetField(i);		// �ֶζ���
				int fldType = pField->GetType();		// ���ֶ�����
				int fldLen = pField->GetLength();		// �ֶγ���

				// DBF �б������ char
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

			// ����м�¼
			this->Add(rowTemp.Clone());

		}// rec

		_deletea(pRecordValue);

		// �ر��ļ�
		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	// ȡ��ָ����¼���ָ���ֶε�ֵ
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

	// ����
	BOOL KDbaseFile::Update()
	{
		int i;

		// ϵͳʱ��
		SYSTEMTIME stm;
		::GetSystemTime(&stm);

		// �ļ���
		KString strFile = this->GetName();
		FILE* fp = _tfopen((LPCTSTR)strFile,_T("r+b"));
		if(!fp)
			return FALSE;

		//-------------------------------------------------
		// ��¼����
		int iRowCount = this->GetRowCount();
		if(iRowCount==0)
			return FALSE;

		// �ֶ�����
		int iFieldCount = this->GetRow(0)->GetFieldCount();
		// ÿ����¼����
		int iRecordLength = this->GetRow(0)->GetRecordLength();
		// ÿ����¼�ַ������Ϊ 4000
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

		// �ܼ�¼��
		header.lNumberOfRecord	= iRowCount;
		// �ļ�ͷ + �ֶνṹ�������ֵĳ��� + ��ֹ��ʶ0x0d
		header.iLengthOfHeader	= sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount + 1;
		// ÿ����¼����
		header.iLengthOfRecord	= iRecordLength;
		// Language Driver ID
		header.cLdid = 0x4d;

		fseek(fp,0,SEEK_SET);
		fwrite(&header,sizeof(DBFHeader),1,fp);
		//-------------------------------------------------
		// �ֶ�������
		for(i=0;i<iFieldCount;i++)
		{
			FieldDescriptor fldd;
			memset(&fldd,0,sizeof(FieldDescriptor));

			KField* pField = this->GetFields()->GetField(i);

			// �ֶ���
			strncpy(fldd.szName,KString::ToMByte(pField->GetName()),10);
			// �ֶ�����
			fldd.cType = (BYTE)pField->GetType();
			// �ֶ���������ڸü�¼��ƫ����
			fldd.lOffset = 0;
			// �ֶο��
			fldd.cFieldLength = pField->GetLength();
			// С�����λ��
			fldd.cFieldDecimal = pField->GetDecimal();

			fwrite(&fldd,sizeof(FieldDescriptor),1,fp);
		}
		//-------------------------------------------------
		char cTerminator = 0x0d;	// ��ֹ���� 0x0d
		fwrite(&cTerminator,sizeof(char),1,fp);
		//-------------------------------------------------
		// ��¼
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
		char cEnd = 0x1a;	// �ļ���β
		fwrite(&cEnd,sizeof(char),1,fp);
		//-------------------------------------------------
		fflush(fp);
		fclose(fp);
		fp = NULL;

		return TRUE;
	}

	// ���¼�¼
	BOOL KDbaseFile::Update(KRow* pRow)
	{
		if(!pRow)
			return FALSE;

		// ϵͳʱ��
		SYSTEMTIME stm;
		::GetSystemTime(&stm);

		// �ļ���
		LPCTSTR lpszFile = (LPCTSTR)this->GetName();
		FILE* fp = _tfopen(lpszFile,_T("r+b"));
		if(!fp)
			return FALSE;

		//-------------------------------------------------
		// ��¼����
		int iRecordCount = this->GetRowCount();
		if(iRecordCount==0)
			return FALSE;

		// �ֶ�����
		int iFieldCount = this->GetRow(0)->GetFieldCount();
		// ÿ����¼����
		int iRecordLength = this->GetRow(0)->GetRecordLength();
		// ÿ����¼�ַ������Ϊ 4000
		if(iRecordLength>=4000)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}

		// �������ļ�ͷ
		DBFHeader header;
		memset(&header,0,sizeof(DBFHeader));

		header.cVersion			= 0x03;
		header.cYear			= (BYTE)(stm.wYear - 1900);
		header.cMonth			= (BYTE)stm.wMonth;
		header.cDay				= (BYTE)stm.wDay;

		// �ܼ�¼��
		header.lNumberOfRecord	= iRecordCount;
		// �ļ�ͷ + �ֶνṹ�������ֵĳ��� + ��ֹ��ʶ0x0d
		header.iLengthOfHeader	= sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount + 1;
		// ÿ����¼����
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
	// ���¼�¼
	BOOL KDbaseFile::Update(int iRecNo)
	{
		KRow* pRow = this->GetRowById(iRecNo);
		if(!pRow)	return FALSE;
		else		return this->Update(pRow);
	}

	// �����ֶ�
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

	// ����
	BOOL KDbaseFile::Save()
	{
		return this->Update();
	}
	// ����Ϊ
	BOOL KDbaseFile::SaveAs(LPCTSTR lpszFile)
	{
		return KDbaseFile::Create(lpszFile,this);
	}

	//static
	BOOL KDbaseFile::Create(LPCTSTR lpszFile,KRows* pRows)
	{
		int i;

		// ϵͳʱ��
		SYSTEMTIME stm;
		::GetSystemTime(&stm);

		// ���ļ�
		FILE* fp = _tfopen(lpszFile,_T("wb"));
		if(!fp)
			return FALSE;

		//-------------------------------------------------
		// ��¼����
		int iRowCount = pRows->GetRowCount();
		if(iRowCount==0)
			return FALSE;

		// �ֶ�����
		int iFieldCount = pRows->GetRow(0)->GetFieldCount();
		// ÿ����¼����
		int iRecordLength = pRows->GetRow(0)->GetRecordLength();
		// ÿ����¼�ַ������Ϊ 4000
		if(iRecordLength>=4000)
		{
			fclose(fp);
			fp = NULL;
			return FALSE;
		}
		//-------------------------------------------------
		// �ļ�ͷ
		DBFHeader header;
		memset(&header,0,sizeof(DBFHeader));

		header.cVersion			= 0x03;
		header.cYear			= (BYTE)(stm.wYear - 1900);
		header.cMonth			= (BYTE)stm.wMonth;
		header.cDay				= (BYTE)stm.wDay;

		// �ܼ�¼��
		header.lNumberOfRecord	= iRowCount;
		// �ļ�ͷ + �ֶνṹ�������ֵĳ��� + ��ֹ��ʶ0x0d
		header.iLengthOfHeader	= sizeof(DBFHeader) + sizeof(FieldDescriptor) * iFieldCount + 1;
		// ÿ����¼����
		header.iLengthOfRecord	= iRecordLength;
		// Language Driver ID
		header.cLdid = 0x4d;

		fseek(fp,0,SEEK_SET);
		fwrite(&header,sizeof(DBFHeader),1,fp);
		//-------------------------------------------------
		// �ֶ�������
		for(i=0;i<iFieldCount;i++)
		{
			FieldDescriptor fldd;
			memset(&fldd,0,sizeof(FieldDescriptor));

			KField* pField = pRows->GetFields()->GetField(i);

			// �ֶ���
			strncpy(fldd.szName,KString::ToMByte(pField->GetName()),10);
			// �ֶ�����
			fldd.cType = (BYTE)pField->GetType();
			// �ֶ���������ڸü�¼��ƫ����
			fldd.lOffset = 0;
			// �ֶο��
			fldd.cFieldLength = pField->GetLength();
			// С�����λ��
			fldd.cFieldDecimal = pField->GetDecimal();

			fwrite(&fldd,sizeof(FieldDescriptor),1,fp);
		}
		//-------------------------------------------------
		char cTerminator = 0x0d;	// ��ֹ���� 0x0d
		fwrite(&cTerminator,sizeof(char),1,fp);
		//-------------------------------------------------
		// ��¼
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
		char cEnd = 0x1a;	// �ļ���β
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

		// ��¼����
		int iRecordCount = pRows->GetRowCount();
		// �ֶ�����
		int iFieldCount = pRows->GetFields()->GetFieldCount();
		// ÿ����¼����
		int iRecordLength = pRows->GetRecordLength();

		// ÿ����¼�ַ������Ϊ 4000
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

		// �ܼ�¼��
		header.lNumberOfRecord	= iRecordCount;
		// �ļ�ͷ + �ֶνṹ�������ֵĳ��� + ��ֹ��ʶ0x0d
		header.iLengthOfHFLD	= sizeof(DBFHeader) + sizeof(DBFieldDescriptor) * iFieldCount + 1;
		// ÿ����¼����
		header.iLengthOfRecord	= iRecordLength;

		// Language Driver ID
		header.cLdid = 0x4d;

		fwrite(&header,sizeof(DBFHeader),1,fp);
		//-------------------------------------------------
		// �ֶ�����
		for(i=0;i<iFieldCount;i++)
		{
			KField* pField = pRows->GetFields()->GetField(i);

			DBFieldDescriptor fldd;
			memset(&fldd,0,sizeof(DBFieldDescriptor));

			// �ֶ���
			strncpy(fldd.szName,KString::ToMByte(pField->GetName()),10);
			// �ֶ�����
			fldd.cType = (BYTE)pField->GetType();
			// �ֶ���������ڸü�¼��ƫ����
			fldd.lOffset = 0;
			// �ֶο��
			fldd.cFieldLength = pField->GetLength();
			// С�����λ��
			fldd.cFieldDecimal = pField->GetDecimal();

			fwrite(&fldd,sizeof(DBFieldDescriptor),1,fp);
		}
		//-------------------------------------------------
		char cTerminator = 0x0d;
		fwrite(&cTerminator,sizeof(char),1,fp);
		//-------------------------------------------------
		char szRecord[4000];	// ÿ����¼�ַ������Ϊ  4000
		for(i=0;i<iRecordCount;i++)
		{
			memset(szRecord,0,sizeof(char)*4000);
			KRow* pRow = pRows->GetRowById(i+1);

			// ��¼ת��Ϊ�ַ�
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
