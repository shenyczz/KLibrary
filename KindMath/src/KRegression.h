/******************************************************************************
 ** Header File: KRegression.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KRegression
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

#ifndef __INCLUDED_REGRESSION_H__
#define __INCLUDED_REGRESSION_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KRegression
{
private:
	KRegression(void);

public:	//�ع����
	//һԪ���Իع����
	static void Linear1D(double* x,double* y,int n,double a[2],double dt[6]);

	//��Ԫ���Իع����
	static void LinearMD(KMatrix& xy);

	//�𲽻ع����
	static void Stepwise(KMatrix& xy,double f1,double f2,
		double* b,double* xx,double* v,double* s,
		double* ye,double* yr,
		KMatrix& r,
		double dt[2],
		double eps=1.0e-12);


};

#endif // #ifndef __INCLUDED_REGRESSION_H__
