/******************************************************************************
 ** KDataInfo.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridInfo
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

class KIND_EXT_CLASS KGridInfo
{
public:
	KGridInfo(void);
	virtual ~KGridInfo(void);

public:
	// X向格点数
	int& XNum()				{ return this->xNum; }
	// Y向格点数
	int& YNum()				{ return this->yNum; }

	// 最小X 
	double& XMin()			{ return this->xMin; }
	// 最小Y
	double& YMin()			{ return this->yMin; }

	// 最大X
	double& XMax()			{ return this->xMax; }
	// 最大Y
	double& YMax()			{ return this->yMax; }

	// X间隔
	double& XInterval()		{ return this->xInterval; }
	// Y间隔
	double& YInterval()		{ return this->yInterval; }

protected:
	// x 坐标
	int		xNum;			// X方向格点数(列数)
	double	xMin;			// X坐标最小值
	double	xMax;			// X坐标最大值
	double	xInterval;		// X坐标间隔值

	// y 坐标
	int		yNum;			// Y方向格点数(行数)
	double	yMin;			// Y坐标最小值
	double	yMax;			// Y坐标最大值
	double	yInterval;		// Y坐标间隔值
};

