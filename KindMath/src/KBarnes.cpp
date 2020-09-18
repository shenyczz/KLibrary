#include "StdAfx.h"
#include "KBarnes.h"

typedef T2DMemAllocator<float> KFloat2DMemAllocator;

KBarnes::KBarnes(void)
{
	Init();
}

KBarnes::~KBarnes(void)
{
	Cleanup();
}

void KBarnes::Init()
{
	m_pX	= NULL;
	m_pY	= NULL;

	m_ppD	= NULL;
	m_ppF0	= NULL;
	m_ppF1	= NULL;
}

void KBarnes::Cleanup()
{
	if(m_pX)	delete [] m_pX;
	if(m_pY)	delete [] m_pY;

	if(m_ppD)	KFloat2DMemAllocator::FreeMemory2D(m_ppD);
	if(m_ppF0)	KFloat2DMemAllocator::FreeMemory2D(m_ppF0);
	if(m_ppF1)	KFloat2DMemAllocator::FreeMemory2D(m_ppF1);
}

///////////////////////////////////////////////////////////////////////////
// 名    称: SetParam_Filter
// 功    能: 设置滤波参数
// 访    问: public
// 参    数:
//			[i] g - 滤波参数g
//			[i] c - 滤波参数c
// 返    回: 
// 注    释: 
//			第一次滤波传入参数 g0,c0
//			第二次滤波传入参数 g1,c1
///////////////////////////////////////////////////////////////////////////
void KBarnes::SetParam_Filter(float g, float c)
{
	m_g	= g;	m_c	= c;
}

