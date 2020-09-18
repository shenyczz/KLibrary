#pragma once

namespace Shapefile
{
	/*
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
		// ��Χ
		KExtents m_Extents;
	};

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
