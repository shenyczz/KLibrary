#include "StdAfx.h"
#include "KEarth.h"

#include "KVector3.h"
#include "KMathEngine.h"
#include "CustomVertex.h"
#include "KRenderEngine.h"

namespace Vision3D
{

	KEarth::KEarth(void)
	{
		//m_dRadius = ;
		m_dLonMin = -180;
		m_dLonMax =  180;

		m_dLatMin = -90;
		m_dLatMax =  90;

		m_iMeshPointCount = 64;
		m_EquatorialRadius = 6378137.0;

		m_pVB = NULL;
		m_pIB = NULL;
		m_pTexture = NULL;
	}


	KEarth::~KEarth(void)
	{
		if(NULL != m_pVB)
		{
			m_pVB->Release();
			m_pVB = NULL;
		}
		if(NULL != m_pIB)
		{
			m_pIB->Release();
			m_pIB = NULL;
		}
		if(NULL != m_pTexture)
		{
			m_pTexture->Release();
			m_pTexture = NULL;
		}
	}

	void KEarth::SetupMatrices(IDirect3DDevice9* pDevice3d)
	{
		// 世界矩阵
		D3DXMATRIX matWorld;
		D3DXMatrixIdentity(&matWorld);
		pDevice3d->SetTransform(D3DTS_WORLD,&matWorld);

		// 观察矩阵
		//D3DXVECTOR3 vEyePt		(0.0f, 0.0f , -20.0f);
		D3DXVECTOR3 vEyePt		(0.0f, 0.0f , -100.0f);
		D3DXVECTOR3 vLookatPt	(0.0f, 0.0f , 0.0f);
		D3DXVECTOR3 vUpvec		(0.0f, 1.0f , 0.0f);
		D3DXMATRIX matView;
		D3DXMatrixLookAtLH(&matView,&vEyePt,&vLookatPt,&vUpvec);
		pDevice3d->SetTransform(D3DTS_VIEW,&matView);

		// 投影矩阵
		D3DXMATRIX matProj;
		//D3DXMatrixPerspectiveFovLH(&matProj,D3DX_PI / 4, 1.33f, 1.0f, 100.0f);
		D3DXMatrixPerspectiveFovLH(&matProj,D3DX_PI / 4, 1.33f, 1.0f, 1000.0f);
		pDevice3d->SetTransform(D3DTS_PROJECTION, &matProj);
	}

	void KEarth::Initialize(KRenderEngine* pRenderEngine)
	{
		BOOL bCreateMesh = FALSE;
		bCreateMesh = CreateMesh(pRenderEngine->GetDevice3d());
		//bCreateMesh = CreateMeshTest(pRenderEngine->GetDevice3d());
		this->IsInitialized = bCreateMesh;
	}
	void KEarth::Update(KRenderEngine* pRenderEngine)
	{
		if(!this->IsInitialized)
		{
			this->Initialize(pRenderEngine);
		}
	}
	void KEarth::Render(KRenderEngine* pRenderEngine)
	{
		IDirect3DDevice9* pDevice3d = pRenderEngine->GetDevice3d();
		if(NULL == pDevice3d)
			return;

		this->SetupMatrices(pDevice3d);

		pDevice3d->SetRenderState(D3DRS_LIGHTING,FALSE);

		pDevice3d->SetTexture(0,m_pTexture);
		pDevice3d->SetStreamSource(0,m_pVB,0,sizeof(CustomVertex::PositionTextured));
		pDevice3d->SetFVF(CustomVertex::PositionTextured::FVF);
		pDevice3d->DrawPrimitive(D3DPT_TRIANGLESTRIP,0,2);

		//pRenderEngine->Present();
		return;
	}

	BOOL KEarth::CreateMeshTest(IDirect3DDevice9* pDevice3d)
	{
		if(FAILED(D3DXCreateTextureFromFileW(pDevice3d, L"e:\\temp\\ndvi.bmp", &m_pTexture)))
			return FALSE;

		// 顶点数量
		int verticesNum = 4;
		CustomVertex::PositionTextured vertices[4];
		vertices[0] = CustomVertex::PositionTextured(-3,-3,0, 0,1);
		vertices[1] = CustomVertex::PositionTextured(-3, 3,0, 0,0);
		vertices[2] = CustomVertex::PositionTextured( 3,-3,0, 1,1);
		vertices[3] = CustomVertex::PositionTextured( 3, 3,0, 1,0);

		// 顶点缓冲区长度
		int vbLength = verticesNum*sizeof(CustomVertex::PositionTextured);

		// 创建顶点缓冲区
		pDevice3d->CreateVertexBuffer(vbLength,
			0,CustomVertex::PositionTextured::FVF,D3DPOOL_MANAGED,&m_pVB,NULL);

		// 填充顶点缓冲区
		void* pVertices;
		if(FAILED(m_pVB->Lock(0,vbLength,&pVertices,0)))
			return FALSE;

		memcpy(pVertices,vertices,vbLength);
		m_pVB->Unlock();

		return TRUE;
	}

