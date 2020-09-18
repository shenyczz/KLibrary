#include "StdAfx.h"
#include "KSFunction.h"

KSFunction::KSFunction(void)
{
}

//伽马函数
double KSFunction::Gamma(double x)
{
    double t;
    double a[11] = 
	{
		 0.0000677106, -0.0003442342,  0.0015397681,
		-0.0024467480,  0.0109736958, -0.0002109075,
		 0.0742379071,  0.0815782188,  0.4118402518,
		 0.4227843370,  1.0
	};

	//x必须大于0
    if(x<0.0 || fabs(x)<1.0e-12)	//x==0
    {
		//cout << "Error **x<=0!" << endl; 
		return(-1.0);
	}
    double y = x;
    if(y<1.0 || fabs(y-1.0)<1.0e-12)
    {
		t=1.0/(y*(y+1.0)); 
		y=y+2.0;
	}
    else
		if(y<2.0 || fabs(y-2.0)<1.0e-12)
		{
			t=1.0/y; 
			y=y+1.0;
		}
		else
			if(y<3.0 || fabs(y-3.0)<1.0e-12) t=1.0;
			else
			{
				t=1.0;
				while(y>3.0)
				{
					y=y-1.0;
					t=t*y;
				}
			}
    double s=a[0]; 
	double u = y - 2.0;
    for(int i=1; i<11; i++)	s = s * u + a[i];
    s = s * t;
    return(s);
}

//不完全伽马函数
double KSFunction::IncompleteGamma(double a, double x)
{
	int n;
    double p,d,s,s1,p0,q0,p1,q1;
    if((a<0.0 || fabs(a-0.0)<1.0e-12) || (x<0.0))
    { 
		//if(a<0.0 || FloatEqual(a,0.0)) cout <<"Error **a<=0!" <<endl;
        //if(x<0.0) cout << "Error **x<0!" << endl;
        return(-1.0);
    }
    if(fabs(x-0.0)<1.0e-12) return(0.0);
    if(x>1.0e+35) return(1.0);
    double q = log(x); 
	q = a * q; 
	double qq = exp(q);
    if(x<1.0+a)
    {
		p=a; 
		d=1.0/a; 
		s=d;
        for(n=1; n<101; n++)
        {
			p=1.0+p;
			d=d*x/p;
			s=s+d;
			if(fabs(d)<fabs(s)*1.0e-12)
            {
				s=s*exp(-x)*qq/Gamma(a);
                return(s);
            }
        }
    }
    else
    { 
		s=1.0/x; 
		p0=0.0; 
		p1=1.0;
		q0=1.0;
		q1=x;
        for(n=1; n<101; n++)
        { 
			p0=p1+(n-a)*p0;
			q0=q1+(n-a)*q0;
            p=x*p0+n*p1;
			q=x*q0+n*q1;
            if(!(fabs(q-0.0)<1.0e-12))	//q!=0
            {
				s1=p/q; 
				p1=p; 
				q1=q;
                if(fabs((s1-s)/s1)<1.0e-12)
                { 
					s=s1*exp(-x)*qq/Gamma(a);
                    return(1.0-s);
                }
                s=s1;
            }
            p1=p; 
			q1=q;
        }
    }
    //cout << "a too large !" << endl;
    s=1.0-s*exp(-x)*qq/Gamma(a);
    return(s);
}

//误差函数
double KSFunction::Error(double x)
{
	double y;

    if(x>0.0 || fabs(x)<1.0e-12)	//x>=0.0
      y=IncompleteGamma(0.5,x*x);
    else
      y=-IncompleteGamma(0.5,x*x);
    return(y);
}

//不完全贝塔函数
double KSFunction::IncompleteBeta(double a, double b, double x)
{
	double y;

    if(a<0.0 || fabs(a)<1.0e-12)
    { 
		//cout << "err**a<=0!";
		return(-1.0);
	}
    if(b<0.0 || fabs(b)<1.0e-12)
    { 
		//cout << "err**b<=0!";
		return(-1.0);
	}
    if((x<0.0)||(x>1.0))
    {
		//cout << "err**x<0 or x>1 !";
        return(1.0e+70);
    }
    if(fabs(x)<1.0e-12||fabs(x-1.0)<1.0e-12)
	{
		y=0.0;
	}
    else
    { 
		y=a*log(x)+b*log(1.0-x);
        y=exp(y);
        y=y*Gamma(a+b)/(Gamma(a)*Gamma(b));
    }
    if(x<(a+1.0)/(a+b+2.0))
      y=y*_beta(a,b,x)/a;
    else
      y=1.0-y*_beta(b,a,1.0-x)/b;
    return(y);
}
//不完全贝塔函数的从属函数
double KSFunction::_beta(double a, double b, double x)
{
    double p0(0.0), q0(1.0), p1(1.0), q1(1.0), s1;

    for(int k=1; k<101; k++)
    {
		double d=(a+k)*(a+b+k)*x;
		d=-d/((a+k+k)*(a+k+k+1.0));
        p0=p1+d*p0;
		q0=q1+d*q0;
		double s0=p0/q0;
        d=k*(b-k)*x;
        d=d/((a+k+k-1.0)*(a+k+k));
        p1=p0+d*p1;
		q1=q0+d*q1;
		s1=p1/q1;
        if(fabs(s1-s0)<fabs(s1)*1.0e-07)	return(s1);
    }
    //cout << "a or b too big !";
    return(s1);
}

//正态分布函数
// a	-->随机变量的数学期望
// d	-->随机变量的标准差
// x	-->随机变量
double KSFunction::NormalDistribution(double a, double d, double x)
{
	double y;
    
    if(d<0.0 || fabs(d-0.0)<1.0e-12) d=1.0e-10;
    y=0.5+0.5*Error((x-a)/(sqrt(2.0)*d));
    return(y);
}

//t-分布函数
double KSFunction::tDistribution(double t, int n)
{
	double y;
    
    if(t<0.0) t = -t;
    y=1.0-IncompleteBeta(n/2.0, 0.5, n/(n+t*t));
    return(y);
}

//X^2-分布函数
double KSFunction::X2Distribution(double x, int n)
{
	double y;
    
    if(x<0.0) x=-x;
    y=IncompleteGamma(n/2.0,x/2.0);
    return(y);
}

//F-分布函数
double KSFunction::FDistribution(double f, int n1, int n2)
{
	double y;
    //extern double lbeta();
    if(f<0.0) f=-f;
    y=IncompleteBeta(n2/2.0,n1/2.0,n2/(n2+n1*f));
    return(y);
}
