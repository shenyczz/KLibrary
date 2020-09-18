/******************************************************************************
 ** Header File: KInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KInterpolate - 插值类
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

#ifndef __INCLUDED_INTERPOLATE_H__
#define __INCLUDED_INTERPOLATE_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KInterpolate
{
private:
	KInterpolate(void);

public:
	//拉格朗日全区间不等距插值
	static double LagrangeNotIsometry(double* x, double* y, int n,double t);
	//拉格朗日全区间等距插值
	static double LagrangeIsometry(double x0, double h, double* y, int n,double t);

	//抛物线不等距插值
	static double ParabolaNotIsometry(double* x, double* y, int n,double t);
	//抛物线等距插值
	static double ParabolaIsometry(double x0, double h, double* y, int n,double t);

	//连分式不等距插值
	static double FractionNotIsometry(double* x, double* y, int n,double t);
	//连分式等距插值
	static double FractionIsometry(double x0, double h, double* y, int n,double t);

	//埃尔米特不等距插值
	static double HermiteNotIsometry(double* x, double* y, double* dy, int n,double t);
	//埃尔米特等距插值
	static double HermiteIsometry(double x0, double h, double* y, double* dy, int n,double t);

	//埃特金不等距逐步插值
	static double AitkenNotIsometry(double* x, double* y, int n,double t, double eps=1.0e-6);
	//埃特金等距逐步插值
	static double AitkenIsometry(double x0, double h, double* y, int n,double t, double eps=1.0e-6);

	//阿克玛光滑不等距插值
	static void AkimaNotIsometry(double* x, double* y, int n, int k, double t, double* s,int sn=5);
	//阿克玛光滑等距插值
	static void AkimaIsometry(double x0, double h, double* y, int n, int k, double t, double* s,int sn=5);


	//第一种边界条件的三次样条函数插值、微商与积分
	static double SplineBoundary1(double* x, double* y, double* dy, double* ddy, int n,
				double* t, double* z, double* dz,double* ddz,int m);
	//第二种边界条件的三次样条函数插值、微商与积分
	static double SplineBoundary2(double* x, double* y, double* dy, double* ddy, int n,
				double* t, double* z, double* dz,double* ddz,int m);
	//第三种边界条件的三次样条函数插值、微商与积分
	static double SplineBoundary3(double* x, double* y, double* dy, double* ddy, int n,
				double* t, double* z, double* dz,double* ddz,int m);

	//二元三点插值
	static double InterpV2P3(double*x,int n,double* y,int m,KMatrix z,double u,double v);
	//二元全区间插值
	static double InterpV2Whole(double*x,int n,double* y,int m,KMatrix z,double u,double v);
};

#endif // #ifndef __INCLUDED_INTERPOLATE_H__