	// 创建网格
	BOOL KEarth::CreateMesh(IDirect3DDevice9* pDevice3d)
	{
		if(FAILED(D3DXCreateTextureFromFileW(pDevice3d, L"e:\\temp\\ndvi.bmp", &m_pTexture)))
			return FALSE;

		double latRange, lonRange;

		// 纬度范围
		latRange = abs(m_dLatMax - m_dLatMin);
		// 经度范围
		if (m_dLonMin < m_dLonMax)
			lonRange = m_dLonMax - m_dLonMin;
		else
			lonRange = 360.0f + m_dLonMax - m_dLonMin;

		int upperBound = m_iMeshPointCount - 1;		// m_iMeshPointCount = 64
		float scaleFactor = 1.0f / upperBound;      // 缩放因子

		//m_EquatorialRadius = 80;
		//// 顶点数量
		//int verticesNum = m_iMeshPointCount * m_iMeshPointCount;
		//CustomVertex::PositionTextured* vertices = new CustomVertex::PositionTextured[verticesNum];
		//// 顶点缓冲区长度
		//int vbLength = verticesNum * sizeof(CustomVertex::PositionTextured);

		//for(int i=0;i<m_iMeshPointCount;i++)
		//{
		//	for(int j=0;j<m_iMeshPointCount;j++)
		//	{
		//		double height = 0;
		//		KVector3 pos = KMathEngine::Spherical2Cartesian(
		//			m_dLatMax - scaleFactor * latRange * i,
		//			m_dLonMin + scaleFactor * lonRange * j,
		//			m_EquatorialRadius + height);

		//		vertices[i * m_iMeshPointCount + j].X = (float)pos.X;
		//		vertices[i * m_iMeshPointCount + j].Y = (float)pos.Y;
		//		vertices[i * m_iMeshPointCount + j].Z = (float)pos.Z;

		//		//vertices[i * m_iMeshPointCount + j].Tu = j * scaleFactor;
		//		//vertices[i * m_iMeshPointCount + j].Tv = i * scaleFactor;
		//	}
		//}

		// 顶点数量
		int verticesNum = 4;
		// 顶点缓冲区长度
		int vbLength = verticesNum * sizeof(CustomVertex::PositionTextured);
		CustomVertex::PositionTextured* vertices = new CustomVertex::PositionTextured[verticesNum];
		vertices[0] = CustomVertex::PositionTextured(-3,-3,0, 0,1);
		vertices[1] = CustomVertex::PositionTextured(-3, 3,0, 0,0);
		vertices[2] = CustomVertex::PositionTextured( 3,-3,0, 1,1);
		vertices[3] = CustomVertex::PositionTextured( 3, 3,0, 1,0);

		// 创建顶点缓冲区
		if(NULL != m_pVB)
		{
			m_pVB->Release();
			m_pVB = NULL;
		}
		pDevice3d->CreateVertexBuffer(vbLength,
			0,CustomVertex::PositionTextured::FVF,D3DPOOL_MANAGED,&m_pVB,NULL);

		// 填充定点缓冲区
		void* pVertices;
		if(FAILED(m_pVB->Lock(0,vbLength,(void**)&pVertices,0)))
			return FALSE;

		memcpy(pVertices,vertices,vbLength);
		m_pVB->Unlock();

		/*
		int indicesNum = 2 * upperBound * upperBound * 3;
		WORD* indices = new WORD[indicesNum]; 
		for (int i = 0; i < upperBound; i++)
		{
			for (int j = 0; j < upperBound; j++)
			{
				indices[(2 * 3 * i * upperBound) + 6 * j + 0] = (WORD)(i * m_iMeshPointCount + j);
				indices[(2 * 3 * i * upperBound) + 6 * j + 1] = (WORD)((i + 1) * m_iMeshPointCount + j);
				indices[(2 * 3 * i * upperBound) + 6 * j + 2] = (WORD)(i * m_iMeshPointCount + j + 1);

				indices[(2 * 3 * i * upperBound) + 6 * j + 3] = (WORD)(i * m_iMeshPointCount + j + 1);
				indices[(2 * 3 * i * upperBound) + 6 * j + 4] = (WORD)((i + 1) * m_iMeshPointCount + j);
				indices[(2 * 3 * i * upperBound) + 6 * j + 5] = (WORD)((i + 1) * m_iMeshPointCount + j + 1);
			}
		}

		// 创建索引缓冲区
		if(NULL != m_pIB)
		{
			m_pIB->Release();
			m_pIB = NULL;
		}
		pDevice3d->CreateIndexBuffer(2 * upperBound * upperBound * 3,
			0,D3DFMT_INDEX16,D3DPOOL_DEFAULT,&m_pIB,NULL);

		// 填充定点缓冲区
		void* pIndices;
		if(SUCCEEDED(m_pIB->Lock(0,indicesNum,(void**)&pIndices,0)))
		{
			memcpy(pIndices,indices,indicesNum);
			m_pIB->Unlock();
		}
		*/
		// 计算法线
		//calculate_Normals(ref vertices, indices);

		_deletea(vertices);
		//_deletea(indices);

		return TRUE;
	}

}// namespace Vision3D