//
///////////////////////////////////////////////////////////////////////////
// 名    称: SetParam_Source
// 功    能: 设置源参数
// 访    问: public
// 参    数:
//			[i] nx - X方向格点数
//			[i] ny - Y方向格点数
//			[i] x0 - 网格区域左下角经度
//			[i] y0 - 网格区域左下角纬度
//			[i] xInterval - 经度网格间距
//			[i] yInterval - 纬度网格间距
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::SetParam_Source(int nx, int ny, float x0, float y0, float xInterval, float yInterval)
{
	m_nCols		= nx;			//列
	m_nRows		= ny;			//行

	m_x0		= x0;			//网格区域左下角经度
	m_y0		= y0;			//网格区域左下角纬度

	m_xInterval	= xInterval;	//经度网格间距
	m_yInterval	= yInterval;	//纬度网格间距
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Prepare
// 功    能: 为滤波处理作准备
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::Prepare()
{
	PrepareMemory();
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Transact
// 功    能: 滤波处理
// 访    问: public
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::Transact()
{
	//---
	int i,j;
	float x,y;

	//---第一次滤波,计算初值场
	for(i=0;i<m_nRows;i++)
	{
		y = m_pY[i];
		for(j=0;j<m_nCols;j++)
		{
			x = m_pX[j];
			m_ppF0[i][j]		= (float)Calculate(x,y,1.0,m_c,m_ppData);	//f0
			m_ppD[i][j]			= m_ppData[i][j] - m_ppF0[i][j];			//误差
		}
	}

	//---第一次滤波修正
	for(i=0;i<m_nRows;i++)
	{
		y = m_pY[i];
		for(j=0;j<m_nCols;j++)
		{
			x = m_pX[j];
			m_ppF1[i][j]		= (float)Calculate(x,y,m_g,m_c,m_ppD);	//插值误差
			m_ppF1[i][j]		= m_ppF0[i][j] + m_ppF1[i][j];			//f1
		}
	}

	//---
	return;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GetBarnes
// 功    能: 返回滤波初始场/滤波场(index=0/1)
// 访    问: public
// 参    数:
//			[o] ppRetData - 数据指针
//			[o] nRow - 数据行
//			[o] nCol - 数据列
//			[i] flag - 返回数据标记: 0-第一次滤波场 1-第二次滤波场
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::GetBarnes(float** ppRetData,int* nRow,int* nCol,int flag)
{
	float** pp=NULL;
	//---
	if(flag==0) pp = m_ppF0;
	if(flag==1) pp = m_ppF1;
	//---
	ppRetData	= pp;
	*nRow	= m_nRows;
	*nCol	= m_nCols;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: GetBarnes
// 功    能: 返回滤波初始场/滤波场(index=0/1)
// 访    问: public
// 参    数:
//			[o] pRetData - 数据指针
//			[o] nRow - 数据行
//			[o] nCol - 数据列
//			[i] flag - 返回数据标记: 0-第一次滤波场 1-第二次滤波场
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::GetBarnes(float* pRetData,int* nRow,int* nCol,int flag)
{
	float* p=NULL;
	//---
	if(flag==0) p = *m_ppF0;
	if(flag==1) p = *m_ppF1;
	//---
	pRetData	= p;
	*nRow	= m_nRows;
	*nCol	= m_nCols;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: PrepareMemory
// 功    能: 为滤波处理准备内存
// 访    问: protect
// 参    数:
// 返    回: 
// 注    释: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::PrepareMemory()
{
	int i,j;
	//-----------------------------
	//先清理要使用的内存指针
	if(m_pX)	{delete [] m_pX; m_pX = NULL;}
	if(m_pY)	{delete [] m_pY; m_pY = NULL;}

	if(m_ppD)	KFloat2DMemAllocator::FreeMemory2D(m_ppD);
	if(m_ppF0)	KFloat2DMemAllocator::FreeMemory2D(m_ppF0);
	if(m_ppF1)	KFloat2DMemAllocator::FreeMemory2D(m_ppF1);
	//-----------------------------
	m_pY = new float [m_nRows];	//Y向坐标
	m_pX = new float [m_nCols];	//X向坐标
	for(i=0;i<m_nRows;i++)
		m_pY[i] = m_y0 + i*m_yInterval;
	for(j=0;j<m_nCols;j++)
		m_pX[j] = m_x0 + j*m_xInterval;
	//-----------------------------
	//滤波数据内存
	KFloat2DMemAllocator::AllocMemory2D(m_ppD,m_nRows,m_nCols);
	KFloat2DMemAllocator::AllocMemory2D(m_ppF0,m_nRows,m_nCols);
	KFloat2DMemAllocator::AllocMemory2D(m_ppF1,m_nRows,m_nCols);

	for(i=0;i<m_nRows;i++)
	{
		for(j=0;j<m_nCols;j++)
		{
			m_ppD[i][j]		= 0.0;
			m_ppF0[i][j]	= 0.0;
			m_ppF1[i][j]	= 0.0;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// 名    称: Calculate
// 功    能: 计算某点的滤波值
// 访    问: protected
// 参    数:
//			[i] x0 - 网格区域左下角经度
//			[i] y0 - 网格区域左下角纬度
//			[i] g - 滤波参数 g
//			[i] c - 滤波参数 c
//			[i] ppData - 滤波数据(二维)
// 返    回: 某点的滤波值
// 注    释: 利用高斯滤波方法
///////////////////////////////////////////////////////////////////////////
float KBarnes::Calculate(float x0, float y0, float g,float c,float** ppData)
{
	int i,j;

	float rk,fk;
	float w;
	float sumw=0.0, sumwfk=0.0;

	float x,y;
	//-----------------------------
	for(i=0;i<m_nRows;i++)
	{
		y = m_pY[i];				//行
		for(j=0;j<m_nCols;j++)
		{
			x = m_pX[j];			//列

			fk=ppData[i][j];		//原始场数据

			rk=_distance(x0,y0,x,y);	//计算距离
			rk*=111.0;				//一个经纬度为111.0km

			w =  _getGauss(rk,g,c);	//计算权重系数

			sumw	+= w;			//权重系数和
			sumwfk	+= w*fk;		//原始场数据权重值的和
		}
	}
	//-----------------------------
	return (sumwfk / sumw);
}

///////////////////////////////////////////////////////////////////////////
// 名    称: _distance
// 功    能: 计算两点间距离
// 访    问: privatte
// 参    数:
//			[i] x0 - 点P0的x坐标
//			[i] y0 - 点P0的y坐标
//			[i] x1 - 点P1的x坐标
//			[i] y1 - 点P1的y坐标
// 返    回: 两点间距离
// 注    释: 
///////////////////////////////////////////////////////////////////////////
float KBarnes::_distance(float x0, float y0, float x1, float y1)
{
	float dValue = 0.0;
	//-----------------------------
	dValue = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
	dValue = sqrt(dValue);
	//-----------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// 名    称: _getGauss
// 功    能: 计算高斯权重系数
// 访    问: private
// 参    数:
//			[i] rk - 权重点和计算点的距离
//			[i] g - 滤波参数 g
//			[i] c - 滤波参数 c
// 返    回: 高斯权重系数
// 注    释: 
///////////////////////////////////////////////////////////////////////////
float KBarnes::_getGauss(float rk,float g,float c)
{
	return (float)exp(-rk*rk/(4.0*g*c));
}
