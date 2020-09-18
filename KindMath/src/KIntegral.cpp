#include "StdAfx.h"
#include "KIntegral.h"

KIntegral::KIntegral(void)
{
}

//------------------------------------------------------------------
//�䲽�����η����
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::Trapezia(double a, double b, double (*pFun)(double),double eps)
{
	if(!pFun) throw;	// ����ָ��

	double t;
    double fa = (*pFun)(a);	//f(a)
	double fb = (*pFun)(b);	//f(b)
    int n(1);
	double h = b - a;
    double t1 = h * (fa + fb) / 2.0;
    double p = eps + 1.0;
    while(p > eps || fabs(p-eps) < 1.0e-12)	//p==eps
    {
		double s(0);
        for(int k=0; k<n; k++)
        {
			double x = a + (k + 0.5) * h;
            s = s + (*pFun)(x);
          }
        t = (t1 + h * s) / 2.0;
        p = fabs(t1 - t);
        t1 = t;
		n = n + n;
		h = h / 2.0;
    }
    return(t);
}

//�䲽�������������
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::Simpson(double a, double b, double (*pFun)(double),double eps)
{
    double s2;
    int n(1);
	double h = b - a;			//����
    double t1 = h * ((*pFun)(a) + (*pFun)(b)) / 2.0;
    double s1 = t1;
    double ep = eps + 1.0;
    while(ep > eps || fabs(ep-eps)<1.0e-12)	//ep==eps
    {
		double p(0);
        for(int k=0; k<n; k++)
        {
			double x = a + (k + 0.5) * h;
            p = p + (*pFun)(x);
        }
        double t2 = (t1 + h * p) / 2.0;
        s2 = (4.0 * t2 - t1) / 3.0;
        ep = fabs(s2 - s1);
        t1 = t2;
		s1 = s2;
		n = n + n;
		h = h / 2.0;
    }
    return(s2);
}

//����Ӧ���η����
// ���ڼ��㱻������f(x)�ڻ���������ǿ��Ķ�����
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// d	-->�Ի���������зָ����С����
//         ����������С��dʱ,��ʹû�����㾫��Ҫ��Ҳ���ټ����ָ�
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
//
double KIntegral::TrapeziaSelfAdapt(double a, double b, double (*pFun)(double),double d,double eps)
{
    double t[2];

    double h = b - a;
	t[0]=0.0;
    double f0 = (*pFun)(a);
	double f1 = (*pFun)(b);
    double t0 = h * (f0 + f1) / 2.0;
    _itsa(a,b,h,f0,f1,t0,t,d,eps,pFun);
    double z = t[0];

	//---
	return(z);
}
//����Ӧ���η������������
int KIntegral::_itsa(double x0, double x1, double h, double f0, double f1, double t0, double t[], double d, double eps, double (*pFun)(double))
{
    double x = x0 + h / 2.0;
	double f = (*pFun)(x);
    double t1=h*(f0+f)/4.0;
	double t2=h*(f+f1)/4.0;
    double p=fabs(t0-(t1+t2));
    if((p<eps)||(h/2.0<d))
	{
		t[0]=t[0]+(t1+t2);
		return(0);
	}
    else
    {
		double g=h/2.0; 
		double eps1=eps/1.4;
        _itsa(x0,x,g,f0,f,t1,t,d,eps1,*pFun);	//��*pFun
        _itsa(x,x1,g,f,f1,t2,t,d,eps1,pFun);	//��pFun��һ��
        return(1);
    }
}

//���������
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::Romberg(double a, double b, double (*pFun)(double),double eps)
{
    double y[10], q;
    double h = b - a;			//����
    y[0] = h * ((*pFun)(a) + (*pFun)(b)) / 2.0;
    int m(1), n(1);
	double ep=eps+1.0;
    while((ep>eps || fabs(ep-eps)<1.0e-12) && (m <= 9))
    {
		double p(0);
        for(int i=0;i<n;i++)
        {
			double x = a + (i + 0.5) * h;
            p = p + (*pFun)(x);
        }
        p = (y[0] + h * p) / 2.0;
        double s(1);
        for(int k=1; k<=m; k++)
        {
			s = 4.0 * s;
            q = (s * p - y[k-1]) / (s - 1.0);
            y[k-1] = p;
			p = q;
        }
        ep = fabs(q - y[m-1]);
        m = m + 1;
		y[m-1] = q; 
		n = n + n;
		h = h / 2.0;
    }
    return(q);
}

