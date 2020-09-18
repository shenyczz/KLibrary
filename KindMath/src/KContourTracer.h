/******************************************************************************
 ** KContourTracer.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KContourTracer - 等值线追踪器
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

              // 1
              use

******************************************************************************/
#pragma once

namespace ContourBuilder
{

	class KIND_EXT_CLASS KContourTracer
	{
	public:
		KContourTracer(void);
		virtual ~KContourTracer(void);

	public:
		// 追踪(在派生类中定义)
		virtual BOOL Tracing() { throw; return FALSE; }

	public:
		// 设置/获取数据
		void SetData(void* pData) { m_pData = pData; }
		void* GetData() { return m_pData; }

		// 设置/获取数据信息
		void SetDataInfo(void* pDataInfo) { m_pDataInfo = pDataInfo; }
		void* GetDataInfo() { return m_pDataInfo; }

		// 设置/获取追踪值
		void SetTraceValue(double dTraceValue) { m_dTraceValue = dTraceValue; }
		double GetTraceValue() { return m_dTraceValue; }

		// 设置输出数据的存放位置--追踪出来的等值线链的坐标(需由外部在执行追踪前设定)
		void SetContour(KContour* pContour) { m_pContour = pContour; }
		KContour* GetAt() { return m_pContour; }

		// 增加一条等值线
		void AddCurve(KCurve* pCurve);

		// 复位标记
		void ResetTracerFlag() { m_dwTracerFlag = GetCurrentTime(); }

	protected:
		// 追踪值
		double m_dTraceValue;

		// 数据指针(不负责内存管理)
		void* m_pData;
		// 数据信息(不负责内存管理)
		void* m_pDataInfo;
		// 同值等值线组(不负责内存管理)
		KContour* m_pContour;

	protected:
		// 追踪标记(为追踪到的每条曲线做标记)
		DWORD m_dwTracerFlag;
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
