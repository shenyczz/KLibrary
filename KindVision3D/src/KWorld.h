#pragma once

#include "KRenderableTarget3D.h"

namespace Vision3D
{

	class KIND_EXT_CLASS KWorld : public KRenderableTarget3D
	{
	public:
		KWorld(void);
		virtual ~KWorld(void);

	public:
		virtual void Initialize(KRenderEngine* pRenderEngine);
		virtual void Update(KRenderEngine* pRenderEngine);
		virtual void Render(KRenderEngine* pRenderEngine);

	protected:
		double m_Radius;			// �뾶
		double m_EquatorialRadius;	// ����뾶

		///// <summary>
		///// ���δ�ȡ��
		///// </summary>
		//ITerrainAccessor TerrainAccessor { get; }

		///// <summary>
		///// �����Ƿ����
		///// </summary>
		//Boolean IsEarth { get; }
	};

}// namespace Vision3D
