#include "StdAfx.h"
#include "KExtents.h"

KExtents::KExtents(void)
	:Xmin(0), Ymin(0), Xmax(0), Ymax(0)
{
}

KExtents::KExtents(const KExtents& extents)
	:Xmin(extents.Xmin), Ymin(extents.Ymin), Xmax(extents.Xmax), Ymax(extents.Ymax)
{
}

KExtents::KExtents(double xmin,double ymin,double xmax,double ymax)
	:Xmin(xmin), Ymin(ymin), Xmax(xmax), Ymax(ymax)
{
}

KExtents::~KExtents(void)
{
}

// 操作符重载 ==
bool KExtents::operator ==(const KExtents& other) const
{
	return fabs(this->Xmin - other.Xmin) < 1.0e-12
		&& fabs(this->Ymin - other.Ymin) < 1.0e-12
		&& fabs(this->Xmax - other.Xmax) < 1.0e-12
		&& fabs(this->Ymax - other.Ymax) < 1.0e-12;
}

// 操作符重载 !=
bool KExtents::operator !=(const KExtents& other) const
{
	return fabs(this->Xmin - other.Xmin) > 1.0e-12
		|| fabs(this->Ymin - other.Ymin) > 1.0e-12
		|| fabs(this->Xmax - other.Xmax) > 1.0e-12
		|| fabs(this->Ymax - other.Ymax) > 1.0e-12;
}

// 是否为空
bool KExtents::IsEmpty() const
{
	return (this->Width()<EPS && this->Height()<EPS);
}

// 点在范围内
bool KExtents::PtInside(double x, double y) const
{
	return true
		&& (x>Xmin || fabs(x-Xmin)<EPS) && (x<Xmax || fabs(x-Xmax)<EPS)
		&& (y>Ymin || fabs(y-Ymin)<EPS)	&& (y<Ymax || fabs(y-Ymax)<EPS)
		;
}

// 合并
void KExtents::Join(const KExtents& other)
{
	if(Xmin > other.Xmin)	Xmin = other.Xmin;
	if(Ymin > other.Ymin)	Ymin = other.Ymin;
	if(Xmax < other.Xmax)	Xmax = other.Xmax;
	if(Ymax < other.Ymax)	Ymax = other.Ymax;
}

// 交集
KExtents KExtents::Intersection(const KExtents& other)
{
	// A最大值 < B最小值 or A最小值 > B最大值 => 
	bool xEmpty = this->Xmax < other.Xmin || this->Xmin > other.Xmax;
	bool yEmpty = this->Ymax < other.Ymin || this->Ymin > other.Ymax;
	if(xEmpty || yEmpty)
		return KExtents(0,0,0,0);

	this->Xmin = max(this->Xmin,other.Xmin);
	this->Xmax = min(this->Xmax,other.Xmax);
	this->Ymin = max(this->Ymin,other.Ymin);
	this->Ymax = min(this->Ymax,other.Ymax);
	return *this;
}
