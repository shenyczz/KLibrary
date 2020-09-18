#include "StdAfx.h"
#include "KStatistic.h"

KStatistic::KStatistic(void)
{
}

KStatistic::~KStatistic(void)
{
}

//=======================================================
// ��    ��: RsAnalyse
// ��    ��: public
//
// ��    ��: �����������
//
// ��    ��:
//			 [i] x - һά����,����Ϊn������������x��n��������
//			 [i] n - һά����x����Ϊn
//			 [o] dt - һά����,����Ϊ3
//			     dt[0]�����������������ƽ��ֵ
//			     dt[1]������������ķ���
//			     dt[2]������������ı�׼��s
//
// ��    ��: 
//
// ע    ��: 
//
//=======================================================
void KStatistic::RsAnalyse(double* x,int n,double dt[3])
{
	int i;

    dt[0] = 0;
	//�������������ƽ��ֵ
    for(i=0; i<n; i++)
	{
		dt[0]=dt[0]+x[i]/n;
	}

    dt[1] = 0;
    for(i=0; i<n; i++)
	{
      dt[1] = dt[1] + (x[i] - dt[0]) * (x[i] - dt[0]);
	}
    dt[1] = dt[1] / n;
    dt[2] = sqrt(dt[1]);
}

//�������������ƽ��ֵ
// x	-->һά����,����Ϊn������������x��n��������
double KStatistic::RsAvg(double* x,int n)
{
	double avg = 0.0;
	//---
    for(int i=0; i<n; i++)
	{
		avg += x[i]/n;
	}
	//---
	return avg;
}

//��������ķ���
// x	-->һά����,����Ϊn������������x��n��������
double KStatistic::RsPd(double* x,int n)
{
	double dValue=0.0;
	//---
	double dAvg = RsAvg(x,n);	//�������������ƽ��ֵ

    for(int i=0; i<n; i++)
	{
		dValue += (x[i] - dAvg) * (x[i] - dAvg);
	}
	dValue /= n;
	//---
	return dValue;
}

//��������ı�׼��
// x	-->һά����,����Ϊn������������x��n��������
double KStatistic::RsSd(double* x,int n)
{
	return sqrt(RsPd(x,n));
}

//��������ľ�ƽֵ
// x - һά����,����Ϊn������������x��n��������
///ע�⣺�������ʱΪ��������ľ�ƽֵ
void KStatistic::RsDAvg(double* x,int n)
{
	//ƽ��ֵ
	double dAvg = RsAvg(x,n);

	for(int i=0;i<n;i++)
	{
		x[i] -= dAvg;
	}
}

//��������ı�׼��
// x - һά����,����Ϊn������������x��n��������
///ע��: �������ʱΪ��������ı�׼��ֵ
void KStatistic::RsNormal(double* x,int n)
{
	// ��׼��
	double sd = RsSd(x,n);

	// ��ƽֵ
	RsDAvg(x,n);

	for(int i=0;i<n;i++)
	{
		x[i] /= sd;
	}
}

//���������Э����
// x1 - һά����,����Ϊn������������x��n��������
// x2 - һά����,����Ϊn������������x��n��������
///ע��: �������ʱΪ��������ľ�ƽֵ
double KStatistic::RsCov(double* x1,double* x2,int n)
{
	double dCov = 0.0;

	//x1�ľ�ƽ
	RsDAvg(x1,n);

	//x2�ľ�ƽ
	RsDAvg(x2,n);

	for(int i=0; i<n; i++)
	{
		dCov += x1[i] * x2[i];
	}

	dCov /= n;

	return dCov;
}

//������������ϵ��
// x1 - һά����,����Ϊn������������x��n��������
// x2 - һά����,����Ϊn������������x��n��������
///ע��: �������ʱΪ��������ľ�ƽֵ
double KStatistic::RsRc(double* x1,double* x2,int n)
{
	double drc = 0.0;

	//���׼��
	double sd1 = RsSd(x1,n);
	double sd2 = RsSd(x2,n);

	//Э����
	double cov = RsCov(x1,x2,n);

	drc = cov / (sd1 * sd2);

	return drc;
}

