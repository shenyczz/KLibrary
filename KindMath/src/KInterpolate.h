/******************************************************************************
 ** Header File: KInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KInterpolate - ��ֵ��
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

#ifndef __INCLUDED_INTERPOLATE_H__
#define __INCLUDED_INTERPOLATE_H__

#include "KMatrix.h"

class KIND_EXT_CLASS KInterpolate
{
private:
	KInterpolate(void);

public:
	//��������ȫ���䲻�Ⱦ��ֵ
	static double LagrangeNotIsometry(double* x, double* y, int n,double t);
	//��������ȫ����Ⱦ��ֵ
	static double LagrangeIsometry(double x0, double h, double* y, int n,double t);

	//�����߲��Ⱦ��ֵ
	static double ParabolaNotIsometry(double* x, double* y, int n,double t);
	//�����ߵȾ��ֵ
	static double ParabolaIsometry(double x0, double h, double* y, int n,double t);

	//����ʽ���Ⱦ��ֵ
	static double FractionNotIsometry(double* x, double* y, int n,double t);
	//����ʽ�Ⱦ��ֵ
	static double FractionIsometry(double x0, double h, double* y, int n,double t);

	//�������ز��Ⱦ��ֵ
	static double HermiteNotIsometry(double* x, double* y, double* dy, int n,double t);
	//�������صȾ��ֵ
	static double HermiteIsometry(double x0, double h, double* y, double* dy, int n,double t);

	//���ؽ𲻵Ⱦ��𲽲�ֵ
	static double AitkenNotIsometry(double* x, double* y, int n,double t, double eps=1.0e-6);
	//���ؽ�Ⱦ��𲽲�ֵ
	static double AitkenIsometry(double x0, double h, double* y, int n,double t, double eps=1.0e-6);

	//������⻬���Ⱦ��ֵ
	static void AkimaNotIsometry(double* x, double* y, int n, int k, double t, double* s,int sn=5);
	//������⻬�Ⱦ��ֵ
	static void AkimaIsometry(double x0, double h, double* y, int n, int k, double t, double* s,int sn=5);


	//��һ�ֱ߽���������������������ֵ��΢�������
	static double SplineBoundary1(double* x, double* y, double* dy, double* ddy, int n,
				double* t, double* z, double* dz,double* ddz,int m);
	//�ڶ��ֱ߽���������������������ֵ��΢�������
	static double SplineBoundary2(double* x, double* y, double* dy, double* ddy, int n,
				double* t, double* z, double* dz,double* ddz,int m);
	//�����ֱ߽���������������������ֵ��΢�������
	static double SplineBoundary3(double* x, double* y, double* dy, double* ddy, int n,
				double* t, double* z, double* dz,double* ddz,int m);

	//��Ԫ�����ֵ
	static double InterpV2P3(double*x,int n,double* y,int m,KMatrix z,double u,double v);
	//��Ԫȫ�����ֵ
	static double InterpV2Whole(double*x,int n,double* y,int m,KMatrix z,double u,double v);
};

#endif // #ifndef __INCLUDED_INTERPOLATE_H__
