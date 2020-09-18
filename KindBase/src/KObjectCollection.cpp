#include "StdAfx.h"
#include "KObjectCollection.h"


KObjectCollection::KObjectCollection(void)
{
	m_pCollection = new TCollection<KObject>();
}


KObjectCollection::~KObjectCollection(void)
{
	_delete(m_pCollection);
}
