/******************************************************************************
 **
 ** Announce: If it works, I know it was written by
 **           ShenYongchen(shenyczz@163.com), ZhengZhou, HeNan.
 **           if not, then I don't know nothing.
 **
 **     Name: KViewport3D - 3D ”ø⁄
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
 ** DateTime: 2000 - 
 ** Web Site: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

namespace Vision3D
{

	class KIND_EXT_CLASS KViewport3D
	{
	public:
		KViewport3D(void);
		virtual ~KViewport3D(void);

	public:
		int X,Y;
		int Width,Height;

		float MinZ,MaxZ;
	};

}// namespace Vision3D
