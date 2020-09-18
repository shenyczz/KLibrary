/******************************************************************************
 ** KQContourTracer.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KQContourTracer - �ı����������ݵĵ�ֵ��׷����
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

		//1.���������
		KQContourTracer kqContourTracer;

		//2.��������������Ϣ
		kqContourTracer.SetDataInfo(TDataInfo* pDataInfo);

		//3.����������������
		kqContourTracer.SetData(double* pData);

		//4.����׷��ֵ
		kqContourTracer.SetTraceValue(double dTraceValue);

		//5.�����������ָ��
		kqContourTracer.SetOutput(KCurves* pCurves);

		//6.׷�ٵ�ֵ��
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
		// ׷��
		virtual BOOL Tracing();

	protected:
		// �����ڴ�
		void PrepareMemory();

		// �ͷ��ڴ�
		void FreeMemory();

		// ɨ�������ݺ��,�ڲ��ֵ��
		BOOL InterpolateTracingValue();

		// ׷�ٿ�·����
		BOOL TracingOpenedContour();

		// ׷�ٱպ�����
		BOOL TracingClosedContour();

	private:
		// �Ƿ���ڵ�ֵ��
		BOOL IsExistPoint(double flag);

		// ׷��һ����·��ֵ��
		void TracingOneOpenedContour();

		// ׷��һ���պϵ�ֵ��
		void TracingOneClosedContour(int startPt_i, int startPt_j);

		// �������ֵ��ͷ������
		void CalcOnePointCoord(int i, int j, BOOL bHorizon,double &x, double &y);

		// ׷�ٳ���һ����a3
		void TracingNextPoint(KCurve* pCurve);

		// ��������׷��
		void FromBottom2TopTracing();
		void FromBottom2TopTracingV2();

		// ��������׷��
		void FromTop2BottomTracing();
		void FromTop2BottomTracingV2();

		// ��������׷��
		void FromLeft2RightTracing();
		void FromLeft2RightTracingV2();

		// ��������׷��
		void FromRight2LeftTracing();
		void FromRight2LeftTracingV2();

		// ����һ����ֵ���ҵ�������Ӧ�Ĵ���
		void HandlingAfterNextPointFounded(int i, int j, BOOL bHorizon);

	private:
		// �������
		int m_nx, m_ny;
		// ������ʼ����
		double m_x0, m_y0;
		// ������
		double m_xInterval,m_yInterval;

		// m_xSide(i,j)---->��ʶ���������Ƿ��е�ǰ��ֵ��
		// m_ySide(i,j)---->��ʶ�����ݱ����Ƿ��е�ǰ��ֵ��
		// m_xSide(i,j),m_ySide(i,j)�д洢��ֵΪ-2.0(˵���ñ����޵�ֵ��) ������ r(r��[0,1]��˵���ñ����е�ֵ��)
		double *m_xSide;
		double *m_ySide; 

		int m_xcols;
		int m_ycols;

		// ������
		struct AssistPoint
		{
			int i;		//�õ�ֵ�����ڱߵ��к�  
			int j;		//�õ�ֵ�����ڱߵ��к�

			double x;	//x����
			double y;	//y����

			BOOL bHorV; //�ں�߻����б��� 1-->��ߣ�0-->�ݱ�

			AssistPoint(){	memset(this,0,sizeof(AssistPoint));}
		};
		//׷�ٹ�������Ҫ������������������ֵ�����׷�٣� 
		//m_PreviousPoint��-->ǰһ����ֵ��
		//m_CurrentPoint ��-->��ǰ��ֵ��
		//m_NextPoint ��-->Ҫ׷�ٵ���һ����ֵ��
		// ����������ֵ�����ڵĺ�����������ߵ����,�Լ�����
		AssistPoint m_PreviousPoint,m_CurrentPoint,m_NextPoint;
	};

//---------------------------------------------------------
}// namespace ContourBuilder - end
//---------------------------------------------------------
