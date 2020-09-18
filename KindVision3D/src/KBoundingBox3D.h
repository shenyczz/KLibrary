#pragma once

#include "KVector3.h"

namespace Vision3D
{

	class KIND_EXT_CLASS KBoundingBox3D
	{
	public:
		KBoundingBox3D(KVector3 v0, KVector3 v1, KVector3 v2, KVector3 v3, KVector3 v4, KVector3 v5, KVector3 v6, KVector3 v7);
		KBoundingBox3D(float south, float north, float west, float east, float radius1, float radius2);
		virtual ~KBoundingBox3D(void);

	public:
		KVector3 CalculateCenter();

	protected:
		KVector3 m_corners[8];
	};

}// namespace Vision3D
