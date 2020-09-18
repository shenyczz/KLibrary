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
		// 保存数据
		virtual BOOL SaveData(LPCTSTR lpszFile);

	public:
		virtual LPCTSTR GetDbfFile() { return m_strDbfFile; }
		virtual LPCTSTR GetShxFile() { return m_strShxFile; }

	public:
		// 属性数据
		KDbaseFile* GetDbaseFile() { return m_pDbf; }
		// 数量
		int GetShapeCount() { return this->Count(); }

	protected:
		virtual BOOL LoadData_bin(LPCTSTR lpszFile);

	protected:
		// 填充数据信息
		virtual void FillDataInfo(LPVOID lpvData) {}

	protected:
		// 读取 Shape 数据
		bool ReadShape(LPCTSTR lpszFile);

	private:
		// 读取Shape数据(使用索引文件shx)
		bool ReadShape_WithIndexfile(LPCTSTR lpszFile);
		// 读取Shape数据(不使用索引文件shx)
		bool ReadShape_WithoutIndexfile(LPCTSTR lpszFile);

	private:
		// 
		bool readPointData_WithoutIndexfile(LPCTSTR lpszFile);
		bool readPolylineData_WithoutIndexfile(LPCTSTR lpszFile);
		bool readPolygonData_WithoutIndexfile(LPCTSTR lpszFile);

	private:
		// 从文件指定位置读取一个形状
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

	public: // 静态函数
		// 是否Shape文件
		static BOOL IsShapeFile(LPCTSTR lpszFile);
	};

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
