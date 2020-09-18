#include "StdAfx.h"
#include "KFitting.h"

KFitting::KFitting(void)
{
}

//最小二乘曲线拟合
// x	-->一维数组，长度为n，存放给定n个数据点的x值
// y	-->一维数组，长度为n，存放给定n个数据点的y值
// a	-->一维数组，长度为m，m<=n且m<=20；
//		   返回时,存放拟合多项式的m个系数(m0,m1,...,am-1)
// dt	-->一维数组，长度为4; 返回时,
//         dt[0]保存拟合多项式与数据点的误差平方和
//         dt[1]保存拟合多项式与数据点的误差绝对值之和
//         dt[2]保存拟合多项式与数据点的误差绝对值最大值
//         dt[3]保存x的平均值
//
// y = a0 + a1*(x-xavg) + a2*(x-xavg)^2 + ...
//      
//
void KFitting::LeastSquares(double x[],double y[],int n,double a[],int m,double dt[4])
{
	int i,j;

    double g,q,d2,s[20],t[20],b[20];
	
	//要求m<=n且m<=20。若m>n或m>20，本函数自动按m=min{n,20}处理。

    for( i=0; i<m; i++)
	{
		a[i]=0.0;
	}

    if(m>n) m = n;
    if(m>20) m=20;

    double z(0), p(0), c(0);
    for(i=0; i<n; i++)
	{
		z = z + x[i] / n;
	}

	dt[3]=z;
    b[0]=1.0;

	double d1 = n;
    for(i=0; i<n; i++)
	{
		p=p+(x[i]-z);
		c=c+y[i];
	}

	c=c/d1; 
	p=p/d1;
	a[0]=c*b[0];

	if(m>1)
	{
		t[1]=1.0;
		t[0]=-p;
		d2=0.0;
		c=0.0; 
		g=0.0;
		for(i=0; i<n; i++)
		{ 
			q=x[i]-z-p; d2=d2+q*q;
			c=c+y[i]*q;
			g=g+(x[i]-z)*q*q;
		}
		c=c/d2;
		p=g/d2;
		q=d2/d1;
		d1=d2;
		a[1]=c*t[1]; 
		a[0]=c*t[0]+a[0];
	}

	for( j=2; j<m; j++)
	{ 
		s[j]=t[j-1];
		s[j-1]=-p*t[j-1]+t[j-2];
		if(j>=3)
		  for(int k=j-2; k>=1; k--)	s[k]=-p*t[k]+t[k-1]-q*b[k];
		s[0]=-p*t[0]-q*b[0];
		d2=0.0;
		c=0.0;
		g=0.0;
		for(i=0; i<n; i++)
		{ 
			q=s[j];
			for(int k=j-1; k>=0; k--)	q=q*(x[i]-z)+s[k];
			d2=d2+q*q; c=c+y[i]*q;
			g=g+(x[i]-z)*q*q;
		}
		c=c/d2; 
		p=g/d2; 
		q=d2/d1;
		d1=d2;
		a[j]=c*s[j]; t[j]=s[j];
		for(int k=j-1; k>=0; k--)
		{
			a[k]=c*s[k]+a[k];
			b[k]=t[k]; t[k]=s[k];
		}
	}

	dt[0]=0.0;
	dt[1]=0.0;
	dt[2]=0.0;

    for(i=0; i<n; i++)
    { 
		q=a[m-1];
        for(int k=m-2; k>=0; k--)
		{
			q=a[k]+q*(x[i]-z);
		}
        p=q-y[i];
        if(fabs(p)>dt[2])
		{
			dt[2]=fabs(p);
		}
        dt[0]=dt[0]+p*p;
        dt[1]=dt[1]+fabs(p);
    }
}

