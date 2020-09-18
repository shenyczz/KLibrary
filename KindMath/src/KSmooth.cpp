#include "StdAfx.h"
#include "KSmooth.h"

//#define PI 3.1415926

KSmooth::KSmooth(void)
{
	Init();
}

KSmooth::~KSmooth(void)
{
	Cleanup();
}

void KSmooth::Init()
{
	m_px		= NULL;
	m_py		= NULL;
	m_iNum		= 0;
}

void KSmooth::Cleanup()
{
	if(m_px)	{delete[] m_px; m_px=NULL;}
	if(m_py)	{delete[] m_py; m_py=NULL;}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Akima
// ��    ��: ��㷨(Akima��)ƽ���㷨
// ��    ��: public
// ��    ��:
//			[i,o] x - ƽ�����ݵ��x��������ָ��
//			[i,o] y - ƽ�����ݵ��y��������ָ��
//			[i,o] n - ƽ�����ݵ�����
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KSmooth::GetOutput(double*& x,double*& y,int* n)
{
	x	= m_px;
	y	= m_py;
	*n	= m_iNum;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Akima
// ��    ��: ��㷨(Akima��)ƽ���㷨
// ��    ��: public
// ��    ��:
//			[i] x - Ҫƽ�����ݵ��x��������ָ��
//			[i] y - Ҫƽ�����ݵ��y��������ָ��
//			[i] n - Ҫƽ�����ݵ�����
//
// ��    ��: 
//			�ɹ�����1
//			ʧ�ܷ���0
// ע    ��: ���д���??
///////////////////////////////////////////////////////////////////////////
int KSmooth::Akima(double* x,double* y,int n)
{
	int i,j;
	//------------------------------------------
	//�Ƿ�պ�����
	bool bClose = fabs(x[0]-x[n-1])<1.0e-12 && fabs(y[0]-y[n-1])<1.0e-12;
	//------------------------------------------
	int iPoint = 4;	//ÿ�β�ֵ��ĵ���
	//------------------------------------------
	//ÿ�β������6����,�ܵ���=(n-1)*iPoint-n+2
	PrepareMemory((n-1)*iPoint-n+2);	//Ϊ���׼���ڴ�
	//------------------------------------------
	int N=n+4;	//����ǰ�������˵�Ҫ�岹������
	double* xx = new double[N];
	double* yy = new double[N];
	for(i=0;i<n;i++)
	{
		xx[i+2]=x[i];
		yy[i+2]=y[i];
	}
	//�岹ǰ��4����
	if(bClose)	//������
	{
		//�����ߣ�xx��ǰ�����Ӧx��n-2 n-3
		xx[0] = x[n-2];
		xx[1] = x[n-3];
		yy[0] = y[n-2];
		yy[1] = y[n-3];
		//�����ߣ�xx�ĺ������Ӧx��1 2
		xx[N-2] = x[1];
		xx[N-1] = x[2];
		yy[N-2] = y[1];
		yy[N-1] = y[2];
	}
	else	//������
	{
		//������,�������湫ʽ�岹,��֤��β�˵�������,ʹ��
		//���������������ߵ��׻�β3��λ��ͬһ��������
		//x[i+4]-x[i+2] = x[i+3]-x[i+1] = x[i+2]-x[i]
		//k[i+3]-k[i+2] = k[i+2]-k[i+1] = k[i+1]-k[i]

		//1.�׶˲岹
		//xx:  0  1  2  3  4  ��xx[1] xx[0]
		// x: -2 -1  0  1  2 
		xx[1] = xx[3]-xx[4]+xx[2];
		xx[0] = xx[2]-xx[3]+xx[1];

		double kk[6];
		kk[3] = atan(angle(xx[3],yy[3],xx[4],yy[4]));
		kk[2] = atan(angle(xx[2],yy[2],xx[3],yy[3]));
		kk[1] = 2*kk[2]-kk[3];

		yy[1] = yy[2]-(2*kk[2]-kk[3])*(xx[2]-xx[1]);
		yy[0] = yy[1]-(2*kk[1]-kk[2])*(xx[1]-xx[0]);
		//2.β�˲岹
		//xx: N-5 N-4  N-3  N-2  N-1  ��xx[1] xx[0]
		// x: n-3 n-2  n-1  n    n+1 
		xx[N-2] = xx[N-4]-xx[N-5]+xx[N-3];
		xx[N-1] = xx[N-3]-xx[N-4]+xx[N-2];

		kk[5] = atan(angle(xx[N-5],yy[N-5],xx[N-4],yy[N-4]));
		kk[4] = atan(angle(xx[N-4],yy[N-4],xx[N-3],yy[N-3]));
		kk[3] = 2*kk[4]-kk[5];
		kk[2] = 2*kk[3]-kk[4];

		yy[N-2] = yy[N-3]+kk[3]*(xx[N-2]-xx[N-3]);
		yy[N-1] = yy[N-2]+kk[2]*(xx[N-1]-xx[N-2]);

	}
	//��xx[0--N]�����б�� k[i]=(y[i+1]-y[i])/(x[i+1]-x[i])
	double* k = new double [N];
	for(i=0;i<N;i++)
	{
		k[i] = tan(angle(xx[i],yy[i],xx[i+1],yy[i+1]));
	}
	//��x[0--n]�����tht
	double* tht = new double[n];
	for(i=2;i<N-2;i++)
	{
		//k[i] = tan(angle(xx[i],yy[i],xx[i+1],yy[i+1]));
		double pr = fabs(k[i-2]-k[i-1]);
		double pl = fabs(k[i]-k[i+1]);
		if(fabs(pr+pl)<1.0e-12)
		{
			tht[i-2] = (double)((pr*k[i]+pl*k[i])>0 ? 3.1415926/2 : -3.1415926/2);	//������б�Ƕ�
		}
		else
		{
			tht[i-2] = (double)(atan(pr*k[i]+pl*k[i])/(pr+pl));	//������б�Ƕ�
		}
	}
	//------------------------------------------
	//------------------------------------------
	double xin[2],yin[2];
	double alf[2];
	double* xout = new double[iPoint];
	double* yout = new double[iPoint];
	//------------------------------------------
	for(i=0;i<n-1;i++)
	{
		xin[0]	= x[i];		xin[1]	= x[i+1];
		yin[0]	= y[i];		yin[1]	= y[i+1];
		alf[0]	= tht[i];	alf[1]	= tht[i+1];

		_DoSubsection(xin,yin,alf,xout,yout,iPoint);

		for(j=0;j<iPoint-1;j++)	//ÿ�ε�ĩβ�㶪���������¶ε����
		{
			if(j<1)
			{
				if(i==0)
				{
					m_px[i*(iPoint-1)+j] = xout[0];
					m_py[i*(iPoint-1)+j] = yout[0];
				}
				else
				{
					m_px[i*(iPoint-1)+j] = xout[1];
					m_py[i*(iPoint-1)+j] = yout[1];
				}
			}
			else
			{
				m_px[i*(iPoint-1)+j] = xout[j];
				m_py[i*(iPoint-1)+j] = yout[j];
			}
		}

		if(i==n-2)
		{
			m_px[(n-1)*iPoint-n+1] = xout[iPoint-1];
			m_py[(n-1)*iPoint-n+1] = yout[iPoint-1];
		}
	}
	//------------------------------------------
	delete []xx;
	delete []yy;

	delete []k;

	delete []tht;

	delete []xout;
	delete []yout;
	//------------------------------------------
	return 1;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SubsectionPolynomial
// ��    ��: �ֶ����ζ���ʽƽ���㷨( ?? Ϊʲô�ڽڵ㴦���⻬)
// ��    ��: public
// ��    ��:
//			[i] x - Ҫƽ�����ݵ��x��������ָ��
//			[i] y - Ҫƽ�����ݵ��y��������ָ��
//			[i] n - Ҫƽ�����ݵ�����
//
// ��    ��: 
//			�ɹ�����1
//			ʧ�ܷ���0
// ע    ��: ���д���??
///////////////////////////////////////////////////////////////////////////
int KSmooth::SubsectionPolynomial(double *x, double *y, int n)
{
	int i,j;
	//------------------------------------------
	bool bClose = fabs(x[0]-x[n-1])<1.0e-12 && fabs(y[0]-y[n-1])<1.0e-12;
	//------------------------------------------
	int iPoint = 6;	//ÿ�β�ֵ��ĵ���
	//------------------------------------------
	//ÿ�β������10����,�ܵ���=(n-1)*iPoint-n+2
	PrepareMemory((n-1)*iPoint-n+2);	//Ϊ���׼���ڴ�
	//------------------------------------------
	int N=n+2;	//����ǰ�������˵�Ҫ�岹һ����
	double* xx = new double[N];
	double* yy = new double[N];
	for(i=0;i<n;i++)
	{
		xx[i+1]=x[i];
		yy[i+1]=y[i];
	}
	//�岹ǰ��2����
	if(bClose)	//������
	{
		//�����ߣ�xx��ǰ�����Ӧx��n-2
		xx[0] = x[n-2];
		yy[0] = y[n-2];
		//�����ߣ�xx�ĺ������Ӧx��1
		xx[N-1] = x[1];
		yy[N-1] = y[1];
	}
	else	//������
	{
		//������,�������湫ʽ�岹,��֤��β�˵�������,ʹ��
		//���������������ߵ��׻�β3��λ��ͬһ��������
		//x[i+4]-x[i+2] = x[i+3]-x[i+1] = x[i+2]-x[i]
		//k[i+3]-k[i+2] = k[i+2]-k[i+1] = k[i+1]-k[i]

		double kk[5];
		//1.�׶˲岹
		//xx:  0  1  2  3  4  ��xx[1] xx[0]
		// x: -2 -1  0  1  2 
		xx[0] = xx[2]-xx[3]+xx[1];

		kk[3] = atan(angle(xx[3],yy[3],xx[4],yy[4]));
		kk[2] = atan(angle(xx[2],yy[2],xx[3],yy[3]));
		kk[1] = 2*kk[2]-kk[3];
		kk[0] = 2*kk[1]-kk[2];

		yy[0] = yy[1]-kk[0]*(xx[1]-xx[0]);

		//2.β�˲岹
		//xx: N-5 N-4  N-3  N-2  N-1  ��xx[1] xx[0]
		// x: n-3 n-2  n-1  n    n+1 
		xx[N-1] = xx[N-3]-xx[N-4]+xx[N-2];

		kk[4] = atan(angle(xx[N-4],yy[N-4],xx[N-3],yy[N-3]));
		kk[3] = atan(angle(xx[N-3],yy[N-3],xx[N-2],yy[N-2]));
		kk[2] = 2*kk[3]-kk[4];

		yy[N-1] = yy[N-2]+kk[2]*(xx[N-1]-xx[N-2]);
	}
	//------------------------------------------
	double* tht = new double[n];	//����ÿ�����б�ʶ�Ӧ�ĽǶ�
	for(i=1;i<N-1;i++)
	{
		tht[i-1] = angle(xx[i-1],yy[i-1],xx[i+1],yy[i+1]);
	}
	//------------------------------------------
	//------------------------------------------
	double xin[2],yin[2];
	double alf[2];
	double* xout = new double[iPoint];
	double* yout = new double[iPoint];
	//------------------------------------------
	for(i=0;i<n-1;i++)
	{
		xin[0]	= x[i];		xin[1]	= x[i+1];
		yin[0]	= y[i];		yin[1]	= y[i+1];
		alf[0]	= tht[i];	alf[1]	= tht[i+1];

		_DoSubsection(xin,yin,alf,xout,yout,iPoint);

		for(j=0;j<iPoint-1;j++)	//ÿ�ε�ĩβ�㶪���������¶ε����
		{
			if(j<1)
			{
				if(i==0)
				{
					m_px[i*(iPoint-1)+j] = xout[0];
					m_py[i*(iPoint-1)+j] = yout[0];
				}
				else
				{
					m_px[i*(iPoint-1)+j] = xout[1];
					m_py[i*(iPoint-1)+j] = yout[1];
				}
			}
			else
			{
				m_px[i*(iPoint-1)+j] = xout[j];
				m_py[i*(iPoint-1)+j] = yout[j];
			}
		}

		if(i==n-2)
		{
			m_px[(n-1)*iPoint-n+1] = xout[iPoint-1];
			m_py[(n-1)*iPoint-n+1] = yout[iPoint-1];
		}
	}
	//------------------------------------------
	delete []tht;

	delete []xout;
	delete []yout;
	//------------------------------------------
	return 1;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: PrepareMemory
// ��    ��: ƽ����������׼���ڴ�
// ��    ��: public
// ��    ��:
//			[i] iSize - ƽ���������г���,Ĭ��=0
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KSmooth::PrepareMemory(int iSize)
{
	if(m_px)	{delete[] m_px; m_px=NULL;}
	if(m_py)	{delete[] m_py; m_py=NULL;}

	if(iSize<=0) return;

	m_iNum	= iSize;
	m_px	= new double[m_iNum];
	m_py	= new double[m_iNum];
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: angle
// ��    ��: ���������㹹���ߵĽǶ�
// ��    ��: private
// ��    ��:
//			[i] x1 - ��1��x����
//			[i] y1 - ��1��y����
//			[i] x2 - ��2��x����
//			[i] y2 - ��2��y����
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KSmooth::angle(double x1, double y1, double x2, double y2)
{
	double alf=0.0;

	if(x2==x1)
	{
		if(y2>y1)	alf=3.1415926/2;
		if(y2<y1)	alf=-3.1415926/2;
	}
	else
	{
		alf=atan( (y2-y1)/(x2-x1) );
	}

	return alf;	//�Ƕ�
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: distance
// ��    ��: ��������֮�����
// ��    ��: private
// ��    ��:
//			[i] x1 - ��1��x����
//			[i] y1 - ��1��y����
//			[i] x2 - ��2��x����
//			[i] y2 - ��2��y����
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KSmooth::distance(double x1, double y1, double x2, double y2)
{
	double dx = x2-x1;
	double dy = y2-y1;
	return sqrt(dx*dx + dy*dy);
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: _DoSubsection
// ��    ��: �ֶβ�ֵ
// ��    ��: private
// ��    ��:
//			[i] x - 2ά����,��ʾ�߶����˵�x����
//			[i] y - 2ά����,��ʾ�߶����˵�y����
//			[i] alf - 2ά,��ʾ�߶����˵��������б�Ƕ�
//			[o] xout - nά����(n=iPoint),��ֵ���������е�x����
//			[o] yout - nά����(n=iPoint),��ֵ���������е�y����
//			[i] iPoint - ��ֵ���ܵ���
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KSmooth::_DoSubsection(double x[],double y[],double alf[],double* xout,double *yout,int iPoint)
{
	double a[4],b[4];
	double r = distance(x[0],y[0],x[1],y[1]);

	int i=0;

	//bool bTemp = x[0]<x[1];
	bool bTemp = false;
	if(bTemp)
	{
		double dTemp;
		dTemp = x[0];
		x[0] = x[1];
		x[1] = dTemp;

		dTemp = y[0];
		y[0] = y[1];
		y[1] = dTemp;
	}

	a[0] = x[i];
	a[1] = r*cos(alf[i]);
	a[2] = 3*(x[i+1]-x[i]) - r*(cos(alf[i+1])+2*cos(alf[i]));
	a[3] = -2*(x[i+1]-x[i]) + r*(cos(alf[i+1])+cos(alf[i]));

	b[0] = y[i];
	b[1] = r*sin(alf[i]);
	b[2] = 3*(y[i+1]-y[i]) - r*(sin(alf[i+1])+2*sin(alf[i]));
	b[3] = -2*(y[i+1]-y[i]) + r*(sin(alf[i+1])+sin(alf[i]));

	double t=0;
	double dt = 1.0f/(iPoint-1);
	for(i=0;i<iPoint;i++)
	{
		int iPlace=i;
		if(bTemp)
		{
			iPlace = iPoint-1-i;
		}
		double t=i*dt;
		xout[iPlace] = a[0] + a[1]*t + a[2]*t*t + a[3]*t*t*t;
		yout[iPlace] = b[0] + b[1]*t + b[2]*t*t + b[3]*t*t*t;
	}
}
