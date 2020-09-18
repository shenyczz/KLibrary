#pragma once

class KIND_EXT_CLASS KRandom
{
private:
	KRandom(void);

public:
	//产生一个[0,1]区间内均匀分布伪随机数
	static double Rand_01_One(double& seed);
	//产生多个[0,1]区间内均匀分布伪随机数
	static void Rand_01_Series(double *dp, const size_t stCount,long seed=0);

	//产生任意[a,b]区间内一个均匀分布伪随机整数
	static size_t Rand_ab_One(size_t a, size_t b,long& seed);
	//产生多个任意[a,b]区间内均匀分布伪随机整数序列
	static void Rand_ab_Series(size_t a, size_t b, size_t stCount, long *sp, long seed=0);

	//产生任意均值与方差的一个正态分布随机数
	//函数的参数：mu和ro(μ和σ值)要事先给定，也可从其它程序中获得。
	//如果算法中的n值要取的更大(超过12)，程序要稍加修改。
	static double Rand_ND_One(double mu, double ro, double& seed);
	//产生任意均值与方差的正态分布随机数序列
	//函数的参数：mu和ro(μ和σ值)要事先给定，也可从其它程序中获得。
	//如果算法中的n值要取的更大(超过12)，程序要稍加修改。
	static void Rand_ND_Series(double mu, double ro, double* dp, size_t stCount, long seed=0);
};
