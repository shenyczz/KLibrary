#include "StdAfx.h"
#include "KRenderableTarget3D.h"

#include "KRenderEngine.h"

namespace Vision3D
{

	KRenderableTarget3D::KRenderableTarget3D(void)
	{
		IsInitialized = FALSE;
		m_pChildren = new TPtrCollection<KRenderableTarget3D*>();
	}


	KRenderableTarget3D::~KRenderableTarget3D(void)
	{
		_delete(m_pChildren);
	}

}// namespace Vision3D
