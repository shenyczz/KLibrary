/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KFrustum - �ӽ���
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

#include "KPlane.h"
#include "KVector3.h"

namespace Vision3D
{

	class KIND_EXT_CLASS KFrustum
	{
	public:
		KFrustum(void);
		virtual ~KFrustum(void);

	public:
		void Update(D3DXMATRIX mat);
		BOOL Contains(const KVector3& vec3);

	protected:
		// ��ͷ����6����
		D3DXPLANE m_Planes[6];

	};

}// namespace Vision3D


