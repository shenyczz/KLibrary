#include "StdAfx.h"
#include "KEarthCamera.h"

namespace Vision3D { namespace Cameras
{

	KEarthCamera::KEarthCamera(void)
	{
		m_equatorialRadius = 6378137.0;			// ³àµÀ°ë¾¶
		m_worldRadius = m_equatorialRadius;		// Ã×
		m_layerRadius = m_worldRadius;
		m_distance = 2 * m_worldRadius;
		m_altitude = m_distance;
		m_altitudeMax = 20 * m_worldRadius;
	}

	KEarthCamera::~KEarthCamera(void)
	{
	}

}}// namespace Vision3D::Cameras
