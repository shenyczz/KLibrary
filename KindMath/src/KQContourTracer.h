/******************************************************************************
 ** KQContourTracer.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KQContourTracer - 四边形网格数据的等值线追踪器
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

		//1.声明类对象
		KQContourTracer kqContourTracer;

		//2.设置网格数据信息
		kqContourTracer.SetDataInfo(TDataInfo* pDataInfo);

		//3.设置设置网格数据
		kqContourTracer.SetData(double* pData);

		//4.设置追踪值
		kqContourTracer.SetTraceValue(double dTraceValue);

		//5.设置输出数据指针
		kqContourTracer.SetOutput(KCurves* pCurves);

		//6.追踪等值线
		kqContourTracer.Tracing();	

******************************************************************************/

#pragma once

#include "KContourTracer.h"

namespace ContourBuilder
{

	class KIND_EXT_CLASS KQContourTracer : public KContourTracer
	{
	public:
		KQContourTracer(void);
		virtual ~KQContourTracer(void);

	public:
		// 追踪
		virtual BOOL Tracing();

	protected:
		// 分配内存
		void PrepareMemory();

		// 释放内存
		void FreeMemory();

		// 扫描网格纵横边,内插等值点
		BOOL InterpolateTracingValue();

		// 追踪开路曲线
		BOOL TracingOpenedContour();

		// 追踪闭合曲线
		BOOL TracingClosedContour();

	private:
		// 是否存在等值点
		BOOL IsExistPoint(double flag);

		// 追踪一条开路等值线
		void TracingOneOpenedContour();

		// 追踪一条闭合等值线
		void TracingOneClosedContour(int startPt_i, int startPt_j);

		// 计算出等值线头的坐标
		void CalcOnePointCoord(int i, int j, BOOL bHorizon,double &x, double &y);

		// 追踪出下一个点a3
		void TracingNextPoint(KCurve* pCurve);

		// 自下向上追踪
		void FromBottom2TopTracing();
		void FromBottom2TopTracingV2();

		// 自上向下追踪
		void FromTop2BottomTracing();
		void FromTop2BottomTracingV2();

		// 自左向右追踪
		void FromLeft2RightTracing();
		void FromLeft2RightTracingV2();

		// 自右向左追踪
		void FromRight2LeftTracing();
		void FromRight2LeftTracingV2();

		// 当下一个等值点找到后做相应的处理
		void HandlingAfterNextPointFounded(int i, int j, BOOL bHorizon);

	private:
		// 格点数量
		int m_nx, m_ny;
		// 网格起始坐标
		double m_x0, m_y0;
		// 网格间距
		double m_xInterval,m_yInterval;

		// m_xSide(i,j)---->标识网格横边上是否有当前等值点
		// m_ySide(i,j)---->标识网格纵边上是否有当前等值点
		// m_xSide(i,j),m_ySide(i,j)中存储的值为-2.0(说明该边上无等值点) 或者是 r(r∈[0,1]，说明该边上有等值点)
		double *m_xSide;
		double *m_ySide; 

		int m_xcols;
		int m_ycols;

		// 辅助点
		struct AssistPoint
		{
			int i;		//该等值点所在边的行号  
			int j;		//该等值点所在边的列号

			double x;	//x坐标
			double y;	//y坐标

			BOOL bHorV; //在横边还是列边上 1-->横边；0-->纵边

			AssistPoint(){	memset(this,0,sizeof(AssistPoint));}
		};
		//追踪过程中需要利用如下三个辅助等值点完成追踪： 
		//m_PreviousPoint点-->前一个等值点
		//m_CurrentPoint 点-->当前等值点
		//m_NextPoint 点-->要追踪的下一个等值点
		// 三个辅助等值点所在的横向、纵向网格边的序号,以及坐标
		AssistPoint m_PreviousPoint,m_CurrentPoint,m_NextPoint;
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
