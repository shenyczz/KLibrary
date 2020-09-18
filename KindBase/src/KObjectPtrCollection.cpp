#include "StdAfx.h"
#include "KObjectPtrCollection.h"


KObjectPtrCollection::KObjectPtrCollection(void)
{
	m_pCollection = new TPtrCollection<KObject*>();
}


KObjectPtrCollection::~KObjectPtrCollection(void)
{
	_delete(m_pCollection)
}