//切比雪夫曲线拟合
// x	-->一维数组，长度为n，存放给定n个数据点的x值,其中x0<x1<...<xn-1
// y	-->一维数组，长度为n，存放给定n个数据点的y值
// a	-->一维数组，长度为m，m<=n且m<=20；
//		   返回时,存放拟合多项式的m个系数(m0,m1,...,am-1)
// 返回值:拟合多项式偏差的最大值
//
// y = a0 + a1*x + a2*x^2 + ...
//
double KFitting::Chebyshev(double x[],double y[],int n,double a[],int m)
{
	int i,k;

	int ii,im,ix[21];
    double h[21],y1,y2,h1,h2,d,hm;
	double am;

	double* aa=new double[m+1];

	//要求m<n且m<=20。若m>=n或m>20，本函数自动按m=min{n-1,20}处理。

    for( i=0; i<=m; i++) aa[i]=0.0;
    if(m>=n) m=n-1;
    if(m>=20) m=19;
    int m1 = m + 1;
    double ha(0);
    ix[0] = 0; 
	ix[m] = n - 1;
    int l = (n - 1) / m; 
	int j = l;
    for(i=1; i<m; i++)
    {
		ix[i]=j;
		j=j+l;
	}
    while(1)
    {
		double hh=1.0;
        for(i=0; i<=m; i++)
        {
			aa[i]=y[ix[i]];
			h[i]=-hh;
			hh=-hh;
		}
        for( j=1; j<=m; j++)
        {
			ii=m1;
			y2=aa[ii-1]; 
			h2=h[ii-1];
            for(i=j; i<=m; i++)
            {
				d=x[ix[ii-1]]-x[ix[m1-i-1]];
                y1=aa[m-i+j-1];
                h1=h[m-i+j-1];
                aa[ii-1]=(y2-y1)/d;
                h[ii-1]=(h2-h1)/d;
                ii=m-i+j;
				y2=y1;
				h2=h1;
            }
        }
        hh=-aa[m]/h[m];
        for(i=0; i<=m; i++)	aa[i]=aa[i]+h[i]*hh;
        for(j=1; j<m; j++)
        {
			ii=m-j;
			d=x[ix[ii-1]];
            y2=aa[ii-1];
            for(k=m1-j; k<=m; k++)
            {
				y1=aa[k-1];
				aa[ii-1]=y2-d*y1;
                y2=y1; 
				ii=k;
            }
        }
        hm = fabs(hh);
        if(hm < ha || fabs(hm-ha)<1.0e-12)	//hm==ha
		{
			aa[m] = -hm;
			goto END;
		}
        aa[m]=hm; 
		ha=hm; 
		im=ix[0]; 
		h1=hh;
        j=0;
        for(i=0; i<n; i++)
        {
			if (i==ix[j])
            { 
				if (j<m) j=j+1;
			}
			else
			{ 
				h2=aa[m-1];
				for(k=m-2; k>=0; k--)	h2=h2*x[i]+aa[k];
				h2=h2-y[i];
				if(fabs(h2)>hm)
				{
					hm=fabs(h2); 
					h1=h2;
					im=i;
				}
			}
		}
		if(im==ix[0]) goto END;
		i=0;
		l=1;
		while(l==1)
		{ 
			l=0;
			if(im>=ix[i])
			{
				i=i+1;
				if (i<=m) l=1;
			}
		}
		if(i>m) i=m;
		if(i==(i/2)*2) h2=-hh;
		else h2=hh;
		if(h1*h2>=0.0) ix[i]=im;
		else
		{
			if(im<ix[0])
			{ 
				for(j=m-1; j>=0; j--)	ix[j+1]=ix[j];
				ix[0]=im;
			}
			else
			{
				if(im>ix[m])
				{
					for(j=1; j<=m; j++)	ix[j-1]=ix[j];
					ix[m]=im;
				}
				else 
					ix[i-1]=im;
			}
		}
	}
	//---
END:
	//---
	am = aa[m];
	for(i=0;i<m;i++)
		a[i]=aa[i];
	//---
	delete []aa;
	//---
	return am;
}

