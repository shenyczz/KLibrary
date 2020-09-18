#include "StdAfx.h"
#include "KStatistic.h"

KStatistic::KStatistic(void)
{
}

KStatistic::~KStatistic(void)
{
}

//=======================================================
// 名    称: RsAnalyse
// 访    问: public
//
// 功    能: 随机样本分析
//
// 参    数:
//			 [i] x - 一维数组,长度为n，存放随机变量x的n个样本点
//			 [i] n - 一维数组x长度为n
//			 [o] dt - 一维数组,长度为3
//			     dt[0]返回随机样本的算术平均值
//			     dt[1]返回随机样本的方差
//			     dt[2]返回随机样本的标准差s
//
// 返    回: 
//
// 注    释: 
//
//=======================================================
void KStatistic::RsAnalyse(double* x,int n,double dt[3])
{
	int i;

    dt[0] = 0;
	//随机样本的算术平均值
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

//随机样本的算术平均值
// x	-->一维数组,长度为n，存放随机变量x的n个样本点
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

//随机样本的方差
// x	-->一维数组,长度为n，存放随机变量x的n个样本点
double KStatistic::RsPd(double* x,int n)
{
	double dValue=0.0;
	//---
	double dAvg = RsAvg(x,n);	//随机样本的算术平均值

    for(int i=0; i<n; i++)
	{
		dValue += (x[i] - dAvg) * (x[i] - dAvg);
	}
	dValue /= n;
	//---
	return dValue;
}

//随机样本的标准差
// x	-->一维数组,长度为n，存放随机变量x的n个样本点
double KStatistic::RsSd(double* x,int n)
{
	return sqrt(RsPd(x,n));
}

//随机样本的矩平值
// x - 一维数组,长度为n，存放随机变量x的n个样本点
///注意：计算完成时为随机样本的矩平值
void KStatistic::RsDAvg(double* x,int n)
{
	//平均值
	double dAvg = RsAvg(x,n);

	for(int i=0;i<n;i++)
	{
		x[i] -= dAvg;
	}
}

//随机样本的标准化
// x - 一维数组,长度为n，存放随机变量x的n个样本点
///注意: 计算完成时为随机样本的标准化值
void KStatistic::RsNormal(double* x,int n)
{
	// 标准差
	double sd = RsSd(x,n);

	// 矩平值
	RsDAvg(x,n);

	for(int i=0;i<n;i++)
	{
		x[i] /= sd;
	}
}

//随机样本的协方差
// x1 - 一维数组,长度为n，存放随机变量x的n个样本点
// x2 - 一维数组,长度为n，存放随机变量x的n个样本点
///注意: 计算完成时为随机样本的矩平值
double KStatistic::RsCov(double* x1,double* x2,int n)
{
	double dCov = 0.0;

	//x1的距平
	RsDAvg(x1,n);

	//x2的距平
	RsDAvg(x2,n);

	for(int i=0; i<n; i++)
	{
		dCov += x1[i] * x2[i];
	}

	dCov /= n;

	return dCov;
}

//随机样本的相关系数
// x1 - 一维数组,长度为n，存放随机变量x的n个样本点
// x2 - 一维数组,长度为n，存放随机变量x的n个样本点
///注意: 计算完成时为随机样本的矩平值
double KStatistic::RsRc(double* x1,double* x2,int n)
{
	double drc = 0.0;

	//求标准差
	double sd1 = RsSd(x1,n);
	double sd2 = RsSd(x2,n);

	//协方差
	double cov = RsCov(x1,x2,n);

	drc = cov / (sd1 * sd2);

	return drc;
}

//随机样本的峰度系数(kurtosis coefficient)
// x	-->随机样本数组
// n	-->样本容量
double KStatistic::RsKc(double* x,int n)
{
	double kc = 0;

	double m2 = _rsdck(x,n,2);
	double m4 = _rsdck(x,n,4);

	kc = m4/(m2*m2);
	kc -= 3;

	return kc;
}

//随机样本的偏度系数(departure coefficient)
// x	-->随机样本数组
// n	-->样本容量
double KStatistic::RsDc(double* x,int n)
{
	double dc = 0;

	double m2=_rsdck(x,n,2);
	double m3=_rsdck(x,n,3);

	dc = m3/pow(m2,3/2);

	return dc;
}

//随机样本的自协方差
// x	- 样本数组
// n	- 样本容量
// tao	- 时间间隔数
double KStatistic::RsSCov(double* x,int n,int tao)
{
	double scov = 0;

	//平均值
	double avg = RsAvg(x,n);

	for(int t=0;t<n-tao;t++)
	{
		scov += (x[t]-avg) * (x[t+tao]-avg);
	}

	scov /= (n-tao);

	return scov;
}

//随机样本的落后自协方差
// x	- 样本数组
// n	- 样本容量
// tao	- 时间间隔数
double KStatistic::RsFSCov(double* x,int n,int tao)
{
	double fscov = 0;

	//平均值
	double avg = RsAvg(x,n);

	for(int t=0;t<n-tao;t++)
	{
		fscov += (x[t]-avg) * (x[t+tao]-avg);
	}

	fscov /= n;

	return fscov;
}


//随机样本的落后交叉协方差
// x	-->样本数组
// y	-->样本数组
// n	-->样本容量
// tao	-->时间间隔数
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

//随机样本的自相关系数
// x	- 样本数组
// n	- 样本容量
// tao	- 时间间隔数
double KStatistic::RsSRc(double* x,int n,int tao)
{
	//自协方差
	double scov = RsSCov(x,n,tao);

	//方差
	double pd = RsPd(x,n);

	return scov / pd;
}

//随机样本的落后自相关系数
// x	-->样本数组
// n	-->样本容量
// tao	-->时间间隔数
double KStatistic::RsFSRc(double* x,int n,int tao)
{
	double fsrc = 0;

	double avg = RsAvg(x,n);	//平均值
	double pd = RsPd(x,n);		//方差

	for(int t=0;t<n-tao;t++)
	{
		fsrc += (x[t]-avg) * (x[t+tao]-avg) / pd;
	}

	fsrc /= n;

	return fsrc;
}

//随机样本的落后交叉相关系数
// x	-->样本数组
// y	-->样本数组
// n	-->样本容量
// tao	-->时间间隔数
double KStatistic::RsFCRc(double* x,double* y,int n,int tao)
{
	double fcrc = 0;

	double xsd = RsSd(x,n);	//x标准方差
	double ysd = RsSd(y,n);	//y标准方差
	double fccov = RsFCCov(x,y,n,tao);	//落后交叉协方差

	fcrc = fccov / (xsd*ysd);

	return fcrc;
}

//private
//随机样本的k阶中心距(private)
// x	-->随机样本数组
// n	-->样本容量
// k	-->中心距阶数(2,3,4,...)
double KStatistic::_rsdck(double* x,int n,int k)
{
	double dValue(0);

	if(k<=1) 
		return double(0);	//k==1时为0

	double xavg=RsAvg(x,n);		//x平均值
	for(int i=0;i<n;i++)
	{
		dValue += pow(x[i]-xavg,k);
	}
	dValue/=n;

	return dValue;
}
