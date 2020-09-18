/******************************************************************************
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KContour - �ȸ���
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

#include "KCurve.h"

namespace Geometries
{

	class KIND_EXT_CLASS KContour : public KGeometry, public TPtrCollection<KCurve*>
	{
	public:
		KContour(void);
		KContour(const KContour& contour);
		virtual ~KContour(void);

	public:
		virtual void Offset(double dx,double dy);
		virtual void GetMinCoordinate(double& x, double& y);
		virtual void GetMaxCoordinate(double& x, double& y);

	public:
		// ��ָ������ǰ��������
		void InsertBefor(int index, KCurve* pCurve);
		// �������Ϊ��ֵ����ӱպ�����(����)
		void AddCurveByArea(KCurve* pCurve);

		// ����-��ȡ����ֵ
		double& Value() { return m_dValue; }
		void SetValue(double dValue) { m_dValue = dValue; }
		double GetValue() { return m_dValue; }

		KCurve* GetCurve(int index) { return this->GetAt(index); }

	private:
		// ֵ
		double m_dValue;
	};

	typedef TPtrCollection<KContour*> KContourPtrCollection;

}
