#include "StdAfx.h"
#include "KShapeFileHeader.h"

namespace Shapefile
{

	KShapeFileHeader::KShapeFileHeader(void)
	{
	}

	KShapeFileHeader::KShapeFileHeader(const KShapeFileHeader& shapeFileHeader)
	{
		lFileCode	= shapeFileHeader.lFileCode;
		lFileLength = shapeFileHeader.lFileLength;
		lVersion	= shapeFileHeader.lVersion;
		lShapeType	= shapeFileHeader.lShapeType;

		xMin		= shapeFileHeader.xMin;
		yMin		= shapeFileHeader.yMin;
		xMax		= shapeFileHeader.xMax;
		yMax		= shapeFileHeader.yMax;
		zMin		= shapeFileHeader.zMin;
		zMax		= shapeFileHeader.zMax;
		mMin		= shapeFileHeader.mMin;
		mMax		= shapeFileHeader.mMax;

		m_Extents	= KExtents(xMin,yMin,xMax,yMax);
	}

	KShapeFileHeader::KShapeFileHeader(const SHAPE_FILE_HEADER& shapeFileHeader)
	{
		lFileCode	= shapeFileHeader.lFileCode;
		lFileLength = shapeFileHeader.lFileLength;
		lVersion	= shapeFileHeader.lVersion;
		lShapeType	= shapeFileHeader.lShapeType;

		xMin		= shapeFileHeader.xMin;
		yMin		= shapeFileHeader.yMin;
		xMax		= shapeFileHeader.xMax;
		yMax		= shapeFileHeader.yMax;
		zMin		= shapeFileHeader.zMin;
		zMax		= shapeFileHeader.zMax;
		mMin		= shapeFileHeader.mMin;
		mMax		= shapeFileHeader.mMax;

		m_Extents	= KExtents(xMin,yMin,xMax,yMax);
	}

	KShapeFileHeader::~KShapeFileHeader(void)
	{
	}

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
