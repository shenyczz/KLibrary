#include "StdAfx.h"
#include "KGridInfo.h"


KGridInfo::KGridInfo(void)
{
	// x 坐标
	xNum		= 0;	// X方向格点数(列数)
	xMin		= 0;	// X坐标最小值
	xMax		= 0;	// X坐标最大值
	xInterval	= 0;	// X坐标间隔值

	// y 坐标
	yNum		= 0;	// Y方向格点数(行数)
	yMin		= 0;	// Y坐标最小值
	yMax		= 0;	// Y坐标最大值
	yInterval	= 0;	// Y坐标间隔值
}


KGridInfo::~KGridInfo(void)
{
}
