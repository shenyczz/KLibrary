#pragma once

#include "KWorld.h"

namespace Vision3D
{

	class KIND_EXT_CLASS KEarth : public KWorld
	{
	public:
		KEarth(void);
		virtual ~KEarth(void);

	public:
		virtual void Initialize(KRenderEngine* pRenderEngine);
		virtual void Update(KRenderEngine* pRenderEngine);
		virtual void Render(KRenderEngine* pRenderEngine);

	protected:
		BOOL CreateMesh(IDirect3DDevice9* pDevice3d);
		BOOL CreateMeshTest(IDirect3DDevice9* pDevice3d);

	protected:
		void SetupMatrices(IDirect3DDevice9* pDevice3d);

	protected:
		int m_iMeshPointCount;

		double m_dLonMin;
		double m_dLonMax;

		double m_dLatMin;
		double m_dLatMax;

		IDirect3DVertexBuffer9* m_pVB;
		IDirect3DIndexBuffer9* m_pIB;

		IDirect3DTexture9* m_pTexture;
	};

}// namespace Vision3D
