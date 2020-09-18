/******************************************************************************
 ** Header File: KGridExtract.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KGridExtract
 ** Version:
 ** Function: 从网格中提取指定区域的子网格数据

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. 声明类对象
		KGridExtract	ge;

		//2. 设置源网格属性参数
		ge.SetInput(int& nx,int& ny,double& x0,double& y0,const double xInterval,const double yInterval,double*& pData);

		//3. 设置抽取参数
		ge.SetExtractParam(int nxStart,int nxCount,int nyStart,int nyCount);
		//或者
		ge.SetExtractParam(double xmin,double ymin,double xmax,double ymax);

		//5. 实施
		if(!ge.Transact())
		{
			//注意:在返回之前调用者要释放非本类分配的内存
			//     否则会造成内存泄露
			return;
		}

		//6. 取得子网格属性数据
		ge.GetOutput(int* nx,int* ny,double* x,double* y,double* xInterval,double* yInterval,double*& pData);

******************************************************************************/

#pragma once

class KIND_EXT_CLASS  KGridExtract
{
public:
	KGridExtract(void);
	virtual ~KGridExtract(void);

protected:
	void Init();
	void Cleanup();

public:
	BOOL IsEmpty() {return m_pv1==NULL;}

public:
	void Empty();

public:
	//设置源网格属性参数
	void SetInput(int nx,int ny,double x0,double y0,const double xInterval,const double yInterval,double* pData);

	//设置抽取参数
	virtual void SetExtractParam(int nxStart,int nxCount,int nyStart,int nyCount);
	virtual void SetExtractParam(double xmin,double ymin,double xmax,double ymax);

	//准备
	BOOL Prepare();

	//实施
	BOOL Transact();

	//取得子网格属性数据
	void GetOutput(int* nx,int* ny,double* x,double* y,double* xInterval,double* yInterval,double*& pData);

protected:
	//校验输入数据
	BOOL VerifyInputData();

	//准备内存
	void PrepareMemory();

protected:
	//---源网格属性参数
	int			m_nx0;				// 网格点X向格点数(列数)
	int			m_ny0;				// 网格点Y向格点数(行数)

	double		m_x0;				// 网格点X向起始值
	double		m_y0;				// 网格点Y向起始值

	double		m_xInterval;		// 网格点X向间隔
	double		m_yInterval;		// 网格点Y向间隔

	double*		m_pv0;				// 网格点要素值指针(由调用者分配和释放内存)

	//---子网格属性参数
	int			m_nxStart;			//x方向抽取的开始格点序号
	int			m_nyStart;			//y方向抽取的开始格点序号

	int			m_nxCount;			//x方向抽取的格点数(列数)
	int			m_nyCount;			//y方向抽取的格点数(行数)

	double		m_xStart;			// 网格点X向起始值
	double		m_yStart;			// 网格点Y向起始值

	double*		m_pv1;				// 网格点要素值指针(由本类分配和释放内存)
};
