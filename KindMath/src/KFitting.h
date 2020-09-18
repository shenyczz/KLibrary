/******************************************************************************
 ** Header File: KFitting.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KFitting - ���
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

#ifndef __INCLUDED_FITTING_H__
#define __INCLUDED_FITTING_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KFitting
{
private:
	KFitting(void);

public:
	//��С����������� 
	static void LeastSquares(double x[],double y[],int n,double a[],int m,double dt[3]);

	//�б�ѩ���������
	static double Chebyshev(double x[],double y[],int n,double a[],int m);

	//���һ�±ƽ�����ʽ�����ȷ�
	static double Remez(double a,double b,double p[],int n,double (*pFun)(double),double eps=1.0e-22);

	//���������С�����������
	static void LeastSquares2D(double x[],double y[],KMatrix& z,KMatrix& a,double dt[5]);
	//���������С�����������
	static void LeastSquares2D(double x[], double y[], double z[], int n, int m, double a[], int p, int q, double dt[]);

	//�������
	static void Curvefitting(double y[], int n);
};

#endif // #ifndef __INCLUDED_FITTING_H__
