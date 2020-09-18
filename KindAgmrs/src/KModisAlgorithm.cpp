#include "StdAfx.h"
#include "KModisAlgorithm.h"

namespace Agmrs { namespace Modis 
{

	KModisAlgorithm::KModisAlgorithm(void)
	{
	}

	KModisAlgorithm::~KModisAlgorithm(void)
	{
	}

	// 全反照率
	// 1 2 3 4 8 9 10 11 14 15 17 19
	double KModisAlgorithm::Abe()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		double a[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		memset(&a[0],0,sizeof(double)*(AGMRS_MODIS_DATA_CHANNEL_NUM_MAX));

		{
			a[ 1] = 0.137;	a[ 2] = 0.071;	a[ 3] = 0.142;	a[ 4] = 0.128;	a[ 8] = 0.099;	a[ 9] = 0.081;
			a[10] = 0.082;	a[11] = 0.080;	a[14] = 0.037;	a[15] = 0.043;	a[17] = 0.039;	a[19] = 0.059;
		}

		WORD wDataCh[AGMRS_MODIS_DATA_CHANNEL_NUM_MAX];
		for(int i=1;i<=37;i++)
		{
			wDataCh[i] = pwData[i]>999 ? 0 :  pwData[i];
		}

		double dAbe = (	a[ 1]*wDataCh[ 1] + a[ 2]*wDataCh[ 2] + a[ 3]*wDataCh[ 3] + a[ 4]*wDataCh[ 4] + 
				  		a[ 8]*wDataCh[ 8] + a[ 9]*wDataCh[ 9] +
						a[10]*wDataCh[10] + a[11]*wDataCh[11] + a[14]*wDataCh[14] + a[15]*wDataCh[15] + 
						a[17]*wDataCh[17] + a[19]*wDataCh[19]
					  ) *0.001;

		dValue = dAbe;
		return dValue;
	}

	// 水汽
	double KModisAlgorithm::Vapour()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh2 = pwData[2] & 0x0FFF;
		WORD wDataCh19 = pwData[19] & 0x0FFF;

		if( wDataCh2== 0x0FFF || wDataCh19== 0x0FFF )
		{// 屏蔽无效数据
			return 0;
		}

