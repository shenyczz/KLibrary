/******************************************************************************
 ** Header File: KV2VInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KV2VInterpolate
 ** Version:
 ** Function: 根据任意点数据再插补到任意点上
			  1. 算法的时间耗费在自动计算扫描半径上
			  2. 在V2V中不分配内存,也不释放内存,内存管理由调用者负责

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. 声明插值对象
		KV2VInterpolate v2v;

		//2. 设置源数据
		v2v.SetInput(double* pxi,double* pyi,double* pvi,int ni);

		//3. 设置目的数据
		v2v.SetOutput(double* pxo,double* pyo,double* pvo,int no);

		//4. 设置扫描半径(km)
		//   如果扫描半径 =0,则自动计算扫描半径,,使扫描半径内至少有3个点最多9个点
		//   如果扫描半径!=0,则按扫描半径计算,nVertexMin 参数无效
		v2v.SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

		//5. 是否实施严格内插
		//v2v.SetStrict(BOOL bStrict=TRUE);

		//6. 实施插值
		v2v.Transact(iMethod);

******************************************************************************/

#pragma once


#define VERTEX_NUM_MIN	3	//插值时最小使用定点数

class KIND_EXT_CLASS KV2VInterpolate : public KProgressObject
{
public:
	KV2VInterpolate(void);
	virtual ~KV2VInterpolate(void);

//protected:
//	void Init();
//	void Cleanup();

public:
	//设置源数据
	virtual void SetInput(double* pxi,double* pyi,double* pvi,int ni);		//设置源离散点数据指针

	//设置输出数据
	virtual void SetOutput(double* pxo,double* pyo,double* pvo,int no);	//设置输出离散点数据指针

	//设置扫描半径(km)
	virtual void SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

	//为插值处理作准备
	//virtual void Prepare();

	//是否实施严格内插
	virtual void SetStrict(BOOL bStrict = TRUE) { m_bStrict = bStrict; }

	//实施插值
	virtual void Transact(int iMethod=0);

protected:
	//准备内存
	virtual void PrepareMemory();

	//自动计算各个被插补点的扫描半径
	void AutoRadius();

	//逐个对被插补点进行插值处理
	void Interpolate(int iMethod);

	//对坐标点(x,y)插补要素值
	// iMethod: 插值方法标志
	//       0: Gressman 插值
	//       1: Gauss 插值
	double InterpolateOnePoint(double x, double y, int iMethod);

	//Gressman 插值
	double GressmanInterp(double x, double y);

	//Gauss 插值
	double GaussInterp(double x, double y);

private:
	//计算扫描半径r,使得以(x0,y0)为圆心的圆能覆盖 n 个离散点
	double _CalculateRadius(double x0, double y0,int n);

	//计算两点间距离
	double _distance(double x0, double y0, double x1, double y1);

	//Gressman 权重因子
	double GressmanCoefficient(double r, double d);

	//Gauss 权重因子
	double GaussCoefficient(double r, double d);

protected:
	double	m_dRadius;			//扫描半径
	bool	m_bAutoRadius;		//如果扫描半径=0,则自动计算扫描半径,,使扫描半径内至少有m_nStation个点
	int		m_nVertexMin;		//自动计算扫描半径时，扫描半径内至少包含的离散点数(>=3) VERTEX_NUM_MIN

	double	m_xmin,m_xmax;		//插值x范围
	double	m_ymin,m_ymax;		//插值y范围

	double*	m_pRadiusArray;		//扫描半径数组,用于保存自动扫描得到的合适半径

	BOOL	m_bCorrect;			//是否进行插值误差修正

	BOOL	m_bStrict;			//是否严格内插(要看源数据的范围)

	//有调用者负责分配和释放内存
	double*	m_pxi;				//源数据x坐标指针
	double*	m_pyi;				//源数据y坐标指针
	double*	m_pvi;				//源数据离散点要素值指针
	int				m_ni;				//源数据数量

	//有调用者负责分配和释放内存
	double*	m_pxo;				//目的数据x坐标指针
	double*	m_pyo;				//目的源数据y坐标指针
	double*	m_pvo;				//目的源数据离散点要素值指针
	int		m_no;				//目的数据数量
};
