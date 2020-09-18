/******************************************************************************
 ** KExtents.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KExtents - 区块
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
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
	// X 最小值
	double& MinX() { return Xmin; }
	// Y 最小值
	double& MinY() { return Ymin; }
	// X 最大值
	double& MaxX() { return Xmax; }
	// Y 最大值
	double& MaxY() { return Ymax; }
	// 宽度
	double Width() const { return fabs(Xmax - Xmin); }
	// 高度
	double Height() const { return fabs(Ymax - Ymin); }
	// 中点
	void GetCenter(double& cx,double& cy) { cx = 0.5 * (Xmin + Xmax); cy = 0.5 * (Ymin + Ymax); }

public:
	// 是否为空
	bool IsEmpty() const;
	// 点在范围内
	bool PtInside(double x, double y) const;
	// 合并
	void Join(const KExtents& other);
	// 交集
	KExtents Intersection(const KExtents& bbox);

public:
	// 操作符重载 ==
	bool operator ==(const KExtents& other) const;
	// 操作符重载 !=
	bool operator !=(const KExtents& other) const;

public:
	double Xmin, Ymin;
	double Xmax, Ymax;
};
