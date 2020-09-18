#pragma once

namespace Shapefile
{
	/*
	typedef struct tagShapeFileHeader
	{
		unsigned long lFileCode;	// [BE] 文件代码 = 0x270A 
		unsigned long lUnused[5];	// [BE] 
		unsigned long lFileLength;	// [BE] 文件长度(字长,*2 转换为字节长度)

		unsigned long lVersion;		// [LE] 文件版本
		unsigned long lShapeType;	// [LE] 文件类型

		double xMin;				// [LE] 包围盒坐标
		double yMin;				// [LE] 包围盒坐标

		double xMax;				// [LE] 包围盒坐标
		double yMax;				// [LE] 包围盒坐标

		double zMin;				// [LE] 包围盒坐标
		double zMax;				// [LE] 包围盒坐标

		double mMin;				// [LE] 包围盒坐标
		double mMax;				// [LE] 包围盒坐标

	}SHAPE_FILE_HEADER;
	*/

	class KIND_EXT_CLASS KShapeFileHeader : public SHAPE_FILE_HEADER
	{
	public:
		KShapeFileHeader(void);
		KShapeFileHeader(const SHAPE_FILE_HEADER& shapeFileHeader);
		KShapeFileHeader(const KShapeFileHeader& shapeFileHeader);
		virtual ~KShapeFileHeader(void);

	public:
		const KExtents& GetExtents() { return m_Extents; }
		ShapeType GetType() { return (ShapeType)this->lShapeType; }

	private:
		// 范围
		KExtents m_Extents;
	};

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
