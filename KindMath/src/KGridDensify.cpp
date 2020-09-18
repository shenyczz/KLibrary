#include "StdAfx.h"
#include "KGridDensify.h"

KGridDensify::KGridDensify(void)
{
	Init();
}

KGridDensify::~KGridDensify(void)
{
	Cleanup();
}

void KGridDensify::Init()
{
	m_nx0 = 0;					// �����X������(����)
	m_ny0 = 0;					// �����Y������(����)
	m_x0 = 0.0f;				// �����X����ʼֵ
	m_y0 = 0.0f;				// �����Y����ʼֵ
	m_xInterval0 = 0.0f;		// �����X����
	m_yInterval0 = 0.0f;		// �����Y����
	m_pv0 = NULL;				// �����Ҫ��ֵָ��(�ɵ����߷�����ͷ��ڴ�)

	m_nx1 = 0;					// �����X������(����)
	m_ny1 = 0;					// �����Y������(����)
	//m_x1 = 0.0f;				// �����X����ʼֵ(��ʼֵһ�� = m_x0)
	//m_y1 = 0.0f;				// �����Y����ʼֵ(��ʼֵһ�� = m_y0)
	m_xInterval1 = 0.0f;		// �����X����
	m_yInterval1 = 0.0f;		// �����Y����
	m_pv1 = NULL;				// �����Ҫ��ֵָ��(�ɱ��������ͷ��ڴ�)
}