//��������ķ��ϵ��(kurtosis coefficient)
// x	-->�����������
// n	-->��������
double KStatistic::RsKc(double* x,int n)
{
	double kc = 0;

	double m2 = _rsdck(x,n,2);
	double m4 = _rsdck(x,n,4);

	kc = m4/(m2*m2);
	kc -= 3;

	return kc;
}

//���������ƫ��ϵ��(departure coefficient)
// x	-->�����������
// n	-->��������
double KStatistic::RsDc(double* x,int n)
{
	double dc = 0;

	double m2=_rsdck(x,n,2);
	double m3=_rsdck(x,n,3);

	dc = m3/pow(m2,3/2);

	return dc;
}

//�����������Э����
// x	- ��������
// n	- ��������
// tao	- ʱ������
double KStatistic::RsSCov(double* x,int n,int tao)
{
	double scov = 0;

	//ƽ��ֵ
	double avg = RsAvg(x,n);

	for(int t=0;t<n-tao;t++)
	{
		scov += (x[t]-avg) * (x[t+tao]-avg);
	}

	scov /= (n-tao);

	return scov;
}

//��������������Э����
// x	- ��������
// n	- ��������
// tao	- ʱ������
double KStatistic::RsFSCov(double* x,int n,int tao)
{
	double fscov = 0;

	//ƽ��ֵ
	double avg = RsAvg(x,n);

	for(int t=0;t<n-tao;t++)
	{
		fscov += (x[t]-avg) * (x[t+tao]-avg);
	}

	fscov /= n;

	return fscov;
}


//�����������󽻲�Э����
// x	-->��������
// y	-->��������
// n	-->��������
// tao	-->ʱ������
double KStatistic::RsFCCov(double* x,double* y,int n,int tao)
{
	double fccov = 0;

	double xavg = RsAvg(x,n);
	double yavg = RsAvg(y,n);

	for(int t=0;t<n-tao;t++)
	{
		fccov = (x[t]-xavg) * (y[t+tao]-yavg);
	}

	fccov /= (n-tao);

	return fccov;
}

//��������������ϵ��
// x	- ��������
// n	- ��������
// tao	- ʱ������
double KStatistic::RsSRc(double* x,int n,int tao)
{
	//��Э����
	double scov = RsSCov(x,n,tao);

	//����
	double pd = RsPd(x,n);

	return scov / pd;
}

//�����������������ϵ��
// x	-->��������
// n	-->��������
// tao	-->ʱ������
double KStatistic::RsFSRc(double* x,int n,int tao)
{
	double fsrc = 0;

	double avg = RsAvg(x,n);	//ƽ��ֵ
	double pd = RsPd(x,n);		//����

	for(int t=0;t<n-tao;t++)
	{
		fsrc += (x[t]-avg) * (x[t+tao]-avg) / pd;
	}

	fsrc /= n;

	return fsrc;
}

//�����������󽻲����ϵ��
// x	-->��������
// y	-->��������
// n	-->��������
// tao	-->ʱ������
double KStatistic::RsFCRc(double* x,double* y,int n,int tao)
{
	double fcrc = 0;

	double xsd = RsSd(x,n);	//x��׼����
	double ysd = RsSd(y,n);	//y��׼����
	double fccov = RsFCCov(x,y,n,tao);	//��󽻲�Э����

	fcrc = fccov / (xsd*ysd);

	return fcrc;
}

//private
//���������k�����ľ�(private)
// x	-->�����������
// n	-->��������
// k	-->���ľ����(2,3,4,...)
double KStatistic::_rsdck(double* x,int n,int k)
{
	double dValue(0);

	if(k<=1) 
		return double(0);	//k==1ʱΪ0

	double xavg=RsAvg(x,n);		//xƽ��ֵ
	for(int i=0;i<n;i++)
	{
		dValue += pow(x[i]-xavg,k);
	}
	dValue/=n;

	return dValue;
}
