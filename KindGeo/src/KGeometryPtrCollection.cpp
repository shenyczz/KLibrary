#include "StdAfx.h"
#include "KGeometryPtrCollection.h"

namespace Geometries
{

	KGeometryPtrCollection::KGeometryPtrCollection(void)
	{
		m_pCollection = new TPtrCollection<KGeometry*>();
	}

	KGeometryPtrCollection::~KGeometryPtrCollection(void)
	{
		_delete(m_pCollection);
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
