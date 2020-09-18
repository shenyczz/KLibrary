#include "StdAfx.h"
#include "KGridExtract.h"

KGridExtract::KGridExtract(void)
{
	Init();
}

KGridExtract::~KGridExtract(void)
{
	Cleanup();
}

void KGridExtract::Init()
{
	//---Դ�������Բ���
	m_nx0 = 0;				// �����X������(����)
	m_ny0 = 0;				// �����Y������(����)

	m_x0 = 0;				// �����X����ʼֵ
	m_y0 = 0;				// �����Y����ʼֵ

	m_xInterval = 0.0f;		// �����X����
	m_yInterval = 0.0f;		// �����Y����

	m_pv0 = NULL;			// �����Ҫ��ֵָ��(�ɵ����߷�����ͷ��ڴ�)

	//---���������Բ���
	m_nxStart = 0;			//x�����ȡ�Ŀ�ʼ������
	m_nyStart = 0;			//y�����ȡ�Ŀ�ʼ������

	m_nxCount = 0;			//x�����ȡ�ĸ����(����)
	m_nyCount = 0;			//y�����ȡ�ĸ����(����)

	m_xStart = 0.0f;		// �����X����ʼֵ
	m_yStart = 0.0f;		// �����Y����ʼֵ

	m_pv1 = NULL;			// �����Ҫ��ֵָ��(�ɱ��������ͷ��ڴ�)
}

