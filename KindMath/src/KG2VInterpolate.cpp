#include "StdAfx.h"
#include "KG2VInterpolate.h"

KG2VInterpolate::KG2VInterpolate(void)
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

KG2VInterpolate::~KG2VInterpolate(void)
{
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}

}
//
//void KG2VInterpolate::Init()
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
//
//	//KV2VInterpolate::Init();
//}
//
//void KG2VInterpolate::Cleanup()
//{
//	if(m_px)		{delete []m_px; m_px=NULL;}
//	if(m_py)		{delete []m_py; m_py=NULL;}
//
//	KV2VInterpolate::Cleanup();
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
//			[i] pData - �����Ҫ��ֵָ��
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KG2VInterpolate::SetGridParam(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData)
{
	if(!pData) throw;
	//--------------------------------
	m_nx		= nx;			// �����X������(����)
	m_ny		= ny;			// �����Y������(����)

	m_x0		= x0;			// �����X����Сֵ	
	m_y0		= y0;			// �����Y����Сֵ

	m_xInterval	= xInterval;	// �����X����
	m_yInterval	= yInterval;	// �����Y����

	m_pv		= pData;		// �����Ҫ��ֵָ��
	//--------------------------------
}

//
///////////////////////////////////////////////////////////////////////////
// ��    ��: SetInput
// ��    ��: �����������Բ���(����v2v�е�SetInput,����ͬSetGridParam)
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
void KG2VInterpolate::SetInput(int nx,int ny,double x0,double y0,double xInterval,double yInterval,double* pData)
{
	SetGridParam(nx,ny,x0,y0,xInterval,yInterval,pData);
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Prepare
// ��    ��: Ϊ��ֵ������׼��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
//void KG2VInterpolate::Prepare()
//{
//	KV2VInterpolate::Prepare();
//
//	//׼���ڴ�
//	this->PrepareMemory();
//}


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
void KG2VInterpolate::Transact(int iMethod)
{
	//-----------------------------
	//׼���ڴ�
	PrepareMemory();
	//-----------------------------
	KV2VInterpolate::SetInput(m_px,m_py,m_pv,m_nx*m_ny);

	KV2VInterpolate::Transact(iMethod);
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: PrepareMemory
// ��    ��: ׼���ڴ�
// ��    ��: protected
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KG2VInterpolate::PrepareMemory()
{
	//-----------------------------------------------
	//���ͷ�Ϊ�������������ڴ�
	if(m_px)		{delete []m_px; m_px=NULL;}
	if(m_py)		{delete []m_py; m_py=NULL;}
	//-----------------------------------------------
	m_px = new double[m_nx*m_ny];
	m_py = new double[m_nx*m_ny];

	for(int i=0;i<m_ny;i++)
	{
		for(int j=0;j<m_nx;j++)
		{
			*(m_px+i*m_nx+j) = m_x0 + j*m_xInterval;
			*(m_py+i*m_nx+j) = m_y0 + i*m_yInterval;
		}
	}
}
