/******************************************************************************
 ** Header File: KLinearEquation.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KLinearEquation - 线性方程
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

#ifndef __INCLUDED_LINEAR_EQUATION_H__
#define __INCLUDED_LINEAR_EQUATION_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KLinearEquation
{
private:
	KLinearEquation(void);

public:
	//全选主元高斯消去法
	static int TotalChoiceGauss(KMatrix& a, KMatrix& b);

	//全选主元高斯-约当消去法
	static int TotalChoiceGaussJordan(KMatrix& a, KMatrix& b);

	//求解对称正定方程组的平方根法
	static int SymmetryRegularEuationSquareRoot(KMatrix& a, KMatrix& d);
};

#endif // #ifndef __INCLUDED_LINEAR_EQUATION_H__
