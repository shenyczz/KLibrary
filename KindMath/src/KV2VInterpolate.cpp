#include "StdAfx.h"
#include "KV2VInterpolate.h"

KV2VInterpolate::KV2VInterpolate(void)
{
	m_pxi	= NULL;
	m_pyi	= NULL;
	m_pvi	= NULL;
	m_ni	= 0;

	m_pxo	= NULL;
	m_pyo	= NULL;
	m_pvo	= NULL;
	m_no	= 0;

	m_dRadius		= 0.0f;
	m_nVertexMin	= VERTEX_NUM_MIN;
	m_bAutoRadius	= TRUE;
	m_bCorrect		= TRUE;
	m_bStrict		= FALSE;

	m_xmin = 1.0e+12f;
	m_xmax = 1.0e-12f;		//��ֵx��Χ

	m_ymin = 1.0e+12f;
	m_ymax = 1.0e-12f;		//��ֵy��Χ


	m_pRadiusArray = NULL;
}

KV2VInterpolate::~KV2VInterpolate(void)
{
	_deletea(m_pRadiusArray);
}

//void KV2VInterpolate::Init()
//{
//	m_pxi	= NULL;
//	m_pyi	= NULL;
//	m_pvi	= NULL;
//	m_ni	= 0;
//
//	m_pxo	= NULL;
//	m_pyo	= NULL;
//	m_pvo	= NULL;
//	m_no	= 0;
//
//	m_dRadius		= 0.0f;
//	m_nVertexMin	= VERTEX_NUM_MIN;
//	m_bAutoRadius	= TRUE;
//	m_bCorrect		= TRUE;
//	m_bStrict		= FALSE;
//
//	m_xmin = 1.0e+12f;
//	m_xmax = 1.0e-12f;		//��ֵx��Χ
//
//	m_ymin = 1.0e+12f;
//	m_ymax = 1.0e-12f;		//��ֵy��Χ
//
//
//	m_pRadiusArray = NULL;
//}

