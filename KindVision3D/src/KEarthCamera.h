/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KEarthCamera - ���������
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

#include "KWorldCamera.h"

namespace Vision3D { namespace Cameras
{

	class KIND_EXT_CLASS KEarthCamera : public KWorldCamera
	{
	public:
		KEarthCamera(void);
		virtual ~KEarthCamera(void);

	protected:
		double m_equatorialRadius;	// ����뾶
	};

}}// namespace Vision3D::Cameras
