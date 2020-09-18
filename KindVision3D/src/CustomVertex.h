#pragma once

namespace Vision3D
{

#pragma pack(1)

	class KIND_EXT_CLASS CustomVertex
	{
	private:
		CustomVertex();

		//PositionOnly
		//PositionColored
		//PositionColoredTextured
		//PositionNormal
		//PositionNormalColored
		//PositionNormalTextured
		//PositionTextured
		//Transformed
		//TransformedColored
		//TransformedColoredTextured
		//TransformedTextured

		// FVF --> Flexible vertex format
	public:

		struct PositionOnly
		{
			//D3DXVECTOR3 pos;
			FLOAT X,Y,Z;
			static CONST DWORD FVF;	// D3DFVF_XYZ;

			PositionOnly() : X(0),Y(0),Z(0) {}
			PositionOnly(float x, float y, float z) : X(x),Y(y),Z(z) {}
		};

		struct PositionColored
		{
			FLOAT X,Y,Z;
			DWORD Color;
			static CONST DWORD FVF;	// D3DFVF_XYZ | D3DFVF_DIFFUSE;

			PositionColored() : X(0),Y(0),Z(0),Color(0) {}
			PositionColored(float x, float y, float z, DWORD color) : X(x),Y(y),Z(z),Color(color) {}
		};

		struct PositionColoredTextured
		{
			FLOAT X,Y,Z;
			DWORD Color;
			FLOAT Tu,tv;
			static CONST DWORD FVF;	// D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;
		};

		struct PositionNormal
		{
			FLOAT X,Y,Z;
			FLOAT Nx,Ny,Nz;
			static CONST DWORD FVF;	// D3DFVF_XYZ | D3DFVF_NORMAL;
		};

		struct PositionNormalColored
		{
			FLOAT X,Y,Z;
			FLOAT Nx,Ny,Nz;
			DWORD Color;
			static CONST DWORD FVF;	// D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;
		};

		struct PositionNormalTextured
		{
			FLOAT X,Y,Z;
			FLOAT Nx,Ny,Nz;
			FLOAT Tu,Tv;
			static CONST DWORD FVF;	// D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;
		};

		struct PositionTextured
		{
			FLOAT X,Y,Z;
			FLOAT Tu,Tv;
			static CONST DWORD FVF;	// D3DFVF_XYZ | D3DFVF_TEX1;

			PositionTextured() : X(0),Y(0),Z(0),Tu(0),Tv(0) {}
			PositionTextured(const PositionTextured& rhs) : X(rhs.X),Y(rhs.Y),Z(rhs.Z),Tu(rhs.Tu),Tv(rhs.Tv) {}
			PositionTextured(float x, float y, float z, float u, float v) : X(x),Y(y),Z(z),Tu(u),Tv(v) {}
		};

		struct Transformed
		{
			FLOAT X,Y,Z,Rhw;
			static CONST DWORD FVF;	// D3DFVF_XYZRHW;
		};

		struct TransformedColored
		{
			FLOAT X,Y,Z,Rhw;
			DWORD Color;
			static CONST DWORD FVF;	// D3DFVF_XYZRHW | D3DFVF_DIFFUSE;
		};

		struct TransformedColoredTextured
		{
			FLOAT X,Y,Z,Rhw;
			FLOAT Tu,tv;
			DWORD Color;
			static CONST DWORD FVF;	// D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
		};

		struct TransformedTextured
		{
			FLOAT X,Y,Z,Rhw;
			FLOAT Tu,tv;
			static CONST DWORD FVF;	// D3DFVF_XYZRHW | D3DFVF_TEX1;
		};

	};

#pragma pack()

}// namespace Vision3D