//里米兹法最佳一致逼近多项式
// a	-->区间左端点值
// b	-->区间右端点值
// p	-->一维数组,长度为n
//		   返回最佳一致逼近多项式的n个系数p0,p1,...,p(n-1)
// pFun	-->计算f(x)函数指针,由用户编写,把函数名以实参形式传入
// eps	-->控制精度,默认为1.0e-22
// 返回值：最佳一致逼近多项式的偏差绝对值u
double KFitting::Remez(double a,double b,double p[],int n,double (*pFun)(double),double eps)
{
	int i,j,k;

    double x[21],g[21],t,s,xx,x0,h,yy;

	//int n = p.size()-1;	//n-1次最佳一致逼近多项式的项数
	//要求n<=20; 若n>20，函数自动取n=20

	double* pp = new double[n+1];

    if(n>20) n=20;
    int m=n+1; 
	double d(1.0e+35);
    for( k=0; k<=n; k++)
    {
		t = cos((n - k) * 3.1415926 / n);
        x[k] = (b + a + (b-a) * t) / 2.0;
    }
    while(1)
    {
		double u(1);
        for( i=0; i<m; i++)
        {
			pp[i]=(*pFun)(x[i]);
            g[i]=-u;
			u=-u;
        }
        for( j=0; j<n; j++)
        {
			k=m;
			s=pp[k-1];
			xx=g[k-1];
            for(i=j; i<n; i++)
            {
				t=pp[n-i+j-1];
				x0=g[n-i+j-1];
                pp[k-1]=(s-t)/(x[k-1]-x[m-i-2]);
                g[k-1]=(xx-x0)/(x[k-1]-x[m-i-2]);
                k=n-i+j;
				s=t;
				xx=x0;
            }
        }
        u=-pp[m-1]/g[m-1];
        for(i=0; i<m; i++)	pp[i]=pp[i]+g[i]*u;
        for(j=1; j<n; j++)
        {
			k=n-j; 
			h=x[k-1];
			s=pp[k-1];
            for(i=m-j; i<=n; i++)
            {
				t=pp[i-1];
				pp[k-1]=s-h*t;
                s=t; 
				k=i;
            }
        }
        pp[m-1]=fabs(u);
		u=pp[m-1];
        if(fabs(u-d) <= eps) goto END;
        d=u; 
		h=0.1*(b-a)/(1.0*n);
        xx=a; 
		x0=a;
        while(x0<=b)
        {
			s=(*pFun)(x0);
			t=pp[n-1];
            for(i=n-2; i>=0; i--)	t=t*x0+pp[i];
            s=fabs(s-t);
            if(s>u) 
			{
				u=s; 
				xx=x0;
			}
            x0=x0+h;
        }
        s=(*pFun)(xx); 
		t=pp[n-1];
        for(i=n-2; i>=0; i--)	t=t*xx+pp[i];
        yy=s-t; 
		i=1;
		j=n+1;
        while((j-i)!=1)
        {
			k=(i+j)/2;
            if(xx<x[k-1]) j=k;
            else i=k;
        }
        if(xx<x[0])
        {
			s=(*pFun)(x[0]); 
			t=pp[n-1];
            for(k=n-2; k>=0; k--)	t=t*x[0]+pp[k];
            s=s-t;
            if(s*yy>0.0) x[0]=xx;
            else
            {
				for(k=n-1; k>=0; k--)	x[k+1]=x[k];
                x[0]=xx;
            }
        }
        else
        {
			if(xx>x[n])
            {
				s=(*pFun)(x[n]); 
				t=pp[n-1];
                for(k=n-2; k>=0; k--)	t=t*x[n]+pp[k];
                s=s-t;
                if(s*yy>0.0) x[n]=xx;
                else
                {
					for(k=0; k<n; k++)	x[k]=x[k+1];
                    x[n]=xx;
                }
            }
            else
            {
				i=i-1;
				j=j-1;
                s=(*pFun)(x[i]);
				t=pp[n-1];
                for(k=n-2; k>=0; k--)	t=t*x[i]+pp[k];
                s=s-t;
                if(s*yy>0.0) x[i]=xx;
                else x[j]=xx;
            }
        }
    }

END:
	for( i=0;i<n;i++)
		p[i]=pp[i];
	double pn=pp[n];
	delete []pp;
	return pn;
}

