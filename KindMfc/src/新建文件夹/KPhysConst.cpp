// PhysConst.cpp: Implementierung der Klasse CPhysConst.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "KPhysConst.h"

//#ifdef _DEBUG
//#undef THIS_FILE
//static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
//#endif

namespace Math
{

	CPhysConst::CPhysConst()
	{
		// 13 Zeichen
		m_strPhysConst.Add(_T("")); // soll bei 1 anfangen
		m_strPhysConst.Add(_T("_LAMBDA_BAR_e"));
		m_strPhysConst.Add(_T("_SIGMA_0_BARN"));
		m_strPhysConst.Add(_T("_SIN2_THETA_W"));

		// 11 Zeichen
		m_strPhysConst.Add(_T("_h_BAR_MeVs"));
		m_strPhysConst.Add(_T("_MU_B_MeV_T"));
		m_strPhysConst.Add(_T("_LAMBDA_1EV"));
		m_strPhysConst.Add(_T("_MU_N_MeV_T"));

		// 10 Zeichen

		// 9 Zeichen
		m_strPhysConst.Add(_T("_R_INF_EV"));
		m_strPhysConst.Add(_T("_SIGMA_SB"));

		// 8 Zeichen
		m_strPhysConst.Add(_T("_LAMBDAT"));
		m_strPhysConst.Add(_T("_m_D_MeV"));
		m_strPhysConst.Add(_T("_m_e_MeV"));
		m_strPhysConst.Add(_T("_m_P_MeV"));
		m_strPhysConst.Add(_T("_m_P_M_E"));
		m_strPhysConst.Add(_T("_V_MOLAR"));

		// 7 Zeichen
		m_strPhysConst.Add(_T("_hBARc2"));
		m_strPhysConst.Add(_T("_K_B_EV"));
		m_strPhysConst.Add(_T("_m_e_kg"));
		m_strPhysConst.Add(_T("_m_P_kg"));

		// 6 Zeichen
		m_strPhysConst.Add(_T("_Alpha"));
		m_strPhysConst.Add(_T("_EPS_0"));
		m_strPhysConst.Add(_T("_e_ESU"));
		m_strPhysConst.Add(_T("_E_M_e"));
		m_strPhysConst.Add(_T("_E_M_P"));
		m_strPhysConst.Add(_T("_h_BAR"));
		m_strPhysConst.Add(_T("_hBARc"));
		m_strPhysConst.Add(_T("_m_P_u"));
		m_strPhysConst.Add(_T("_u_MeV"));

		// 5 Zeichen
		m_strPhysConst.Add(_T("_G_SI"));
		m_strPhysConst.Add(_T("_M_Z0"));
		m_strPhysConst.Add(_T("_MU_0"));
		m_strPhysConst.Add(_T("_u_kg"));

		// 4 Zeichen
		m_strPhysConst.Add(_T("_a_0"));
		m_strPhysConst.Add(_T("_e_C"));
		m_strPhysConst.Add(_T("_G_F"));
		m_strPhysConst.Add(_T("_G_P"));
		m_strPhysConst.Add(_T("_G_S"));
		m_strPhysConst.Add(_T("_K_B"));
		m_strPhysConst.Add(_T("_M_W"));
		m_strPhysConst.Add(_T("_N_A"));
		m_strPhysConst.Add(_T("_r_e"));

		//2 Zeichen
		m_strPhysConst.Add(_T("_c"));
		m_strPhysConst.Add(_T("_h"));
		m_strPhysConst.Add(_T("_g"));

		m_strPhysConst.Add(_T("***")); 
	}

	CPhysConst::~CPhysConst()
	{
	}

	LPCTSTR CPhysConst::GetEachPhysConst(int i) //Reihenfolge mu?von längstem zu kürzerem
	{
		return (LPCTSTR)m_strPhysConst[i];
	}

