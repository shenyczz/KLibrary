/******************************************************************************
 ** KContourTracer.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KContourTracer - ��ֵ��׷����
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: ������.֣�� (shenyczz@163.com)
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
		// ׷��(���������ж���)
		virtual BOOL Tracing() { throw; return FALSE; }

	public:
		// ����/��ȡ����
		void SetData(void* pData) { m_pData = pData; }
		void* GetData() { return m_pData; }

		// ����/��ȡ������Ϣ
		void SetDataInfo(void* pDataInfo) { m_pDataInfo = pDataInfo; }
		void* GetDataInfo() { return m_pDataInfo; }

		// ����/��ȡ׷��ֵ
		void SetTraceValue(double dTraceValue) { m_dTraceValue = dTraceValue; }
		double GetTraceValue() { return m_dTraceValue; }

		// ����������ݵĴ��λ��--׷�ٳ����ĵ�ֵ����������(�����ⲿ��ִ��׷��ǰ�趨)
		void SetContour(KContour* pContour) { m_pContour = pContour; }
		KContour* GetAt() { return m_pContour; }

		// ����һ����ֵ��
		void AddCurve(KCurve* pCurve);

		// ��λ���
		void ResetTracerFlag() { m_dwTracerFlag = GetCurrentTime(); }

	protected:
		// ׷��ֵ
		double m_dTraceValue;

		// ����ָ��(�������ڴ����)
		void* m_pData;
		// ������Ϣ(�������ڴ����)
		void* m_pDataInfo;
		// ֵͬ��ֵ����(�������ڴ����)
		KContour* m_pContour;

	protected:
		// ׷�ٱ��(Ϊ׷�ٵ���ÿ�����������)
		DWORD m_dwTracerFlag;
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
