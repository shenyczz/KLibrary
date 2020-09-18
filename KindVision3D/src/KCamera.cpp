#include "StdAfx.h"
#include "KCamera.h"

namespace Vision3D { namespace Cameras
{

	KCamera::KCamera(void)
	{
		m_fov = 0;
		m_distance = 0;

		// 世界矩阵
		D3DXMatrixIdentity(&m_worldMatrix);
		// 四元数
		m_orientation = KQuaternion::EulerToQuaternion(0,0,0);
	}

	KCamera::~KCamera(void)
	{
	}

	/*
	// 俯仰 rotate on right vector
	void KCamera::Pitch(double angle)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T,&m_Right,(FLOAT)angle);

		// rotate _up and _look around _right vector
		D3DXVec3TransformCoord(&m_Up,&m_Up, &T);
		D3DXVec3TransformCoord(&m_Look,&m_Look, &T);
	}

	void KCamera::Yaw(double angle)
	{
		D3DXMATRIX T;

		// rotate around world y (0, 1, 0) always for land object
		if( m_CameraType == LandObject )
			D3DXMatrixRotationY(&T, (FLOAT)angle);

		// rotate around own up vector for aircraft
		if( m_CameraType == AirCraft )
			D3DXMatrixRotationAxis(&T, &m_Up, (FLOAT)angle);

		// rotate _right and _look around _up or y-axis
		D3DXVec3TransformCoord(&m_Right,&m_Right, &T);
		D3DXVec3TransformCoord(&m_Look,&m_Look, &T);
	}

	void KCamera::Roll(double angle)
	{
		if( m_CameraType == AirCraft )
		{
			D3DXMATRIX T;
			D3DXMatrixRotationAxis(&T, &m_Look,	(FLOAT)angle);

			// rotate _up and _right around _look vector
			D3DXVec3TransformCoord(&m_Right,&m_Right, &T);
			D3DXVec3TransformCoord(&m_Up,&m_Up, &T);
		}
	}

	// left/right
	void KCamera::Strafe(double units)
	{
		// move only on xz plane for land object
		if( m_CameraType == LandObject )
			m_Pos += D3DXVECTOR3(m_Right.x, 0.0f, m_Right.z) * (float)units;

		if( m_CameraType == AirCraft )
			m_Pos += m_Right * (float)units;
	}
	// up/down
	void KCamera::Fly(double units)
	{
		if( m_CameraType == AirCraft )
			m_Pos += m_Up * (float)units;
	}
	// forward/backword
	void KCamera::Walk(double units)
	{
		// move only on xz plane for land object
		if( m_CameraType == LandObject )
			m_Pos += D3DXVECTOR3(m_Look.x, 0.0f, m_Look.z) * (float)units;

		if( m_CameraType == AirCraft )
			m_Pos += m_Look * (float)units;
	}
	// 取得观察矩阵
	D3DXMATRIX KCamera::GetViewMatrix()
	{
		D3DXMATRIX matView;

		// Keep camera's axes orthogonal to eachother
		D3DXVec3Normalize(&m_Look, &m_Look);

		D3DXVec3Cross(&m_Up, &m_Look, &m_Right);
		D3DXVec3Normalize(&m_Up, &m_Up);

		D3DXVec3Cross(&m_Right, &m_Up, &m_Look);
		D3DXVec3Normalize(&m_Right, &m_Right);

		// Build the view matrix:
		float x = -D3DXVec3Dot(&m_Right, &m_Pos);
		float y = -D3DXVec3Dot(&m_Up, &m_Pos);
		float z = -D3DXVec3Dot(&m_Look, &m_Pos);

		matView(0,0) = m_Right.x; matView(0, 1) = m_Up.x; matView(0, 2) = m_Look.x; matView(0, 3) = 0.0f;
		matView(1,0) = m_Right.y; matView(1, 1) = m_Up.y; matView(1, 2) = m_Look.y; matView(1, 3) = 0.0f;
		matView(2,0) = m_Right.z; matView(2, 1) = m_Up.z; matView(2, 2) = m_Look.z; matView(2, 3) = 0.0f;
		matView(3,0) = x;         matView(3, 1) = y;      matView(3, 2) = z;        matView(3, 3) = 1.0f;

		return matView;
	}
	*/

}}// namespace Vision3D::Cameras
