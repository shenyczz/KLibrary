#include "StdAfx.h"
#include "KFrustum.h"

namespace Vision3D
{

	KFrustum::KFrustum(void)
	{
	}


	KFrustum::~KFrustum(void)
	{
	}

	void KFrustum::Update(D3DXMATRIX m)
	{
		//bottom (down) plane
		m_Planes[0] = D3DXPLANE(
			m._14 + m._12, //a
			m._24 + m._22, //b
			m._34 + m._32, //c
			m._44 + m._42 //d
			);

		//far plane
		m_Planes[1] = D3DXPLANE(
			m._14 - m._13,
			m._24 - m._23,
			m._34 - m._33,
			m._44 - m._43
			);

		//right side plane
		m_Planes[2] = D3DXPLANE(
			m._14 - m._11, //a
			m._24 - m._21, //b
			m._34 - m._31, //c
			m._44 - m._41 //d
			);

		//left side plane
		m_Planes[3] = D3DXPLANE(
			m._14 + m._11,	//a
			m._24 + m._21,	//b
			m._34 + m._31,	//c
			m._44 + m._41	//d
			);

		//near plane
		m_Planes[4] = D3DXPLANE(
			m._13,
			m._23,
			m._33,
			m._43);

		//top (up) plane
		m_Planes[5] = D3DXPLANE(
			m._14 - m._12, //a
			m._24 - m._22, //b
			m._34 - m._32, //c
			m._44 - m._42 //d
			);

		for(int i=0;i<6;i++)
		{
			KPlane* pPlane = (KPlane*)&m_Planes[i];
			pPlane->Normalize();
		}
	}

	BOOL KFrustum::Contains(const KVector3& vec3)
	{
		for(int i=0;i<6;i++)
		{
			KPlane* pPlane = (KPlane*)&m_Planes[i];
			//vec3.Do
			pPlane->Normalize();
		}

		return FALSE;

		/*
		public Boolean Contains(Vector3 v)
		{
			foreach (Plane plane in this.Planes)
			{
				if (Vector3.Dot(new Vector3(plane.A, plane.B, plane.C), v) + plane.D < 0)
					return false;
			}
			return true;
		}
		*/
	}

}// namespace Vision3D
