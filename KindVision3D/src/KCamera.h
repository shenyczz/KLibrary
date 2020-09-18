/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KCamera - 摄像机
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

#include "KViewport3D.h"
#include "KQuaternion.h"

namespace Vision3D { namespace Cameras
{

	class KIND_EXT_CLASS KCamera
	{
	public:
		KCamera(void);
		virtual ~KCamera(void);

	public:

	protected:
		double m_fov;					// Field of view (degrees)
		double m_distance;				// 摄像机到目标的距离

		KQuaternion m_orientation;		// 四元数
		KViewport3D m_viewport;			// 视口

		D3DXMATRIX m_viewMatrix;        // 观察矩阵
		D3DXMATRIX m_worldMatrix;       // 世界矩阵
		D3DXMATRIX m_projectionMatrix;	// 投影矩阵

		D3DXVECTOR3 m_position;			// 摄像机位置
	};

}}// namespace Vision3D::Cameras

using namespace Vision3D::Cameras;

/*
protected:
D3DXVECTOR3 m_Pos;			// 位置向量
D3DXVECTOR3 m_Right;		// 右向量		: 旋转 - 俯仰(pitch)	移动 - 扫视(strafe)
D3DXVECTOR3 m_Up;			// 上向量		: 旋转 - 偏航(yaw)		移动 - 升降(fly)
D3DXVECTOR3 m_Look;			// 观察向量		: 旋转 - 滚动(roll)		移动 - 平动(walk)

public:
void Pitch(double angle);	// rotate on right vector
void Yaw(double angle);		// rotate on up vector
void Roll(double angle);	// rotate on look vector

void Strafe(double units);	// left/right
void Fly(double units);		// up/down
void Walk(double units);	// forward/backword

D3DXMATRIX GetViewMatrix();	// 观察矩阵
*/
