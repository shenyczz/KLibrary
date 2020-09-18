#include "StdAfx.h"
#include "KVirrAlgorithm.h"

namespace Agmrs { namespace Virr
{

	KVirrAlgorithm::KVirrAlgorithm(void)
	{
	}


	KVirrAlgorithm::~KVirrAlgorithm(void)
	{
	}

	// �ر��¶�
	// ʹ�÷��Ѵ��㷨
	// Ts = A0 + P*(T11+T12)/2 + M*(T11-T12)/2
	// Ts	- �ر��¶ȣ�K��
	// A0	- �����״���йص�ϵ��
	// T11	- �Ⱥ���ͨ��11��m�������£�K��
	// T12	- �Ⱥ���ͨ��12��m�������£�K��
	// P	- �����Ⱥ���ͨ���ر�ȷ����ʵĺ���
	// M	- �����Ⱥ���ͨ���ر�ȷ����ʵĺ���
	double KVirrAlgorithm::Lst(int iMethod)
	{
		double dValue = 0;
		//-------------------------------------------------
		if(!m_pData)
			return dValue;

		PWORD pwData = (PWORD)m_pData;

		double A0 = 0.0113;

		WORD wDataT11um = pwData[KAgmrsAlgorithm::T11um] & 0x0FFF;	// �Ⱥ���ͨ��11��m��������
		WORD wDataT12um = pwData[KAgmrsAlgorithm::T12um] & 0x0FFF;	// �Ⱥ���ͨ��12��m��������
		if(0x0FFF == wDataT11um || 0x0FFF == wDataT12um)
			return dValue;

		double T11 = 0.1 * wDataT11um;	// K
		double T12 = 0.1 * wDataT12um;	// K

		// �Ⱥ���ͨ���ر�ȷ�����
		double es11 = 0.9719, es12 = 0.9714;	// �Ⱥ���ͨ���ر�ȷ����ʣ�������
		double lse11 = Lse(es11);
		double lse12 = Lse(es12);

		double ev = (lse11+lse12)/2;
		double ed = lse11 - lse12;

		double a=0.1197,b=-0.4891;
		double P = 1 + a*(1-ev)/ev + b*ed/(ev*ev);

		double a1=5.6573,b1=9.9238,c1=5.6538;
		double M = c1 + a1*(1-ev)/ev + b1*ed/(ev*ev);

		double Ts = A0 + P*(T11+T12)/2 + M*(T11-T12)/2;
		//-------------------------------------------------
		dValue = Ts;
		return dValue;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------
