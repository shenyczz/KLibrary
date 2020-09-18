/******************************************************************************
 ** KLine.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLine
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

#include "KRect.h"

namespace Geometries
{

	class KIND_EXT_CLASS KEllipse : public KRect
	{
	public:
		KEllipse(void);
		KEllipse(const KEllipse& ellipse);
		KEllipse(double x,double y,double w,double h);
		virtual ~KEllipse(void);
	};

	typedef TPtrCollection<KEllipse*> KEllipsePtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
