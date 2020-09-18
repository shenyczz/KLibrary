#include "StdAfx.h"
#include "KBarnes.h"

typedef T2DMemAllocator<float> KFloat2DMemAllocator;

KBarnes::KBarnes(void)
{
	Init();
}

KBarnes::~KBarnes(void)
{
	Cleanup();
}

void KBarnes::Init()
{
	m_pX	= NULL;
	m_pY	= NULL;

	m_ppD	= NULL;
	m_ppF0	= NULL;
	m_ppF1	= NULL;
}

void KBarnes::Cleanup()
{
	if(m_pX)	delete [] m_pX;
	if(m_pY)	delete [] m_pY;

	if(m_ppD)	KFloat2DMemAllocator::FreeMemory2D(m_ppD);
	if(m_ppF0)	KFloat2DMemAllocator::FreeMemory2D(m_ppF0);
	if(m_ppF1)	KFloat2DMemAllocator::FreeMemory2D(m_ppF1);
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetParam_Filter
// ��    ��: �����˲�����
// ��    ��: public
// ��    ��:
//			[i] g - �˲�����g
//			[i] c - �˲�����c
// ��    ��: 
// ע    ��: 
//			��һ���˲�������� g0,c0
//			�ڶ����˲�������� g1,c1
///////////////////////////////////////////////////////////////////////////
void KBarnes::SetParam_Filter(float g, float c)
{
	m_g	= g;	m_c	= c;
}

//
///////////////////////////////////////////////////////////////////////////
// ��    ��: SetParam_Source
// ��    ��: ����Դ����
// ��    ��: public
// ��    ��:
//			[i] nx - X��������
//			[i] ny - Y��������
//			[i] x0 - �����������½Ǿ���
//			[i] y0 - �����������½�γ��
//			[i] xInterval - ����������
//			[i] yInterval - γ��������
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::SetParam_Source(int nx, int ny, float x0, float y0, float xInterval, float yInterval)
{
	m_nCols		= nx;			//��
	m_nRows		= ny;			//��

	m_x0		= x0;			//�����������½Ǿ���
	m_y0		= y0;			//�����������½�γ��

	m_xInterval	= xInterval;	//����������
	m_yInterval	= yInterval;	//γ��������
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Prepare
// ��    ��: Ϊ�˲�������׼��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::Prepare()
{
	PrepareMemory();
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Transact
// ��    ��: �˲�����
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::Transact()
{
	//---
	int i,j;
	float x,y;

	//---��һ���˲�,�����ֵ��
	for(i=0;i<m_nRows;i++)
	{
		y = m_pY[i];
		for(j=0;j<m_nCols;j++)
		{
			x = m_pX[j];
			m_ppF0[i][j]		= (float)Calculate(x,y,1.0,m_c,m_ppData);	//f0
			m_ppD[i][j]			= m_ppData[i][j] - m_ppF0[i][j];			//���
		}
	}

	//---��һ���˲�����
	for(i=0;i<m_nRows;i++)
	{
		y = m_pY[i];
		for(j=0;j<m_nCols;j++)
		{
			x = m_pX[j];
			m_ppF1[i][j]		= (float)Calculate(x,y,m_g,m_c,m_ppD);	//��ֵ���
			m_ppF1[i][j]		= m_ppF0[i][j] + m_ppF1[i][j];			//f1
		}
	}

	//---
	return;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GetBarnes
// ��    ��: �����˲���ʼ��/�˲���(index=0/1)
// ��    ��: public
// ��    ��:
//			[o] ppRetData - ����ָ��
//			[o] nRow - ������
//			[o] nCol - ������
//			[i] flag - �������ݱ��: 0-��һ���˲��� 1-�ڶ����˲���
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::GetBarnes(float** ppRetData,int* nRow,int* nCol,int flag)
{
	float** pp=NULL;
	//---
	if(flag==0) pp = m_ppF0;
	if(flag==1) pp = m_ppF1;
	//---
	ppRetData	= pp;
	*nRow	= m_nRows;
	*nCol	= m_nCols;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GetBarnes
// ��    ��: �����˲���ʼ��/�˲���(index=0/1)
// ��    ��: public
// ��    ��:
//			[o] pRetData - ����ָ��
//			[o] nRow - ������
//			[o] nCol - ������
//			[i] flag - �������ݱ��: 0-��һ���˲��� 1-�ڶ����˲���
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::GetBarnes(float* pRetData,int* nRow,int* nCol,int flag)
{
	float* p=NULL;
	//---
	if(flag==0) p = *m_ppF0;
	if(flag==1) p = *m_ppF1;
	//---
	pRetData	= p;
	*nRow	= m_nRows;
	*nCol	= m_nCols;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: PrepareMemory
// ��    ��: Ϊ�˲�����׼���ڴ�
// ��    ��: protect
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KBarnes::PrepareMemory()
{
	int i,j;
	//-----------------------------
	//������Ҫʹ�õ��ڴ�ָ��
	if(m_pX)	{delete [] m_pX; m_pX = NULL;}
	if(m_pY)	{delete [] m_pY; m_pY = NULL;}

	if(m_ppD)	KFloat2DMemAllocator::FreeMemory2D(m_ppD);
	if(m_ppF0)	KFloat2DMemAllocator::FreeMemory2D(m_ppF0);
	if(m_ppF1)	KFloat2DMemAllocator::FreeMemory2D(m_ppF1);
	//-----------------------------
	m_pY = new float [m_nRows];	//Y������
	m_pX = new float [m_nCols];	//X������
	for(i=0;i<m_nRows;i++)
		m_pY[i] = m_y0 + i*m_yInterval;
	for(j=0;j<m_nCols;j++)
		m_pX[j] = m_x0 + j*m_xInterval;
	//-----------------------------
	//�˲������ڴ�
	KFloat2DMemAllocator::AllocMemory2D(m_ppD,m_nRows,m_nCols);
	KFloat2DMemAllocator::AllocMemory2D(m_ppF0,m_nRows,m_nCols);
	KFloat2DMemAllocator::AllocMemory2D(m_ppF1,m_nRows,m_nCols);

	for(i=0;i<m_nRows;i++)
	{
		for(j=0;j<m_nCols;j++)
		{
			m_ppD[i][j]		= 0.0;
			m_ppF0[i][j]	= 0.0;
			m_ppF1[i][j]	= 0.0;
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Calculate
// ��    ��: ����ĳ����˲�ֵ
// ��    ��: protected
// ��    ��:
//			[i] x0 - �����������½Ǿ���
//			[i] y0 - �����������½�γ��
//			[i] g - �˲����� g
//			[i] c - �˲����� c
//			[i] ppData - �˲�����(��ά)
// ��    ��: ĳ����˲�ֵ
// ע    ��: ���ø�˹�˲�����
///////////////////////////////////////////////////////////////////////////
float KBarnes::Calculate(float x0, float y0, float g,float c,float** ppData)
{
	int i,j;

	float rk,fk;
	float w;
	float sumw=0.0, sumwfk=0.0;

	float x,y;
	//-----------------------------
	for(i=0;i<m_nRows;i++)
	{
		y = m_pY[i];				//��
		for(j=0;j<m_nCols;j++)
		{
			x = m_pX[j];			//��

			fk=ppData[i][j];		//ԭʼ������

			rk=_distance(x0,y0,x,y);	//�������
			rk*=111.0;				//һ����γ��Ϊ111.0km

			w =  _getGauss(rk,g,c);	//����Ȩ��ϵ��

			sumw	+= w;			//Ȩ��ϵ����
			sumwfk	+= w*fk;		//ԭʼ������Ȩ��ֵ�ĺ�
		}
	}
	//-----------------------------
	return (sumwfk / sumw);
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: _distance
// ��    ��: ������������
// ��    ��: privatte
// ��    ��:
//			[i] x0 - ��P0��x����
//			[i] y0 - ��P0��y����
//			[i] x1 - ��P1��x����
//			[i] y1 - ��P1��y����
// ��    ��: ��������
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
float KBarnes::_distance(float x0, float y0, float x1, float y1)
{
	float dValue = 0.0;
	//-----------------------------
	dValue = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
	dValue = sqrt(dValue);
	//-----------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: _getGauss
// ��    ��: �����˹Ȩ��ϵ��
// ��    ��: private
// ��    ��:
//			[i] rk - Ȩ�ص�ͼ����ľ���
//			[i] g - �˲����� g
//			[i] c - �˲����� c
// ��    ��: ��˹Ȩ��ϵ��
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
float KBarnes::_getGauss(float rk,float g,float c)
{
	return (float)exp(-rk*rk/(4.0*g*c));
}
