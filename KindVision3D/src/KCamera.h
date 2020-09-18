/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KCamera - �����
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
		double m_distance;				// �������Ŀ��ľ���

		KQuaternion m_orientation;		// ��Ԫ��
		KViewport3D m_viewport;			// �ӿ�

		D3DXMATRIX m_viewMatrix;        // �۲����
		D3DXMATRIX m_worldMatrix;       // �������
		D3DXMATRIX m_projectionMatrix;	// ͶӰ����

		D3DXVECTOR3 m_position;			// �����λ��
	};

}}// namespace Vision3D::Cameras

using namespace Vision3D::Cameras;

/*
protected:
D3DXVECTOR3 m_Pos;			// λ������
D3DXVECTOR3 m_Right;		// ������		: ��ת - ����(pitch)	�ƶ� - ɨ��(strafe)
D3DXVECTOR3 m_Up;			// ������		: ��ת - ƫ��(yaw)		�ƶ� - ����(fly)
D3DXVECTOR3 m_Look;			// �۲�����		: ��ת - ����(roll)		�ƶ� - ƽ��(walk)

public:
void Pitch(double angle);	// rotate on right vector
void Yaw(double angle);		// rotate on up vector
void Roll(double angle);	// rotate on look vector

void Strafe(double units);	// left/right
void Fly(double units);		// up/down
void Walk(double units);	// forward/backword

D3DXMATRIX GetViewMatrix();	// �۲����
*/