//矩形域的最小二乘曲面拟合
// x	-->一维数组,长度为n,保存给定数据点的X坐标
// y	-->一维数组,长度为m,保存给定数据点的Y坐标
// z	-->矩阵,体积为m*n,存放网格点(xi,yj)上的函数值zij
// a	-->矩阵,体积为p*q,返回时保存二元拟合多项式的系数aij
// dt	-->一维数组,长度为5
//		   dt[0]保存二元拟合多项式与数据点误差的平方和
//		   dt[1]保存二元拟合多项式与数据点误差的绝对值和
//		   dt[2]保存二元拟合多项式与数据点误差的绝对值的最大值
//		   dt[3]保存n个x的平均值
//		   dt[4]保存m个y的平均值
void KFitting::LeastSquares2D(double x[],double y[],KMatrix& z,KMatrix& a,double dt[5])
{
	int i,j,k;

	int l,kk;
    double apx[20],apy[20],bx[20],by[20];
    double t[20],t1[20],t2[20],d2,g,g1,g2;
    double x2,dd,y1,x1;

	//int n = x.size();			//给定数据点的X坐标个数
	//int m = y.size();			//给定数据点的Y坐标个数

	int n = z.GetColNum();			//给定数据点的X坐标个数
	int m = z.GetRowNum();			//给定数据点的Y坐标个数

	KMatrix v(20,m), u(20,20);

	int p = a.GetRowNum();		//拟合多项式中变量x的最高次数加1
	                            //并要求p<=m且p<=20; 否则在本函数中自动取p=min{n,20}处理

	int q = a.GetColNum();		//拟合多项式中变量y的最高次数加1
	                            //并要求q<=n且q<=20; 否则在本函数中自动取q=min{m,20}处理

    if(p>n)		p=n;
    if(p>20)	p=20;
    if(q>m)		q=m;
    if(q>20)	q=20;

	a.SetSize(p,q);

    for( i=0; i<p; i++)
	{
        for( j=0; j<q; j++)
		{
			a(i,j)=0.0;
		}
	}

    double xx(0), yy(0), d1(n); 
    for(i=0; i<n; i++)	xx = xx + x[i] / n;
	dt[3]=xx;
    for(i=0; i<m; i++)  yy = yy + y[i] / m;
	dt[4]=yy;
    apx[0]=0.0;
    for(i=0; i<n; i++)	apx[0] = apx[0] + x[i] - xx;
    apx[0] = apx[0] / d1;
    for(int j=0; j<m; j++)	//?????
    { 
		v(0,j)=0.0;
        for(i=0; i<n; i++)	v(0,j)=v(0,j)+z(i,j);
        v(0,j)=v(0,j)/d1;
    }
    if(p>1)
    { 
		d2=0.0;
		apx[1]=0.0;
        for(i=0; i<n; i++)
        {
			g=x[i]-xx-apx[0];
            d2=d2+g*g;
            apx[1]=apx[1]+(x[i]-xx)*g*g;
        }
        apx[1]=apx[1]/d2;
        bx[1]=d2/d1;
        for(j=0; j<m; j++)
        { 
			v(1,j)=0.0;
            for(i=0; i<n; i++)
            {
				g=x[i]-xx-apx[0];
                v(1,j)=v(1,j)+z(i,j)*g;
            }
            v(1,j)=v(1,j)/d2;
        }
        d1=d2;
    }
    for( k=2; k<p; k++)
    { 
		d2=0.0; 
		apx[k]=0.0;
        for(j=0; j<m; j++) v(k,j)=0.0;
        for(i=0; i<n; i++)
        {
			g1=1.0; g2=x[i]-xx-apx[0];
            for(j=2; j<=k; j++)
            {
				g=(x[i]-xx-apx[j-1])*g2-bx[j-1]*g1;
                g1=g2; g2=g;
            }
            d2=d2+g*g;
            apx[k]=apx[k]+(x[i]-xx)*g*g;
            for(j=0; j<m; j++)	v(k,j)=v(k,j)+z(i,j)*g;
        }
        for(j=0; j<m; j++)	v(k,j)=v(k,j)/d2;
        apx[k]=apx[k]/d2;
        bx[k]=d2/d1;
        d1=d2;
    }
    d1=m; 
	apy[0]=0.0;
    for(i=0; i<m; i++)	apy[0]=apy[0]+y[i]-yy;
    apy[0]=apy[0]/d1;
    for(j=0; j<p; j++)
    { 
		u(j,0)=0.0;
        for(i=0; i<m; i++)	u(j,0)=u(j,0)+v(j,i);
		u(j,0)=u(j,0)/d1;
    }
    if(q>1)
    { 
		d2=0.0; 
		apy[1]=0.0;
        for(i=0; i<m; i++)
        {
			g=y[i]-yy-apy[0];
            d2=d2+g*g;
            apy[1]=apy[1]+(y[i]-yy)*g*g;
        }
        apy[1]=apy[1]/d2;
        by[1]=d2/d1;
        for(j=0; j<p; j++)
		{
			u(j,1)=0.0;
            for(i=0; i<m; i++)
            {
				g=y[i]-yy-apy[0];
				u(j,1)=u(j,1)+v(j,i)*g;
            }
			u(j,1)=u(j,1)/d2;
        }
        d1=d2;
    }
    for(k=2; k<q; k++)
    { 
		d2=0.0;
		apy[k]=0.0;
		for(j=0; j<p; j++) u(j,k)=0.0;
        for(i=0; i<m; i++)
        {
			g1=1.0;
            g2=y[i]-yy-apy[0];
            for(j=2; j<=k; j++)
            {
				g=(y[i]-yy-apy[j-1])*g2-by[j-1]*g1;
                g1=g2;
				g2=g;
            }
            d2=d2+g*g;
            apy[k]=apy[k]+(y[i]-yy)*g*g;
            for(j=0; j<p; j++)	u(j,k)=u(j,k)+v(j,i)*g;
        }
        for(j=0; j<p; j++)	u(j,k)=u(j,k)/d2;
        apy[k]=apy[k]/d2;
        by[k]=d2/d1;
        d1=d2;
    }
    v(0,0)=1.0;
	v(1,0)=-apy[0]; 
	v(1,1)=1.0;
    for(i=0; i<p; i++)
      for(j=0; j<q; j++)	a(i,j)=0.0;
    for(i=2; i<q; i++)
    {
		v(i,i)=v(i-1,i-1);
        v(i,i-1)=-apy[i-1]*v(i-1,i-1)+v(i-1,i-2);
        if(i>=3)
          for(k=i-2; k>=1; k--)
            v(i,k)=-apy[i-1]*v(i-1,k)+v(i-1,k-1)-by[i-1]*v(i-2,k);
        v(i,0)=-apy[i-1]*v(i-1,0)-by[i-1]*v(i-2,0);
    }
    for(i=0; i<p; i++)
    { 
		if(i==0)
		{
			t[0]=1.0;
			t1[0]=1.0;
		}
        else
        {
			if(i==1)
            {
				t[0]=-apx[0];
				t[1]=1.0;
                t2[0]=t[0];
				t2[1]=t[1];
            }
            else
            { 
				t[i]=t2[i-1];
                t[i-1]=-apx[i-1]*t2[i-1]+t2[i-2];
                if(i>=3)
                  for(k=i-2; k>=1; k--)
                    t[k]=-apx[i-1]*t2[k]+t2[k-1]
                         -bx[i-1]*t1[k];
                t[0]=-apx[i-1]*t2[0]-bx[i-1]*t1[0];
                t2[i]=t[i];
                for(k=i-1; k>=0; k--)
                { 
					t1[k]=t2[k];
					t2[k]=t[k];
				}
            }
        }
        for(j=0; j<q; j++)
          for(k=i; k>=0; k--)
            for(l=j; l>=0; l--)
				a(k,l)=a(k,l)+u(i,j)*t[k]*v(j,l);
    }
    dt[0]=0.0; 
	dt[1]=0.0;
	dt[2]=0.0;
    for(i=0; i<n; i++)
    {
		x1=x[i]-xx;
        for(j=0; j<m; j++)
        {
			y1=y[j]-yy;
            x2=1.0; 
			dd=0.0;
            for(k=0; k<p; k++)
            {
				g=a(k,q-1);
                for(kk=q-2; kk>=0; kk--)	g=g*y1+a(k,kk);
                g=g*x2;
				dd=dd+g;
				x2=x2*x1;
            }
            dd=dd-z(i,j);
            if(fabs(dd)>dt[2]) dt[2]=fabs(dd);
            dt[0]=dt[0]+dd*dd;
            dt[1]=dt[1]+fabs(dd);
        }
    }
}

