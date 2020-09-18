#include "StdAfx.h"
#include "KRect.h"

namespace Geometries
{

	KRect::KRect(void)
		: KGeometry(), X(0), Y(0), Width(0), Height(0)
	{
		SetType(GeometryType_Rect);
	}

	KRect::KRect(const KRect& rect)
		: KGeometry(rect), X(rect.X), Y(rect.Y), Width(rect.Width), Height(rect.Height)
	{
		SetType(GeometryType_Rect);
	}

	KRect::KRect(double x,double y,double w,double h)
		: KGeometry(), X(x), Y(y), Width(w), Height(h)
	{
		SetType(GeometryType_Rect);
	}

	KRect::~KRect(void)
	{
	}

	void KRect::Offset(double dx,double dy)
	{
		X += dx;
		Y += dy;
	}

	void KRect::GetMinCoordinate(double& x, double& y)
	{
		x = this->X;
		y = this->Y - this->Height;
	}
	void KRect::GetMaxCoordinate(double& x, double& y)
	{
		x = this->X - this->Width;
		y = this->Y;
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
