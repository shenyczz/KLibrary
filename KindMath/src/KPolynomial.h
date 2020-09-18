/******************************************************************************
 ** Header File: KPolynomial.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KPolynomial - 多项式和连分式类
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

#ifndef __INCLUDED_POLYNOMIAL_H__
#define __INCLUDED_POLYNOMIAL_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KPolynomial
{
private:
	KPolynomial(void);

public:
	//求一维实多项式p(x)在x处的值
	static double PolyValue1D(double* dCoff, size_t stN,double x);
	//求二维多项式值
	static double PolyValue2D(const KMatrix& mCoff, double dX, double dY);
	//两一维多项式相乘
	// S(x)=P(x) * Q(x)
	static KMatrix PolyMultip(double* dCoffP,int m, double* dCoffQ,int n);
	//两一维多项式除法
	int	PolyDiv(double* dCoffP,int m, double* dCoffQ,int n, KMatrix& matCoffS, KMatrix& matCoffR);
	// 计算连分式函数值
	static double FractionValue(double* dXpara, double* dCoff, int n,double dX);
};

#endif // #ifndef __INCLUDED_POLYNOMIAL_H__