//矩形域的最小二乘曲面拟合
// x	-->一维数组,长度为n,保存给定数据点的X坐标
// y	-->一维数组,长度为m,保存给定数据点的Y坐标
// z	-->矩阵,体积为m*n,存放网格点(xi,yj)上的函数值zij
// a	-->矩阵,体积为p*q,返回时保存二元拟合多项式的系数aij
// dt	-->一维数组,长度为5
//		   dt[0]保存二元拟合多项式与数据点误差的平方和
//		   dt[1]保存二元拟合多项式与数据点误差的绝对值和
//		   dt[2]保存二元拟合多项式与数据点误差的绝对值的最大值
//		   dt[3]保存n个x的平均值
//		   dt[4]保存m个y的平均值
void LeastSquares2D(double x[], double y[], double z[], int n, int m, double a[], int p, int q, double dt[])
{
	int i,j,k,l,kk;
	double apx[20],apy[20],bx[20],by[20],u[20][20];
	double t[20],t1[20],t2[20],xx,yy,d1,d2,g,g1,g2;
	double x2,dd,y1,x1,*v;
	v=(double *)malloc(20*m*sizeof(double));
	for (i=0; i<=p-1; i++)
	{
		l=i*q;
		for (j=0; j<=q-1; j++)
			a[l+j]=0.0;
	}
	if (p>n)
		p=n;
	if (p>20)
		p=20;
	if (q>m)
		q=m;
	if (q>20)
		q=20;
	xx=0.0;
	for (i=0; i<=n-1; i++)
		xx=xx+x[i]/(1.0*n);
	yy=0.0;
	for (i=0; i<=m-1; i++)
		yy=yy+y[i]/(1.0*m);
	d1=1.0*n;
	apx[0]=0.0;
	for (i=0; i<=n-1; i++)
		apx[0]=apx[0]+x[i]-xx;
	apx[0]=apx[0]/d1;
	for (j=0; j<=m-1; j++)
	{
		v[j]=0.0;
		for (i=0; i<=n-1; i++)
			v[j]=v[j]+z[i*m+j];
		v[j]=v[j]/d1;
	}
	if (p>1)
	{
		d2=0.0;
		apx[1]=0.0;
		for (i=0; i<=n-1; i++)
		{
			g=x[i]-xx-apx[0];
			d2=d2+g*g;
			apx[1]=apx[1]+(x[i]-xx)*g*g;
		}
		apx[1]=apx[1]/d2;
		bx[1]=d2/d1;
		for (j=0; j<=m-1; j++)
		{
			v[m+j]=0.0;
			for (i=0; i<=n-1; i++)
			{
				g=x[i]-xx-apx[0];
				v[m+j]=v[m+j]+z[i*m+j]*g;
			}
			v[m+j]=v[m+j]/d2;
		}
		d1=d2;
	}
	for (k=2; k<=p-1; k++)
	{
		d2=0.0;
		apx[k]=0.0;
		for (j=0; j<=m-1; j++)
			v[k*m+j]=0.0;
		for (i=0; i<=n-1; i++)
		{
			g1=1.0;
			g2=x[i]-xx-apx[0];
			for (j=2; j<=k; j++)
			{
				g=(x[i]-xx-apx[j-1])*g2-bx[j-1]*g1;
				g1=g2;
				g2=g;
			}
			d2=d2+g*g;
			apx[k]=apx[k]+(x[i]-xx)*g*g;
			for (j=0; j<=m-1; j++)
				v[k*m+j]=v[k*m+j]+z[i*m+j]*g;
		}
		for (j=0; j<=m-1; j++)
			v[k*m+j]=v[k*m+j]/d2;
		apx[k]=apx[k]/d2;
		bx[k]=d2/d1;
		d1=d2;
	}
	d1=m;
	apy[0]=0.0;
	for (i=0; i<=m-1; i++)
		apy[0]=apy[0]+y[i]-yy;
	apy[0]=apy[0]/d1;
	for (j=0; j<=p-1; j++)
	{
		u[j][0]=0.0;
		for (i=0; i<=m-1; i++)
			u[j][0]=u[j][0]+v[j*m+i];
		u[j][0]=u[j][0]/d1;
	}
	if (q>1)
	{
		d2=0.0;
		apy[1]=0.0;
		for (i=0; i<=m-1; i++)
		{
			g=y[i]-yy-apy[0];
			d2=d2+g*g;
			apy[1]=apy[1]+(y[i]-yy)*g*g;
		}
		apy[1]=apy[1]/d2;
		by[1]=d2/d1;
		for (j=0; j<=p-1; j++)
		{
			u[j][1]=0.0;
			for (i=0; i<=m-1; i++)
			{
				g=y[i]-yy-apy[0];
				u[j][1]=u[j][1]+v[j*m+i]*g;
			}
			u[j][1]=u[j][1]/d2;
		}
		d1=d2;
	}
	for (k=2; k<=q-1; k++)
	{
		d2=0.0;
		apy[k]=0.0;
		for (j=0; j<=p-1; j++
			) u[j][k]=0.0;
			for (i=0; i<=m-1; i++)
			{
				g1=1.0;
				g2=y[i]-yy-apy[0];
				for (j=2; j<=k; j++)
				{
					g=(y[i]-yy-apy[j-1])*g2-by[j-1]*g1;
					g1=g2;
					g2=g;
				}
				d2=d2+g*g;
				apy[k]=apy[k]+(y[i]-yy)*g*g;
				for (j=0; j<=p-1; j++)
					u[j][k]=u[j][k]+v[j*m+i]*g;
			}
			for (j=0; j<=p-1; j++)
				u[j][k]=u[j][k]/d2;
			apy[k]=apy[k]/d2;
			by[k]=d2/d1;
			d1=d2;
	}
	v[0]=1.0;
	v[m]=-apy[0];
	v[m+1]=1.0;
	for (i=0; i<=p-1; i++)
		for (j=0; j<=q-1; j++)
			a[i*q+j]=0.0;
	for (i=2; i<=q-1; i++)
	{
		v[i*m+i]=v[(i-1)*m+(i-1)];
		v[i*m+i-1]=-apy[i-1]*v[(i-1)*m+i-1]+v[(i-1)*m+i-2];
		if (i>=3)
			for (k=i-2; k>=1; k--)
				v[i*m+k]=-apy[i-1]*v[(i-1)*m+k]+v[(i-1)*m+k-1]-by[i-1]*v[(i-2)*m+k];
		v[i*m]=-apy[i-1]*v[(i-1)*m]-by[i-1]*v[(i-2)*m];
	}
	for (i=0; i<=p-1; i++)
	{
		if (i==0)
		{
			t[0]=1.0;
			t1[0]=1.0;
		}
		else
		{
			if (i==1)
			{
				t[0]=-apx[0];
				t[1]=1.0;
				t2[0]=t[0];
				t2[1]=t[1];
			}
			else
			{
				t[i]=t2[i-1];
				t[i-1]=-apx[i-1]*t2[i-1]+t2[i-2];
				if (i>=3)
					for (k=i-2; k>=1; k--)
						t[k]=-apx[i-1]*t2[k]+t2[k-1]-bx[i-1]*t1[k];
				t[0]=-apx[i-1]*t2[0]-bx[i-1]*t1[0];
				t2[i]=t[i];
				for (k=i-1; k>=0; k--)
				{
					t1[k]=t2[k];
					t2[k]=t[k];
				}
			}
		}
		for (j=0; j<=q-1; j++)
			for (k=i; k>=0; k--)
				for (l=j; l>=0; l--)
					a[k*q+l]=a[k*q+l]+u[i][j]*t[k]*v[j*m+l];
	}
	dt[0]=0.0;
	dt[1]=0.0;
	dt[2]=0.0;
	for (i=0; i<=n-1; i++)
	{
		x1=x[i]-xx;
		for (j=0; j<=m-1; j++)
		{
			y1=y[j]-yy;
			x2=1.0;
			dd=0.0;
			for (k=0; k<=p-1; k++)
			{
				g=a[k*q+q-1];
				for (kk=q-2; kk>=0; kk--)
					g=g*y1+a[k*q+kk];
				g=g*x2;
				dd=dd+g;
				x2=x2*x1;
			}
			dd=dd-z[i*m+j];
			if (fabs(dd)>dt[2])
				dt[2]=fabs(dd);
			dt[0]=dt[0]+dd*dd;
			dt[1]=dt[1]+fabs(dd);
		}
	}
	free(v);
	return;
}

