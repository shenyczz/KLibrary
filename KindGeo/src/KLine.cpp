#include "StdAfx.h"
#include "KLine.h"

namespace Geometries
{

	KLine::KLine(void)
		: KGeometry()
	{
		SetType(GeometryType_Line);
	}

	KLine::KLine(const KLine& line)
		: KGeometry(line)
	{
		SetType(GeometryType_Line);
	}

	KLine::KLine(KPoint headPoint,KPoint tailPoint)
		: KGeometry()
		, m_pointHead(headPoint)
		, m_pointTail(tailPoint)
	{
		SetType(GeometryType_Line);
	}

	KLine::KLine(double x1,double y1,double x2,double y2)
		: KGeometry()
		, m_pointHead(x1,y1)
		, m_pointTail(x2,y2)
	{
		SetType(GeometryType_Line);
	}

	KLine::~KLine(void)
	{
	}

	void KLine::Offset(double dx,double dy)
	{
		m_pointHead.Offset(dx, dy);
		m_pointTail.Offset(dx, dy);
	}

	void KLine::GetMinCoordinate(double& x, double& y)
	{
		x = min(m_pointHead.X, m_pointTail.X);
		y = min(m_pointHead.Y, m_pointTail.Y);
	}
	void KLine::GetMaxCoordinate(double& x, double& y)
	{
		x = max(m_pointHead.X, m_pointTail.X);
		y = max(m_pointHead.Y, m_pointTail.Y);
	}

	// 是否闭合
	bool KLine::IsClose()
	{
		return m_pointHead == m_pointTail;
	}

	// 长度
	double KLine::Length()
	{
		return m_pointHead.Distance(m_pointTail);
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
