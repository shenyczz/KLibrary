#ifndef __INCLUDED_SHAPE_INL__
#define __INCLUDED_SHAPE_INL__

namespace Shape
{
//---------------------------------------------------------
}// namespace Shape - end
//---------------------------------------------------------
using namespace Shape;

#define SHAPE_FILE_CODE		0x0000270A		// Shape �ļ�����

enum ShapeType
{
	ShapeType_Null			= 0,	// Null Shape

	ShapeType_Point			= 1,	// ��Ŀ��
	ShapeType_Polyline		= 3,	// ��Ŀ��
	ShapeType_Polygon		= 5,	// ��Ŀ��
	ShapeType_MultiPoint	= 8,	// ����Ŀ��

	ShapeType_PointZ		= 11,	// 
	ShapeType_PolylineZ		= 13,	// 
	ShapeType_PolygonZ		= 15,	// 
	ShapeType_MultiPointZ	= 18,	// 

	ShapeType_Annotation	= 20,	// ע��
	ShapeType_PointM		= 21,	// 
	ShapeType_PolylineM		= 23,	// Measure����Ŀ��
	ShapeType_PolygonM		= 25,	// 
	ShapeType_Measure		= 27,	// Measure ����Ŀ��
	ShapeType_MultiPointM	= 28,	// 

	ShapeType_MultiPatch	= 31,	// 
};

#pragma pack(1)

// shp �ļ�ͷ
//
typedef struct tagShapeFileHeader
{
	unsigned long lFileCode;	// [BE] �ļ����� = 0x270A 
	unsigned long lUnused[5];	// [BE] 
	unsigned long lFileLength;	// [BE] �ļ�����(�ֳ�,*2 ת��Ϊ�ֽڳ���)

	unsigned long lVersion;		// [LE] �ļ��汾
	unsigned long lShapeType;	// [LE] �ļ�����

	double xMin;				// [LE] ��Χ������
	double yMin;				// [LE] ��Χ������

	double xMax;				// [LE] ��Χ������
	double yMax;				// [LE] ��Χ������

	double zMin;				// [LE] ��Χ������
	double zMax;				// [LE] ��Χ������

	double mMin;				// [LE] ��Χ������
	double mMax;				// [LE] ��Χ������

}SHAPE_FILE_HEADER;

// shp ��¼ͷ
//
struct ShapeRecordHead
{
	unsigned long lRecordNo;		// [BE] ��¼��(RecordNo begin at 1) (==0 ʱ��ʲô��˼)
	unsigned long lContentLength;	// [BE] ���ݳ���(�ֳ�,*2 ת��Ϊ�ֽڳ���)
};

// shx ��¼
//
struct ShapeIndexRecord
{
	unsigned long lOffset;			// [BE] ��¼��ƫ�Ƶ�ַ(�ֳ�,*2 ת��Ϊ�ֽڳ���)
	unsigned long lContentLength;	// [BE] ���ݳ���(�ֳ�,*2 ת��Ϊ�ֽڳ���)
};

#pragma pack()

#endif	//#ifndef __INCLUDED_SHAPE_INL__