//void KV2VInterpolate::Cleanup()
//{
//	_deletea(m_pRadiusArray);
//}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetInput
// ��    ��: ����Դ��ɢ������ָ��
// ��    ��: public
// ��    ��:
//			[i] pxi - Դ��ɢ�����ݵ�x����ָ��
//			[i] pyi - Դ��ɢ�����ݵ�y����ָ��
//			[i] pvi - Դ��ɢ�����ݵ�Ҫ��ֵָ��
//			[i] ni - Դ��ɢ������(�൱�����鳤��)
// ��    ��: 
// ע    ��: pxi,pyi,pvi��ָ�ڴ��е����߸��������ͷ�,���һ��Ҫע��,����
//			 ������ڴ�й¶
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::SetInput(double* pxi,double* pyi,double* pvi,int ni)
{
	m_pxi	= pxi;
	m_pyi	= pyi;
	m_pvi	= pvi;
	m_ni	= ni;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetOutput
// ��    ��: ���������ɢ������ָ��
// ��    ��: public
// ��    ��:
//			[i] pxi - Դ��ɢ�����ݵ�x����ָ��
//			[i] pyi - Դ��ɢ�����ݵ�y����ָ��
//			[i] pvi - Դ��ɢ�����ݵ�Ҫ��ֵָ��
//			[i] ni - Դ��ɢ������(�൱�����鳤��)
// ��    ��: 
// ע    ��: pxi,pyi,pvi��ָ�ڴ��е����߸��������ͷ�,���һ��Ҫע��,����
//			 ������ڴ�й¶
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::SetOutput(double* pxo,double* pyo,double* pvo,int no)
{
	m_pxo	= pxo;
	m_pyo	= pyo;
	m_pvo	= pvo;
	m_no	= no;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetRadius
// ��    ��: ����ɨ��뾶(km)
// ��    ��: public
// ��    ��:
//			[i] dRadius - ɨ��뾶,Ĭ��=0.0
//				���ɨ��뾶<=0,���Զ�����ɨ��뾶,ʹɨ��뾶��������nVertexMin����
//				���ɨ��뾶 >0,���Զ�����ɨ��뾶����,nVertexMin ������Ч
//			[i] nVertexMin - ɨ�踲�Ƿ�Χ�ڰ�������С���ݵ���,Ĭ��=VERTEX_NUM_MIN
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::SetRadius(double dRadius,int nVertexMin)
{
	m_dRadius		= dRadius;
	m_nVertexMin	= nVertexMin;

	if(m_nVertexMin>9)
	{// ���9����
		m_nVertexMin = 9;
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Prepare
// ��    ��: Ϊ��ֵ������׼��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
//void KV2VInterpolate::Prepare()
//{
//	//1. ����Դ����������ֵ��Χ��������Χ��Ч
//	//�Ƿ��ϸ��ڲ�
//	if(m_bStrict)
//	{
//		for(int i=0;i<m_ni;i++)
//		{
//			m_xmin = min(m_xmin,m_pxi[i]);
//			m_xmax = max(m_xmax,m_pxi[i]);
//
//			m_ymin = min(m_ymin,m_pyi[i]);
//			m_ymax = max(m_ymax,m_pyi[i]);
//		}
//	}
//
//	//2. ɨ��뾶С�ڵ���0��Ҫ���Զ�ɨ��
//	if(fabs(m_dRadius) < 1.e-12 || m_dRadius < 0)
//	{
//		m_bAutoRadius = TRUE;
//
//		KfxAssert(m_nVertexMin >= 3);		//�Զ�ɨ��Ҫ��ɨ��뾶�ڲ�վ�������ڵ���3
//		KfxAssert(m_nVertexMin < m_ni);		//�Զ�ɨ��Ҫ��ɨ��뾶�ڲ�վ�������ܴ���������
//	}
//	else
//	{
//		m_bAutoRadius = false;
//	}
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
void KV2VInterpolate::Transact(int iMethod)
{
	//��V2V�в������ڴ�,Ҳ���ͷ��ڴ�,�ڴ�����ɵ����߸���
	//���Բ��õ���PrepareMemory()
	//1. ����Դ����������ֵ��Χ��������Χ��Ч
	//�Ƿ��ϸ��ڲ�
	if(m_bStrict)
	{
		for(int i=0;i<m_ni;i++)
		{
			m_xmin = min(m_xmin,m_pxi[i]);
			m_xmax = max(m_xmax,m_pxi[i]);

			m_ymin = min(m_ymin,m_pyi[i]);
			m_ymax = max(m_ymax,m_pyi[i]);
		}
	}

	//2. ɨ��뾶С�ڵ���0��Ҫ���Զ�ɨ��
	if(fabs(m_dRadius) < 1.e-12 || m_dRadius < 0)
	{
		m_bAutoRadius = TRUE;

		// �Զ�ɨ��Ҫ��ɨ��뾶�ڲ�վ�������ڵ���3
		// �Զ�ɨ��Ҫ��ɨ��뾶�ڲ�վ�������ܴ���������
		if(m_nVertexMin<3 || m_nVertexMin>=m_ni)
			throw;
	}
	else
	{
		m_bAutoRadius = FALSE;
	}
	//------------------------------------------------
	//�Զ�����������岹���ɨ��뾶,ʹ��Ӧ���ɨ��뾶�������� nVertex ����
	//������ɺ�,����ֵ����ɢ���Ӧ��ɨ��뾶������ m_RadiusArray
	if(m_bAutoRadius)
	{
		AutoRadius();
	}
	else
	{
		//�������ṩ�̶���ɨ��뾶,�� m_dRadius;
	}
	//------------------------------------------------
	//����Ա��岹����в�ֵ����
	if(iMethod > 1) iMethod = 0;

	Interpolate(iMethod);
	//------------------------------------------------
	//У��
	if(m_bCorrect)
	{
		//�ѵ�һ�β�ֵ�����Ϊ����,�ز嵽ԭ����ɢ����,����õ���ֵ���
		//�ٰѲ�ֵ������²嵽��ֵ���Ͻ�������
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: PrepareMemory
// ��    ��: ׼���ڴ�
// ��    ��: protected
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::PrepareMemory()
{
	//���÷����ڴ�
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Interpolate
// ��    ��: ����Ա��岹����в�ֵ����
// ��    ��: protected
// ��    ��:
//			[i] iMethod - ��ֵ������־,Ĭ��=0
//				0-Gressman��ֵ
//				1-Gauss��ֵ
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::Interpolate(int iMethod)
{
	//------------------------------------
	int i;
	double x, y;
	//------------------------------------
	// �ص�����
	CALLBACK_FUNC pCallbackFunc = this->CallbackFunc();
	// ����������
	ProgressParam* pCallbackParam = (ProgressParam*)this->CallbackParam();
	//------------------------------------
	if(pCallbackParam) pCallbackParam->SetInfo(_T("��ֵ����..."));
	for(i=0;i<m_no;i++)
	{
		x = m_pxo[i];
		y = m_pyo[i];

		//-------------
		if(m_bAutoRadius)	//�Զ�����ɨ��뾶(km)
		{
			//m_dRadius = m_RadiusArray[i];
			m_dRadius = *(m_pRadiusArray+i);
		}
		//-------------
		//if(m_bStrict)
		//{
		//	if(x<m_xmin || x>m_xmax || y<m_ymin || y>m_ymax)
		//	{
		//		m_pvo[i] = 0;
		//	}
		//	else
		//	{
		//		m_pvo[i] = InterpolateOnePoint(x,y,iMethod);	//��ֵ
		//	}
		//}
		//else
		{
			m_pvo[i] = InterpolateOnePoint(x,y,iMethod);	//��ֵ
		}

		// ��ʾ����
		if(pCallbackFunc)
		{
			if(pCallbackParam)
			{
				pCallbackParam->lTotalNums = m_no;
				pCallbackParam->lCurrentPos = i;
			}

			pCallbackFunc(pCallbackParam);
		}
	}
	//------------------------------------
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: InterpolateOnePoint
// ��    ��: �������(x,y)�岹Ҫ��ֵ
// ��    ��: protected
// ��    ��:
//			[i] iMethod - ��ֵ������־,Ĭ��=0
//				0-Gressman��ֵ
//				1-Gauss��ֵ
// ��    ��: ����õ��Ĳ�ֵ���Ҫ��ֵ
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::InterpolateOnePoint(double x, double y, int iMethod)
{
	double dValue(0);
	//-------------------------------------------
	switch(iMethod)
	{
	case 0:
		dValue = GressmanInterp(x,y);	//�͹۷�����ֵ
		break;
	case 1:
		dValue = GaussInterp(x,y);		//��˹Ȩ�ز�ֵ
		break;
	}
	//-------------------------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GressmanInterp
// ��    ��: Gressman ��ֵ
// ��    ��: protected
// ��    ��:
//			[i] x - ��ֵ���x����
//			[i] y - ��ֵ���y����
//
// ��    ��: ����õ��Ĳ�ֵ���Ҫ��ֵ
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GressmanInterp(double x, double y)
{
	double dValue(0);
	//---------------------------------
	double qz;	//Ȩ��
	double fk;	//k���ֵ
	double sumqz(0.0),sumfk(0.0);
	double dk;
	//---
	for(int i=0;i<m_ni;i++)
	{
		dk = _distance(x,y,m_pxi[i],m_pyi[i]);	//�������
		
		if(dk > m_dRadius)	continue;	//��ɨ�跶Χ֮��

		dk *= 111.37f;		//km

		fk = m_pvi[i];		//ȡ��Դ���ݵ��Ҫ��ֵ

		//---����Ȩ��
		qz  = GressmanCoefficient(m_dRadius,dk);
		//-------------

		fk *= qz;		//��Ȩֵ

		sumqz += qz;	//Ȩ�غ�
		sumfk += fk;	//Դ���ݵ�ļ�Ȩֵ֮��
	}
	//---------------------------------
	dValue = sumqz < 1.e-12 ? 0.0f : sumfk/sumqz;
	//-------------------------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GaussInterp
// ��    ��: Gauss ��ֵ
// ��    ��: protected
// ��    ��:
//			[i] x - ��ֵ���x����
//			[i] y - ��ֵ���y����
//
// ��    ��: ����õ��Ĳ�ֵ���Ҫ��ֵ
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GaussInterp(double x, double y)
{
	double dValue(0);
	//---------------------------------
	double qz;	//Ȩ��
	double fk;	//k���ֵ
	double sumqz(0.0),sumfk(0.0);
	double dk;
	//---
	for(int i=0;i<m_ni;i++)
	{
		dk = _distance(x,y,m_pxi[i],m_pyi[i]);	//�������
		dk *= 111.37f;	//km
		fk = m_pvi[i];

		//---����Ȩ��
		qz  = GaussCoefficient(m_dRadius,dk);
		//-------------

		fk *= qz;

		sumqz += qz;	//Ȩ�غ�
		sumfk += fk;	//
	}
	//---------------------------------
	dValue = sumqz < 1.e-12 ? 0.0f : sumfk/sumqz;
	//-------------------------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: AutoRadius
// ��    ��: �Զ�����������岹���ɨ��뾶,ʹ��Ӧ���ɨ��뾶�������� m_nVertexMin ����
//			 ������ɺ�,����ֵ����ɢ���Ӧ��ɨ��뾶������ m_RadiusArray
// ��    ��: protected
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KV2VInterpolate::AutoRadius()
{
	_deletea(m_pRadiusArray);

	m_pRadiusArray = new double[m_no];
	memset(m_pRadiusArray,0,sizeof(double)*m_no);

	if(m_pCallbackParam)
	{
		((ProgressParam*)m_pCallbackParam)->SetInfo(_T("�Զ�����ɨ��뾶..."));
	}

	for(int i=0;i<m_no;i++)
	{
		double x0=m_pxo[i];
		double y0=m_pyo[i];

		//��������(x0,y0)ΪԲ�ĵ�ɨ�跶Χ�ڰ��� m_nVertexMin �����ݵ��ɨ��뾶
		double dRadius=_CalculateRadius(x0,y0,m_nVertexMin);

		//����ÿ���Զ�ɨ��õ���ÿ�����岹���Ӧ��ɨ��뾶
		//m_RadiusArray.push_back(dRadius);
		*(m_pRadiusArray+i) = dRadius;

		// ��ʾ����
		if(m_pCallbackFunc)
		{
			if(m_pCallbackParam)
			{
				ProgressParam* pProgressParam = (ProgressParam*)m_pCallbackParam;
				pProgressParam->lTotalNums = m_no;
				pProgressParam->lCurrentPos = i;
			}

			m_pCallbackFunc(m_pCallbackParam);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: _calculateRadius
// ��    ��: ����ɨ��뾶r,ʹ����(x0,y0)ΪԲ�ĵ�Բ�ܸ��� n ����ɢ��
// ��    ��: private
// ��    ��:
//			[i] x0 - ��P0��x����
//			[i] y0 - ��P0��y����
//
// ��    ��: ɨ��뾶
// ע    ��: �㷨��ʱ��ķ����Զ��Զ�����ɨ��뾶��
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::_CalculateRadius(double x0, double y0,int n)
{
	int i,j;
	double dRadius = 0.0;
	//TDZ tdz;
	int iNum=0;
	double dr = 1.85f;		//1.85km�൱��1/60��γ��(1��)
	//---------------------------
	// ���9����
	n = n>9 ? 9 : n;

	double radius[10];
	for(j=0;j<10;j++)
	{
		radius[j] = 1.0e+12f;
	}
	//---------------------------
	for(i=0;i<m_ni;i++)
	{
		double d = _distance(x0,y0,m_pxi[i],m_pyi[i]);
		d*=111.37f;	//��ɹ���

		for(j=0;j<10;j++)
		{// ��С����
			if(d<radius[j])
			{
				for(int k=9;k>j;k--)
				{// ����
					radius[k] = radius[k-1];
				}

				radius[j] = d;
				break;
			}
		}
	}
	//---------------------------
	dRadius = radius[n];
	dRadius += radius[n-1];
	dRadius /= 2.0;
	//---------------------------
	return dRadius;
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
double KV2VInterpolate::_distance(double x0, double y0, double x1, double y1)
{
	double dValue = 0.0;
	//-----------------------------
	dValue = (x1-x0)*(x1-x0) + (y1-y0)*(y1-y0);
	dValue = sqrt(dValue);
	//-----------------------------
	return dValue;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GressmanCoefficient
// ��    ��: GressmanȨ������
// ��    ��: private
// ��    ��:
//			[i] r - ɨ��뾶
//			[i] d - ��������
//
// ��    ��: Ȩ������
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GressmanCoefficient(double r, double d)
{
	double dCoefficient = 0.0;
	//---
	if(d < r)	//��������С��ɨ��뾶
	{
		double rr = r*r;
		double dd = d*d;
		dCoefficient = (rr-dd)/(rr+dd);
	}
	else
	{
		dCoefficient = 0.0;
	}
	//---
	return dCoefficient;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GaussCoefficient
// ��    ��: Gauss Ȩ������
// ��    ��: private
// ��    ��:
//			[i] r - ɨ��뾶
//			[i] d - ��������
//
// ��    ��: Ȩ������
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KV2VInterpolate::GaussCoefficient(double r, double d)
{
	double dCoefficient(0);
	//-------------------------------------------
	if(d < r)	//��������С��ɨ��뾶
	{
		double rr=r*r;
		double dd=d*d;
		dCoefficient= (double)exp(-d*d/(4.0*10000));
		//dCoefficient= exp((rr+dd)/(rr-dd)/(4.0*10000));
	}
	else
	{
		dCoefficient = 0.0f;
	}
	//-------------------------------------------
	return dCoefficient;
}
