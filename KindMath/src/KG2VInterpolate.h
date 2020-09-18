/******************************************************************************
 ** Header File: KG2VInterpolate.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KG2VInterpolate
 ** Version:
 ** Function: ���ݹ����ı�����������ݲ岹���������

 ** Author: ShenYongchen(shenyczz@163.com) [������]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

		//1. ���������
		KG2VInterpolate	g2v;

		//2. ����Դ����
		g2v.SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);
			��
		g2v.SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);

		//3. ����Ŀ������
		g2v.SetOutput(double* px,double* py,double* pv,int n);

		//4. ����ɨ��뾶(km)
		//   ���ɨ��뾶 =0,���Զ�����ɨ��뾶,,ʹɨ��뾶��������3����
		//   ���ɨ��뾶!=0,��ɨ��뾶����,nVertexMin ������Ч
		g2v.SetRadius(double dRadius=0.0,int nVertexMin=VERTEX_NUM_MIN);

		//5. ʵʩ��ֵ
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
	//�����������Բ���[i]
	void SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);

	//�����������Բ���(����v2v�е�SetInput,����ͬSetGridParam)
	virtual void SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData);

	//Ϊ��ֵ��׼��
	//void Prepare();

	//ʵʩ��ֵ
	void Transact(int iMethod=0);

protected:
	//׼���ڴ�
	void PrepareMemory();

protected:
	//---�������Բ���
	int			m_nx;				// �����X������(����)
	int			m_ny;				// �����Y������(����)

	double		m_x0;				// �����X����ʼֵ
	double		m_y0;				// �����Y����ʼֵ

	double		m_xInterval;		// �����X����
	double		m_yInterval;		// �����Y����

	double*		m_pv;				// �����Ҫ��ֵָ��(�ɵ����߷�����ͷ��ڴ�)

	//���ฺ�������ͷ��ڴ�
	double*		m_px;				// �����X������(������)
	double*		m_py;				// �����Y������(������)
};
