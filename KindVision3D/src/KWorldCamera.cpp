#include "StdAfx.h"
#include "KWorldCamera.h"

namespace Vision3D { namespace Cameras
{

	KWorldCamera::KWorldCamera(void)
	{
		m_worldRadius = 0;
		m_layerRadius = 0;
	}

	KWorldCamera::KWorldCamera(double radius)
	{
		m_worldRadius = radius;
		m_layerRadius = m_worldRadius;
		m_distance = 2 * m_worldRadius;
		m_altitude = m_distance;
		m_altitudeMax = 20 * m_worldRadius;
	}


	KWorldCamera::~KWorldCamera(void)
	{
	}

}}// namespace Vision3D::Cameras