void KGridExtract::Cleanup()
{
	if(m_pv1) {delete []m_pv1; m_pv1=NULL;}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Empty
// ��    ��: �������Ա����
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::Empty()
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
//			[i] nx - ����Դ����x��������
//			[i] ny - ����Դ����y��������
//			[i] x0 - �����x����Сֵ
//			[i] y0 - �����y����Сֵ
//			[i] xInterval - x�����������.�������Դ���������ͬ�ļ��
//			[i] yInterval - y�����������.�������Դ���������ͬ�ļ��
//			[i] pData - �����Ҫ��ֵָ��.(�е����⸺���ڴ�ķ�����ͷ�)
//                                       
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::SetInput(int nx,int ny,double x0,double y0,const double xInterval,const double yInterval,double* pData)
{
	if(!pData) throw;
	if( !(xInterval>0) ) throw;	// x����������>0
	//-----------------------------
	int i;
	//-----------------------------
	m_nx0 = nx;
	m_ny0 = ny;
	m_x0 = x0;
	m_y0 = y0;
	m_xInterval = xInterval;
	m_yInterval = yInterval;
	m_pv0 = pData;
	//-----------------------------
	if(m_yInterval<0)
	{
		//���y����С��0,Ҫ��������
		m_y0 = m_y0 + m_yInterval*(ny-1);
		m_yInterval = -m_yInterval;

		double* pDataTemp = new double[nx*ny];
		//-------------------------
		double* pEndLine = pData + nx*ny-nx;
		for(i=0;i<ny;i++)
		{
			double* pCurrentLine = pEndLine - i*nx;
			memcpy(pDataTemp+i*nx,pCurrentLine,sizeof(double)*nx);
		}

		memcpy(pData,pDataTemp,sizeof(double)*nx*ny);

		delete[] pDataTemp;
		pDataTemp = NULL;
	}
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetExtractParam
// ��    ��: ���ó�ȡ����
// ��    ��: public
// ��    ��:
//			[i] nxStart - x�����ȡ����ʼ������
//			[i] nxCount - x�����ȡ�ĸ����
//			[i] nyStart - y�����ȡ����ʼ������
//			[i] nyCount - y�����ȡ�ĸ����
//                                       
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::SetExtractParam(int nxStart,int nxCount,int nyStart,int nyCount)
{
	////��ʼ�к�Ҫ>=0,��ȡ����Ҫ>0
	if( !(nyStart>=0) ) throw;
	if( !(nyCount>0) ) throw;

	//��ʼ�к�Ҫ>=0,��ȡ����Ҫ>0
	if( !(nxStart>=0) ) throw;
	if( !(nxCount>0) ) throw;

	//��ʼ�к�Ҫ>=0,��ȡ����Ҫ>0
	if( !(nyStart+nyCount<=m_ny0) ) throw;	//��ȡ������к�Ҫ<=Ԫ���������
	if( !(nxStart+nxCount<=m_nx0) ) throw;	//��ȡ������к�Ҫ<=Ԫ���������
	//-----------------------------
	m_nxStart = nxStart;			//x�����ȡ����ʼ������
	m_nxCount = nxCount;			//x�����ȡ�ĸ����
	m_nyStart = nyStart;			//y�����ȡ����ʼ������
	m_nyCount = nyCount;			//y�����ȡ�ĸ����
	//-----------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetExtractParam
// ��    ��: ���ó�ȡ����
// ��    ��: public
// ��    ��:
//			[i] xmin - �������ڵ���С��Χ�е�x��С����
//			[i] ymin - �������ڵ���С��Χ�е�y��С����
//			[i] xmax - �������ڵ���С��Χ�е�x�������
//			[i] ymax - �������ڵ���С��Χ�е�y�������
//                                       
// ��    ��: 
// ע    ��: ���ó�ȡ�������ڵ���С��Χ��
//			 Ҫ����С��Χ�з�Χת����x,y�����ϳ�ȡ��
//			 ��ʼ�����źͳ�ȡ�ĸ����
///////////////////////////////////////////////////////////////////////////
void KGridExtract::SetExtractParam(double xmin,double ymin,double xmax,double ymax)
{
	int i,j;

	int nxStart,nxCount;	//x�����ȡ����ʼ�����ź�x�����ȡ�ĸ����
	int nyStart,nyCount;	//y�����ȡ����ʼ�����ź�y�����ȡ�ĸ����

	BOOL bFind_nxStart=FALSE;	//�Ƿ��ҵ�x�����ȡ����ʼ������
	BOOL bFind_nyStart=FALSE;	//�Ƿ��ҵ�y�����ȡ����ʼ������

	double x, y;
	//-----------------------------
	//����x������ʼʼ������
	i=0;
	while(1)
	{
		x = m_x0 + m_xInterval*i;
		//����x������ʼʼ������
		if(x>=xmin && !bFind_nxStart)
		{
			nxStart = i;
			bFind_nxStart = TRUE;
		}
		i++;
		x = m_x0 + m_xInterval*i;
		if(bFind_nxStart && x>=xmax)
		{
			if(x>xmax) i--;

			nxCount = i-nxStart+1;
			break;
		}

		if((x>m_x0+m_xInterval*(m_nx0-1)))	//����Դ����Χ
		{
			nxCount = 0;
			break;
		}
	}
	//-----------------------------
	//����y������ʼʼ������
	j=0;
	while(1)
	{
		y = m_y0 + m_yInterval*j;
		//����x������ʼʼ������
		if(y>=ymin && !bFind_nyStart)
		{
			nyStart = j;
			bFind_nyStart = TRUE;
		}
		j++;
		y = m_y0 + m_yInterval*j;
		if(bFind_nyStart && y>=ymax)
		{
			if(y>ymax) j--;

			nyCount = j-nyStart+1;
			break;
		}

		if(y>m_y0+m_yInterval*(m_ny0-1))	//����Դ����Χ
		{
			nyCount = 0;
			break;
		}
	}
	//-----------------------------
	SetExtractParam(nxStart,nxCount,nyStart,nyCount);
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Prepare
// ��    ��: ׼��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
BOOL KGridExtract::Prepare()
{
	if(!VerifyInputData()) return FALSE;
	//-----------------------------
	//������������ʼx,y����
	m_xStart = m_x0+m_xInterval*m_nxStart;
	m_yStart = m_y0+m_yInterval*m_nyStart;
	//-----------------------------
	//׼���ڴ�
	PrepareMemory();
	//-----------------------------
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Transact
// ��    ��: ʵʩ
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
BOOL KGridExtract::Transact()
{
	int i,j;
	int ii,jj;
	//-----------------------------
	if(!Prepare()) return FALSE;
	//-----------------------------
	for(i=0;i<m_nyCount;i++)
	{
		ii = m_nyStart+i;		//ii��ʾ��Դ�����е��к�
		for(j=0;j<m_nxCount;j++)
		{
			jj = m_nxStart+j;	//jj��ʾ��Դ�����е��к�
			*(m_pv1+i*m_nxCount+j) = *(m_pv0+ii*m_nx0+jj);
		}
	}
	//-----------------------------
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GetOutput
// ��    ��: ȡ������������
// ��    ��: public
// ��    ��:
//			[i] nx - x��������
//			[i] ny - y��������
//			[i] xStart - �����x����ʼֵ
//			[i] xStart - �����y����ʼֵ
//			[i] xInterval - x�����������
//			[i] yInterval - y�����������
//			[i] pData - �����Ҫ��ֵָ��
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::GetOutput(int* nx,int* ny,double* xStart,double* yStart,double* xInterval,double* yInterval,double*& pData)
{
	*nx = m_nxCount;
	*ny = m_nyCount;
	*xStart = m_xStart;
	*yStart = m_yStart;
	*xInterval = m_xInterval;
	*yInterval = m_yInterval;
	pData = m_pv1;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: VerifyInputData
// ��    ��: У����������
// ��    ��: protected
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
BOOL KGridExtract::VerifyInputData()
{
	//-----------------------------
	if(m_nyStart<0) 
	{
		::MessageBox(NULL, _T("��ʼ�к�Ҫ���ڵ���0"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if(m_nxStart<0) 
	{
		::MessageBox(NULL, _T("��ʼ�к�Ҫ���ڵ���0"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	
	if(m_nyCount<1)
	{
		::MessageBox(NULL, _T("��ȡ����������Ҫ���ڵ���1"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if(m_nxCount<1)
	{
		::MessageBox(NULL, _T("��ȡ����������Ҫ���ڵ���1"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}

	if(m_nyStart+m_nyCount>m_ny0)
	{
		::MessageBox(NULL, _T("��ȡ������кų���Դ����Χ!"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	if(m_nxStart+m_nxCount>m_nx0)
	{
		::MessageBox(NULL, _T("��ȡ������кų���Դ����Χ!"), _T("��Ϣ"), MB_OK | MB_ICONWARNING);
		return FALSE;
	}
	//-----------------------------
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: PrepareMemory
// ��    ��: ׼���ڴ�
// ��    ��: protected
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KGridExtract::PrepareMemory()
{
	if(m_pv1) {delete []m_pv1; m_pv1=NULL;}

	m_pv1 = new double[m_nxCount*m_nyCount];	//��ȡ�������Ҫ��ָ��
}
