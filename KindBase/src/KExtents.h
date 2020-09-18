/******************************************************************************
 ** KExtents.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KExtents - ����
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

class KIND_EXT_CLASS KExtents
{
public:
	KExtents(void);
	KExtents(const KExtents& extents);
	KExtents(double xmin,double ymin,double xmax,double ymax);

	virtual~KExtents(void);

public:
	// X ��Сֵ
	double& MinX() { return Xmin; }
	// Y ��Сֵ
	double& MinY() { return Ymin; }
	// X ���ֵ
	double& MaxX() { return Xmax; }
	// Y ���ֵ
	double& MaxY() { return Ymax; }
	// ���
	double Width() const { return fabs(Xmax - Xmin); }
	// �߶�
	double Height() const { return fabs(Ymax - Ymin); }
	// �е�
	void GetCenter(double& cx,double& cy) { cx = 0.5 * (Xmin + Xmax); cy = 0.5 * (Ymin + Ymax); }

public:
	// �Ƿ�Ϊ��
	bool IsEmpty() const;
	// ���ڷ�Χ��
	bool PtInside(double x, double y) const;
	// �ϲ�
	void Join(const KExtents& other);
	// ����
	KExtents Intersection(const KExtents& bbox);

public:
	// ���������� ==
	bool operator ==(const KExtents& other) const;
	// ���������� !=
	bool operator !=(const KExtents& other) const;

public:
	double Xmin, Ymin;
	double Xmax, Ymax;
};
