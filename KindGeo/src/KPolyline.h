/******************************************************************************
 ** KPolyline.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KPolyline
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

	class KIND_EXT_CLASS KPolyline : public KGeometry, public TPtrCollection<KPoint*>
	{
	public:
		KPolyline(void);
		KPolyline(const KPolyline& polyline);
		virtual ~KPolyline(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);

	public:
		// �Ƿ�պ�
		bool IsClose();
		// ����
		double Length();
		//�����������
		double Area();

		// ���Ƿ����ڲ�
		bool PtInside(KPoint point);

		// ��¡
		KPolyline* Clone();
	};

	//typedef TPtrCollection<KPolyline*> KPolylinePtrCollection;

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
