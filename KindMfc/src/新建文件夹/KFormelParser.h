//
// FormulaParser.h: interface for the CFormulaParser class.
// Copyright: 2004, Ralf Wirtz
//////////////////////////////////////////////////////////////////////

#pragma once

#include "KPhysConst.h"

#define ANZFUNKTKONST 10


namespace Math
{

	class KIND_EXT_CLASS CFormulaParser  
	{
	public:
		CFormulaParser();
		virtual ~CFormulaParser();

	public:
		void StripFormula(CString& strFormula);

		// 设置/获取公式
		LPCTSTR GetFormula();
		void SetFormula(CString Formula);

		// 设置常量 A,B,C,D,E,F,G,H
		void SetFunctConst(int index, double val);
		// 计算
		virtual double Calculation(CString strFormula, double xValue);
		virtual double Calculation(CString strFormula, double xValue, int& ErrorPosition, CString& Errortext, BOOL strip = true);

	private:
		double SignFactor(int& nPosition, CString& strCharacter);
		double Expression(int& nPosition, CString& strCharacter);
		double SimpleExpression(int& nPosition, CString& strCharacter);
		double Term(int& nPosition, CString& strCharacter);
		double Factor(int& nPosition, CString& strCharacter);
		double Char_n(int& nPosition, CString& strCharacter);
		CString strChar_(BYTE DecimalZahl);

		CString GetNextToken(CString& strSrc, const CString strDelim);
		double SINQ(double Winkel_grad);
		double COSQ(double Winkel_grad);
		double DEG(double x /* rad */) ;
		double RAD(double x /* grad */);
		double cot(double x);
		long double signl(long double x);
		double ArSinh(double x);
		double ArCosh(double x);
		double ArTanh(double x);
		double ArCoth(double x);
		double sqr(double x);

	private:
		CPhysConst m_PhysConst; 

		CString m_strFormula;
		CString m_strFunction;
		CString m_strErrortext;

		double m_dFktValue;
		double m_dFunctionConstant[ANZFUNKTKONST];
		CStringArray m_strStandardFunction;

	private:
		static CString g_strF;

	};

//---------------------------------------------------------
}// namespace Math - end
//---------------------------------------------------------
