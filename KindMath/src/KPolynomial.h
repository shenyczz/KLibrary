/******************************************************************************
 ** Header File: KPolynomial.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KPolynomial - ����ʽ������ʽ��
 ** Version:
 ** Function:

 ** Author: ShenYongchen(shenyczz@163.com) [������]
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
	//��һάʵ����ʽp(x)��x����ֵ
	static double PolyValue1D(double* dCoff, size_t stN,double x);
	//���ά����ʽֵ
	static double PolyValue2D(const KMatrix& mCoff, double dX, double dY);
	//��һά����ʽ���
	// S(x)=P(x) * Q(x)
	static KMatrix PolyMultip(double* dCoffP,int m, double* dCoffQ,int n);
	//��һά����ʽ����
	int	PolyDiv(double* dCoffP,int m, double* dCoffQ,int n, KMatrix& matCoffS, KMatrix& matCoffR);
	// ��������ʽ����ֵ
	static double FractionValue(double* dXpara, double* dCoff, int n,double dX);
};

#endif // #ifndef __INCLUDED_POLYNOMIAL_H__
