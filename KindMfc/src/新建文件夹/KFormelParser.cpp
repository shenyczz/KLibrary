//////////////////////////////////////////////////////////////////////
// FormulaParser.cpp: implementation of the CFormulaParser class.
// Copyright: 2004, Ralf Wirtz
// 
// Usage:
// 
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "math.h"
#include "float.h"
#include "KFormelParser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#define PI				3.1415926535897932


namespace Math
{

	CString CFormulaParser::g_strF = _T("");


	CFormulaParser::CFormulaParser()
	{	
		m_strStandardFunction.Add(_T(""));
		m_strStandardFunction.Add(_T("PI"));
		m_strStandardFunction.Add(_T("ABS"));
		m_strStandardFunction.Add(_T("SQRT"));
		m_strStandardFunction.Add(_T("SINH"));
		m_strStandardFunction.Add(_T("COSH"));
		m_strStandardFunction.Add(_T("TANH"));
		m_strStandardFunction.Add(_T("ARCTAN"));
		m_strStandardFunction.Add(_T("LN"));
		m_strStandardFunction.Add(_T("LOG"));
		m_strStandardFunction.Add(_T("EXP"));
		m_strStandardFunction.Add(_T("SIN")); //mu?in der Reihenfolge nach SINH stehen!!!
		m_strStandardFunction.Add(_T("COS"));
		m_strStandardFunction.Add(_T("TAN"));
		m_strStandardFunction.Add(_T("COT"));
		m_strStandardFunction.Add(_T("ARCSIN"));
		m_strStandardFunction.Add(_T("ARCCOS"));
		m_strStandardFunction.Add(_T("INT"));
		m_strStandardFunction.Add(_T("RAD"));
		m_strStandardFunction.Add(_T("DEG"));
		m_strStandardFunction.Add(_T("SIGN"));
		m_strStandardFunction.Add(_T("ARSINH"));
		m_strStandardFunction.Add(_T("ARCOSH"));
		m_strStandardFunction.Add(_T("ARTANH"));
		m_strStandardFunction.Add(_T("KGV"));
		m_strStandardFunction.Add(_T("GGT"));
		CString str;
		for (int i = 1; i < 100; i++)
		{
			str = m_PhysConst.GetEachPhysConst(i);
			if (str == "***") break;
			m_strStandardFunction.Add(str);
		}
	}

	CFormulaParser::~CFormulaParser()
	{

	}

	//////////////////////////////////////////////////////////////////////
	// Methoden
	//////////////////////////////////////////////////////////////////////

	double CFormulaParser::SignFactor(int& nPosition, CString& strCharacter)
	{
		if (strCharacter == "-")
		{
			Char_n(nPosition, strCharacter);
			return (-1.0) * Factor(nPosition, strCharacter);
		}
		else 
		{
			return Factor(nPosition, strCharacter);
		}
	}

