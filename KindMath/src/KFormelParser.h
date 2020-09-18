//////////////////////////////////////////////////////////////////////
// KFormulaParser.h: interface for the KFormulaParser class.
// Copyright: 2004, Ralf Wirtz
//////////////////////////////////////////////////////////////////////

#pragma once

#include "KPhysConst.h"

#define ANZFUNKTKONST 10


namespace Math
{

	class KIND_EXT_CLASS KFormulaParser  
	{
	public:
		KFormulaParser();
		virtual ~KFormulaParser();

	public:
		void StripFormula(KString& strFormula);

		// 设置/获取公式
		LPCTSTR GetFormula();
		void SetFormula(KString Formula);

		// 设置常量 A,B,C,D,E,F,G,H
		void SetFunctConst(int index, double val);
		// 计算
		virtual double Calculation(KString strFormula, double xValue);
		virtual double Calculation(KString strFormula, double xValue, int& ErrorPosition, KString& Errortext, BOOL strip = true);

	private:
		double SignFactor(int& nPosition, KString& strCharacter);
		double Expression(int& nPosition, KString& strCharacter);
		double SimpleExpression(int& nPosition, KString& strCharacter);
		double Term(int& nPosition, KString& strCharacter);
		double Factor(int& nPosition, KString& strCharacter);
		double Char_n(int& nPosition, KString& strCharacter);
		KString strChar_(BYTE DecimalZahl);

		KString GetNextToken(KString& strSrc, const KString strDelim);
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
		KPhysConst m_PhysConst; 

		KString m_strFormula;
		KString m_strFunction;
		KString m_strErrortext;

		double m_dFktValue;
		double m_dFunctionConstant[ANZFUNKTKONST];
		KStringArray m_strStandardFunction;

	private:
		//static CString g_strF;
		static KString g_strF;

	};

//---------------------------------------------------------
}// namespace Math - end
//---------------------------------------------------------
