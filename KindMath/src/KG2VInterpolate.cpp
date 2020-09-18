#include "StdAfx.h"
#include "KG2VInterpolate.h"

KG2VInterpolate::KG2VInterpolate(void)
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

KG2VInterpolate::~KG2VInterpolate(void)
{
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}

}
//
//void KG2VInterpolate::Init()
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
//
//	//KV2VInterpolate::Init();
//}
//
//void KG2VInterpolate::Cleanup()
//{
//	if(m_px)		{delete []m_px; m_px=NULL;}
//	if(m_py)		{delete []m_py; m_py=NULL;}
//
//	KV2VInterpolate::Cleanup();
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
//			[i] pData - 网格点要素值指针
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KG2VInterpolate::SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData)
{
	if(!pData) throw;
	//--------------------------------
	m_nx		= nx;			// 网格点X向格点数(列数)
	m_ny		= ny;			// 网格点Y向格点数(行数)

	m_x0		= x0;			// 网格点X向最小值	
	m_y0		= y0;			// 网格点Y向最小值

	m_xInterval	= xInterval;	// 网格点X向间隔
	m_yInterval	= yInterval;	// 网格点Y向间隔

	m_pv		= pData;		// 网格点要素值指针
	//--------------------------------
}

//
///////////////////////////////////////////////////////////////////////////
// 名    称: SetInput
// 功    能: 设置网格属性参数(重载v2v中的SetInput,作用同SetGridParam)
// 访    问: public
// 参    数:
//			[i] nx - x方向格点数
//			[i] ny - y方向格点数
//			[i] x0 - 网格点x向最小值
//			[i] y0 - 网格点y向最小值
//			[i] xInterval - x方向网格点间隔
//			[i] yInterval - y方向网格点间隔
//			[i] pData - 网格点要素值指针
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KG2VInterpolate::SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData)
{
	SetGridParam(nx,ny,x0,y0,xInterval,yInterval,pData);
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Prepare
// 功    能: 为插值处理作准备
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
//void KG2VInterpolate::Prepare()
//{
//	KV2VInterpolate::Prepare();
//
//	//准备内存
//	this->PrepareMemory();
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
void KG2VInterpolate::Transact(int iMethod)
{
	//-----------------------------
	//准备内存
	PrepareMemory();
	//-----------------------------
	KV2VInterpolate::SetInput(m_px,m_py,m_pv,m_nx*m_ny);

	KV2VInterpolate::Transact(iMethod);
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: PrepareMemory
// 功    能: 准备内存
// 访    问: protected
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KG2VInterpolate::PrepareMemory()
{
	//-----------------------------------------------
	//先释放为网格分配的坐标内存
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}
	//-----------------------------------------------
	m_px = new double[m_nx*m_ny];
	m_py = new double[m_nx*m_ny];

	for(int i=0;i<m_ny;i++)
	{
		for(int j=0;j<m_nx;j++)
		{
			*(m_px+i*m_nx+j) = m_x0 + j*m_xInterval;
			*(m_py+i*m_nx+j) = m_y0 + i*m_yInterval;
		}
	}
}
