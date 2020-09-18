/******************************************************************************
 ** Header File: KBarnes.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KBarnes - 低通滤波类
 ** Version:
 ** Function: 低通滤波类
              针对GrADS绘图软件的二进制网格数据文件,也可以用于其它格点数据

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		1. KBarnes barnes;			//声明对象
		2. SetParam_Filter();		//设置滤波参数
		3. SetParam_Source();		//设置源数据属性参数
		5. SetData();				//传入源数据指针
		4. Prepare();				//准备
		6. Transact();				//滤波处理

		7. GetBarnes(0/1);			//返回滤波初始场/滤波场

******************************************************************************/

#pragma once

class KIND_EXT_CLASS KBarnes 
{
public:
	KBarnes (void);
	virtual ~KBarnes (void);

protected:
	void Init();
	void Cleanup();

public:
	//设置参数
	void SetParam_Filter(float g, float c);
	void SetParam_Source(int nx, int ny, float x0, float y0, float xInterval, float yInterval);

	//设置数据
	void SetData(float* pData,int nx,int ny)	{SetData(&pData,nx,ny);}
	void SetData(float** ppData,int nx,int ny)	{m_ppData = ppData;m_nCols=nx;m_nRows=ny;}

	//为滤波处理作准备
	void Prepare();

	//滤波处理
	void Transact();

	//返回滤波初始场/滤波场(0/1)
	void GetBarnes(float** ppRetData,int* nRow,int* nCol,int flag);
	void GetBarnes(float* pRetData,int* nRow,int* nCol,int flag);

protected:
	//为滤波处理作准备内存
	void PrepareMemory();	//初始化

	//计算某点的滤波值
	float Calculate(float x0, float y0,float g,float c,float** ppData);

private:
	//计算两点间距离
	float _distance(float x0,float y0,float x1,float y1);

	//计算高斯权重
	float _getGauss(float rk,float g,float c);

protected:
	//---初始化参数
	int			m_nRows;		// 源Y方向格点数 (行)
	int			m_nCols;		// 源X方向格点数 (列)

	float		m_x0;			// 源网格区域左下角经度
	float		m_y0;			// 源网格区域左下角纬度

	float		m_xInterval;	// 源经度网格间距
	float		m_yInterval;	// 源纬度网格间距

	float		m_g,m_c;		// 滤波参数

	//---本类分配内存并负责释放之
	float*		m_pX;
	float*		m_pY;

	float**		m_ppD;			// 误差
	float**		m_ppF0;			// 第一次滤波结果
	float**		m_ppF1;			// 第二次滤波结果

	//---需要外部设置
	float**		m_ppData;		// 原始场网格数据指针(m_ny,m_nx)
};
