#include "StdAfx.h"
#include "KBoundingBox3D.h"

#include "KMathEngine.h"

namespace Vision3D
{

	KBoundingBox3D::KBoundingBox3D(KVector3 v0, KVector3 v1, KVector3 v2, KVector3 v3, KVector3 v4, KVector3 v5, KVector3 v6, KVector3 v7)
	{
		m_corners[0] = v0;
		m_corners[1] = v1;
		m_corners[2] = v2;
		m_corners[3] = v3;
		m_corners[4] = v4;
		m_corners[5] = v5;
		m_corners[6] = v6;
		m_corners[7] = v7;
	}

	KBoundingBox3D::KBoundingBox3D(float south, float north, float west, float east, float radius1, float radius2)
	{
		float scale = radius2 / radius1;
		m_corners[0] = KMathEngine::Spherical2Cartesian(south, west, radius1);
		m_corners[1] = KVector3::Scale(m_corners[0], scale);
		m_corners[2] = KMathEngine::Spherical2Cartesian(south, east, radius1);
		m_corners[3] = KVector3::Scale(m_corners[2], scale);
		m_corners[4] = KMathEngine::Spherical2Cartesian(north, west, radius1);
		m_corners[5] = KVector3::Scale(m_corners[4], scale);
		m_corners[6] = KMathEngine::Spherical2Cartesian(north, east, radius1);
		m_corners[7] = KVector3::Scale(m_corners[6], scale);
	}

	KBoundingBox3D::~KBoundingBox3D(void)
	{
	}

	KVector3 KBoundingBox3D::CalculateCenter()
	{
		KVector3 res;
		for(int i=0;i<8;i++)
		{
			KVector3 corner = m_corners[i];
			res += corner;
		}
		res /= 8;
		return res;
	}
	/*
	/// <summary>
	/// Calculate the screen area (pixels) covered by the bottom of the bounding box.
	/// </summary>
	public float CalcRelativeScreenArea(ICamera camera)
	{
		Vector3 a = camera.Project(corners[0]);
		Vector3 b = camera.Project(corners[2]);
		Vector3 c = camera.Project(corners[6]);
		Vector3 d = camera.Project(corners[4]);

		Vector3 ab = Vector3.Subtract(b, a);
		Vector3 ac = Vector3.Subtract(c, a);
		Vector3 ad = Vector3.Subtract(d, a);

		float tri1SqArea = Vector3.Cross(ab, ac).LengthSq();
		float tri2SqArea = Vector3.Cross(ad, ac).LengthSq();

		return tri1SqArea + tri2SqArea;
	}
	*/

}// namespace Vision3D