//һά����ʽ�����
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::Fraction(double a, double b, double (*pFun)(double),double eps)
{
    double h[10], bb[10];
    int m(1), n(1), k, l, j;
    double g, hh = b - a;
	h[0] = hh;
    double t1 = hh * ((*pFun)(a) + (*pFun)(b)) / 2.0;
    double s1 = t1;
	bb[0]=s1;
	double ep = 1.0 + eps;
    while((ep>eps || fabs(ep-eps)<1.0e-12) && (m <= 9))
    { 
		double s(0);
        for(k=0; k<n; k++)
        {
			double x = a + (k + 0.5) * hh;
            s = s + (*pFun)(x);
        }
        double t2 = (t1 + hh * s) / 2.0;
        m++;
        h[m-1] = h[m-2] / 2.0;
        g = t2;
        l = 0;
		j = 2;
        while((l==0) && (j<=m))
        {
			s = g - bb[j-2];
            if(fabs(s)<1.0e-12) l = 1;
            else g = (h[m-1] - h[j-2]) / s;
            j++;
        }
        bb[m-1] = g;
        if(l!=0) bb[m-1] = 1.0e+35;
        g = bb[m-1];
        for(j=m; j>=2; j--)	g = bb[j-2] - h[j-2] / g;
        ep = fabs(g-s1);
        s1 = g;
		t1 = t2;
		hh = hh / 2.0;
		n = n + n;
    }
    return(g);
}

//���񵴺��������
// F(x) = f(x)cosmx+jf(x)sinmx
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// m	-->�����������񵴺����Ľ�Ƶ��
// da	-->һά���飬���f(x)�ڻ�������˵�x=a���ĸ��׵���ֵ
// db	-->һά���飬���f(x)�ڻ�������˵�x=b���ĸ��׵���ֵ
// n	-->����da��db�ĳ���
// s	-->һά����,����=2;
//         s[0]���f(x)cosmx��[a,b]�ϵĻ���ֵ
//         s[1]���f(x)sinmx��[a,b]�ϵĻ���ֵ
void KIntegral::Surge(double a, double b, int m, double* da,double* db,int n,double s[2])
{
    double sa[4],sb[4],ca[4],cb[4];

	//int n = fa.size();		//���������������˵��ϵĵ�����߽���+1
	int mm=1;

    double sma=sin(m*a);
	double smb=sin(m*b);
    double cma=cos(m*a);
	double cmb=cos(m*b);
    
	sa[0] = ca[3] = sma;
	sa[1] = ca[0] = cma;
	sa[2] = ca[1] = -sma; 
	sa[3] = ca[2] = -cma;
    sb[0] = cb[3] = smb;
	sb[1] = cb[0] = cmb;
	sb[2] = cb[1] = -smb;
	sb[3] = cb[2] = -cmb;

    s[0] = s[1] = 0.0; 

    for(int k=0; k<n; k++)
    {
		int j = k;
        while(j>=4) j = j - 4;
        mm = mm * m;
        s[0] = s[0]+(db[k]*sb[j]-da[k]*sa[j])/(1.0*mm);
        s[1] = s[1]+(db[k]*cb[j]-da[k]*ca[j])/(1.0*mm);
    }
    
	s[1] = -s[1];
}

//���õ�-��˹�����
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::LegendreGauss(double a, double b, double (*pFun)(double),double eps)
{
    double t[5] = 
	{
		-0.9061798459, -0.5384693101, 0.0, 0.5384693101, 0.9061798459
	};
    
	double c[5] = 
	{
		0.2369268851, 0.4786286705, 0.5688888889, 0.4786286705, 0.2369268851
	};

    int m(1);
	double g;
    double h = b - a;
	double s = fabs(0.001 * h);
    double p = 1.0e+35;
	double ep = eps + 1.0;

    while((ep > eps || fabs(ep-eps)<1.0e-12) && (fabs(h) > s))
    {
		g = 0;
        for(int i=1; i<=m; i++)
        {
			double aa = a + (i - 1.0) * h;
			double bb = a + i * h;
            double w(0);
            for(int j=0; j<5; j++)
            {
				double x = ((bb - aa) * t[j] + (bb + aa)) / 2.0;
                w = w + (*pFun)(x) * c[j];
            }
            g += w;
        }
        g = g * h / 2.0;
        ep = fabs(g - p) / (1.0 + fabs(g));
        p = g;
		m++;
		h = (b - a) / m;
    }
    return(g);
}