		if( (wDataCh2>0 && wDataCh2<1000) && (wDataCh19>0 && wDataCh19<1000) )
		{//反射率在 (0,1)

			dValue = log(1.0 * wDataCh19 / wDataCh2);
			dValue = (0.02 - dValue)/0.651;
			dValue = dValue * dValue;
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

	// 地表温度
	double KModisAlgorithm::Lst(int iMethod)
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		double dAngle(30);		// 传感器视角
		double pv(0);			// 陆地像元的植被覆盖率
		double rv(0);			// 陆地像元的植被辐射比率
		double rs(0);			// 陆地像元的裸土辐射比率
		double t31(0),t32(0);	// 31 32 通道校正温度

		double r31(0.99683),r32(0.99254);		// 31 32通道的地表比辐射率
		double tao31(0.30762),tao32(0.18143);	// 31 32通道的大气透过率

		// 归一化植被指数
		double dVix = Ndvi();
		// 水汽
		double dVapour = Vapour();

		// 1.求 31 32 通道的地表比辐射率
		if(dVix<0)
		{
			r31 = 0.99683;
			r32 = 0.99254;
		}
		else
		{
			pv = (dVix - 0.15) / 0.75;
			rv = 0.92762 + 0.070330 * pv;
			rs = 0.99782 + 0.083692 * pv;

			r31 = 0.98672 * rv * pv + 0.96767 * rs * (1-pv) + 0.003796 * min(pv,(1-pv));
			r32 = 0.98990 * rv * pv + 0.97790 * rs * (1-pv) + 0.003796 * min(pv,(1-pv));
		}

		// 2.求 31 32 通道的大气透过率(10度传感器视角时大气透过率计算)
		if(dVapour<0.4)
		{//(?,0.4)
			tao31 = 0.962802;
			tao32 = 0.948284;
		}
		else if(dVapour>6)
		{//(6,?)
			tao31 = 0.30762;
			tao32 = 0.18143;
		}
		else if((dVapour>0.4 || fabs(dVapour-0.4)<EPS) && dVapour<2)
		{//[0.4,2)
			tao31 = 0.99513 - 0.08082 * dVapour;
			tao32 = 0.99376 - 0.11369 * dVapour;
		}
		else if(dVapour>2  && (dVapour<4 || fabs(dVapour-4)<EPS))
		{//(2,4]
			tao31 = 1.08692 - 0.12759 * dVapour;
			tao32 = 1.07900 - 0.15925 * dVapour;
		}
		else if(dVapour>4  && (dVapour<6 || fabs(dVapour-6)<EPS))
		{//(4,6]
			tao31 = 1.07268 - 0.12751 * dVapour;
			tao32 = 0.93821 - 0.12613 * dVapour;
		}

		// 温度校正
		double T31(0),T32(0);

		WORD wDataCh31 = pwData[31] & 0xFFF;	// 31 通道亮温(K)
		WORD wDataCh32 = pwData[32] & 0xFFF;	// 32 通道亮温(K)

		if( 0x0FFF != wDataCh31 )	T31 = 0.1 * wDataCh31;	// K
		if( 0x0FFF != wDataCh32 )	T32 = 0.1 * wDataCh32;	// K

		if(T31>318)			t31 = 0.080;
		else if(T31>278)	t31 = -0.050 + 0.00325 * (T31-278);
		else				t31 = -0.050;

		if(T32>318)			t32 = 0.095;
		else if(T32>278)	t32 = -0.065 + 0.00400 * (T32-278);
		else				t32 = -0.065;

		// 传感器视角校正
		double a = 0.00001 * dAngle * 3.1415926 / 180;
		double dAngle2 = -0.00322 + 3.0967 * a * a;

		// 校正大气透过率
		tao31 = tao31 + t31 - dAngle2;
		tao32 = tao32 + t32 - dAngle2;

		// 计算 LST
		switch(iMethod)
		{
		case LstMethod::Becker:
			dValue = Lst_Becker(r31,r32,tao31,tao32,dVapour,dAngle);
			break;

		case LstMethod::Sobrino:
			dValue = Lst_Sobrino(r31,r32,tao31,tao32,dVapour,dAngle);
			break;

		case LstMethod::Qin:
			dValue = Lst_Qin(r31,r32,tao31,tao32);
			break;

		case LstMethod::Regress:
			dValue = Lst_regress();
			break;

		default:
			dValue = 0;
			break;
		}

		// 变成 K 
		if(dValue>0 && dValue<273.15+200)
		{
			dValue = dValue;
		}
		else
		{
			dValue = 273.15;
		}

		return dValue;	// K
	}
	// r - 地表比辐射率
	// tao - 大气透射率
	double KModisAlgorithm::Lst_Qin(double r31,double r32,double tao31,double tao32)
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh31 = pwData[31] & 0xFFF;	//31 通道亮温(K)
		WORD wDataCh32 = pwData[32] & 0x0FFF;	//32 通道亮温(K)

		if(0xFFF == wDataCh31 || 0xFFF == wDataCh32)
		{// 屏蔽无效数据
			return 0;
		}
		if(wDataCh31>(2731+2000) || wDataCh32>(2731+2000))
		{// 大于 200 摄氏度
			return 0;
		}

		double a31(-64.60363),	a32(-68.72585);
		double b31(0.440817),	b32(0.473453);

		// 31 32通道大气透过比率
		double C31 = r31 * tao31;
		double C32 = r32 * tao32;

		double D31 = (1 - tao31) * (1 + (1 - r31) * tao31);
		double D32 = (1 - tao32) * (1 + (1 - r32) * tao32);

		double E0 = D32*C31 - D31*C32;

		if(fabs(E0)<EPS)
		{//E0==0
			return 0;
		}

		double E1 = D32 * (1 - C31 - D31) / E0;
		double E2 = D31 * (1 - C32 - D32) / E0;

		double A = D31 / E0;

		double A0 = a31*E1 - a32*E2;
		double A1 = 1 + A + b31*E1;
		double A2 = A + b32*E2;

		double T31 = 0.1 * wDataCh31;
		double T32 = 0.1 * wDataCh32;

		double Ts = A0 + A1*T31 - A2*T32;	// (K)
		dValue = Ts;

		return dValue;
	}
	double KModisAlgorithm::Lst_Becker(double r31,double r32,double tao31,double tao32,double vapour,double angle)
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh31 = pwData[31] & 0x0FFF;	//31 通道亮温(K)
		WORD wDataCh32 = pwData[32] & 0x0FFF;	//32 通道亮温(K)

