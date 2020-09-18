#include "StdAfx.h"
#include "KV2VInterpolate.h"

KV2VInterpolate::KV2VInterpolate(void)
{
	m_pxi	= NULL;
	m_pyi	= NULL;
	m_pvi	= NULL;
	m_ni	= 0;

	m_pxo	= NULL;
	m_pyo	= NULL;
	m_pvo	= NULL;
	m_no	= 0;

	m_dRadius		= 0.0f;
	m_nVertexMin	= VERTEX_NUM_MIN;
	m_bAutoRadius	= TRUE;
	m_bCorrect		= TRUE;
	m_bStrict		= FALSE;

	m_xmin = 1.0e+12f;
	m_xmax = 1.0e-12f;		//插值x范围

	m_ymin = 1.0e+12f;
	m_ymax = 1.0e-12f;		//插值y范围


	m_pRadiusArray = NULL;
}

KV2VInterpolate::~KV2VInterpolate(void)
{
	_deletea(m_pRadiusArray);
}

//void KV2VInterpolate::Init()
//{
//	m_pxi	= NULL;
//	m_pyi	= NULL;
//	m_pvi	= NULL;
//	m_ni	= 0;
//
//	m_pxo	= NULL;
//	m_pyo	= NULL;
//	m_pvo	= NULL;
//	m_no	= 0;
//
//	m_dRadius		= 0.0f;
//	m_nVertexMin	= VERTEX_NUM_MIN;
//	m_bAutoRadius	= TRUE;
//	m_bCorrect		= TRUE;
//	m_bStrict		= FALSE;
//
//	m_xmin = 1.0e+12f;
//	m_xmax = 1.0e-12f;		//插值x范围
//
//	m_ymin = 1.0e+12f;
//	m_ymax = 1.0e-12f;		//插值y范围
//
//
//	m_pRadiusArray = NULL;
//}

