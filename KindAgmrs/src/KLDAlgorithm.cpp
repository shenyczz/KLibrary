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

	// 归一化植被指数[0,1]
	// Ndvi = (Nir-Red)/(Nir+Red)
	// Red为红光波段反射率
	// Nir为近红外波段反射率
	double KLDAlgorithm::Ndvi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// 红光通道反射率值
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// 近红外通道反射率值

		// 无效检测
		BOOL bInvalid = FALSE
			|| wDataNir == 0x0FFF			// 屏蔽无效数据
			|| wDataRed == 0x0FFF			// 屏蔽无效数据
			|| 0 == (wDataNir+wDataRed)		// 分母不能为0
			;

		if(!bInvalid)
		{
			dValue = (double)(wDataNir-wDataRed) / (wDataNir+wDataRed);	// 计算 NDVI
			dValue = dValue<0 ? 0 : dValue;								// 把归一化植被指数限制在[0,1]
		}
		//-------------------------------------------------
		return dValue;
	}

	// 比值植被指数
	// Rvi = Nir/Red
	// Red为红光波段反射率
	// Nir为近红外波段反射率
	double KLDAlgorithm::Rvi()
	{
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// 红光通道反射率值
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// 近红外通道反射率值

		// 无效检测
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// 屏蔽无效数据
			|| wDataNir == 0x0FFF			// 屏蔽无效数据
			|| wDataNir == 0 				// 分母不能为0
			;

		if( !bInvalid )
		{
			dValue = (double)wDataNir / wDataRed;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}


	// 差值植被指数
	// Dvi = Nir-Red
	// Red为红光波段反射率
	// Nir为近红外波段反射率
	double KLDAlgorithm::Dvi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// 红光通道反射率值
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// 近红外通道反射率值

		// 无效检测
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// 屏蔽无效数据
			|| wDataNir == 0x0FFF			// 屏蔽无效数据
			|| wDataNir == 0 				// 分母不能为0
			;

		if( !bInvalid )
		{
			dValue = ((double)wDataNir - (double)wDataRed) / 1000;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}

	// 土壤调整植被指数
	// Savi=(1+L)*(Nir-Red)/(Nir+Red+L)
	// Red为红光波段反射率
	// Nir为近红外波段反射率
	// L为土壤调节系数，一般取值0.5
	double KLDAlgorithm::Savi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// 红光通道反射率值
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// 近红外通道反射率值

		double L = 0.5;
		double zae = 0.001*(wDataNir-wDataRed);
		double den = 0.001*(wDataNir+wDataRed) + L;

		// 无效检测
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// 屏蔽无效数据
			|| wDataNir == 0x0FFF			// 屏蔽无效数据
			|| fabs(den) < EPS 				// 分母不能为0
			;

		if(!bInvalid)
		{
			dValue = (1.0+L) * zae / den;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}

	// 增强型植被指数
	// Evi=G*(Nir-Red)/(Nir+C1*Red-C2*Blu+L)
	// Red为红光波段反射率
	// Nir为近红外波段反射率
	// Blu为蓝光通道反射率
	// G=2.5，C1=6，C2=7.5，L=1.0
	double KLDAlgorithm::Evi()
	{ 
		if(!m_pData)
			return 0;

		double dValue = 0;
		//-------------------------------------------------
		PWORD pwData = (PWORD)m_pData;

		WORD wDataRed = pwData[KAgmrsAlgorithm::Red] & 0x0FFF;	// 红光通道反射率值
		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;	// 近红外通道反射率值
		WORD wDataBlu = pwData[KAgmrsAlgorithm::Blu] & 0x0FFF;	// 蓝光通道反射率值

		double G=2.5,C1=6,C2=7.5,L=1.0;
		double zae = G*(wDataNir*0.001 - wDataRed*0.001);
		double den = (double)wDataNir/1000 + C1*wDataRed/1000 - C2*wDataBlu/1000 + L;

		// 无效检测
		BOOL bInvalid = FALSE
			|| wDataRed == 0x0FFF			// 屏蔽无效数据
			|| wDataNir == 0x0FFF			// 屏蔽无效数据
			|| wDataBlu == 0x0FFF			// 屏蔽无效数据
			|| fabs(den) < EPS 				// 分母不能为0
			;

		if(!bInvalid)
		{
			dValue = zae / den;
			dValue = dValue < 0 ? 0 : dValue;
		}
		//-------------------------------------------------
		return dValue;
	}

	// 叶面积指数 LAI
	// 5月上旬：rvi 效果最好， y=0.698*x+1.483				[0,10] => [1.483,8.463]
	// 7月上旬：rvi 效果最好， y=0.698*x+1.483				[0,10] => [1.483,8.463]
	// 7月中旬：dvi 效果最好， y=0.027*x^2-0.053*x+0.169	[] => []
	// 7月下旬：dvi 效果最好， y=0.122*x^0.445				[] => []
	// 8月上旬：dvi 效果最好， y=0.122*x^0.445				[] => []
	// 8月下旬：savi 效果最好，y=-0.062*x^2+0.418*x-0.369	[] => []
	// 
	double KLDAlgorithm::Lai()
	{
		double dValue = 0;
		//-------------------------------------------------
		if (this->ConfigLai())	// 取得vix及对应方程表达式
		{
			double dVix = 0;
			m_strVix.Upper();
			if (m_strVix == _T("RVI"))			dVix = this->Rvi();
			else if (m_strVix == _T("DVI"))		dVix = this->Dvi() * 10;
			else if (m_strVix == _T("SAVI"))	dVix = this->Savi() * 10;
			else if (m_strVix == _T("NDVI"))	dVix = this->Ndvi() * 10;
			else								dVix = this->Rvi();

			// 计算LAI
			dValue = m_FormulaParser.Calculation(m_strEqu, dVix);
		}
		//-------------------------------------------------
		return dValue;
	}


	// 地表温度
	double KLDAlgorithm::Lst(int iMethod)
	{
		return 0;
	}


	// 水汽
	// v = (a-w/b)^2
	// a=0.02,b=0.651
	// w = ln(R_940/R_Nir)
	// R_940是0.940um通道反射率
	// R_Nir是近红外通道反射率
	double KLDAlgorithm::Vapour()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataNir = pwData[KAgmrsAlgorithm::Nir] & 0x0FFF;
		WORD wDataC940 = pwData[KAgmrsAlgorithm::C940] & 0x0FFF;

		if( wDataNir== 0x0FFF || wDataC940== 0x0FFF )
		{// 屏蔽无效数据
			return 0;
		}

		if( (wDataNir>0 && wDataNir<1000) && (wDataC940>0 && wDataC940<1000) )
		{//反射率在 (0,1)

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

	// 植被覆盖度
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

	// 地表比辐射率
	// e = m*Pv + n
	// es - 裸土地表比辐射率
	// VIRR es11 = 0.9719 es12 = 0.9714
	double KLDAlgorithm::Lse(double es)
	{
		double lse = 0.9898;
		//-------------------------------------------------
		double ev = 0.99;		// 植被完全覆盖地表比辐射率
		double F = 0.55;		// 形状因子
		double m = ev - es - (1-es) * F * ev;
		double n = es + (1-es) * F * ev;
		double pv = this->Pv(0.05,0.75);
		lse = m*pv + n;
		//-------------------------------------------------
		return lse;
	}

	// 热辐射计算
	// L = c1*v^3/[exp(c2*v/T)-1]
	// L	- 定标后的辐射率 [W.m^-2.sr^-1.cm^-1]
	// T	- 亮度温度(K)
	// v	- 探测波段等效中心波数 [cm^-1],是波长的倒数
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




	// 地表温度反演
	// 双通道统计回归法
	// 可用于MODIS、VIRR数据
	double KLDAlgorithm::Lst_regress()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataT11um = pwData[KAgmrsAlgorithm::T11um] & 0x0FFF;	//11um 通道亮温(K)
		WORD wDataT12um = pwData[KAgmrsAlgorithm::T12um] & 0x0FFF;	//12um 通道亮温(K)

		if(0xFFF == wDataT11um || 0xFFF == wDataT12um)
		{// 屏蔽无效数据
			return 0;
		}
		if(wDataT11um>(2731+2000) || wDataT12um>(2731+2000))
		{// 大于 200 摄氏度
			return 0;
		}

		dValue = 0.1* wDataT11um + 3.7618 * (0.1*wDataT11um - 0.1*wDataT12um) + 0.8352;

		return dValue;	// K
	}

//---------------------------------------------------------
}}// namespace Agmrs::LDFile - end
//---------------------------------------------------------