//���Ƕ�-��˹�����
//�������������[0,...)�ϵĻ���
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
double KIntegral::LaguerreGauss(double (*pFun)(double))
{
	double x;
	
	double dValue = 0.0;

    long double t[5] = 
	{ 
		0.26355990, 1.41340290, 3.59642600, 7.08580990, 12.64080000
	};
    
	long double c[5] = 
	{
		0.6790941054, 1.638487956, 2.769426772, 4.315944000, 7.104896230
	};
    
	for(int i=0; i<5; i++)
    {
		x = t[i];
		dValue = dValue + c[i] * (*pFun)(x); 
	}

	return dValue;
}

//��������-��˹�����
//������������[...,...)�ϵĻ���
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
double KIntegral::HermiteGauss(double (*pFun)(double))
{
    double t[5] = 
	{
		-2.02018200, -0.95857190,	0.0, 0.95857190, 2.02018200
	};
    
	double c[5] = 
	{
		1.181469599, 0.9865791417,0.9453089237, 0.9865791417,1.181469599
	};
    
	double dValue = 0;
    
	for(int i=0; i<5; i++)
    {
		double x = t[i];
		dValue = dValue + c[i] * (*pFun)(x);
	}

	return dValue;
}

//�б�ѩ�����
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::Chebyshev(double a, double b, double (*pFun)(double),double eps)
{
	double t[5]={-0.8324975, -0.3745414, 0.0, 0.3745414, 0.8324975};
	int m(1);
	double g;
    double h = b - a;
	double d = fabs(0.001 * h);
    double p = 1.0e+35;
	double ep = eps + 1.0;
    while((ep > eps || fabs(ep-eps)<1.0e-12) && (fabs(h) > d))
    {
		g = 0.0;
        for(int i=1; i<=m; i++)
        {
			double aa = a + (i - 1.0) * h;
			double bb = a + i * h;
            double s(0);
			for(int j=0; j<5; j++)
            {
				double x = ((bb - aa) * t[j] + (bb + aa)) / 2.0;
                s = s + (*pFun)(x);
            }
            g += s;
        }
        g = g * h / 5.0;
        ep = fabs(g - p) / (1.0 + fabs(g));
        p = g;
		m++;
		h = (b - a) / m;
    }
    return(g);
}

//һά���ؿ��巨���
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x)��ָ��
//         ���㱻������f(x)ֵ�ĺ�����Ҫ�û����ݾ����f(x)��д,�Ѻ�������Ϊ
//         ʵ�δ���
double KIntegral::MonteCarlo(double a, double b,double (*pFun)(double))
{
    double r(1), s(0), d(10000);

    for(int m=0; m<10000; m++)
    {
		double x = a + (b - a) * KRandom::Rand_01_One(r);	//ȡ�����

        s = s + (*pFun)(x) / d;		//���ñ�������ֵ
    }
    
	s = s * (b - a);
    
	return(s);
}

//���ػ��ֱ䲽�������������
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x,y)��ָ��
//         ���㱻������f(x,y)ֵ�ĺ�����Ҫ�û����ݾ����f(x,y)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// pFunb-->�����޺�����ָ��
//         ������Ҫ�û����ݾ��������д,�Ѻ�������Ϊ
//         ʵ�δ���; Ҫ�� ���޺���ֵ>���޺���ֵ
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::Simpson2D(double a, double b,
							   double (*pFun)(double,double),
							   void (*pFunb)(double,double*),
							   double eps)
{
	int n(1);
	double h = 0.5 * (b - a);			//����
	double d = fabs((b - a) * 1.0e-06);
    double s1 = _IntegralSimp2(a, eps,pFun,pFunb);	//����IntegralSimp2����
	double s2 = _IntegralSimp2(b, eps,pFun,pFunb);
    double t1 = h * (s1 + s2);
    double s0(1.0e+35), s;
	double ep = eps + 1.0;
    while((ep>eps||fabs(ep-eps)<1.0e-12)&&((fabs(h)>d)||(n<16)))
	{
		double x = a - h;
		double t2 = 0.5 * t1;
        for(int j=1; j<=n; j++)
        {
			x = x + 2.0 * h;
            t2 = t2 + h * _IntegralSimp2(x,eps,pFun,pFunb);
        }
        s = (4.0 * t2 - t1) / 3.0;
        ep = fabs(s - s0) / (1.0 + fabs(s));
        n = n + n;
		s0 = s;
		t1 = t2; 
		h = h * 0.5;
    }
    return(s);
}

