#include "StdAfx.h"
#include "KShapeFile.h"
#include "KShapeFileIndex.h"

namespace Shapefile
{

	KShapeFileIndex::KShapeFileIndex(KShapeFile* pShapeFile) throw(KException)
		:m_pShapeFile(NULL)
	{
		m_pShapeFile = (KShapeFile*)pShapeFile;
		if(!Open(pShapeFile->GetShxFile()))
		{
			throw KException(_T("Open .shx file error."));
		}
	}

	KShapeFileIndex::~KShapeFileIndex(void)
	{
	}

	// ���ļ�
	bool KShapeFileIndex::Open(LPCTSTR lpszFile)
	{
		if(!readFileHeader(lpszFile))
			return false;

		if(!readRecordEntries(lpszFile))
			return false;

		return true;
	}

	bool KShapeFileIndex::readFileHeader(LPCTSTR lpszFile)
	{
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return false;

		// ��ȡ�ļ�ͷ����
		SHAPE_FILE_HEADER shapeFileHeader;
		memset(&shapeFileHeader, 0, sizeof(SHAPE_FILE_HEADER));
		fread(&shapeFileHeader, sizeof(SHAPE_FILE_HEADER), 1, fp);

		KBytesOrder::ReverseBytes(&shapeFileHeader.lFileCode);
		KBytesOrder::ReverseBytes(&shapeFileHeader.lFileLength);

		fclose(fp);
		fp = NULL;

		// �ļ�ͷ��Ϣ
		_delete(m_pShapeFile->m_pShapeFileHeader);
		m_pShapeFile->m_pShapeFileHeader = new KShapeFileHeader(shapeFileHeader);

		// �������ͺͷ�Χ
		m_pShapeFile->SetType(m_pShapeFile->m_pShapeFileHeader->GetType());
		m_pShapeFile->SetExtents(m_pShapeFile->m_pShapeFileHeader->GetExtents());

		return true;
	}

	bool KShapeFileIndex::readRecordEntries(LPCTSTR lpszFile)
	{
		FILE* fp = _tfopen(lpszFile,_T("rb"));
		if(fp==NULL)
			return false;

		//m_pShapeRecordEntryCollection = new TPtrCollection<KShapeRecordEntry*>();

		// �����ļ�ͷ
		fseek(fp,sizeof(SHAPE_FILE_HEADER),SEEK_SET);

		while(!feof(fp))
		{
			unsigned long lOffset;
			unsigned long lContentLength;

			fread(&lOffset, sizeof(long), 1, fp);
			fread(&lContentLength, sizeof(long), 1, fp);

			KBytesOrder::ReverseBytes(&lOffset);
			KBytesOrder::ReverseBytes(&lContentLength);

			this->Add(new KShapeRecordEntry(lOffset,lContentLength));
			//m_pShapeRecordEntryCollection->Add(new KShapeRecordEntry(lOffset,lContentLength));
		}

		fclose(fp);
		fp = NULL;

		return true;
	}

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
