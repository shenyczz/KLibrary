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

#include "KGeometry.h"

namespace Geometries
{

	class KIND_EXT_CLASS KRect : public KGeometry
	{
	public:
		KRect(void);
		KRect(const KRect& rect);
		KRect(double x,double y,double w,double h);
		virtual ~KRect(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);

	public:
		double X;
		double Y;
		double Width;
		double Height;
	};

	typedef TPtrCollection<KRect*> KRectPtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
