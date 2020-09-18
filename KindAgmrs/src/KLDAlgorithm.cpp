#include "StdAfx.h"
#include "KLDAlgorithm.h"

namespace Agmrs { namespace LDFile
{

	KLDAlgorithm::KLDAlgorithm(void)
	{
	}


	KLDAlgorithm::~KLDAlgorithm(void)
	{
	}

	// ��һ��ֲ��ָ��[0,1]
	// Ndvi = (Nir-Red)/(Nir+Red)
	// RedΪ��Ⲩ�η�����
	// NirΪ�����Ⲩ�η�����
	double KLDAlgorithm::Ndvi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// ���ͨ��������ֵ
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// ������ͨ��������ֵ

		// ��Ч���
		BOOL bInvalid = FALSE
			|| wDataNir == 0x0FFF			// ������Ч����
			|| wDataRed == 0x0FFF			// ������Ч����
			|| 0 == (wDataNir+wDataRed)		// ��ĸ����Ϊ0
			;

		if(!bInvalid)
		{
			dValue = (double)(wDataNir-wDataRed) / (wDataNir+wDataRed);	// ���� NDVI
			dValue = dValue<0 ? 0 : dValue;								// �ѹ�һ��ֲ��ָ��������[0,1]
		}
		//-------------------------------------------------
		return dValue;
	}

	// ��ֲֵ��ָ��
	// Rvi = Nir/Red
	// RedΪ��Ⲩ�η�����
	// NirΪ�����Ⲩ�η�����
	double KLDAlgorithm::Rvi()
	{
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// ���ͨ��������ֵ
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// ������ͨ��������ֵ

		// ��Ч���
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// ������Ч����
			|| wDataNir == 0x0FFF			// ������Ч����
			|| wDataNir == 0 				// ��ĸ����Ϊ0
			;

		if( !bInvalid )
		{
			dValue = (double)wDataNir / wDataRed;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}


	// ��ֲֵ��ָ��
	// Dvi = Nir-Red
	// RedΪ��Ⲩ�η�����
	// NirΪ�����Ⲩ�η�����
	double KLDAlgorithm::Dvi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// ���ͨ��������ֵ
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// ������ͨ��������ֵ

		// ��Ч���
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// ������Ч����
			|| wDataNir == 0x0FFF			// ������Ч����
			|| wDataNir == 0 				// ��ĸ����Ϊ0
			;

		if( !bInvalid )
		{
			dValue = ((double)wDataNir - (double)wDataRed) / 1000;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}

	// ��������ֲ��ָ��
	// Savi=(1+L)*(Nir-Red)/(Nir+Red+L)
	// RedΪ��Ⲩ�η�����
	// NirΪ�����Ⲩ�η�����
	// LΪ��������ϵ����һ��ȡֵ0.5
	double KLDAlgorithm::Savi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// ���ͨ��������ֵ
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// ������ͨ��������ֵ

		double L = 0.5;
		double zae = 0.001*(wDataNir-wDataRed);
		double den = 0.001*(wDataNir+wDataRed) + L;

		// ��Ч���
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// ������Ч����
			|| wDataNir == 0x0FFF			// ������Ч����
			|| fabs(den) < EPS 				// ��ĸ����Ϊ0
			;

		if(!bInvalid)
		{
			dValue = (1.0+L) * zae / den;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}

	// ��ǿ��ֲ��ָ��
	// Evi=G*(Nir-Red)/(Nir+C1*Red-C2*Blu+L)
	// RedΪ��Ⲩ�η�����
	// NirΪ�����Ⲩ�η�����
	// BluΪ����ͨ��������
	// G=2.5��C1=6��C2=7.5��L=1.0
	double KLDAlgorithm::Evi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// ���ͨ��������ֵ
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// ������ͨ��������ֵ
		WORD wDataBlu = pwData[KAgmrsAlgorithm::Blu] & 0x0FFF;	// ����ͨ��������ֵ

		double G=2.5,C1=6,C2=7.5,L=1.0;
		double zae = G*(wDataNir*0.001 - wDataRed*0.001);
		double den = (double)wDataNir/1000 + C1*wDataRed/1000 - C2*wDataBlu/1000 + L;

		// ��Ч���
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// ������Ч����
			|| wDataNir == 0x0FFF			// ������Ч����
			|| wDataBlu == 0x0FFF			// ������Ч����
			|| fabs(den) < EPS 				// ��ĸ����Ϊ0
			;

		if(!bInvalid)
		{
			dValue = zae / den;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}

	// Ҷ���ָ�� LAI
	// 5����Ѯ��rvi Ч����ã� y=0.698*x+1.483				[0,10] => [1.483,8.463]
	// 7����Ѯ��rvi Ч����ã� y=0.698*x+1.483				[0,10] => [1.483,8.463]
	// 7����Ѯ��dvi Ч����ã� y=0.027*x^2-0.053*x+0.169	[] => []
	// 7����Ѯ��dvi Ч����ã� y=0.122*x^0.445				[] => []
	// 8����Ѯ��dvi Ч����ã� y=0.122*x^0.445				[] => []
	// 8����Ѯ��savi Ч����ã�y=-0.062*x^2+0.418*x-0.369	[] => []
	// 
	double KLDAlgorithm::Lai()
	{
		double dValue = 0;
		//-------------------------------------------------
		if (this->ConfigLai())	// ȡ��vix����Ӧ���̱��ʽ
		{
			double dVix = 0;
			m_strVix.Upper();
			if (m_strVix == _T("RVI"))			dVix = this->Rvi();
			else if (m_strVix == _T("DVI"))		dVix = this->Dvi() * 10;
			else if (m_strVix == _T("SAVI"))	dVix = this->Savi() * 10;
			else if (m_strVix == _T("NDVI"))	dVix = this->Ndvi() * 10;
			else								dVix = this->Rvi();

			// ����LAI
			dValue = m_FormulaParser.Calculation(m_strEqu, dVix);
		}
		//-------------------------------------------------
		return dValue;
	}


	// �ر��¶�
	double KLDAlgorithm::Lst(int iMethod)
	{
		return 0;
	}


	// ˮ��
	// v = (a-w/b)^2
	// a=0.02,b=0.651
	// w = ln(R_940/R_Nir)
	// R_940��0.940umͨ��������
	// R_Nir�ǽ�����ͨ��������
	double KLDAlgorithm::Vapour()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;
		WORD wDataC940 = pwData[KAgmrsAlgorithm::C940] & 0x0FFF;

		if( wDataNir== 0x0FFF || wDataC940== 0x0FFF )
		{// ������Ч����
			return 0;
		}

		if( (wDataNir>0 && wDataNir<1000) && (wDataC940>0 && wDataC940<1000) )
		{//�������� (0,1)

			double a = 0.02;
			double b = 0.651;
			double w = log(1.0 * wDataC940 / wDataNir);
			dValue = (a - w)/b;
			dValue *= dValue;	// ^2
		}
		else
		{
			dValue = 0.01;
		}
		
		if(dValue<0.01)
		{
			dValue = 0.01;
		}

		return dValue;
	}

