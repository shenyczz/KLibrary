/******************************************************************************
 ** Header File: KV2GInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KV2GInterpolate
 ** Version:
 ** Function: ������������ݲ岹�������ı����������

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. ���������
		KV2GInterpolate	v2g;

		//2. ����Դ����
		v2g.SetInput(double* pxi,double* pyi,double* pvi,int ni);

		//3. �����������Բ���
		v2g.SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval);
		//	��
		v2g.SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval);

		//4. ����ɨ��뾶(km)
		//   ���ɨ��뾶 =0,���Զ�����ɨ��뾶,,ʹɨ��뾶��������3����
		//   ���ɨ��뾶!=0,��ɨ��뾶����,nVertexMin ������Ч
		v2g.SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

		//5. ʵʩ��ֵ
		v2g.Transact(iMethod);

		//ȡ�ý��
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
	//�����������Բ���
	void SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval);
	void SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval);

	//Ԥ����
	//void Prepare();

	//��ֵ����
	// iMethod: ��ֵ������־
	//       0: Gressman��ֵ
	//       1: Gauss��ֵ
	void Transact(int iMethod=0);

	//ȡ�ý��
	double* GetOutput(int* nx,int* ny);

protected:
	//׼���ڴ�
	void PrepareMemory();

protected:
	//---�������Բ���
	int			m_nx;				// �����X������(����)
	int			m_ny;				// �����Y������(����)

	double		m_x0;				// �����X����Сֵ
	double		m_y0;				// �����Y����Сֵ

	double		m_xInterval;		// �����X����
	double		m_yInterval;		// �����Y����

	//���ฺ�������ͷ��ڴ�
	double*		m_px;				// �����X������(������)
	double*		m_py;				// �����Y������(������)
	double*		m_pv;				// �����Ҫ��ֵ
};