//void KV2VInterpolate::Cleanup()
//{
//	_deletea(m_pRadiusArray);
//}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetInput
// 功    能: 设置源离散点数据指针
// 访    问: public
// 参    数:
//			[i] pxi - 源离散点数据的x坐标指针
//			[i] pyi - 源离散点数据的y坐标指针
//			[i] pvi - 源离散点数据的要素值指针
//			[i] ni - 源离散点数量(相当于数组长度)
// 返    回: 
// 注    释: pxi,pyi,pvi所指内存有调用者负责分配和释放,这点一定要注意,否则
//			 会产生内存泄露
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::SetInput(double* pxi,double* pyi,double* pvi,int ni)
{
	m_pxi	= pxi;
	m_pyi	= pyi;
	m_pvi	= pvi;
	m_ni	= ni;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetOutput
// 功    能: 设置输出离散点数据指针
// 访    问: public
// 参    数:
//			[i] pxi - 源离散点数据的x坐标指针
//			[i] pyi - 源离散点数据的y坐标指针
//			[i] pvi - 源离散点数据的要素值指针
//			[i] ni - 源离散点数量(相当于数组长度)
// 返    回: 
// 注    释: pxi,pyi,pvi所指内存有调用者负责分配和释放,这点一定要注意,否则
//			 会产生内存泄露
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::SetOutput(double* pxo,double* pyo,double* pvo,int no)
{
	m_pxo	= pxo;
	m_pyo	= pyo;
	m_pvo	= pvo;
	m_no	= no;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetRadius
// 功    能: 设置扫描半径(km)
// 访    问: public
// 参    数:
//			[i] dRadius - 扫描半径,默认=0.0
//				如果扫描半径<=0,则自动计算扫描半径,使扫描半径内至少有nVertexMin个点
//				如果扫描半径 >0,则自动利用扫描半径计算,nVertexMin 参数无效
//			[i] nVertexMin - 扫描覆盖范围内包含的最小数据点数,默认=VERTEX_NUM_MIN
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::SetRadius(double dRadius,int nVertexMin)
{
	m_dRadius		= dRadius;
	m_nVertexMin	= nVertexMin;

	if(m_nVertexMin>9)
	{// 最大9个点
		m_nVertexMin = 9;
	}
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Prepare
// 功    能: 为插值处理作准备
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
//void KV2VInterpolate::Prepare()
//{
//	//1. 根据源数据搜索插值范围，超过范围无效
//	//是否严格内插
//	if(m_bStrict)
//	{
//		for(int i=0;i<m_ni;i++)
//		{
//			m_xmin = min(m_xmin,m_pxi[i]);
//			m_xmax = max(m_xmax,m_pxi[i]);
//
//			m_ymin = min(m_ymin,m_pyi[i]);
//			m_ymax = max(m_ymax,m_pyi[i]);
//		}
//	}
//
//	//2. 扫描半径小于等于0，要求自动扫描
//	if(fabs(m_dRadius) < 1.e-12 || m_dRadius < 0)
//	{
//		m_bAutoRadius = TRUE;
//
//		KfxAssert(m_nVertexMin >= 3);		//自动扫描要求扫描半径内测站数量大于等于3
//		KfxAssert(m_nVertexMin < m_ni);		//自动扫描要求扫描半径内测站数量不能大于总数量
//	}
//	else
//	{
//		m_bAutoRadius = false;
//	}
//}

///////////////////////////////////////////////////////////////////////////
// 名    称: Transact
// 功    能: 插值处理
// 访    问: public
// 参    数:
//			[i] iMethod - 插值方法标志,默认=0
//				0-Gressman插值
//				1-Gauss插值
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::Transact(int iMethod)
{
	//在V2V中不分配内存,也不释放内存,内存管理由调用者负责
	//所以不用调用PrepareMemory()
	//1. 根据源数据搜索插值范围，超过范围无效
	//是否严格内插
	if(m_bStrict)
	{
		for(int i=0;i<m_ni;i++)
		{
			m_xmin = min(m_xmin,m_pxi[i]);
			m_xmax = max(m_xmax,m_pxi[i]);

			m_ymin = min(m_ymin,m_pyi[i]);
			m_ymax = max(m_ymax,m_pyi[i]);
		}
	}

	//2. 扫描半径小于等于0，要求自动扫描
	if(fabs(m_dRadius) < 1.e-12 || m_dRadius < 0)
	{
		m_bAutoRadius = TRUE;

		// 自动扫描要求扫描半径内测站数量大于等于3
		// 自动扫描要求扫描半径内测站数量不能大于总数量
		if(m_nVertexMin<3 || m_nVertexMin>=m_ni)
			throw;
	}
	else
	{
		m_bAutoRadius = FALSE;
	}
	//------------------------------------------------
	//自动计算各个被插补点的扫描半径,使对应点的扫描半径内至少有 nVertex 个点
	//函数完成后,被插值的离散点对应的扫描半径保存在 m_RadiusArray
	if(m_bAutoRadius)
	{
		AutoRadius();
	}
	else
	{
		//调用者提供固定的扫描半径,在 m_dRadius;
	}
	//------------------------------------------------
	//逐个对被插补点进行插值处理
	if(iMethod > 1) iMethod = 0;

	Interpolate(iMethod);
	//------------------------------------------------
	//校正
	if(m_bCorrect)
	{
		//把第一次插值结果作为输入,回插到原来离散点上,计算得到插值误差
		//再把插值误差重新插到插值点上进行误差订正
	}
}

///////////////////////////////////////////////////////////////////////////
// 名    称: PrepareMemory
// 功    能: 准备内存
// 访    问: protected
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::PrepareMemory()
{
	//不用分配内存
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Interpolate
// 功    能: 逐个对被插补点进行插值处理
// 访    问: protected
// 参    数:
//			[i] iMethod - 插值方法标志,默认=0
//				0-Gressman插值
//				1-Gauss插值
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::Interpolate(int iMethod)
{
	//------------------------------------
	int i;
	double x, y;
	//------------------------------------
	// 回调函数
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	// 进度条参数
	ProgressParam* pCallbackParam = (ProgressParam*)this->CallbackParam();
	//------------------------------------
	if(pCallbackParam) pCallbackParam->SetInfo(_T("插值处理..."));
	for(i=0;i<m_no;i++)
	{
		x = m_pxo[i];
		y = m_pyo[i];

		//-------------
		if(m_bAutoRadius)	//自动计算扫描半径(km)
		{
			//m_dRadius = m_RadiusArray[i];
			m_dRadius = *(m_pRadiusArray+i);
		}
		//-------------
		//if(m_bStrict)
		//{
		//	if(x<m_xmin || x>m_xmax || y<m_ymin || y>m_ymax)
		//	{
		//		m_pvo[i] = 0;
		//	}
		//	else
		//	{
		//		m_pvo[i] = InterpolateOnePoint(x,y,iMethod);	//插值
		//	}
		//}
		//else
		{
			m_pvo[i] = InterpolateOnePoint(x,y,iMethod);	//插值
		}

		// 显示进度
		if(pCallbackFunc)
		{
			if(pCallbackParam)
			{
				pCallbackParam->lTotalNums = m_no;
				pCallbackParam->lCurrentPos = i;
			}

			pCallbackFunc(pCallbackParam);
		}
	}
	//------------------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: InterpolateOnePoint
// 功    能: 对坐标点(x,y)插补要素值
// 访    问: protected
// 参    数:
//			[i] iMethod - 插值方法标志,默认=0
//				0-Gressman插值
//				1-Gauss插值
// 返    回: 计算得到的插值点的要素值
// 注    释: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::InterpolateOnePoint(double x, double y, int iMethod)
{
	double dValue(0);
	//-------------------------------------------
	switch(iMethod)
	{
	case 0:
		dValue = GressmanInterp(x,y);	//客观分析插值
		break;
	case 1:
		dValue = GaussInterp(x,y);		//高斯权重插值
		break;
	}
	//-------------------------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GressmanInterp
// 功    能: Gressman 插值
// 访    问: protected
// 参    数:
//			[i] x - 插值点的x坐标
//			[i] y - 插值点的y坐标
//
// 返    回: 计算得到的插值点的要素值
// 注    释: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GressmanInterp(double x, double y)
{
	double dValue(0);
	//---------------------------------
	double qz;	//权重
	double fk;	//k点的值
	double sumqz(0.0),sumfk(0.0);
	double dk;
	//---
	for(int i=0;i<m_ni;i++)
	{
		dk = _distance(x,y,m_pxi[i],m_pyi[i]);	//两点距离
		
		if(dk > m_dRadius)	continue;	//在扫描范围之外

		dk *= 111.37f;		//km

		fk = m_pvi[i];		//取得源数据点的要素值

		//---计算权重
		qz  = GressmanCoefficient(m_dRadius,dk);
		//-------------

		fk *= qz;		//加权值

		sumqz += qz;	//权重和
		sumfk += fk;	//源数据点的加权值之和
	}
	//---------------------------------
	dValue = sumqz < 1.e-12 ? 0.0f : sumfk/sumqz;
	//-------------------------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GaussInterp
// 功    能: Gauss 插值
// 访    问: protected
// 参    数:
//			[i] x - 插值点的x坐标
//			[i] y - 插值点的y坐标
//
// 返    回: 计算得到的插值点的要素值
// 注    释: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GaussInterp(double x, double y)
{
	double dValue(0);
	//---------------------------------
	double qz;	//权重
	double fk;	//k点的值
	double sumqz(0.0),sumfk(0.0);
	double dk;
	//---
	for(int i=0;i<m_ni;i++)
	{
		dk = _distance(x,y,m_pxi[i],m_pyi[i]);	//两点距离
		dk *= 111.37f;	//km
		fk = m_pvi[i];

		//---计算权重
		qz  = GaussCoefficient(m_dRadius,dk);
		//-------------

		fk *= qz;

		sumqz += qz;	//权重和
		sumfk += fk;	//
	}
	//---------------------------------
	dValue = sumqz < 1.e-12 ? 0.0f : sumfk/sumqz;
	//-------------------------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: AutoRadius
// 功    能: 自动计算各个被插补点的扫描半径,使对应点的扫描半径内至少有 m_nVertexMin 个点
//			 函数完成后,被插值的离散点对应的扫描半径保存在 m_RadiusArray
// 访    问: protected
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::AutoRadius()
{
	_deletea(m_pRadiusArray);

	m_pRadiusArray = new double[m_no];
	memset(m_pRadiusArray,0,sizeof(double)*m_no);

	if(m_pCallbackParam)
	{
		((ProgressParam*)m_pCallbackParam)->SetInfo(_T("自动计算扫描半径..."));
	}

	for(int i=0;i<m_no;i++)
	{
		double x0=m_pxo[i];
		double y0=m_pyo[i];

		//计算在以(x0,y0)为圆心的扫描范围内包含 m_nVertexMin 个数据点的扫描半径
		double dRadius=_CalculateRadius(x0,y0,m_nVertexMin);

		//保存每次自动扫描得到的每个被插补点对应的扫描半径
		//m_RadiusArray.push_back(dRadius);
		*(m_pRadiusArray+i) = dRadius;

		// 显示进度
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = m_no;
				pProgressParam->lCurrentPos = i;
			}

			m_pCallbackFunc(m_pCallbackParam);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// 名    称: _calculateRadius
// 功    能: 计算扫描半径r,使得以(x0,y0)为圆心的圆能覆盖 n 个离散点
// 访    问: private
// 参    数:
//			[i] x0 - 点P0的x坐标
//			[i] y0 - 点P0的y坐标
//
// 返    回: 扫描半径
// 注    释: 算法的时间耗费在自动自动计算扫描半径上
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::_CalculateRadius(double x0, double y0,int n)
{
	int i,j;
	double dRadius = 0.0;
	//TDZ tdz;
	int iNum=0;
	double dr = 1.85f;		//1.85km相当于1/60经纬度(1分)
	//---------------------------
	// 最多9个点
	n = n>9 ? 9 : n;

	double radius[10];
	for(j=0;j<10;j++)
	{
		radius[j] = 1.0e+12f;
	}
	//---------------------------
	for(i=0;i<m_ni;i++)
	{
		double d = _distance(x0,y0,m_pxi[i],m_pyi[i]);
		d*=111.37f;	//变成公里

		for(j=0;j<10;j++)
		{// 从小到大
			if(d<radius[j])
			{
				for(int k=9;k>j;k--)
				{// 后移
					radius[k] = radius[k-1];
				}

				radius[j] = d;
				break;
			}
		}
	}
	//---------------------------
	dRadius = radius[n];
	dRadius += radius[n-1];
	dRadius /= 2.0;
	//---------------------------
	return dRadius;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: _distance
// 功    能: 计算两点间距离
// 访    问: private
// 参    数:
//			[i] x0 - 点P0的x坐标
//			[i] y0 - 点P0的y坐标
//			[i] x1 - 点P1的x坐标
//			[i] y1 - 点P1的y坐标
// 返    回: 两点间距离
// 注    释: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::_distance(double x0, double y0, double x1, double y1)
{
	double dValue = 0.0;
	//-----------------------------
	dValue = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
	dValue = sqrt(dValue);
	//-----------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GressmanCoefficient
// 功    能: Gressman权重因子
// 访    问: private
// 参    数:
//			[i] r - 扫描半径
//			[i] d - 两点间距离
//
// 返    回: 权重因子
// 注    释: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GressmanCoefficient(double r, double d)
{
	double dCoefficient = 0.0;
	//---
	if(d < r)	//两点间距离小于扫描半径
	{
		double rr = r*r;
		double dd = d*d;
		dCoefficient = (rr-dd)/(rr+dd);
	}
	else
	{
		dCoefficient = 0.0;
	}
	//---
	return dCoefficient;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GaussCoefficient
// 功    能: Gauss 权重因子
// 访    问: private
// 参    数:
//			[i] r - 扫描半径
//			[i] d - 两点间距离
//
// 返    回: 权重因子
// 注    释: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GaussCoefficient(double r, double d)
{
	double dCoefficient(0);
	//-------------------------------------------
	if(d < r)	//两点间距离小于扫描半径
	{
		double rr=r*r;
		double dd=d*d;
		dCoefficient= (double)exp(-d*d/(4.0*10000));
		//dCoefficient= exp((rr+dd)/(rr-dd)/(4.0*10000));
	}
	else
	{
		dCoefficient = 0.0f;
	}
	//-------------------------------------------
	return dCoefficient;
}