	BOOL CPhysConst::IsPhysConst(LPCTSTR str)
	{
		if (_tcsicmp(str, _T("_c")) == 0) return true;
		if (_tcsicmp(str, _T("_h")) == 0) return true;
		if (_tcsicmp(str, _T("_h_BAR")) == 0) return true;
		if (_tcsicmp(str, _T("_h_BAR_MeVs")) == 0) return true;
		if (_tcsicmp(str, _T("_e_C")) == 0) return true;
		if (_tcsicmp(str, _T("_e_ESU")) == 0) return true;
		if (_tcsicmp(str, _T("_hBARc")) == 0) return true;
		if (_tcsicmp(str, _T("_hBARc2")) == 0) return true;
		if (_tcsicmp(str, _T("_m_e_kg")) == 0) return true;
		if (_tcsicmp(str, _T("_m_e_MeV")) == 0) return true;
		if (_tcsicmp(str, _T("_m_P_MeV")) == 0) return true;
		if (_tcsicmp(str, _T("_m_P_u")) == 0) return true;
		if (_tcsicmp(str, _T("_m_P_kg")) == 0) return true;
		if (_tcsicmp(str, _T("_m_P_M_E")) == 0) return true;
		if (_tcsicmp(str, _T("_m_D_MeV")) == 0) return true;
		if (_tcsicmp(str, _T("_u_MeV")) == 0) return true;
		if (_tcsicmp(str, _T("_u_kg")) == 0) return true;
		if (_tcsicmp(str, _T("_EPS_0")) == 0) return true;
		if (_tcsicmp(str, _T("_MU_0")) == 0) return true;
		if (_tcsicmp(str, _T("_Alpha")) == 0) return true;
		if (_tcsicmp(str, _T("_r_e")) == 0) return true;
		if (_tcsicmp(str, _T("_LAMBDA_BAR_e")) == 0) return true;
		if (_tcsicmp(str, _T("_a_0")) == 0) return true;
		if (_tcsicmp(str, _T("_LAMBDA_1EV")) == 0) return true;
		if (_tcsicmp(str, _T("_R_INF_EV")) == 0) return true;
		if (_tcsicmp(str, _T("_SIGMA_0_BARN")) == 0) return true;
		if (_tcsicmp(str, _T("_MU_B_MeV_T")) == 0) return true;
		if (_tcsicmp(str, _T("_MU_N_MeV_T")) == 0) return true;
		if (_tcsicmp(str, _T("_E_M_e")) == 0) return true;
		if (_tcsicmp(str, _T("_E_M_P")) == 0) return true;
		if (_tcsicmp(str, _T("_G_SI")) == 0) return true;
		if (_tcsicmp(str, _T("_G_P")) == 0) return true;
		if (_tcsicmp(str, _T("_g")) == 0) return true;
		if (_tcsicmp(str, _T("_N_A")) == 0) return true;
		if (_tcsicmp(str, _T("_K_B")) == 0) return true;
		if (_tcsicmp(str, _T("_K_B_EV")) == 0) return true;
		if (_tcsicmp(str, _T("_V_MOLAR")) == 0) return true;
		if (_tcsicmp(str, _T("_LAMBDAT")) == 0) return true;
		if (_tcsicmp(str, _T("_SIGMA_SB")) == 0) return true;
		if (_tcsicmp(str, _T("_G_F")) == 0) return true;
		if (_tcsicmp(str, _T("_SIN2_THETA_W")) == 0) return true;
		if (_tcsicmp(str, _T("_M_W")) == 0) return true;
		if (_tcsicmp(str, _T("_M_Z0")) == 0) return true;
		if (_tcsicmp(str, _T("_G_S")) == 0) return true;

		return false;
	}

