#include "StdAfx.h"
#include "KPolynomial.h"

KPolynomial::KPolynomial(void)
{
}


//求一维实多项式p(x)在x处的值
// p(x) = a0+a1X+a2X^2+...+anX^n
// dCoff	-->系数数组
// stN		-->非负整数,为多项式项数,多项式最高次数为stN-1
// x		-->待求解的x值
double KPolynomial::PolyValue1D(double* dCoff, size_t stN,double x)
{
	double dValue = dCoff[stN-1];
	for(int st = stN - 2; st > -1; st--)
		dValue = dValue * x + dCoff[st];
	return(dValue);			//返回多项式值
}

//求二维多项式值p(x,y)在(x,y)处的值
// mCoff	-->二维多项式系数矩阵(m*n)
// (dX,dY)	-->待求解的(x,y)值
double KPolynomial::PolyValue2D(const KMatrix& mCoff, double dX, double dY)
{
	double dValue = 0, dTemp = 1;

	size_t styNo = mCoff.GetColNum();//二维多项式自变量y的项数，其最高次数为styNo-1
	size_t stxNo = mCoff.GetRowNum();//二维多项式自变量x的项数，其最高次数为stxNo-1

	for(size_t st = 0; st < stxNo; st++)
	{
		double dUS = mCoff(st, styNo-1) * dTemp;
		for(int sr = styNo-2; sr > -1; sr--)
			dUS = dUS * dY + mCoff(st, sr) * dTemp;
		dValue = dValue + dUS;
		dTemp = dTemp * dX;
	}
	return(dValue);		//返回多项式值
}

//两一维多项式相乘
// S(x)=P(x) * Q(x)
// dCoffP	-->P(x)被乘多项式系数数组(m维),最高次数为m-1
// dCoffQ	-->Q(x)乘多项式系数数组(n维),最高次数为n-1
// 返回
//   m+n-1阶方阵,对角线存放积多项式S(x)的系数,最高次数为m+n-2
KMatrix KPolynomial::PolyMultip(double* dCoffP,int m, double* dCoffQ,int n)
{
	int st;

	KMatrix ms(m+n-1,m+n-1);	//m+n-1阶方阵,对角线存放积多项式S(x)的系数,最高次数为m+n-2

	double* dCoffS = new double[m+n-1];
	ZeroMemory(dCoffS,(m+n-1)*sizeof(double));

	for( st = 0; st < m; st++)
	{
		for(int sr = 0; sr < n; sr++)
		{
			dCoffS[st+sr]=dCoffS[st+sr]+dCoffP[st]*dCoffQ[sr];
		}
	}

	for(st=0;st<m+n-1;st++)
	{
		ms(st,st)=dCoffS[st];
	}

	delete []dCoffS;
	//---
	return ms;
}

//两一维多项式除法
// P(x) / Q(x) = S(x) ... R(x)
// dCoffP	-->P(x)被乘多项式系数数组(m维),最高次数为m-1
// dCoffQ	-->Q(x)  乘多项式系数数组(n维),最高次数为n-1
// matCoffS	-->商多项式S(x)系数矩阵(m-n+1阶方阵),对角线存放商多项式S(x)的系数,最高次数为m-n
// matCoffR	-->余多项式R(x)系数矩阵(  n-1阶方阵),对角线存放余多项式R(x)的系数,最高次数为n-2
int	KPolynomial::PolyDiv(double* dCoffP,int m, double* dCoffQ,int n, KMatrix& matCoffS, KMatrix& matCoffR)
{
	int st;

	matCoffS.SetSize(m-n+1,m-n+1);	//商多项式S(x)系数矩阵(m-n+1阶方阵)
	matCoffR.SetSize(n-1,n-1);		//余多项式R(x)系数矩阵(  n-1阶方阵)

	double* dCoffS = new double[m-n+1];			//商多项式S(x)系数数组
	ZeroMemory(dCoffS,(m-n+1)*sizeof(double));	//

	double* dCoffR = new double[n-1];			//余多项式R(x)系数数组
	ZeroMemory(dCoffR,(n-1)*sizeof(double));

	if(fabs(dCoffQ[n-1])<1.0e-12) return (0);
	
	int stk = m - 1;
	
	for( st = m-n+1; st > 0; st--)
	{
		dCoffS[st-1] = dCoffP[stk] / dCoffQ[n-1];
		size_t stm = stk;
		for(int sr = 1; sr < n; sr++)
		{
			dCoffP[stm-1] = dCoffP[stm-1] - dCoffS[st-1] * dCoffQ[n-sr-1];
			stm = stm-1;
		}
		stk = stk - 1;
	}
	for(st = 0; st < (n-1); st++) dCoffR[st] = dCoffP[st];
	//---
	for(st=0;st<m-n+1;st++)
	{
		matCoffS(st,st)=dCoffS[st];
	}

	for(st=0;st<n-1;st++)
	{
		matCoffR(st,st)=dCoffR[st];
	}

	
	//---
	delete []dCoffS;
	delete []dCoffR;
	//---
	return (1);
}
//计算连分式函数值
// dXpara	-->n维double数组,存放连分式中各结点Xk(k=0,1,2,...,n-1)的值
// dCoff	-->n维double数组,存放连分式中各结点Bk(k=0,1,2,...,n-1)的值
// n		-->数组维数
// dX		-->连分式自变量x值
double KPolynomial::FractionValue(double* dXpara, double* dCoff, int n,double dX)
{
	double dValue = dCoff[n-1];

	for(int st=n-2; st>-1; st--)
	{
		if (fabs(dValue)<1.0e-12)	//|dValue|==0
			dValue = 1.0e+35 * (dX-dXpara[st]) / fabs(dX-dXpara[st]);
        else
			dValue = dCoff[st] + (dX-dXpara[st]) / dValue;
    }
    return(dValue);				//返回连分式值
}
