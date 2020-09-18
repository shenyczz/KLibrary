/******************************************************************************
 ** Header File: KIntegral.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KIntegral - 数值积分类
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

#ifndef __INCLUDED_INTEGRAL_H__
#define __INCLUDED_INTEGRAL_H__

#include "KRandom.h"

class KIND_EXT_CLASS KIntegral
{
private:
	KIntegral(void);

public:
	//变步长梯形法求积
	static double Trapezia(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//变步长辛卜生法求积
	static double Simpson(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//龙贝格法求积
	double Romberg(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//一维连分式法求积
	static double Fraction(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//勒让德-高斯法求积
	static double LegendreGauss(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//切比雪夫法求积
	static double Chebyshev(double a, double b, double (*pFun)(double),double eps=1.0e-6);

	//一维蒙特卡洛法求积
	static double MonteCarlo(double a, double b,double (*pFun)(double));

	//自适应梯形法求积
	static double TrapeziaSelfAdapt(double a, double b, double (*pFun)(double),double d=1.0e-4,double eps=1.0e-6);

	//高振荡函数法求积
	static void Surge(double a, double b, int m, double* da,double* db,int n,double s[2]);

	//拉盖尔-高斯法求积
	static double LaguerreGauss(double (*pFun)(double));
	//埃尔米特-高斯法求积
	static double HermiteGauss(double (*pFun)(double));

	//二重变步长辛卜生法求积
	static double Simpson2D(double a, double b, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps=1.0e-6);

	//多重高斯法求积
	static double GaussMD(int n,double* js,double (*pFun)(double*,int),void (*pFunGMD)(int j,double*,double*));

	//二重连分式法求积
	static double Fraction2D(double a, double b, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps=1.0e-6);

	//二重蒙特卡洛法求积
	static double MonteCarloMD(double* a, double* b,int n,double (*pFun)(double*,int));

private:
	//自适应梯形法求积辅助函数
	static int _itsa(double x0, double x1, double h, double f0, double f1, double t0, double t[], double d, double eps, double (*pFun)(double));
	//二重变步长辛卜生法求积辅助函数
	static double _IntegralSimp2(double x,double eps, double (*pFun)(double,double),void (*pFunb)(double,double*));
	//二重连分式法求积辅助函数
	static double _Fraction2D2(double x, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps=1.0e-6);
};

#endif // #ifndef __INCLUDED_INTEGRAL_H__
