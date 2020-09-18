/******************************************************************************
 ** Header File: KSFunction.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KSFunction - ���⺯����
 ** Version:
 ** Function:
 ** Explain:

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

#ifndef __INCLUDED_SFUNCTION_H__
#define __INCLUDED_SFUNCTION_H__

class KIND_EXT_CLASS KSFunction
{
private:
	KSFunction(void);

public:
	//٤����
	static double Gamma(double x);
	//����ȫ٤����
	static double IncompleteGamma(double a, double x);
	//����
	static double Error(double x);
	//����ȫ��������
	static double IncompleteBeta(double a, double b, double x);

	//��̬�ֲ�����
	static double NormalDistribution(double a, double d, double x);
	//t-�ֲ�����
	static double tDistribution(double t, int n);
	//X^2-�ֲ�����
	static double X2Distribution(double x, int n);
	//F-�ֲ�����
	static double FDistribution(double f, int n1, int n2);

private:
	//����ȫ���������Ĵ�������
	static double _beta(double a, double b, double x);
};

#endif // #ifndef __INCLUDED_SFUNCTION_H__
