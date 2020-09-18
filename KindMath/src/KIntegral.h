/******************************************************************************
 ** Header File: KIntegral.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KIntegral - ��ֵ������
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

#ifndef __INCLUDED_INTEGRAL_H__
#define __INCLUDED_INTEGRAL_H__

#include "KRandom.h"

class KIND_EXT_CLASS KIntegral
{
private:
	KIntegral(void);

public:
	//�䲽�����η����
	static double Trapezia(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//�䲽�������������
	static double Simpson(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//���������
	double Romberg(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//һά����ʽ�����
	static double Fraction(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//���õ�-��˹�����
	static double LegendreGauss(double a, double b, double (*pFun)(double),double eps=1.0e-6);
	//�б�ѩ�����
	static double Chebyshev(double a, double b, double (*pFun)(double),double eps=1.0e-6);

	//һά���ؿ��巨���
	static double MonteCarlo(double a, double b,double (*pFun)(double));

	//����Ӧ���η����
	static double TrapeziaSelfAdapt(double a, double b, double (*pFun)(double),double d=1.0e-4,double eps=1.0e-6);

	//���񵴺��������
	static void Surge(double a, double b, int m, double* da,double* db,int n,double s[2]);

	//���Ƕ�-��˹�����
	static double LaguerreGauss(double (*pFun)(double));
	//��������-��˹�����
	static double HermiteGauss(double (*pFun)(double));

	//���ر䲽�������������
	static double Simpson2D(double a, double b, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps=1.0e-6);

	//���ظ�˹�����
	static double GaussMD(int n,double* js,double (*pFun)(double*,int),void (*pFunGMD)(int j,double*,double*));

	//��������ʽ�����
	static double Fraction2D(double a, double b, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps=1.0e-6);

	//�������ؿ��巨���
	static double MonteCarloMD(double* a, double* b,int n,double (*pFun)(double*,int));

private:
	//����Ӧ���η������������
	static int _itsa(double x0, double x1, double h, double f0, double f1, double t0, double t[], double d, double eps, double (*pFun)(double));
	//���ر䲽���������������������
	static double _IntegralSimp2(double x,double eps, double (*pFun)(double,double),void (*pFunb)(double,double*));
	//��������ʽ�������������
	static double _Fraction2D2(double x, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps=1.0e-6);
};

#endif // #ifndef __INCLUDED_INTEGRAL_H__