		if(0xFFF == wDataCh31 || 0xFFF == wDataCh32)
		{// 屏蔽无效数据
			return 0;
		}
		if(wDataCh31>(2731+2000) || wDataCh32>(2731+2000))
		{// 大于 200 摄氏度
			return 0;
		}

		double T31 = 0.1 * wDataCh31;
		double T32 = 0.1 * wDataCh32;

		double vap = vapour;
		double cosa = cos(angle * 3.1415926 / 180);

		double A31 = (1 - tao31) * cosa / vap;
		double A32 = (1 - tao32) * cosa / vap;

		double L31 = -64.60363 + 0.440817 * T31;
		double L32 = -68.72575 + 0.473453 * T32;

		double Ls31 = T31 + L31 * (1 - r31) / r31;
		double Ls32 = T32 + L32 * (1 - r32) / r32;

		double cc = A32 / A31;
		double dd = tao31 - tao32;
		double dt = (tao31 + tao32) / 2;
		double rr = 2 * (1 - dt) * cosa;
		double bb = 1 / cosa;
		double xx = dd / (2 * dt * rr);

		double md0 = (cc+1) + xx * bb * (cc-1) * cosa;
		double md1 = (cc-1) + xx * bb * (cc+1) * cosa;

		double md = md0 / md1;
		double Ts = 0.5 * (Ls31+Ls32) + 0.5 * (Ls31-Ls32) * md;

