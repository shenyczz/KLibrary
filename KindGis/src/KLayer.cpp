#include "StdAfx.h"
#include "KLayer.h"

namespace Layers
{

	KLayer::KLayer(void)
	{
		m_lpvData = NULL;
		m_pRender = NULL;
	}

	KLayer::~KLayer(void)
	{
		_delete(m_pRender);
	}

	void KLayer::Rendering(HDC hDC, LPVOID lpvData)
	{
		if(!hDC || !lpvData)
			return;

		if(!m_pRender)
			return;

		m_lpvData = lpvData;
		m_pRender->Rendering(hDC,lpvData);
	}

	KLayerProperty* KLayer::GetProperty()
	{
		KRender* pRender = this->GetRender();
		if(!pRender)
			return NULL;

		return pRender->GetLayerProperty();
	}
	KProvider* KLayer::GetProvider()
	{
		KRender* pRender = this->GetRender();
		if(!pRender)
			return NULL;

		KLayerProperty* pLayerProperty = (KLayerProperty*)pRender->GetLayerProperty();
		if(!pLayerProperty)
			return NULL;

		return pLayerProperty->GetProvider();
	}

//---------------------------------------------------------
}// namespace Layers - end
//---------------------------------------------------------
