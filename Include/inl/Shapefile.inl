#ifndef __INCLUDED_SHAPE_INL__
#define __INCLUDED_SHAPE_INL__

namespace Shape
{
//---------------------------------------------------------
}// namespace Shape - end
//---------------------------------------------------------
using namespace Shape;

#define SHAPE_FILE_CODE		0x0000270A		// Shape 文件代码

enum ShapeType
{
	ShapeType_Null			= 0,	// Null Shape

	ShapeType_Point			= 1,	// 点目标
	ShapeType_Polyline		= 3,	// 线目标
	ShapeType_Polygon		= 5,	// 面目标
	ShapeType_MultiPoint	= 8,	// 多面目标

	ShapeType_PointZ		= 11,	// 
	ShapeType_PolylineZ		= 13,	// 
	ShapeType_PolygonZ		= 15,	// 
	ShapeType_MultiPointZ	= 18,	// 

	ShapeType_Annotation	= 20,	// 注释
	ShapeType_PointM		= 21,	// 
	ShapeType_PolylineM		= 23,	// Measure形线目标
	ShapeType_PolygonM		= 25,	// 
	ShapeType_Measure		= 27,	// Measure 形线目标
	ShapeType_MultiPointM	= 28,	// 

	ShapeType_MultiPatch	= 31,	// 
};

#pragma pack(1)

// shp 文件头
//
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

// shp 记录头
//
struct ShapeRecordHead
{
	unsigned long lRecordNo;		// [BE] 记录号(RecordNo begin at 1) (==0 时是什么意思)
	unsigned long lContentLength;	// [BE] 内容长度(字长,*2 转换为字节长度)
};

// shx 记录
//
struct ShapeIndexRecord
{
	unsigned long lOffset;			// [BE] 记录的偏移地址(字长,*2 转换为字节长度)
	unsigned long lContentLength;	// [BE] 内容长度(字长,*2 转换为字节长度)
};

#pragma pack()

#endif	//#ifndef __INCLUDED_SHAPE_INL__