	void CFormulaParser::StripFormula(CString& strFormula)
	{
		int Level = 0;

		if (strFormula.GetLength() < 1) return;
	//	TRACE("Formel vorher: %s\n", strFormula);

		// Kommas durch Punkte ersetzen, alles runde Klammern
		strFormula.Replace(_T(" "), _T(""));
		strFormula.Replace(_T(","), _T("."));
		strFormula.Replace(_T("["), _T("("));
		strFormula.Replace(_T("]"), _T(")"));
		strFormula.Replace(_T("{"), _T("("));
		strFormula.Replace(_T("}"), _T(")"));

		strFormula.Replace(_T("*(1)"),_T(""));
		strFormula.Replace(_T("(1)*"),_T(""));

		strFormula.Replace(_T("*(x)"),_T("*x"));
		// geht nicht wegen sin(x)* strFormula.Replace(_T("(x)*"),_T("x*"));
		strFormula.Replace(_T("((x)*"),_T("(x*"));
		strFormula.Replace(_T("+(x)*"),_T("+x*"));
		strFormula.Replace(_T("-(x)*"),_T("-x*"));
		strFormula.Replace(_T("*(x)*"),_T("*x*"));
		strFormula.Replace(_T("(sin(x))"),_T("sin(x)"));
		strFormula.Replace(_T("(cos(x))"),_T("cos(x)"));
		strFormula.Replace(_T("(cot(x))"),_T("cot(x)"));
		strFormula.Replace(_T("(tan(x))"),_T("tan(x)"));
		strFormula.Replace(_T("(exp(x))"),_T("exp(x)"));
		strFormula.Replace(_T("(log(x))"),_T("log(x)"));
		strFormula.Replace(_T("(ln(x))"),_T("ln(x)"));

		// Einschließende Klammern beseitigen
		for (int i = 0; i < strFormula.GetLength(); i++)
		{
			switch (strFormula[i])
			{
				case '(': Level++; continue;
				case ')': Level--; continue;
			}
			if (Level == 0 && i < strFormula.GetLength() - 1)
			{
				Level = -1; // Markierung
				break;
			}
		}

		if (Level != -1)
		{
			while (strFormula[0] == '(' && strFormula[strFormula.GetLength() - 1] == ')')
			{
				strFormula = strFormula.Mid(1, strFormula.GetLength() - 2);
			}
		}

		// Führende und abschließende Leerzeichen und positive Vorzeichen entfernen
		while (strFormula[0]=='+' || strFormula[0]==' ') 
		{
			strFormula = strFormula.Mid(1);
		}
		strFormula.TrimRight();

		//Alle unnötigen Klammern beseitigen
		int Pos[1000];
		int j = 0;
		Level = 0;
		int l = strFormula.GetLength();

		for (int i = 0; i < l; i++)
		{
			if (strFormula[i] == '(')
			{
				if (i == 0 || (i > 0 && (strFormula[i-1] == '(' || strFormula[__min(i+1,l-1)] == '(')))
				{
					Level++;
					Pos[++j] = i;
				}
			}
			else if (strFormula[i] == ')')
			{
				Level--;
				if (Level > 0 && strFormula[i+1] == ')')
				{
					//rechte Klammer
					CString left = strFormula.Left(i);
					CString mid = strFormula.Mid(i+1);
					strFormula = left + "|" + mid;

					//linke Klammer
					left = strFormula.Left(Pos[Level] + 1);
					mid = strFormula.Mid(Pos[Level+1] + 1);
					strFormula = left + "|" + mid;

					j = 0;
				}
			}
		}
		strFormula.Replace(_T("|"), _T(""));
		//TRACE("Formel nachher: %s\n", strFormula);
	}

	double CFormulaParser::Calculation(CString strFormula, double xValue)
	{
		int ErrorPosition;
		CString Errortext = _T("");
		return Calculation(strFormula,xValue,ErrorPosition,Errortext,true);
	}

	double CFormulaParser::Calculation(CString strFormula, double xValue, int& ErrorPosition, CString& Errortext, BOOL strip)
	{
		int nPosition;
		CString strCharacter;
		double	ergebnis;
		char buffer[32];

		if (strFormula.GetLength() < 1)
			return 0.0;

		m_strErrortext.Empty();

		try
		{
			if (strip)
				StripFormula(strFormula);

			m_strFunction = strFormula;
			m_dFktValue = xValue;
			if (m_dFktValue == 0)	
				m_dFktValue = FLT_MIN;

			nPosition = 0;
			Char_n(nPosition, strCharacter);

			ergebnis = Expression(nPosition, strCharacter);

			if (strCharacter == strChar_(0))
			{
				Errortext = m_strErrortext;
				ErrorPosition = 0;
			}
			else
			{
				ErrorPosition = nPosition;
				sprintf(buffer, "Error found on position %d!", (int) ErrorPosition);
				Errortext = buffer;
				Errortext += m_strErrortext;
			}

			return ergebnis;
		}
		catch (CException* ex)
		{
			TCHAR szCause[255];        
			ex->GetErrorMessage(szCause, 255);
			Errortext = _T("System Error: ");
			Errortext += szCause;        
			return 0;
		}
	}

	double CFormulaParser::Expression(int& nPosition, CString& strCharacter)
	{
	  CString strOperator;
	  double erg = SimpleExpression(nPosition, strCharacter);
	  while (strCharacter == "+" || strCharacter == "-")
	  {
		strOperator = strCharacter;
		Char_n(nPosition, strCharacter);
		if (strOperator == "+")
			  erg += SimpleExpression(nPosition, strCharacter);
			else if (strOperator == "-")
			  erg -= SimpleExpression(nPosition, strCharacter);
		}
	  return erg;
	}

	double CFormulaParser::SimpleExpression(int& nPosition, CString& strCharacter)
	{
	  double s,dum;
		CString strOperator;
	  s = Term(nPosition, strCharacter);
	  while (strCharacter == "*" || strCharacter == "/")
		{
		strOperator = strCharacter;
		Char_n(nPosition, strCharacter);
		if (strOperator == "*")
				s = s * Term(nPosition, strCharacter);
		else if (strOperator == "/") 
			{
				dum = Term(nPosition, strCharacter);
				if (dum != 0)  
					s = s / dum;
				else
					m_strErrortext = " Divide by zero!";
			}  
		}
	  return s;
	}