//���ر䲽���������������������
double KIntegral::_IntegralSimp2(double x,double eps,
									double (*pFun)(double,double),
									void (*pFunb)(double,double*))
{
    double y[2];
	int n(1);
	(*pFunb)(x,y);
    double h = 0.5 *(y[1] - y[0]);
    double d = fabs(h * 2.0e-06);
    double t1 = h * ((*pFun)(x, y[0]) + (*pFun)(x, y[1]));
    double ep = 1.0 + eps;
	double g0(1.0e+35), g;
    while((ep>eps||fabs(ep-eps)<1.0e-12)&&((fabs(h)>d)||(n<16)))
    {
		double yy = y[0] - h;
        double t2 = 0.5 * t1;
        for(int i=1; i<=n; i++)
        {
			yy = yy + 2.0 * h;
            t2 = t2 + h * (*pFun)(x, yy);
        }
        g = (4.0 * t2 - t1) / 3.0;
        ep = fabs(g - g0) / (1.0 + fabs(g));
        n = n + n;
		g0 = g;
		t1 = t2;
		h = 0.5 * h;
    }
    return(g);
}

//���ظ�˹�����
// n	-->n�ػ���
// js	-->һά���飬����Ϊn; js[i]��ʾ��i��������������ֵ����������
// pFun	-->��������f(x0,x1,...,xn)��ָ��
//         ���㱻������f(x0,x1,...,xn)ֵ�ĺ�����Ҫ�û����ݾ����f(x0,x1,...,xn)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// pFunb-->�����޺�����ָ��; j��ʾ��j�����
//         ������Ҫ�û����ݾ��������д,�Ѻ�������Ϊ
//         ʵ�δ���; Ҫ�� ���޺���ֵ>���޺���ֵ
//         
double KIntegral::GaussMD(int n,double* js,double (*pFun)(double*,int),void (*pFunGMD)(int j,double*,double*))
{
	int j;
    double y[2];
	double p, s;
    double t[5] = 
	{
		-0.9061798459,-0.5384693101,0.0, 0.5384693101,0.9061798459
	};
    
	double c[5] = 
	{
		0.2369268851,0.4786286705,0.5688888889, 0.4786286705,0.2369268851
	};
	
	//int n = js.size();		//��������

	int* is = new int[2*(n+1)];
	double* x = new double[n];
	double* a = new double[2*(n+1)];
	double* b = new double[(n+1)];

	int m(1), l(1);
    
	a[n] = a[2*n+1] = 1.0;

    while(l==1)
    {
		for( j=m; j<=n; j++)
        {
			(*pFunGMD)(j-1,x,y);
            a[j-1]=0.5*(y[1]-y[0])/js[j-1];
            b[j-1]=a[j-1]+y[0];
            x[j-1]=a[j-1]*t[0]+b[j-1];
            a[n+j]=0.0;
            is[j-1]=1; is[n+j]=1;
        }
        j = n;
		int q = 1;
        while(q==1)
        {
			int k = is[j-1];
            if(j==n) p = (*pFun)(x,n);
            else p = 1.0;
            a[n+j]=a[n+j+1]*a[j]*p*c[k-1]+a[n+j];
            is[j-1] = is[j-1] + 1;
            if(is[j-1] > 5)
              if(is[n+j] >= js[j-1])
              {
				  j = j - 1;
				  q = 1;
                  if(j==0)
                  {
					  s = a[n+1] * a[0]; 
                      //return(s);
					  goto END;
                  }
              }
              else
              {
				  is[n+j]=is[n+j]+1;
                  b[j-1]=b[j-1]+a[j-1]*2.0;
                  is[j-1]=1; k=is[j-1];
                  x[j-1]=a[j-1]*t[k-1]+b[j-1];
                  if(j==n) q = 1;
                  else q = 0;
              }
            else
            {
				k = is[j-1];
                x[j-1] = a[j-1]*t[k-1]+b[j-1];
                if(j==n) q = 1;
                else q = 0;
            }
        }
        m = j + 1;
    }

END:
	//---
	delete []is;
	delete []x;
	delete []a;
	delete []b;
	//---
	return s;
}

