#pragma once

#include "KShapeFileHeader.h"
#include "KShapeRecordEntry.h"

namespace Shapefile
{
	class KShapeFile;
//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------


namespace Shapefile
{
	class KIND_EXT_CLASS KShapeFileIndex : public TPtrCollection<KShapeRecordEntry*>
	{
		friend class KShapeFile;

	public:
		KShapeFileIndex(KShapeFile* pShapeFile) throw(KException);
		virtual ~KShapeFileIndex(void);

	protected:
		bool Open(LPCTSTR lpszFile);
		bool readFileHeader(LPCTSTR lpszFile);
		bool readRecordEntries(LPCTSTR lpszFile);

	protected:
		// 填充数据信息
		virtual void FillDataInfo(LPVOID lpvData) {}

	private:
		KShapeFile* m_pShapeFile;	// 外部指针, 本类不负责内存管理

	};

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
