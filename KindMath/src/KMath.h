/******************************************************************************
 ** KMath.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KMath - 数学类
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 该类是一个静态类，定义了数学上的一些
              常量
              常用的数学计算

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KMath
{
private:
	KMath(void) { }

public:
	// 浮点数相等
	static bool FloatEqual(double v1, double v2);

	// 两点间距离
	static double Distance(double x1, double y1, double x2, double y2);

public:
	static const int i = 3;

public:
	static double e;
	static double Pi;
	static double Epslon;
};
