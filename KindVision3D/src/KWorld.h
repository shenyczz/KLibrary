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
		double m_Radius;			// 半径
		double m_EquatorialRadius;	// 赤道半径

		///// <summary>
		///// 地形存取器
		///// </summary>
		//ITerrainAccessor TerrainAccessor { get; }

		///// <summary>
		///// 天体是否地球
		///// </summary>
		//Boolean IsEarth { get; }
	};

}// namespace Vision3D
