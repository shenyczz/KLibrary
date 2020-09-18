/******************************************************************************
 ** KGridImageDataRender.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridImageDataRender
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KRender.h"

namespace Renders
{

	class KIND_EXT_CLASS KGridImageDataRender : public KRender
	{
	public:
		KGridImageDataRender(void);
		virtual ~KGridImageDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
