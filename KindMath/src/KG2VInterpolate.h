/******************************************************************************
 ** Header File: KG2VInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KG2VInterpolate
 ** Version:
 ** Function: 根据规则四边形网格点数据插补到任意点上

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. 声明类对象
		KG2VInterpolate	g2v;

		//2. 设置源数据
		g2v.SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);
			或
		g2v.SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);

		//3. 设置目的数据
		g2v.SetOutput(double* px,double* py,double* pv,int n);

		//4. 设置扫描半径(km)
		//   如果扫描半径 =0,则自动计算扫描半径,,使扫描半径内至少有3个点
		//   如果扫描半径!=0,则按扫描半径计算,nVertexMin 参数无效
		g2v.SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

		//5. 实施插值
		g2v.Transact();

******************************************************************************/

#pragma once

#include "KV2VInterpolate.h"

class KIND_EXT_CLASS KG2VInterpolate : public KV2VInterpolate
{
public:
	KG2VInterpolate(void);
	virtual ~KG2VInterpolate(void);
//
//protected:
//	void Init();
//	void Cleanup();

public:
	//设置网格属性参数[i]
	void SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);

	//设置网格属性参数(重载v2v中的SetInput,作用同SetGridParam)
	virtual void SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);

	//为插值作准备
	//void Prepare();

	//实施插值
	void Transact(int iMethod=0);

protected:
	//准备内存
	void PrepareMemory();

protected:
	//---网格属性参数
	int			m_nx;				// 网格点X向格点数(列数)
	int			m_ny;				// 网格点Y向格点数(行数)

	double		m_x0;				// 网格点X向起始值
	double		m_y0;				// 网格点Y向起始值

	double		m_xInterval;		// 网格点X向间隔
	double		m_yInterval;		// 网格点Y向间隔

	double*		m_pv;				// 网格点要素值指针(由调用者分配和释放内存)

	//本类负责分配和释放内存
	double*		m_px;				// 网格点X向坐标(列坐标)
	double*		m_py;				// 网格点Y向坐标(行坐标)
};
