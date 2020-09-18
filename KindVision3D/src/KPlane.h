#pragma once

namespace Vision3D
{

	class KIND_EXT_CLASS KPlane : public D3DXPLANE
	{
	public:
		KPlane(void);
		virtual ~KPlane(void);

	public:
		KPlane& Normalize();
	};

}// namespace Vision3D