	// ֲ�����Ƕ�
	// Pv = [(NDVI-NDVImin)/(NDVImax-NDVImin)]^2
	double KLDAlgorithm::Pv(double ndvi_min, double ndvi_max)
	{
		double dValue = 0;
		//-------------------------------------------------
		double ndvi = Ndvi();
		if(ndvi<ndvi_min)
		{
			dValue = 0;
		}
		else if(ndvi>ndvi_max)
		{
			dValue = 0.9999;
		}
		else
		{
			dValue = (ndvi - ndvi_min) / ((ndvi_max - ndvi_min));
			dValue *= dValue;
		}
		//-------------------------------------------------
		return dValue;
	}

	// �ر�ȷ�����
	// e = m*Pv + n
	// es - �����ر�ȷ�����
	// VIRR es11 = 0.9719 es12 = 0.9714
	double KLDAlgorithm::Lse(double es)
	{
		double lse = 0.9898;
		//-------------------------------------------------
		double ev = 0.99;		// ֲ����ȫ���ǵر�ȷ�����
		double F = 0.55;		// ��״����
		double m = ev - es - (1-es) * F * ev;
		double n = es + (1-es) * F * ev;
		double pv = this->Pv(0.05,0.75);
		lse = m*pv + n;
		//-------------------------------------------------
		return lse;
	}

	// �ȷ������
	// L = c1*v^3/[exp(c2*v/T)-1]
	// L	- �����ķ����� [W.m^-2.sr^-1.cm^-1]
	// T	- �����¶�(K)
	// v	- ̽�Ⲩ�ε�Ч���Ĳ��� [cm^-1],�ǲ����ĵ���
	// c1	- 1.1910659*10^-8 [W.m^-2.sr^-1.cm^4]
	// c2	- 1.4387752 [K.cm]
	double KLDAlgorithm::Lsensor(double Tb)
	{
		double dValue = 0.1;
		//-------------------------------------------------
		// 11.25 um = 11.25*10^-4 cm
		double lmd = 11.25e-4;					// [cm]
		double v  = 1/lmd;						// [cm^-1]
		double c1 = 1.1910659e-8;				// [W.m^-2.sr^-1.cm^4]
		double c2 = 1.4387752;					// [K.cm]
		double L = c1*v*v*v/(exp(c2*v/Tb)-1);	// [W.m^-2.sr^-1.cm^-1]
		//-------------------------------------------------
		dValue = L;
		return dValue;
	}




	// �ر��¶ȷ���
	// ˫ͨ��ͳ�ƻع鷨
	// ������MODIS��VIRR����
	double KLDAlgorithm::Lst_regress()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataT11um = pwData[KAgmrsAlgorithm::T11um] & 0x0FFF;	//11um ͨ������(K)
		WORD wDataT12um = pwData[KAgmrsAlgorithm::T12um] & 0x0FFF;	//12um ͨ������(K)

		if(0xFFF == wDataT11um || 0xFFF == wDataT12um)
		{// ������Ч����
			return 0;
		}
		if(wDataT11um>(2731+2000) || wDataT12um>(2731+2000))
		{// ���� 200 ���϶�
			return 0;
		}

		dValue = 0.1* wDataT11um + 3.7618 * (0.1*wDataT11um - 0.1*wDataT12um) + 0.8352;

		return dValue;	// K
	}

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