	double CFormulaParser::Term(int& nPosition, CString& strCharacter)
	{
	  double t,vz;
	  t = SignFactor(nPosition, strCharacter);
	  while (strCharacter == "^")
	  {
		  Char_n(nPosition, strCharacter);
		vz = SignFactor(nPosition, strCharacter);
    
			if ((t <= 0 && fabs(vz) <= 1) || (t <= 0 && vz != int(vz)))
				m_strErrortext = " No SQRT from neg. values!";
		else 
				t = pow(t,vz);		
		}
	  return t;
	}

	double CFormulaParser::Char_n(int& nPosition, CString& strCharacter)
	{
	  do
		{
		nPosition ++;	// evtl. nach strCharacter
			if (nPosition <= m_strFunction.GetLength())
				//war geändert auf:  strCharacter = MID(m_strFunction, nPosition, 1);
				strCharacter = m_strFunction.Mid(nPosition - 1, 1);			
			else 
				strCharacter = strChar_(0);

		//	TRACE("strCharacter= "+ strCharacter);
 		}
	  while (strCharacter == " "); 

		return nPosition;
	}

	void CFormulaParser::SetFormula(CString Formula)
	{
		m_strFormula = Formula;
	}

	LPCTSTR CFormulaParser::GetFormula()
	{
		return (LPCTSTR)m_strFormula;
	}

	double CFormulaParser::Factor(int& nPosition, CString& strCharacter)
	{
	  double f = 0.0;
		int wI = 0, wL = 0, wBeginn = 0, wError = 0;

		if	(strCharacter == strChar_(0)) return 0.0;

		if (strCharacter >= _T("0") && strCharacter <= _T("9") || strCharacter == _T("."))
		{
			wBeginn = nPosition;

			do
			{
				Char_n(nPosition, strCharacter);
			} 
			while ((strCharacter >= _T("0") && strCharacter <= _T("9") || strCharacter == _T(".")));
			
			if (strCharacter == ".")
			{
				do
				{
					Char_n(nPosition, strCharacter);
				} 
				while (!(((BYTE)strCharacter[0] >= (BYTE)"0") && ((BYTE)strCharacter[0] <=  (BYTE)"9")  || ((BYTE)strCharacter[0] == (BYTE)".")));
			}
			g_strF = m_strFunction.Mid(wBeginn - 1, nPosition - wBeginn);
			f = _tstof(g_strF);
		
		} 
		else
		{
			CString strCharacterUpper = strCharacter;
			strCharacterUpper.MakeUpper();
			if (strCharacter == "(")
			{
				Char_n(nPosition, strCharacter);
				f = Expression(nPosition, strCharacter);
				if (strCharacter == ")")
					Char_n(nPosition, strCharacter);
			}
			else if (strCharacterUpper == "X")
			{
				Char_n(nPosition, strCharacter);
				f = m_dFktValue;
			}
			else
			{
				BOOL gefunden = false;
				int AnzStdFunctions = m_strStandardFunction.GetSize() - 1;
				for (wI = 1; wI <= AnzStdFunctions; wI++)
				{
					wL = m_strStandardFunction[wI].GetLength();
					CString strFunktionUpper = m_strFunction.Mid(nPosition - 1, wL);
					strFunktionUpper.MakeUpper();
					CString strDummy(m_strStandardFunction[wI]);
					strDummy.MakeUpper();
					if (strFunktionUpper == strDummy)
					{
						gefunden = true;
						nPosition = nPosition + wL - 1;
						Char_n(nPosition, strCharacter);
						//! Rekursion !!!!!!!!!!!!!!!!!!!!!!
						f = Factor(nPosition, strCharacter);
						//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
						if (strFunktionUpper == "ABS")
							f = fabs(f);
						else if (strFunktionUpper == "SQRT")
							if (f >= 0)
								f = sqrt(f);
							else
								wError = -1;
						else if (strFunktionUpper == "SINH")
							f = sinh(f);
						else if (strFunktionUpper == "COSH")
							f = cosh(f);
						else if (strFunktionUpper == "TANH")
							f = tanh(f);
						else if (strFunktionUpper == "ARCTAN")
							f = atan(f);
						else if (strFunktionUpper == "LN")
						{
							if (f >= 0)
								f = log(f);
							else
								wError = -1;
						}
						else if (strFunktionUpper == "LOG")
						{
							if (f >= 0)
								f = log10(f);
							else
								wError = -1;
						}
						else if (strFunktionUpper == "EXP")
						{
							if (f <= 41)
								f = exp(f);
							else
								wError = -1;
						}
						else if (strFunktionUpper == "SIN")
							f = sin(f);
						else if (strFunktionUpper == "COS")
							f = cos(f);
						else if (strFunktionUpper == "COT")
							f = cot(f);
						else if (strFunktionUpper == "TAN")
						{
							if (cos(f) != 0)
								f = tan(f);
							else 
								wError = -1;
						}
						else if (strFunktionUpper == "ARCSIN")
						{
							if (fabs(f) < 1) 
								f = asin(f);
							else
								wError = -1;
						}
						else if (strFunktionUpper == "ARCCOS")
						{
							if (fabs(f) <= 1)
								f = acos(f);
							else 
								wError = -1;
						}
						else if (strFunktionUpper == "INT") 
							f = int(f);
						else if (strFunktionUpper == "SIGN") 
							f = signl(f);
						else if (strFunktionUpper == "PI") 
							f = 3.14159265358979323846;
						else if (strFunktionUpper == "RAD") 
							f = RAD(f);
						else if (strFunktionUpper == "DEG") 
							f = DEG(f);
						else if (strFunktionUpper == "ARSINH") 
							f = ArSinh(f);
						else if (strFunktionUpper == "KGV") 
						{
							;
						}
						else if (strFunktionUpper == "GGT") 
						{
							;
						}
						else if (strFunktionUpper == "ARCOSH")
						{
							if (fabs(f) >= 1)
								f = ArCosh(f);
							else 
								wError = -1;
						}					
						else if (strFunktionUpper == "ARTANH")
						{
							if (fabs(f) <= 1)
								f = ArTanh(f);
							else 
								wError = -1;
						}
						else if (double pc = m_PhysConst.GetPhysConstValue(strFunktionUpper))
						{
							f = pc;
						}
						break;
					}
				}
				if (!gefunden)
				{
					if (strCharacterUpper >= _T("A") && strCharacterUpper <= _T("H"))
					{
						Char_n(nPosition, strCharacter);
						BYTE b = BYTE(strCharacterUpper[0]) - 64;
						f = m_dFunctionConstant[b];
					}
				}
			}
		}
	
		if (wError == -1) 
			m_strErrortext = " General Parser Error blocked!";

	  return f;
	}