		dValue = Ts;
		return dValue;	// K
	}
	double KModisAlgorithm::Lst_Sobrino(double r31,double r32,double tao31,double tao32,double vapour,double angle)
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh31 = pwData[31] & 0x0FFF;	//31 通道亮温(K)
		WORD wDataCh32 = pwData[32] & 0x0FFF;	//32 通道亮温(K)

		if(0xFFF == wDataCh31 || 0xFFF == wDataCh32)
		{// 屏蔽无效数据
			return 0;
		}
		if(wDataCh31>(2731+2000) || wDataCh32>(2731+2000))
		{// 大于 200 摄氏度
			return 0;
		}

		double T31 = 0.1 * wDataCh31;
		double T32 = 0.1 * wDataCh32;

		double vap = vapour;
		double cosa = cos(angle * 3.1415926 / 180);

		double L31 = -64.60363 + 0.440817 * T31;
		double L32 = -68.72575 + 0.473453 * T32;

		double a31 = r31 * tao31 * cosa;
		double a32 = r32 * tao32 * cosa;

		double k31 = (1 - tao31) * cosa / vap;
		double k32 = (1 - tao32) * cosa / vap;

		double b31 = 1 + 2 * tao31 * (1 - r31) * cosa;	b31 *= k31;
		double b32 = 1 + 2 * tao32 * (1 - r32) * cosa;	b31 *= k32;

		double q = a31*b32 - a32*b31;

		double B31 = (1 - r31) / tao31 * a31 * b32 / q * (1 - 2 * k31 * vapour) * L31;
		double B32 = (1 - r32) / tao32 * a32 * b31 / q * (1 - 2 * k32 * vapour) * L32;

		double A = (a32 * b31 + b31 * b32 * vapour) / q;
		double B = B31 - B32;

		double Ts = T31 + A * (T31 - T32) + B;

		dValue = Ts;
		return dValue;	// K
	}
	double KModisAlgorithm::Lst_regress()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh31 = pwData[31] & 0x0FFF;	//31 通道亮温(K)
		WORD wDataCh32 = pwData[32] & 0x0FFF;	//32 通道亮温(K)

		if(0xFFF == wDataCh31 || 0xFFF == wDataCh32)
		{// 屏蔽无效数据
			return 0;
		}
		if(wDataCh31>(2731+2000) || wDataCh32>(2731+2000))
		{// 大于 200 摄氏度
			return 0;
		}

		dValue = 0.1* wDataCh31 + 3.7618 * (0.1*wDataCh31 - 0.1*wDataCh32) + 0.8352;

		return dValue;	// K
	}

	// 植被指数导出产品
	// 玉米LAI
	// 5月上旬：rvi 效果最好， y=0.698x+1.483			[0,2.181]
	// 7月上旬：rvi 效果最好， y=0.698x+1.483			[0,2.181]
	// 7月中旬：dvi 效果最好， y=0.027x2-0.053x+0.169	[]
	// 7月下旬：dvi 效果最好， y=0.122x^0.445	[]
	// 8月上旬：dvi 效果最好， y=0.122x0.445			[]
	// 8月下旬：savi 效果最好，y=-0.062x2+0.418x-0.369	[]
	// 
	// 0501,RVI,	y=0.698x+1.483
	// 0701,RVI,	y=0.698x+1.483
	// 0711,DVI,	y=0.027x^2-0.053x+0.169
	// 0721,DVI,	y=0.122x^0.445
	// 0801,DVI,	y=0.122x^0.445
	// 0811,DVI,	y=0.122x^0.445
	// 0821,SAVI,	y=-0.062x^2+0.418x-0.369
	//

	//double KModisAlgorithm::Lai()
	//{
	//	double dValue = 0;

	//	// 时间
	//	DATETIME* pDateTime= (DATETIME*)(&this->DateTime);
	//	int iMonth = pDateTime->wMonth;
	//	int iDay = pDateTime->wDay;

	//	double dVix = 0;

	//	if(iMonth==7 && iDay<=10)
	//	{// 7月上旬：rvi 效果最好， y=0.698x+1.483
	//	}
	//	else if(iMonth==7 && iDay>10)
	//	{// 7月中旬：dvi 效果最好， y=0.027x2-0.053x+0.169
	//	}
	//	else if(iMonth==8 && iDay<=10)
	//	{// 8月上旬：dvi 效果最好， y=0.122x0.445
	//	}
	//	else if(iMonth==8 && iDay>10)
	//	{// savi 效果最好，y=-0.062x2+0.418x-0.369
	//	}
	//	else
	//	{
	//		//return 0;
	//	}

	//	dVix = this->Rvi();
	//	//dVix = this->Ndvi();

	//	// y=0.698x+1.483
	//	//dValue = dVix<EPS ? 0 : 0.698*dVix+1.483;	// [0,2.181]
	//	dValue = dVix<EPS ? 0 : 2.5-(0.698*dVix+1.483);	// [0,2.181]

	//	return dValue;
	//}

	// 地表含水量指数
	double KModisAlgorithm::Swci()
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue = 0;

		WORD wDataCh6 = pwData[6] & 0x0FFF;
		WORD wDataCh7 = pwData[7] & 0x0FFF;

		if( wDataCh7== 0x0FFF || wDataCh6== 0x0FFF )
		{// 屏蔽无效数据
			return 0;
		}

		if( (wDataCh6+wDataCh7) == 0 )
		{// 分母不能为 0
			return 0;
		}

		dValue = (double)(wDataCh6-wDataCh7) / (wDataCh6+wDataCh7);

		dValue = dValue<0 ? 0 : dValue;

		return dValue;
	}

	// 垂直干旱指数
	// fm - 土壤线斜率
	double KModisAlgorithm::Pdi(double fm)
	{
		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		double dValue(AGMRS_PRODUCT_DATA_INVALID);
		//--------------------------------------------
		WORD wDataCh1 = pwData[1] & 0x0FFF;
		WORD wDataCh2 = pwData[2] & 0x0FFF;
		if( (wDataCh1>0 && wDataCh1<1000) && (wDataCh2>0 && wDataCh2<1000) )
		{
			double dDataCh1 = (double)wDataCh1 / 1000;
			double dDataCh2 = (double)wDataCh2 / 1000;

			dValue = (dDataCh1 + fm*dDataCh2) / sqrt(1+fm*fm);
		}
		//--------------------------------------------
		return dValue;
	}

	// 归一化积雪指数(ch4,ch6)
	double KModisAlgorithm::Ndsi()
	{
		double dValue = 0;

		if(!m_pData)
			return 0;

		PWORD pwData = (PWORD)m_pData;

		WORD wDataCh4 = pwData[4] & 0x0FFF;
		WORD wDataCh6 = pwData[6] & 0x0FFF;

		if( wDataCh4== 0x0FFF || wDataCh6== 0x0FFF )
		{// 屏蔽无效数据
			return 0;
		}
		if( wDataCh4== 0x0FFE || wDataCh6== 0x0FFE )
		{// 屏蔽无效数据
			return 0;
		}

		dValue = (double)(wDataCh4-wDataCh6) / (wDataCh4+wDataCh6);
		dValue = dValue<0 ? 0 : dValue;

		return dValue;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Modis - end
//---------------------------------------------------------
