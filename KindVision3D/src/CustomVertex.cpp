#include "StdAfx.h"
#include "CustomVertex.h"

namespace Vision3D
{

	CustomVertex::CustomVertex() { }

	CONST DWORD CustomVertex::PositionOnly::FVF					= D3DFVF_XYZ;									//   2 = 0x02
	CONST DWORD CustomVertex::PositionColored::FVF				= D3DFVF_XYZ | D3DFVF_DIFFUSE;					//  66 = 0x02 + 0x40
	CONST DWORD CustomVertex::PositionColoredTextured::FVF		= D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;	// 322 = 0x02 + 0x40 + 0x100
	CONST DWORD CustomVertex::PositionNormal::FVF				= D3DFVF_XYZ | D3DFVF_NORMAL;					//  18 = 0x02 + 0x10
	CONST DWORD CustomVertex::PositionNormalColored::FVF		= D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE;	//  82 = 0x02 + 0x10 + 0x40
	CONST DWORD CustomVertex::PositionNormalTextured::FVF		= D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;		// 274 = 0x02 + 0x10 + 0x100
	CONST DWORD CustomVertex::PositionTextured::FVF				= D3DFVF_XYZ | D3DFVF_TEX1;						// 258 = 0x02 + 0x100
	CONST DWORD CustomVertex::Transformed::FVF					= D3DFVF_XYZRHW;								//   4 = 0x04
	CONST DWORD CustomVertex::TransformedColored::FVF			= D3DFVF_XYZRHW | D3DFVF_DIFFUSE;				//  68 = 0x04 + 0x40
	CONST DWORD CustomVertex::TransformedColoredTextured::FVF	= D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;	// 324 = 0x04 + 0x40 + 0x100
	CONST DWORD CustomVertex::TransformedTextured::FVF			= D3DFVF_XYZRHW | D3DFVF_TEX1;					// 260 = 0x04 + 0x100

}// namespace Vision3D
