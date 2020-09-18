/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KWorldCamera - ���������
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
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
		double m_worldRadius;	// ����뾶
		double m_layerRadius;	// ͼ��뾶

		double m_altitude;		// ����
		double m_altitudeMax;	// ��󺣰�
	};

}}// namespace Vision3D::Cameras
