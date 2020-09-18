/******************************************************************************
 ** Header File: KLinearEquation.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KLinearEquation - ���Է���
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

#ifndef __INCLUDED_LINEAR_EQUATION_H__
#define __INCLUDED_LINEAR_EQUATION_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KLinearEquation
{
private:
	KLinearEquation(void);

public:
	//ȫѡ��Ԫ��˹��ȥ��
	static int TotalChoiceGauss(KMatrix& a, KMatrix& b);

	//ȫѡ��Ԫ��˹-Լ����ȥ��
	static int TotalChoiceGaussJordan(KMatrix& a, KMatrix& b);

	//���Գ������������ƽ������
	static int SymmetryRegularEuationSquareRoot(KMatrix& a, KMatrix& d);
};

#endif // #ifndef __INCLUDED_LINEAR_EQUATION_H__
