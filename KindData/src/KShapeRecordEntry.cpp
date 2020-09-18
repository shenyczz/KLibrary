#include "StdAfx.h"
#include "KShapeRecordEntry.h"

namespace Shapefile
{

	KShapeRecordEntry::KShapeRecordEntry(long offset, long length)
		: m_lOffset(offset)
		, m_lLength(length)
	{
	}


	KShapeRecordEntry::~KShapeRecordEntry(void)
	{
	}

//---------------------------------------------------------
}// namespace Shapefile - end
//---------------------------------------------------------
