/******************************************************************************
 ** Header File: KSFunction.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KSFunction - 特殊函数类
 ** Version:
 ** Function:
 ** Explain:

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

#ifndef __INCLUDED_SFUNCTION_H__
#define __INCLUDED_SFUNCTION_H__

class KIND_EXT_CLASS KSFunction
{
private:
	KSFunction(void);

public:
	//伽马函数
	static double Gamma(double x);
	//不完全伽马函数
	static double IncompleteGamma(double a, double x);
	//误差函数
	static double Error(double x);
	//不完全贝塔函数
	static double IncompleteBeta(double a, double b, double x);

	//正态分布函数
	static double NormalDistribution(double a, double d, double x);
	//t-分布函数
	static double tDistribution(double t, int n);
	//X^2-分布函数
	static double X2Distribution(double x, int n);
	//F-分布函数
	static double FDistribution(double f, int n1, int n2);

private:
	//不完全贝塔函数的从属函数
	static double _beta(double a, double b, double x);
};

#endif // #ifndef __INCLUDED_SFUNCTION_H__
