#include "StdAfx.h"
#include "KWorld.h"

#include "KRenderEngine.h"

namespace Vision3D
{

	KWorld::KWorld(void)
	{
		m_Radius = 0;
		m_EquatorialRadius = 0;
	}


	KWorld::~KWorld(void)
	{
	}

	// 初始化
	void KWorld::Initialize(KRenderEngine* pRenderEngine)
	{
		this->IsInitialized = TRUE;
	}

	// 更新
	void KWorld::Update(KRenderEngine* pRenderEngine)
	{
		if(this->IsInitialized)
			this->Initialize(pRenderEngine);
	}

	// 渲染
	void KWorld::Render(KRenderEngine* pRenderEngine)
	{
		if(!m_pChildren)
			return;

		int iCount = m_pChildren->Count();
		for(int i=0;i<iCount;i++)
		{
			KRenderableTarget3D* pRenderableTarget3D = m_pChildren->GetAt(i);
			pRenderableTarget3D->Update(pRenderEngine);
			pRenderableTarget3D->Render(pRenderEngine);
		}
	}

}// namespace Vision3D
