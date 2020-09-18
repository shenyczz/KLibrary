/******************************************************************************
 ** Header File: KFitting.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KFitting - 拟合
 ** Version:
 ** Function:

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	//1. 
		use

******************************************************************************/

#ifndef __INCLUDED_FITTING_H__
#define __INCLUDED_FITTING_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KFitting
{
private:
	KFitting(void);

public:
	//最小二乘曲线拟合 
	static void LeastSquares(double x[],double y[],int n,double a[],int m,double dt[3]);

	//切比雪夫曲线拟合
	static double Chebyshev(double x[],double y[],int n,double a[],int m);

	//最佳一致逼近多项式里米兹法
	static double Remez(double a,double b,double p[],int n,double (*pFun)(double),double eps=1.0e-22);

	//矩形域的最小二乘曲面拟合
	static void LeastSquares2D(double x[],double y[],KMatrix& z,KMatrix& a,double dt[5]);
	//矩形域的最小二乘曲面拟合
	static void LeastSquares2D(double x[], double y[], double z[], int n, int m, double a[], int p, int q, double dt[]);

	//曲线拟合
	static void Curvefitting(double y[], int n);
};

#endif // #ifndef __INCLUDED_FITTING_H__