//���ػ�������ʽ�����
// a	-->��������
// b	-->�������ޣ�Ҫ�� b>a
// pFun	-->��������f(x,y)��ָ��
//         ���㱻������f(x,y)ֵ�ĺ�����Ҫ�û����ݾ����f(x,y)��д,�Ѻ�������Ϊ
//         ʵ�δ���
// pFunb-->�����޺�����ָ��
//         ������Ҫ�û����ݾ��������д,�Ѻ�������Ϊ
//         ʵ�δ���; Ҫ�� ���޺���ֵ>���޺���ֵ
// eps	-->���־���Ҫ��,Ĭ��Ϊ 1.0e-6
double KIntegral::Fraction2D(double a, double b, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps)
{
    double bb[10],h[10],x,s;
	int m(1), n(1);
    double hh = b - a;
	h[0] = hh;
    double s1 = _Fraction2D2(a, pFun, pFunb, eps);
	double s2 = _Fraction2D2(b, pFun, pFunb, eps);
    double t1 = hh * (s1 + s2) / 2.0;
    double s0 = t1;
	bb[0] = t1;
	double ep = 1.0 + eps;
    while((ep > eps || fabs(ep-eps)<1.0e-12) && ( m <= 9))
    {
		double t2 = 0.5 * t1;
        for(int k=0; k<n; k++)
        {
			x = a + (k + 0.5) * hh;
            s1 = _Fraction2D2(a, pFun, pFunb, eps);
            t2 = t2 + 0.5 * s1 * hh;
        }
        m = m + 1;
        h[m-1] = h[m-2] / 2.0;
        double g = t2;
		int l(0), j(2);
        while((l==0)&&(j<=m))
         {
			s = g - bb[j-2];
            if(fabs(s)<1.0e-12) l = 1;
            else g = (h[m-1] - h[j-2]) / s;
            j = j + 1;
        }
        bb[m-1] = g;
        if(l!=0) bb[m-1] = 1.0e+35;
        s = bb[m-1];
        for(j=m; j>=2; j--) s = bb[j-2] -h [j-2] / s;
        ep = fabs(s-s0) / (1.0 + fabs(s));
        n = n + n;
		t1 = t2; 
		s0 = s;
		hh = hh / 2.0;
    }
    return(s);
}

//��������ʽ�������������
double KIntegral::_Fraction2D2(double x, double (*pFun)(double,double),void (*pFunb)(double,double*),double eps)
{
    double b[10],h[10],yy,s;
    int m(1), n(1);

	double y[2];
    (*pFunb)(x, y);	//

    double hh = y[1] - y[0];
	h[0] = hh;
    double t1=0.5*hh*((*pFun)(x,y[0])+(*pFun)(x,y[1]));
    double s0 = t1;
	b[0] = t1;
	double ep = 1.0 + eps;
    while((ep > eps || fabs(ep-eps)<1.0e-12) && (m <= 9))
    {
		double t2 = 0.5 * t1;
        for(int k=0; k<n; k++)
        {
			yy = y[0] +(k + 0.5) * hh;
            t2 = t2 + 0.5 * hh * (*pFun)(x, yy);
		}
        m = m + 1;
        h[m-1] = h[m-2] / 2.0;
        double g = t2;
		int l(0), j(2);
        while((l==0)&&(j<=m))
        {
			s = g - b[j-2];
            if(fabs(s)<1.0e-12) l = 1;
            else g = (h[m-1] - h[j-2]) / s;
            j = j + 1;
        }
        b[m-1] = g;
        if(l!=0) b[m-1] = 1.0e+35;
        s = b[m-1];
        for(j=m; j>=2; j--) s = b[j-2] - h[j-2] / s;
        ep = fabs(s - s0) / (1.0 + fabs(s));
        n = n + n;
		t1 = t2;
		s0 = s;
		hh = 0.5 * hh;
    }
    return(s);
}

//�������ؿ��巨���
// a	-->һά����,����Ϊn����Ż��ֵ�����a0,a1,...,an-1
// b	-->һά����,����Ϊn����Ż��ֵ�����b0,b1,...,bn-1
// pFun	-->��������f(x0,x1,...,xn)��ָ��
//         ���㱻������f(x0,x1,...,xn)ֵ�ĺ�����Ҫ�û����ݾ����f(x0,x1,...,xn)��д,�Ѻ�������Ϊ
//         ʵ�δ���
double KIntegral::MonteCarloMD(double* a, double* b,int n,double (*pFun)(double*,int))
{
	int i;
	//int n = a.size();		//���ֵ�����

	double* x = new double[n];

    double r(1), d(10000), s(0);

    for(int m=0; m<10000; m++)
    {
		for(i=0; i<n; i++)
			x[i] = a[i] + (b[i] - a[i]) * KRandom::Rand_01_One(r);

        s = s + (*pFun)(x, n) / d;
    }
    
	for(i=0; i<n; i++)	s = s * (b[i] - a[i]);
    
	//---
	delete []x;
	//---
	return(s);
}
//------------------------------------------------------------------
