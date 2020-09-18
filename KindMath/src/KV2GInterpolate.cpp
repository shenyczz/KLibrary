#include "StdAfx.h"
#include "KV2GInterpolate.h"

KV2GInterpolate::KV2GInterpolate(void)
{
	m_nx			= 0;
	m_ny			= 0;
	m_x0			= 0.0f;
	m_y0			= 0.0f;
	m_xInterval		= 0.0f;
	m_yInterval		= 0.0f;

	m_px			= NULL;
	m_py			= NULL;
	m_pv			= NULL;
}

KV2GInterpolate::~KV2GInterpolate(void)
{
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}
	if(m_pv)		{delete []m_pv; m_pv=NULL;}
}
//
//void KV2GInterpolate::Init()
//{
//	m_nx			= 0;
//	m_ny			= 0;
//	m_x0			= 0.0f;
//	m_y0			= 0.0f;
//	m_xInterval		= 0.0f;
//	m_yInterval		= 0.0f;
//
//	m_px			= NULL;
//	m_py			= NULL;
//	m_pv			= NULL;
//}
//
//void KV2GInterpolate::Cleanup()
//{
//	//先释放为网格分配的坐标内存
//	if(m_px)		{delete []m_px; m_px=NULL;}
//	if(m_py)		{delete []m_py; m_py=NULL;}
//	if(m_pv)		{delete []m_pv; m_pv=NULL;}
//}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetGridParam
// 功    能: 设置网格属性参数
// 访    问: public
// 参    数:
//			[i] nx - x方向格点数
//			[i] ny - y方向格点数
//			[i] x0 - 网格点x向最小值
//			[i] y0 - 网格点y向最小值
//			[i] xInterval - x方向网格点间隔
//			[i] yInterval - y方向网格点间隔
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KV2GInterpolate::SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval)
{
	m_nx		= nx;			// 网格点X向格点数(列数)
	m_ny		= ny;			// 网格点Y向格点数(行数)

	m_x0		= x0;			// 网格点X向最小值	
	m_y0		= y0;			// 网格点Y向最小值

	m_xInterval	= xInterval;	// 网格点X向间隔
	m_yInterval	= yInterval;	// 网格点Y向间隔
}
void KV2GInterpolate::SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval)
{
	m_nx = (int)(double)((xmax - xmin)/xInterval + 1);
	m_ny = (int)(double)((ymax - ymin) / yInterval) + 1;

	m_x0		= xmin;			// 网格点X向最小值	
	m_y0		= ymin;			// 网格点Y向最小值

	m_xInterval	= xInterval;	// 网格点X向间隔
	m_yInterval	= yInterval;	// 网格点Y向间隔
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Prepare
// 功    能: 为插值处理作准备
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
//void KV2GInterpolate::Prepare()
//{
//	KV2VInterpolate::Prepare();
//
//	//准备内存
//	PrepareMemory();
//}
//
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
void KV2GInterpolate::Transact(int iMethod)
{
	//------------------------------------------
	//准备内存
	PrepareMemory();
	//------------------------------------------
	//调用父类功能
	KV2VInterpolate::SetOutput(m_px,m_py,m_pv,m_nx*m_ny);

	KV2VInterpolate::Transact(iMethod);
	//------------------------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: PrepareMemory
// 功    能: 准备内存
// 访    问: protected
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KV2GInterpolate::PrepareMemory()
{
	//-----------------------------------------------
	//先释放为网格分配的坐标内存
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}
	if(m_pv)		{delete []m_pv; m_pv=NULL;}
	//-----------------------------------------------
	m_px = new double[m_nx*m_ny];
	m_py = new double[m_nx*m_ny];
	m_pv = new double[m_nx*m_ny];

	for(int i=0;i<m_ny;i++)
	{
		for(int j=0;j<m_nx;j++)
		{
			*(m_px+i*m_nx+j) = m_x0 + j*m_xInterval;
			*(m_py+i*m_nx+j) = m_y0 + i*m_yInterval;
			*(m_pv+i*m_nx+j) = 0.0f;
		}
	}
	//-----------------------------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GetOutput
// 功    能: 取得插值后的网格数据
// 访    问: public
// 参    数:
//			[i] nx - x方向格点数
//			[i] ny - y方向格点数
//			[i] pData - 网格点要素值指针
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
//void KGridDensify::GetOutput(int* nx,int* ny,double* x0,double* y0,double* xInterval,double* yInterval,double*& pData)
double* KV2GInterpolate::GetOutput(int* nx,int* ny)
{
	*nx = m_nx;
	*ny = m_ny;

	return m_pv;
}