	double CPhysConst::GetPhysConstValue(LPCTSTR str)
	{
		if (_tcsicmp(str, _T("_c")) == 0) return _c;
		if (_tcsicmp(str, _T("_h")) == 0) return _h;
		if (_tcsicmp(str, _T("_h_BAR")) == 0) return _h_BAR;
		if (_tcsicmp(str, _T("_h_BAR_MeVs")) == 0) return _h_BAR_MeVs;
		if (_tcsicmp(str, _T("_e_C")) == 0) return _e_C;
		if (_tcsicmp(str, _T("_e_ESU")) == 0) return _e_ESU;
		if (_tcsicmp(str, _T("_hBARc")) == 0) return _hBARc;
		if (_tcsicmp(str, _T("_hBARc2")) == 0) return _hBARc2;
		if (_tcsicmp(str, _T("_m_e_kg")) == 0) return _m_e_kg;
		if (_tcsicmp(str, _T("_m_e_MeV")) == 0) return _m_e_MeV;
		if (_tcsicmp(str, _T("_m_P_MeV")) == 0) return _m_P_MeV;
		if (_tcsicmp(str, _T("_m_P_u")) == 0) return _m_P_u;
		if (_tcsicmp(str, _T("_m_P_kg")) == 0) return _m_P_kg;
		if (_tcsicmp(str, _T("_m_P_M_E")) == 0) return _m_P_M_E;
		if (_tcsicmp(str, _T("_m_D_MeV")) == 0) return _m_D_MeV;
		if (_tcsicmp(str, _T("_u_MeV")) == 0) return _u_MeV;
		if (_tcsicmp(str, _T("_u_kg")) == 0) return _u_kg;
		if (_tcsicmp(str, _T("_EPS_0")) == 0) return _EPS_0;
		if (_tcsicmp(str, _T("_MU_0")) == 0) return _MU_0;
		if (_tcsicmp(str, _T("_Alpha")) == 0) return _ALPHA;
		if (_tcsicmp(str, _T("_r_e")) == 0) return _r_e;
		if (_tcsicmp(str, _T("_LAMBDA_BAR_e")) == 0) return _LAMBDA_BAR_e;
		if (_tcsicmp(str, _T("_a_0")) == 0) return _a_0;
		if (_tcsicmp(str, _T("_LAMBDA_1EV")) == 0) return _LAMBDA_1EV;
		if (_tcsicmp(str, _T("_R_INF_EV")) == 0) return _R_INF_EV;
		if (_tcsicmp(str, _T("_SIGMA_0_BARN")) == 0) return _SIGMA_0_BARN;
		if (_tcsicmp(str, _T("_MU_B_MeV_T")) == 0) return _MU_B_MeV_T;
		if (_tcsicmp(str, _T("_MU_N_MeV_T")) == 0) return _MU_N_MeV_T;
		if (_tcsicmp(str, _T("_E_M_e")) == 0) return _E_M_e;
		if (_tcsicmp(str, _T("_E_M_P")) == 0) return _E_M_P;
		if (_tcsicmp(str, _T("_G_SI")) == 0) return _G_SI;
		if (_tcsicmp(str, _T("_G_P")) == 0) return _G_P;
		if (_tcsicmp(str, _T("_g")) == 0) return _g;
		if (_tcsicmp(str, _T("_N_A")) == 0) return _N_A;
		if (_tcsicmp(str, _T("_K_B")) == 0) return _K_B;
		if (_tcsicmp(str, _T("_K_B_EV")) == 0) return _K_B_EV;
		if (_tcsicmp(str, _T("_V_MOLAR")) == 0) return _V_MOLAR;
		if (_tcsicmp(str, _T("_LAMBDAT")) == 0) return _LAMBDAT;
		if (_tcsicmp(str, _T("_SIGMA_SB")) == 0) return _SIGMA_SB;
		if (_tcsicmp(str, _T("_G_F")) == 0) return _G_F;
		if (_tcsicmp(str, _T("_SIN2_THETA_W")) == 0) return _SIN2_THETA_W;
		if (_tcsicmp(str, _T("_M_W")) == 0) return _M_W;
		if (_tcsicmp(str, _T("_M_Z0")) == 0) return _M_Z0;
		if (_tcsicmp(str, _T("_G_S")) == 0) return _G_S;

		return 0.0;
	}

//---------------------------------------------------------
}// namespace Math - end
//---------------------------------------------------------

