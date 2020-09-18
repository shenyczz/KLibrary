/******************************************************************************
 ** Header File: KStatistic.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KStatistic
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

#ifndef __INCLUDED_STATISTIC_H__
#define __INCLUDED_STATISTIC_H__

#include "KMatrix.h"
#include "KLinearEquation.h"

class KIND_EXT_CLASS KStatistic
{
public:
	KStatistic(void);
	virtual ~KStatistic(void);

public:	// Rs 系列
	//随机样本分析
	static void RsAnalyse(double* x,int n,double dt[3]);

	//随机样本的算术平均值
	static double RsAvg(double* x,int n);

	//随机样本的方差
	static double RsPd(double* x,int n);

	//随机样本的标准差
	static double RsSd(double* x,int n);

	//随机样本的矩平值(改变x)
	static void RsDAvg(double* x,int n);

	//随机样本的标准化(改变x)
	static void RsNormal(double* x,int n);

	//随机样本的协方差(改变x1 x2)
	static double RsCov(double* x1,double* x2,int n);

	//随机样本的相关系数(改变x1 x2)
	static double RsRc(double* x1,double* x2,int n);

	//随机样本的峰度系数(kurtosis coefficient)
	static double RsKc(double* x,int n);

	//随机样本的偏度系数(departure coefficient)
	static double RsDc(double* x,int n);

	//随机样本的自协方差
	static double RsSCov(double* x,int n,int tao);
	//随机样本的落后自协方差
	static double RsFSCov(double* x,int n,int tao);
	//随机样本的落后交叉协方差
	static double RsFCCov(double* x,double* y,int n,int tao);

	//随机样本的自相关系数
	static double RsSRc(double* x,int n,int tao);
	//随机样本的落后自相关系数
	static double RsFSRc(double* x,int n,int tao);
	//随机样本的落后交叉相关系数
	static double RsFCRc(double* x,double* y,int n,int tao);

private:
	//随机样本的k阶中心距
	static double _rsdck(double* x,int n,int k);
};

#endif // #ifndef __INCLUDED_STATISTIC_H__
