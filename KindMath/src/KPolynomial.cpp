#include "StdAfx.h"
#include "KPolynomial.h"

KPolynomial::KPolynomial(void)
{
}


//��һάʵ����ʽp(x)��x����ֵ
// p(x) = a0+a1X+a2X^2+...+anX^n
// dCoff	-->ϵ������
// stN		-->�Ǹ�����,Ϊ����ʽ����,����ʽ��ߴ���ΪstN-1
// x		-->������xֵ
double KPolynomial::PolyValue1D(double* dCoff, size_t stN,double x)
{
	double dValue = dCoff[stN-1];
	for(int st = stN - 2; st > -1; st--)
		dValue = dValue * x + dCoff[st];
	return(dValue);			//���ض���ʽֵ
}

//���ά����ʽֵp(x,y)��(x,y)����ֵ
// mCoff	-->��ά����ʽϵ������(m*n)
// (dX,dY)	-->������(x,y)ֵ
double KPolynomial::PolyValue2D(const KMatrix& mCoff, double dX, double dY)
{
	double dValue = 0, dTemp = 1;

	size_t styNo = mCoff.GetColNum();//��ά����ʽ�Ա���y������������ߴ���ΪstyNo-1
	size_t stxNo = mCoff.GetRowNum();//��ά����ʽ�Ա���x������������ߴ���ΪstxNo-1

	for(size_t st = 0; st < stxNo; st++)
	{
		double dUS = mCoff(st, styNo-1) * dTemp;
		for(int sr = styNo-2; sr > -1; sr--)
			dUS = dUS * dY + mCoff(st, sr) * dTemp;
		dValue = dValue + dUS;
		dTemp = dTemp * dX;
	}
	return(dValue);		//���ض���ʽֵ
}

//��һά����ʽ���
// S(x)=P(x) * Q(x)
// dCoffP	-->P(x)���˶���ʽϵ������(mά),��ߴ���Ϊm-1
// dCoffQ	-->Q(x)�˶���ʽϵ������(nά),��ߴ���Ϊn-1
// ����
//   m+n-1�׷���,�Խ��ߴ�Ż�����ʽS(x)��ϵ��,��ߴ���Ϊm+n-2
KMatrix KPolynomial::PolyMultip(double* dCoffP,int m, double* dCoffQ,int n)
{
	int st;

	KMatrix ms(m+n-1,m+n-1);	//m+n-1�׷���,�Խ��ߴ�Ż�����ʽS(x)��ϵ��,��ߴ���Ϊm+n-2

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

//��һά����ʽ����
// P(x) / Q(x) = S(x) ... R(x)
// dCoffP	-->P(x)���˶���ʽϵ������(mά),��ߴ���Ϊm-1
// dCoffQ	-->Q(x)  �˶���ʽϵ������(nά),��ߴ���Ϊn-1
// matCoffS	-->�̶���ʽS(x)ϵ������(m-n+1�׷���),�Խ��ߴ���̶���ʽS(x)��ϵ��,��ߴ���Ϊm-n
// matCoffR	-->�����ʽR(x)ϵ������(  n-1�׷���),�Խ��ߴ�������ʽR(x)��ϵ��,��ߴ���Ϊn-2
int	KPolynomial::PolyDiv(double* dCoffP,int m, double* dCoffQ,int n, KMatrix& matCoffS, KMatrix& matCoffR)
{
	int st;

	matCoffS.SetSize(m-n+1,m-n+1);	//�̶���ʽS(x)ϵ������(m-n+1�׷���)
	matCoffR.SetSize(n-1,n-1);		//�����ʽR(x)ϵ������(  n-1�׷���)

	double* dCoffS = new double[m-n+1];			//�̶���ʽS(x)ϵ������
	ZeroMemory(dCoffS,(m-n+1)*sizeof(double));	//

	double* dCoffR = new double[n-1];			//�����ʽR(x)ϵ������
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
//��������ʽ����ֵ
// dXpara	-->nάdouble����,�������ʽ�и����Xk(k=0,1,2,...,n-1)��ֵ
// dCoff	-->nάdouble����,�������ʽ�и����Bk(k=0,1,2,...,n-1)��ֵ
// n		-->����ά��
// dX		-->����ʽ�Ա���xֵ
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
    return(dValue);				//��������ʽֵ
}