//曲线拟合
//用五点三次平滑公式对等距点上的观测数据进行平滑
// y = a + bx + cx^2 + dx^3
// y: 等距点上的观测数据数组指针,返回时为等距点上的观测数据的平滑结果
// n: 数组长度
void KFitting::Curvefitting(double y[], int n)
{
    if(n<5)	return;

	double* yy = new double[n];
	//-----------------------------------------
	yy[0]=69.0*y[0]+4.0*y[1]-6.0*y[2]+4.0*y[3]-y[4];
    yy[0]=yy[0]/70.0;
    yy[1]=2.0*y[0]+27.0*y[1]+12.0*y[2]-8.0*y[3];
    yy[1]=(yy[1]+2.0*y[4])/35.0;
    for (int i=2; i<n-2; i++)
    {
		yy[i]=-3.0*y[i-2]+12.0*y[i-1]+17.0*y[i];
        yy[i]=(yy[i]+12.0*y[i+1]-3.0*y[i+2])/35.0;
    }
    yy[n-2]=2.0*y[n-5]-8.0*y[n-4]+12.0*y[n-3];
    yy[n-2]=(yy[n-2]+27.0*y[n-2]+2.0*y[n-1])/35.0;
    yy[n-1]=-y[n-5]+4.0*y[n-4]-6.0*y[n-3];
    yy[n-1]=(yy[n-1]+4.0*y[n-2]+69.0*y[n-1])/70.0;
	//-----------------------------------------
	memcpy(y,yy,sizeof(double)*n);
	//-----------------------------------------
	delete[] yy;
}
