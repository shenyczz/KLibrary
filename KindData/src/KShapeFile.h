#pragma once

#include "KDataObject.h"
#include "KDbaseFile.h"
#include "KShapeFileIndex.h"

namespace Shapefile
{
//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------

namespace Shapefile
{
	class KIND_EXT_CLASS KShapeFile : public KDataObject, public TPtrCollection<KGeometry*>
	{
	public:
		KShapeFile(void);
		KShapeFile(LPCTSTR lpszFile) throw(KException);
		virtual ~KShapeFile(void);

	public:
		// ��������
		virtual BOOL SaveData(LPCTSTR lpszFile);

	public:
		virtual LPCTSTR GetDbfFile() { return m_strDbfFile; }
		virtual LPCTSTR GetShxFile() { return m_strShxFile; }

	public:
		// ��������
		KDbaseFile* GetDbaseFile() { return m_pDbf; }
		// ����
		int GetShapeCount() { return this->Count(); }

	protected:
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);

	protected:
		// ���������Ϣ
		virtual void FillDataInfo(LPVOID lpvData) {}

	protected:
		// ��ȡ Shape ����
		bool ReadShape(LPCTSTR lpszFile);

	private:
		// ��ȡShape����(ʹ�������ļ�shx)
		bool ReadShape_WithIndexfile(LPCTSTR lpszFile);
		// ��ȡShape����(��ʹ�������ļ�shx)
		bool ReadShape_WithoutIndexfile(LPCTSTR lpszFile);

	private:
		// 
		bool readPointData_WithoutIndexfile(LPCTSTR lpszFile);
		bool readPolylineData_WithoutIndexfile(LPCTSTR lpszFile);
		bool readPolygonData_WithoutIndexfile(LPCTSTR lpszFile);

	private:
		// ���ļ�ָ��λ�ö�ȡһ����״
		KGeometryPtrCollection* readOneShape(FILE* fp, long lOffset);
		KGeometryPtrCollection* readOnePointShape(FILE* fp, long lOffset);
		KGeometryPtrCollection* readOnePolylineShape(FILE* fp, long lOffset);
		KGeometryPtrCollection* readOnePolygonShape(FILE* fp, long lOffset);

	private:
		KDbaseFile* m_pDbf;
		KShapeFileIndex* m_pShapeFileIndex;
		KShapeFileHeader* m_pShapeFileHeader;

		KString m_strDbfFile;
		KString m_strShxFile;

		BOOL m_bHasDbf;
		BOOL m_bHasIndexFile;

		friend class KShapeFileIndex;

	public: // ��̬����
		// �Ƿ�Shape�ļ�
		static BOOL IsShapeFile(LPCTSTR lpszFile);
	};

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
