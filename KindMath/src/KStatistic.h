/******************************************************************************
 ** Header File: KStatistic.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KStatistic
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

#ifndef __INCLUDED_STATISTIC_H__
#define __INCLUDED_STATISTIC_H__

#include "KMatrix.h"
#include "KLinearEquation.h"

class KIND_EXT_CLASS KStatistic
{
public:
	KStatistic(void);
	virtual ~KStatistic(void);

public:	// Rs ϵ��
	//�����������
	static void RsAnalyse(double* x,int n,double dt[3]);

	//�������������ƽ��ֵ
	static double RsAvg(double* x,int n);

	//��������ķ���
	static double RsPd(double* x,int n);

	//��������ı�׼��
	static double RsSd(double* x,int n);

	//��������ľ�ƽֵ(�ı�x)
	static void RsDAvg(double* x,int n);

	//��������ı�׼��(�ı�x)
	static void RsNormal(double* x,int n);

	//���������Э����(�ı�x1 x2)
	static double RsCov(double* x1,double* x2,int n);

	//������������ϵ��(�ı�x1 x2)
	static double RsRc(double* x1,double* x2,int n);

	//��������ķ��ϵ��(kurtosis coefficient)
	static double RsKc(double* x,int n);

	//���������ƫ��ϵ��(departure coefficient)
	static double RsDc(double* x,int n);

	//�����������Э����
	static double RsSCov(double* x,int n,int tao);
	//��������������Э����
	static double RsFSCov(double* x,int n,int tao);
	//�����������󽻲�Э����
	static double RsFCCov(double* x,double* y,int n,int tao);

	//��������������ϵ��
	static double RsSRc(double* x,int n,int tao);
	//�����������������ϵ��
	static double RsFSRc(double* x,int n,int tao);
	//�����������󽻲����ϵ��
	static double RsFCRc(double* x,double* y,int n,int tao);

private:
	//���������k�����ľ�
	static double _rsdck(double* x,int n,int k);
};

#endif // #ifndef __INCLUDED_STATISTIC_H__
