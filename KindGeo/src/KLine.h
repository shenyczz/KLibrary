/******************************************************************************
 ** KLine.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLine - �߶�
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
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

#include "KPoint.h"

namespace Geometries
{

	class KIND_EXT_CLASS KLine : public KGeometry
	{
	public:
		KLine(void);
		KLine(const KLine& line);
		KLine(KPoint headPoint,KPoint tailPoint);
		KLine(double x1,double y1,double x2,double y2);
		virtual ~KLine(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);


	public:
		// ���
		const KPoint& GetHeadPoint() { return m_pointHead; }
		void SetHeadPoint(KPoint point) { m_pointHead = point; }
		// �յ�
		const KPoint& GetTailPoint() { return m_pointTail; }
		void SetTailPoint(KPoint point) { m_pointTail = point; }

		// �Ƿ�պ�
		bool IsClose();
		// ����
		double Length();

	protected:
		KPoint m_pointHead;
		KPoint m_pointTail;
	};

	typedef TPtrCollection<KLine*> KLinePtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
