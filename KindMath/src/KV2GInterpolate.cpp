#include "StdAfx.h"
#include "KV2GInterpolate.h"

KV2GInterpolate::KV2GInterpolate(void)
{
	m_nx			= 0;
	m_ny			= 0;
	m_x0			= 0.0f;
	m_y0			= 0.0f;
	m_xInterval		= 0.0f;
	m_yInterval		= 0.0f;

	m_px			= NULL;
	m_py			= NULL;
	m_pv			= NULL;
}

KV2GInterpolate::~KV2GInterpolate(void)
{
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}
	if(m_pv)		{delete []m_pv; m_pv=NULL;}
}
//
//void KV2GInterpolate::Init()
//{
//	m_nx			= 0;
//	m_ny			= 0;
//	m_x0			= 0.0f;
//	m_y0			= 0.0f;
//	m_xInterval		= 0.0f;
//	m_yInterval		= 0.0f;
//
//	m_px			= NULL;
//	m_py			= NULL;
//	m_pv			= NULL;
//}
//
//void KV2GInterpolate::Cleanup()
//{
//	//���ͷ�Ϊ�������������ڴ�
//	if(m_px)		{delete []m_px; m_px=NULL;}
//	if(m_py)		{delete []m_py; m_py=NULL;}
//	if(m_pv)		{delete []m_pv; m_pv=NULL;}
//}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetGridParam
// ��    ��: �����������Բ���
// ��    ��: public
// ��    ��:
//			[i] nx - x��������
//			[i] ny - y��������
//			[i] x0 - �����x����Сֵ
//			[i] y0 - �����y����Сֵ
//			[i] xInterval - x�����������
//			[i] yInterval - y�����������
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KV2GInterpolate::SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval)
{
	m_nx		= nx;			// �����X������(����)
	m_ny		= ny;			// �����Y������(����)

	m_x0		= x0;			// �����X����Сֵ	
	m_y0		= y0;			// �����Y����Сֵ

	m_xInterval	= xInterval;	// �����X����
	m_yInterval	= yInterval;	// �����Y����
}
void KV2GInterpolate::SetGridParam(double xmin,double ymin,double xmax,double ymax,double xInterval,double yInterval)
{
	m_nx = (int)(double)((xmax - xmin)/xInterval + 1);
	m_ny = (int)(double)((ymax - ymin) / yInterval) + 1;

	m_x0		= xmin;			// �����X����Сֵ	
	m_y0		= ymin;			// �����Y����Сֵ

	m_xInterval	= xInterval;	// �����X����
	m_yInterval	= yInterval;	// �����Y����
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Prepare
// ��    ��: Ϊ��ֵ������׼��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
//void KV2GInterpolate::Prepare()
//{
//	KV2VInterpolate::Prepare();
//
//	//׼���ڴ�
//	PrepareMemory();
//}
//
///////////////////////////////////////////////////////////////////////////
// ��    ��: Transact
// ��    ��: ��ֵ����
// ��    ��: public
// ��    ��:
//			[i] iMethod - ��ֵ������־,Ĭ��=0
//				0-Gressman��ֵ
//				1-Gauss��ֵ
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KV2GInterpolate::Transact(int iMethod)
{
	//------------------------------------------
	//׼���ڴ�
	PrepareMemory();
	//------------------------------------------
	//���ø��๦��
	KV2VInterpolate::SetOutput(m_px,m_py,m_pv,m_nx*m_ny);

	KV2VInterpolate::Transact(iMethod);
	//------------------------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: PrepareMemory
// ��    ��: ׼���ڴ�
// ��    ��: protected
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KV2GInterpolate::PrepareMemory()
{
	//-----------------------------------------------
	//���ͷ�Ϊ�������������ڴ�
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}
	if(m_pv)		{delete []m_pv; m_pv=NULL;}
	//-----------------------------------------------
	m_px = new double[m_nx*m_ny];
	m_py = new double[m_nx*m_ny];
	m_pv = new double[m_nx*m_ny];

	for(int i=0;i<m_ny;i++)
	{
		for(int j=0;j<m_nx;j++)
		{
			*(m_px+i*m_nx+j) = m_x0 + j*m_xInterval;
			*(m_py+i*m_nx+j) = m_y0 + i*m_yInterval;
			*(m_pv+i*m_nx+j) = 0.0f;
		}
	}
	//-----------------------------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GetOutput
// ��    ��: ȡ�ò�ֵ�����������
// ��    ��: public
// ��    ��:
//			[i] nx - x��������
//			[i] ny - y��������
//			[i] pData - �����Ҫ��ֵָ��
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
//void KGridDensify::GetOutput(int* nx,int* ny,double* x0,double* y0,double* xInterval,double* yInterval,double*& pData)
double* KV2GInterpolate::GetOutput(int* nx,int* ny)
{
	*nx = m_nx;
	*ny = m_ny;

	return m_pv;
}
