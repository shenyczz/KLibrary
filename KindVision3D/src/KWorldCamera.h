/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KWorldCamera - 天体摄像机
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 - 
 ** Web Site: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KCamera.h"
#include "KVector3.h"

namespace Vision3D { namespace Cameras
{

	class KIND_EXT_CLASS KWorldCamera : public KCamera
	{
	public:
		KWorldCamera(void);
		KWorldCamera(double radius);
		virtual ~KWorldCamera(void);

	protected:
		double m_worldRadius;	// 天体半径
		double m_layerRadius;	// 图层半径

		double m_altitude;		// 海拔
		double m_altitudeMax;	// 最大海拔
	};

}}// namespace Vision3D::Cameras