void KGridDensify::Cleanup()
{
	if(m_pv1) {delete []m_pv1; m_pv1 = NULL;}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Empty
// ��    ��: �������Ա����
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::Empty()
{
	//˳�򲻿ɵߵ�
	Cleanup();	//step 1
	Init();		//step 2
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetInput
// ��    ��: ����Դ�������Բ���
// ��    ��: public
// ��    ��:
//			[i] nx - x��������
//			[i] ny - y��������
//			[i] x0 - �����x����Сֵ
//			[i] y0 - �����y����Сֵ
//			[i] xInterval - x�����������
//			[i] yInterval - y�����������
//			[i] pData - �����Ҫ��ֵָ��
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData)
{
	m_nx0 = nx;
	m_ny0 = ny;

	m_x0 = x0;
	m_y0 = y0;

	m_xInterval0 = xInterval;
	m_yInterval0 = yInterval;

	m_pv0 = pData;
}
void KGridDensify::SetInput(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval,double* pData)
{
	m_nx0 = (int)(double)((xmax-xmin) / xInterval + 1);
	m_ny0 = (int)(double)((ymax-ymin) / yInterval + 1);

	m_x0 = xmin;
	m_y0 = ymin;

	m_xInterval0 = xInterval;
	m_yInterval0 = yInterval;

	m_pv0 = pData;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetDensifyParam
// ��    ��: ���ü��ܲ���
// ��    ��: public
// ��    ��:
//			[i] xInterval - x�����������
//			[i] yInterval - y�����������
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::SetDensifyParam(double xInterval,double yInterval)
{
	m_xInterval1 = xInterval;
	m_yInterval1 = yInterval;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Prepare
// ��    ��: ׼��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::Prepare()
{
	double xmin,ymin,xmax,ymax;
	//-----------------------------
	//��������귶Χ
	xmin = m_x0;
	ymin = m_y0;
	xmax = m_x0 + m_xInterval0*(m_nx0-1);
	ymax = m_y0 + m_yInterval0*(m_ny0-1);
	//-----------------------------
	//���ܺ����������
	//������ܺ�ĸ����
	m_nx1 = (int)fabs((xmax-xmin)/m_xInterval1)+1;	//���ܺ�x��������
	m_ny1 = (int)fabs((ymax-ymin)/m_yInterval1)+1;	//���ܺ�y��������
	//-----------------------------
	PrepareMemory();
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Transact
// ��    ��: ����������
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::Transact()
{
	int i,j;
	//-----------------------------
	Prepare();
	//-----------------------------
	double xx,yy,vv(0.0);
	for(i=0;i<m_ny1;i++)
	{
		yy = m_y0 + m_yInterval1*i;		//y����
		for(j=0;j<m_nx1;j++)
		{
			xx = m_x0 + m_xInterval1*j;	//x����

			//��ԭ�������н������Բ�ֵ
			m_pv1[i*m_nx1+j]= InterpolateOnePoint(xx,yy);
		}
	}
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GetOutput
// ��    ��: ȡ�ü��ܺ����������
// ��    ��: public
// ��    ��:
//			[i] nx - x��������
//			[i] ny - y��������
//			[i] pData - �����Ҫ��ֵָ��
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::GetOutput(int* nx,int* ny,double* x0,double* y0,double* xInterval,double* yInterval,double*& pData)
{
	*nx = m_nx1;
	*ny = m_ny1;

	*x0 = m_x0;
	*y0 = m_y0;

	*xInterval = m_xInterval1;
	*yInterval = m_yInterval1;

	pData = m_pv1;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: PrepareMemory
// ��    ��: ׼���ڴ�
// ��    ��: protected
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridDensify::PrepareMemory()
{
	m_pv1 = new double[m_nx1*m_ny1];
}

//
///////////////////////////////////////////////////////////////////////////
// ��    ��: Interpolate
// ��    ��: ��Դ�����н��в�ֵ
// ��    ��: protected
// ��    ��:
//			[i] x - ���������x����
//			[i] y - ���������y����
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KGridDensify::InterpolateOnePoint(double x,double y)
{
	double dValue(0);
	//-----------------------------
	int i,j;	//��(x,y)��Դ�����е��кź��к�

	i = (int)((y-m_y0)/m_yInterval0);	//�к�
	j = (int)((x-m_x0)/m_xInterval0);	//�к�

	if(i<0 || j<0) throw;
	//-----------------------------
	//ȡ����������4������������Ҫ��ֵ
	int ii,jj;
	double xx[4], yy[4], vv[4];

	ii = i; jj = j;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//��������һ��
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//��������һ��
	xx[0] = m_x0+m_xInterval0*jj;
	yy[0] = m_y0+m_yInterval0*ii;
	vv[0] = m_pv0[ii*m_nx0+jj];

	ii = i; jj = j+1;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//��������һ��
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//��������һ��
	xx[1] = m_x0+m_xInterval0*jj;
	yy[1] = m_y0+m_yInterval0*ii;
	vv[1] = m_pv0[ii*m_nx0+jj];

	ii = i+1; jj = j+1;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//��������һ��
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//��������һ��
	xx[2] = m_x0+m_xInterval0*jj;
	yy[2] = m_y0+m_yInterval0*ii;
	vv[2] = m_pv0[ii*m_nx0+jj];

	ii = i+1; jj = j;
	ii = ii>m_ny0-1 ? m_ny0-1 : ii;	//��������һ��
	jj = jj>m_nx0-1 ? m_nx0-1 : jj;	//��������һ��
	xx[3] = m_x0+m_xInterval0*jj;
	yy[3] = m_y0+m_yInterval0*ii;
	vv[3] = m_pv0[ii*m_nx0+jj];
	//-----------------------------
	//�����ֵ
	double dd[4];	//��(x,y)�����������ĸ�����ľ���ĵ���
	double vvsum(0),ddsum(0);
	for(int k=0;k<4;k++)
	{
		double dTemp = _distance(x,y,xx[k],yy[k]);

		//�������Ϊ��
		if(fabs(dTemp) < 1.0e-12)
		{
			dTemp = (double)1.0e-6;
		}

		//dTemp *= 111.37f;

		dd[k] = 1.0f/dTemp;	//����ĵ���

		vvsum += vv[k]*dd[k];
		ddsum += dd[k];
	}
	//-----------------------------
	dValue = vvsum/ddsum;
	//-----------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: _distance
// ��    ��: ������������
// ��    ��: private
// ��    ��:
//			[i] x0 - ��P0��x����
//			[i] y0 - ��P0��y����
//			[i] x1 - ��P1��x����
//			[i] y1 - ��P1��y����
// ��    ��: ��������
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KGridDensify::_distance(double x0, double y0, double x1, double y1)
{
	double dValue = 0.0;
	//-----------------------------
	dValue = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
	dValue = sqrt(dValue);
	//-----------------------------
	return dValue;
}
