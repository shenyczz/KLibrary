/******************************************************************************
 ** Header File: KV2GInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KV2GInterpolate
 ** Version:
 ** Function: 根据任意点数据插补到规则四边形网格点上

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. 声明类对象
		KV2GInterpolate	v2g;

		//2. 设置源数据
		v2g.SetInput(double* pxi,double* pyi,double* pvi,int ni);

		//3. 设置网格属性参数
		v2g.SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval);
		//	或
		v2g.SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval);

		//4. 设置扫描半径(km)
		//   如果扫描半径 =0,则自动计算扫描半径,,使扫描半径内至少有3个点
		//   如果扫描半径!=0,则按扫描半径计算,nVertexMin 参数无效
		v2g.SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

		//5. 实施插值
		v2g.Transact(iMethod);

		//取得结果
		v2g.GetOutput(int* nx,int* ny,double*& pData);

******************************************************************************/

#pragma once

#include "KV2VInterpolate.h"

class KIND_EXT_CLASS KV2GInterpolate  : public KV2VInterpolate
{
public:
	KV2GInterpolate(void);
	virtual ~KV2GInterpolate(void);

protected:
	//void Init();
	//void Cleanup();

public:
	//设置网格属性参数
	void SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval);
	void SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval);

	//预处理
	//void Prepare();

	//插值处理
	// iMethod: 插值方法标志
	//       0: Gressman插值
	//       1: Gauss插值
	void Transact(int iMethod=0);

	//取得结果
	double* GetOutput(int* nx,int* ny);

protected:
	//准备内存
	void PrepareMemory();

protected:
	//---网格属性参数
	int			m_nx;				// 网格点X向格点数(列数)
	int			m_ny;				// 网格点Y向格点数(行数)

	double		m_x0;				// 网格点X向最小值
	double		m_y0;				// 网格点Y向最小值

	double		m_xInterval;		// 网格点X向间隔
	double		m_yInterval;		// 网格点Y向间隔

	//本类负责分配和释放内存
	double*		m_px;				// 网格点X向坐标(列坐标)
	double*		m_py;				// 网格点Y向坐标(行坐标)
	double*		m_pv;				// 网格点要素值
};
