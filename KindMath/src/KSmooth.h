/******************************************************************************
 ** Header File: KSmooth.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KSmooth
 ** Version:
 ** Function: 对折线线数据进行平滑，平滑方法有3种
              1. Akima五点法平滑
              2. 分段三次多项式平滑
              3. 张力样条函数法平滑(没有开发成功)

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		1. KSmooth smth;
		2. smth.Akima(double* x,double* y,int n);					//Akima五点法平滑
 OR        smth.SubsectionPolynomial(double* x,double* y,int n);	//分段三次多项式平滑
 OR        smth.Tension(double* x,double* y,int n,double sigma);	//张力样条函数法平滑(没有开发成功)

		3. smth.GetOutput(double*& x,double*& y,int* n);			//取得平滑后的数据

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KSmooth
{
public:
	KSmooth(void);
	virtual ~KSmooth(void);

protected:
	void Init();
	void Cleanup();

public:
	//五点法(Akima法)
	int Akima(double* x,double* y,int n);

	//分段三次多项式
	int SubsectionPolynomial(double* x,double* y,int n);

	//张力样条函数拟合曲线(waiting ...)
	int Tension(double *x, double *y,int n,double sigma);

public:
	//取得输出
	void GetOutput(double*& x,double*& y,int* n);

protected:
	//准备内存
	void PrepareMemory(int iSize=0);

private:
	virtual double angle(double x1, double y1, double x2, double y2);
	//virtual double angle(double x1, double y1, double x2, double y2);

	//virtual double distance(double x1,double y1,double x2,double y2);
	virtual double distance(double x1,double y1,double x2,double y2);

	//分段插值
	void _DoSubsection(double x[],double y[],double alf[],double* xout,double *yout,int iPoint);

protected:
	int m_iNum;			//曲线光滑后的点数

	//曲线光滑后的坐标,由本类分配和释放内存
	double* m_px;		//
	double* m_py;		//
};
