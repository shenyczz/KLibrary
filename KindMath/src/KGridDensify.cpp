#include "StdAfx.h"
#include "KGridDensify.h"

KGridDensify::KGridDensify(void)
{
	Init();
}

KGridDensify::~KGridDensify(void)
{
	Cleanup();
}

void KGridDensify::Init()
{
	m_nx0 = 0;					// 网格点X向格点数(列数)
	m_ny0 = 0;					// 网格点Y向格点数(行数)
	m_x0 = 0.0f;				// 网格点X向起始值
	m_y0 = 0.0f;				// 网格点Y向起始值
	m_xInterval0 = 0.0f;		// 网格点X向间隔
	m_yInterval0 = 0.0f;		// 网格点Y向间隔
	m_pv0 = NULL;				// 网格点要素值指针(由调用者分配和释放内存)

	m_nx1 = 0;					// 网格点X向格点数(列数)
	m_ny1 = 0;					// 网格点Y向格点数(行数)
	//m_x1 = 0.0f;				// 网格点X向起始值(起始值一样 = m_x0)
	//m_y1 = 0.0f;				// 网格点Y向起始值(起始值一样 = m_y0)
	m_xInterval1 = 0.0f;		// 网格点X向间隔
	m_yInterval1 = 0.0f;		// 网格点Y向间隔
	m_pv1 = NULL;				// 网格点要素值指针(由本类分配和释放内存)
}

void KGridDensify::Cleanup()
{
	if(m_pv1) {delete []m_pv1; m_pv1 = NULL;}
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Empty
// 功    能: 清理类成员数据
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::Empty()
{
	//顺序不可颠倒
	Cleanup();	//step 1
	Init();		//step 2
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetInput
// 功    能: 设置源网格属性参数
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
void KGridDensify::SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData)
{
	m_nx0 = nx;
	m_ny0 = ny;

	m_x0 = x0;
	m_y0 = y0;

	m_xInterval0 = xInterval;
	m_yInterval0 = yInterval;

	m_pv0 = pData;
}
void KGridDensify::SetInput(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval,double* pData)
{
	m_nx0 = (int)(double)((xmax-xmin) / xInterval + 1);
	m_ny0 = (int)(double)((ymax-ymin) / yInterval + 1);

	m_x0 = xmin;
	m_y0 = ymin;

	m_xInterval0 = xInterval;
	m_yInterval0 = yInterval;

	m_pv0 = pData;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetDensifyParam
// 功    能: 设置加密参数
// 访    问: public
// 参    数:
//			[i] xInterval - x方向网格点间隔
//			[i] yInterval - y方向网格点间隔
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::SetDensifyParam(double xInterval,double yInterval)
{
	m_xInterval1 = xInterval;
	m_yInterval1 = yInterval;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Prepare
// 功    能: 准备
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::Prepare()
{
	double xmin,ymin,xmax,ymax;
	//-----------------------------
	//网格的坐标范围
	xmin = m_x0;
	ymin = m_y0;
	xmax = m_x0 + m_xInterval0*(m_nx0-1);
	ymax = m_y0 + m_yInterval0*(m_ny0-1);
	//-----------------------------
	//加密后的网格属性
	//计算加密后的格点数
	m_nx1 = (int)fabs((xmax-xmin)/m_xInterval1)+1;	//加密后x方向格点数
	m_ny1 = (int)fabs((ymax-ymin)/m_yInterval1)+1;	//加密后y方向格点数
	//-----------------------------
	PrepareMemory();
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Transact
// 功    能: 加密网格处理
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::Transact()
{
	int i,j;
	//-----------------------------
	Prepare();
	//-----------------------------
	double xx,yy,vv(0.0);
	for(i=0;i<m_ny1;i++)
	{
		yy = m_y0 + m_yInterval1*i;		//y坐标
		for(j=0;j<m_nx1;j++)
		{
			xx = m_x0 + m_xInterval1*j;	//x坐标

			//在原来网格中进行线性插值
			m_pv1[i*m_nx1+j]= InterpolateOnePoint(xx,yy);
		}
	}
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GetOutput
// 功    能: 取得加密后的网格数据
// 访    问: public
// 参    数:
//			[i] nx - x方向格点数
//			[i] ny - y方向格点数
//			[i] pData - 网格点要素值指针
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::GetOutput(int* nx,int* ny,double* x0,double* y0,double* xInterval,double* yInterval,double*& pData)
{
	*nx = m_nx1;
	*ny = m_ny1;

	*x0 = m_x0;
	*y0 = m_y0;

	*xInterval = m_xInterval1;
	*yInterval = m_yInterval1;

	pData = m_pv1;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: PrepareMemory
// 功    能: 准备内存
// 访    问: protected
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::PrepareMemory()
{
	m_pv1 = new double[m_nx1*m_ny1];
}

//
///////////////////////////////////////////////////////////////////////////
// 名    称: Interpolate
// 功    能: 在源网格中进行插值
// 访    问: protected
// 参    数:
//			[i] x - 加密网格的x坐标
//			[i] y - 加密网格的y坐标
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
double KGridDensify::InterpolateOnePoint(double x,double y)
{
	double dValue(0);
	//-----------------------------
	int i,j;	//点(x,y)在源网格中的行号和列号

	i = (int)((y-m_y0)/m_yInterval0);	//行号
	j = (int)((x-m_x0)/m_xInterval0);	//列号

	if(i<0 || j<0) throw;
	//-----------------------------
	//取得所在网格4个顶点的坐标和要素值
	int ii,jj;
	double xx[4], yy[4], vv[4];

	ii = i; jj = j;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//如果是最后一行
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//如果是最后一列
	xx[0] = m_x0+m_xInterval0*jj;
	yy[0] = m_y0+m_yInterval0*ii;
	vv[0] = m_pv0[ii*m_nx0+jj];

	ii = i; jj = j+1;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//如果是最后一行
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//如果是最后一列
	xx[1] = m_x0+m_xInterval0*jj;
	yy[1] = m_y0+m_yInterval0*ii;
	vv[1] = m_pv0[ii*m_nx0+jj];

	ii = i+1; jj = j+1;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//如果是最后一行
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//如果是最后一列
	xx[2] = m_x0+m_xInterval0*jj;
	yy[2] = m_y0+m_yInterval0*ii;
	vv[2] = m_pv0[ii*m_nx0+jj];

	ii = i+1; jj = j;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//如果是最后一行
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//如果是最后一列
	xx[3] = m_x0+m_xInterval0*jj;
	yy[3] = m_y0+m_yInterval0*ii;
	vv[3] = m_pv0[ii*m_nx0+jj];
	//-----------------------------
	//距离插值
	double dd[4];	//点(x,y)到所在网格四个顶点的距离的倒数
	double vvsum(0),ddsum(0);
	for(int k=0;k<4;k++)
	{
		double dTemp = _distance(x,y,xx[k],yy[k]);

		//如果距离为零
		if(fabs(dTemp) < 1.0e-12)
		{
			dTemp = (double)1.0e-6;
		}

		//dTemp *= 111.37f;

		dd[k] = 1.0f/dTemp;	//距离的倒数

		vvsum += vv[k]*dd[k];
		ddsum += dd[k];
	}
	//-----------------------------
	dValue = vvsum/ddsum;
	//-----------------------------
	return dValue;
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
double KGridDensify::_distance(double x0, double y0, double x1, double y1)
{
	double dValue = 0.0;
	//-----------------------------
	dValue = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
	dValue = sqrt(dValue);
	//-----------------------------
	return dValue;
}