	void CFormulaParser::SetFunctConst(int index, double val)
	{
		if (index >= 1 && index < 10) //zwischen 0 und 9
			m_dFunctionConstant[index] = val;
		else
			AfxMessageBox(_T("ProgrammError in SetFunctConst()"));
	}

	CString CFormulaParser::strChar_(BYTE DecimalZahl)
	{
		CString string;
		string.Format(_T("%c"), DecimalZahl);
		return string;
	}

	double CFormulaParser::SINQ(double Winkel_grad)
	{
		double Winkel_rad = PI * Winkel_grad / 180.0;
		return sin(Winkel_rad);
	}

	double CFormulaParser::COSQ(double Winkel_grad)
	{
	  // const float PI = 3.141592654f;    
		double Winkel_rad = PI * Winkel_grad / 180.0;
		return cos(Winkel_rad);
	}

	double CFormulaParser::cot(double x)
	{
		return cos(x)/sin(x);
	}

	double CFormulaParser::DEG(double x /* rad */) 
	{
		// liefert den Winkel eines arithmetischen Ausdrucks im Gradma?
		return x * 180.0 / PI;
	}

	double CFormulaParser::RAD(double x /* grad */) 
	{
		// liefert das Bogenma?eines im Gradma?vorliegenden Winkels.
		return x * PI / 180.0;
	}

	CString CFormulaParser::GetNextToken(CString& strSrc, const CString strDelim)
	{
		CString token;
		int idx = strSrc.FindOneOf(strDelim);
		if(idx != -1)
		{
			token  = strSrc.Left(idx);
			strSrc = strSrc.Right(strSrc.GetLength() - (idx + 1) );
		}
		else
		{
			token = strSrc;
			strSrc.Empty();
		}
		return token;
	}

	long double CFormulaParser::signl(long double x)
	{
		if (x > 0.0L) return 1.0L;
		if (x < 0.0L) return -1.0L;
		return 0.0L;
	}

	double CFormulaParser::ArSinh(double x)
	{
		if (x < 0)
			return -log(-x + sqrt(sqr(-x) + 1));
		return log(x + sqrt(sqr(x) + 1));
	}

	double CFormulaParser::ArCosh(double x)
	{
		return log(x + sqrt(sqr(x) - 1));
	}

	double CFormulaParser::ArTanh(double x)
	{
		return 0.5*logl((1 + x)/ (1 - x));
	}

	double CFormulaParser::ArCoth(double x)
	{
		return 0.5*log((x + 1)/ (x - 1));
	}

	double CFormulaParser::sqr(double x)    
	{ 
		return x*x;
	}

//---------------------------------------------------------
}// namespace Math - end
//---------------------------------------------------------
