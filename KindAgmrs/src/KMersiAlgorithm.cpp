#include "StdAfx.h"
#include "KMersiAlgorithm.h"

namespace Agmrs { namespace Mersi
{

	KMersiAlgorithm::KMersiAlgorithm(void)
	{
	}

	KMersiAlgorithm::~KMersiAlgorithm(void)
	{
	}

	// �ر��¶�
	// ʹ�������Ե�ͨ���㷨
	// Ts = r*[ (f1*L+f2)/e + f3] + d
	// 
	// Ts	- �ر��¶ȣ�K��
	// e	- �ر�ȷ�����
	// f1	- ��ˮ�������йصĲ��� f1 = 0.14716*w*w - 0.15583*w + 1.12339
	// f2	- ��ˮ�������йصĲ��� f2 = -1.1836*w*w - 0.37607*w - 0.52893
	// f3	- ��ˮ�������йصĲ��� f3 = -0.0455*w*w + 1.87191*w - 0.39071
	// r,d	- �м����
	// r = (c2*L/T)*(B^4*L/c1 + 1/B)
	// d = T - r*L
	// L	- �����ķ�����[W.m^-2.sr^-1.um^-1]
	// T	- ��Ԫ�����¶�(K)
	// B	- ��Ч������MERSI 11.25um��
	// c1	- 1.1910659*10^4 [W.m^-2.sr^-1.um^4]
	// c2	- 1.4387752*10^4 [K.um]
	double KMersiAlgorithm::Lst(int iMethod)
	{
		double dValue = 0;
		//-------------------------------------------------
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double T(0);
		WORD wDataT11um = pwData[KAgmrsAlgorithm::T11um] & 0xFFF;	// 11umͨ������(K)
		if( 0x0FFF != wDataT11um )	T = 0.1 * wDataT11um;	// K

		// �ر�ȷ�����
		double e = this->Lse(0.98);
		// ˮ������(g.cm^2)
		double w = this->Vapour();
		double f1 = 0.14716*w*w - 0.15583*w + 1.12339;
		double f2 = -1.1836*w*w - 0.37607*w - 0.52893;
		double f3 = -0.0455*w*w + 1.87191*w - 0.39071;
		double L  = this->Lsensor(T);
		double B  = 11.25;	// [um]
		double c1 = 1.1910659e4;
		double c2 = 1.4387752e4;
		double r  = (c2*L/T)*(B*B*B*B*L/c1 + 1/B);
		double d  = T - r*L;
		double Ts = r*((f1*L+f2)/e + f3) + d;
		//-------------------------------------------------
		dValue = Ts;
		return dValue;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------
