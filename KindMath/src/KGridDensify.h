/******************************************************************************
 ** Header File: KGridDensify.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KGridDensify
 ** Version:
 ** Function: 加密规则四边形网格数据

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. 声明类对象
		KGridDensify	gd;

		//2. 设置源网格属性参数
		gd.SetInput(int nx0,int ny0,double x0,double y0,double xInterval0,double yInterval0,double* pData0);
			或
		gd.SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval);

		//3. 设置加密参数
		gd.SetDensifyParam(double xInterval,double yInterval);

		//5. 实施
		gd.Transact();

		//6. 取得加密后网格属性数据
		gd.GetOutput(int* nx,int* ny,double*& pData);

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KGridDensify
{
public:
	KGridDensify(void);
	virtual ~KGridDensify(void);

protected:
	void Init();
	void Cleanup();

public:
	BOOL IsEmpty() {return m_pv1==NULL;}

public:
	void Empty();

public:
	//设置源网格属性参数
	void SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);
	void SetInput(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval,double* pData);

	//设置加密参数
	void SetDensifyParam(double xInterval,double yInterval);

	//准备
	void Prepare();

	//实施
	void Transact();


	//取得加密后网格属性数据
	void GetOutput(int* nx,int* ny,double* x0,double* y0,double* xInterval,double* yInterval,double*& pData);

protected:
	//准备内存
	void PrepareMemory();

	//插值
	double InterpolateOnePoint(double x,double y);

private:
	//计算两点间距
	double _distance(double x0, double y0, double x1, double y1);

protected:
	//---源原网格属性参数
	int			m_nx0;				// 网格点X向格点数(列数)
	int			m_ny0;				// 网格点Y向格点数(行数)

	double		m_x0;				// 网格点X向起始值
	double		m_y0;				// 网格点Y向起始值

	double		m_xInterval0;		// 网格点X向间隔
	double		m_yInterval0;		// 网格点Y向间隔

	double*		m_pv0;				// 网格点要素值指针(由调用者分配和释放内存)

	//---加密后网格属性参数
	int			m_nx1;				// 网格点X向格点数(列数)
	int			m_ny1;				// 网格点Y向格点数(行数)

	double		m_xInterval1;		// 网格点X向间隔
	double		m_yInterval1;		// 网格点Y向间隔

	double*		m_pv1;				// 网格点要素值指针(由本类分配和释放内存)
};
