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

class KIND_EXT_CLASS KGridInfo
{
public:
	KGridInfo(void);
	virtual ~KGridInfo(void);

public:
	// X������
	int& XNum()				{ return this->xNum; }
	// Y������
	int& YNum()				{ return this->yNum; }

	// ��СX 
	double& XMin()			{ return this->xMin; }
	// ��СY
	double& YMin()			{ return this->yMin; }

	// ���X
	double& XMax()			{ return this->xMax; }
	// ���Y
	double& YMax()			{ return this->yMax; }

	// X���
	double& XInterval()		{ return this->xInterval; }
	// Y���
	double& YInterval()		{ return this->yInterval; }

protected:
	// x ����
	int		xNum;			// X��������(����)
	double	xMin;			// X������Сֵ
	double	xMax;			// X�������ֵ
	double	xInterval;		// X������ֵ

	// y ����
	int		yNum;			// Y��������(����)
	double	yMin;			// Y������Сֵ
	double	yMax;			// Y�������ֵ
	double	yInterval;		// Y������ֵ
};

