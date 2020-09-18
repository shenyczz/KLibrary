#include "StdAfx.h"
#include "KInterpolate.h"

KInterpolate::KInterpolate(void)
{
}

//拉格朗日全区间不等距插值
// yi=f(xi)
// x	-->存放结点xi值的数组
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::LagrangeNotIsometry(double* x, double* y, int n,double t)
{
	double dValue=0.0;
	//---
	int i,j,k,m;
    double s;
    
	if(n < 1) return(dValue);
    
	if(n == 1) 
	{
		dValue = y[0]; 
		return(dValue);
	}
    
	if(n == 2)
	{
		dValue = (y[0] * (t - x[1]) - y[1] * (t - x[0])) / (x[0] - x[1]);
        return(dValue);
	}
    
	i = 0;
    
	while((x[i] < t) && (i < n)) i++;
    
	k = i - 4;
    
	if(k < 0) k = 0;
    
	m = i + 3;
    
	if(m > (n - 1)) m = n - 1;
    
	for(i = k; i <= m; i ++)
	{
		s = 1.0;
        for(j = k; j <= m; j ++)
		{
			if(j != i)
				s = s * (t - x[j]) / (x[i] - x[j]);
		}
		dValue = dValue + s * y[i];
	}

	//---
	return dValue;
}

//拉格朗日全区间等距插值
// yi=f(xi)
// x0	-->等距结点中第一个结点值
// h	-->等距结点中步长
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::LagrangeIsometry(double x0, double h, double* y, int n,double t)
{
	int i, j, k, m;
    double z(0), s, xi, xj, p, q;
    
    if(n < 1) return(z);
    
	if(n == 1) 
	{
		z = y[0];
		return(z);
	}
    
	if(n == 2)
	{ 
		z = (y[1] * (t - x0) - y[0] * (t - x0 - h)) / h;
		return(z);
	}
    
	if(t > x0)
	{ 
		p = (t - x0) / h;
		i = (int)p; 
		q = (float)i;
        if(p > q) i++;
	}

    else i = 0;
    
	k = i - 4;

    if(k < 0) k = 0;

    m = i + 3;

    if(m > (n-1)) m = n - 1;

    for(i = k; i<=m; i ++)
	{
		s = 1.0;
		xi = x0 + i * h;

        for(j = k; j<=m; j++)
			if(j != i)
            {
				xj = x0 + j * h;
				s = s * (t - xj) / (xi - xj);
            }
			z = z + s * y[i];
	}
    return(z);
}

//抛物线不等距插值
// yi=f(xi)
// x	-->存放结点xi值的数组
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::ParabolaNotIsometry(double* x, double* y, int n,double t)
{
	int i, j, k, m;
	double z(0.0), s;

	if(n < 1) return(z);
	
	if(n==1)
	{
		z = y[0];
		return(z);
	}
	
	if(n == 2)
	{ 
		z = (y[0] * (t - x[1]) - y[1] * (t - x[0])) / (x[0] - x[1]);
		return(z);
	}
	
	if(t <= x[1]) 
	{
		k = 0;
		m = 2;
	}
	else if(t >= x[n-2])
	{
		k = n - 3;
		m = n - 1;
	}
	
	else
	{ 
		k = 1;
		m = n;
		while((m-k) != 1)
		{ 
			i = (k + m) / 2;
			if(t < x[i - 1]) m = i;
			else k = i;
		}
		k = k - 1;
		m = m - 1;
		if(fabs(t - x[k]) < fabs(t - x[m]))
			k = k - 1;
		else
			m = m + 1;
	}
	z = 0.0;
	for(i = k; i <= m; i ++)
	{
		s = 1.0;
		for(j = k;j <= m; j ++)
			if(j != i) s = s * (t - x[j]) / (x[i] - x[j]);
			z = z + s * y[i];
	}

	return(z);
}

//抛物线等距插值
// x0	-->等距结点中第一个结点值
// h	-->等距结点中步长
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::ParabolaIsometry(double x0, double h, double* y, int n,double t)
{
	int i, j, k, m;
    double z(0.0), s, xi, xj;

    if(n < 1) return(z);

    if(n == 1) 
	{
		z = y[0];
		return(z);
	}
    
	if(n == 2)
	{
		z = (y[1] * (t - x0) - y[0] * (t - x0 - h)) / h;
		return(z);
	}
    
	if(t <= (x0 + h))
	{ 
		k = 0;
		m = 2;
	}
    
	else if(t >= (x0+(n-3)*h))
	{
		k = n -3 ; 
		m = n - 1;
	}
    
	else
	{
		i = (int)((t - x0) / h) + 1;
        
		if(fabs(t - x0 - i * h) >= fabs(t - x0 - (i - 1) * h))
		{
			k = i - 2; 
			m = i;
		}
        else
		{
			k = i - 1;
			m = i + 1;
		}
	}
    
	z = 0.0;
    for(i = k; i <= m; i ++)
	{
		s = 1.0;
		xi = x0 + i * h;
        for(j = k; j <= m; j++)
			if(j != i)
            {
				xj = x0 + j * h;
				s = s * (t - xj) / (xi - xj);
			}
        z = z + s * y[i];
	}
 
	return(z);
}

//连分式不等距插值
// yi=f(xi)
// x	-->存放结点xi值的数组
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::FractionNotIsometry(double* x, double* y, int n,double t)
{
	int i,j,k,m,l;
	double z(0), h, b[8];

	if(n < 1) return(z);
	
	if(n == 1)
	{
		z = y[0];
		return(z);
	}
	
	if(n <= 8) 
	{
		k = 0;
		m = n;
	}
	
	else if(t < x[4])
	{
		k = 0; 
		m = 8;
	}
	
	else if(t > x[n - 5])
	{
		k= n - 8; 
		m = 8;
	}
	
	else
	{
		k = 1; 
		j = n;
		while((j-k) != 1)
		{
			i = (k + j) / 2;
			
			if(t < x[i - 1]) j = i;
			else k = i;
		}

		k = k - 4; 
		m = 8;
	}
	
	b[0] = y[k];
	
	for(i = 2; i <= m; i ++)
	{
		h = y[i + k - 1];
		l = 0;
		j = 1;
		
		while((l == 0) && (j <= i - 1))
		{
			if((fabs(h - b[j - 1]) + 1.0) == 1.0) l = 1;
			else h = (x[i + k - 1] - x[j + k - 1]) / (h - b[j - 1]);
			j = j + 1;
		}
		
		b[i - 1] = h;
		
		if(l != 0) 
		{
			b[i - 1] = 1.0e+35;
		}
	}
	
	z = b[m - 1];
	for(i = m - 1; i >= 1; i --)
	{
		z = b[i - 1] + (t - x[i + k - 1]) / z;
	}

	return(z);
}

//连分式等距插值
// x0	-->等距结点中第一个结点值
// h	-->等距结点中步长
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::FractionIsometry(double x0, double h, double* y, int n,double t)
{
	int i,j,k,m,l;
	double z(0.0), hh, xi, xj, b[8];
	
	if(n < 1) return(z);
	
	if(n == 1)
	{
		z = y[0];
		return(z);
	}
	
	if(n <= 8)
	{
		k = 0;
		m = n;
	}
	
	else if(t < (x0 + 4.0 * h)) 
	{
		k = 0; 
		m = 8;
	}
	
	else if(t > (x0 + (n - 5) * h)) 
	{
		k = n - 8;
		m = 8;
	}
	
	else
	{
		k = (int)((t - x0) / h) - 3;
		m = 8;
	}
	
	b[0] = y[k];
	
	for(i = 2; i <= m; i ++)
	{ 
		hh = y[i + k - 1];
		l = 0;
		j = 1;
	 
		while((l == 0) && (j <= (i - 1)))
		{
			if((fabs(hh - b[j - 1]) + 1.0) == 1.0 )
			{
				l=1;
			}
			
			else
			{
				xi = x0 + (i + k - 1) * h;
				xj = x0 + (j + k - 1) * h;
				hh = (xi - xj) / (hh - b[j - 1]);
			}
			
			j = j + 1;
		}
		
		b[i - 1] = hh;
		
		if(l != 0) 
		{
			b[i - 1] = 1.0e+35;
		}
	}
	
	z = b[m - 1];
	
	for(i = m - 1; i >= 1; i --)
	{
		z = b[i - 1] + (t - (x0 + (i + k - 1) * h)) / z;
	}

	return(z);
}

//埃尔米特不等距插值
// yi=f(xi)
// x	-->存放结点xi值的数组
// y	-->存放结点yi值的数组
// dy	-->存放结点yi导数值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::HermiteNotIsometry(double* x, double* y, double* dy, int n,double t)
{
	int i,j;
	double z(0.0), p, q, s;
	
	for(i = 1; i<=n; i ++)
	{
		s = 1.0;
		
		for(j = 1; j <= n;j ++)
			if(j != i)
			{
				s = s * (t - x[j - 1]) / (x[i - 1] - x[j - 1]);
			}
			
			s = s * s;
			p = 0.0;
			
		for(j = 1; j <= n; j++)
			if(j!=i) 
			{
				p = p + 1.0 / (x[i - 1] - x[j - 1]);
			}
			
			q = y[i - 1] + (t - x[i - 1]) * (dy[i - 1] - 2.0 * y[i - 1] * p);
			z = z + q * s;
	}
	
	return(z);
}

//埃尔米特等距插值
// x0	-->等距结点中第一个结点值
// h	-->等距结点中步长
// y	-->存放结点yi值的数组
// dy	-->存放结点yi导数值的数组
// n	-->结点数量
// t	-->待插值结点的x值
double KInterpolate::HermiteIsometry(double x0, double h, double* y, double* dy, int n,double t)
{
	int i, j;
	double z(0.0), s, p, q;
	
	for(i = 1; i <= n; i ++)
	{
		s = 1.0;
		q = x0 + (i - 1) * h;
		
		for(j = 1; j <= n; j ++)
		{
			p = x0 + (j - 1) * h;
			if(j != i) s = s * (t - p) / (q - p);
		}
		
		s = s * s;
		p = 0.0;
		
		for(j = 1; j <= n; j ++)
		{
			if(j != i) 
			{
				p = p + 1.0 / (q - (x0 + (j - 1) * h));
			}
		}
		
		q = y[i - 1] + (t - q) * (dy[i - 1] - 2.0 * y[i - 1] * p);
		z = z + q * s;
		
	}
	
	return(z);
}

//埃特金不等距逐步插值
// yi=f(xi)
// x	-->存放结点xi值的数组
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
// eps	-->插值的精度要求, 默认=1.0e-6
double KInterpolate::AitkenNotIsometry(double* x, double* y, int n, double t, double eps)
{
	int i,j,k,m,l;
	double z(0), xx[10], yy[10];
	
	if(n <1 ) return(z);
	
	if(n == 1)
	{
		z = y[0];
		return(z);
	}
	
	m = 10;
	
	if(m > n) m = n;
	
	if(t <= x[0]) k = 1;
	
	else if(t >= x[n - 1]) k=n;
	
	else
	{
		k = 1;
		j = n;

		while(((k - j) != 1) && ((k - j) != -1))
		{
			l = (k + j) / 2;
			
			if(t < x[l - 1]) j = l;
			
			else k = l;
		}
		if(fabs(t - x[l - 1]) > fabs (t - x[j - 1])) k = j;
	}
	
	j = 1;
	l = 0;
	
	for(i = 1; i <= m; i ++)
	{
		k = k + j * l;

		if((k < 1) || (k > n))
		{
			l = l + 1; 
			j = -j;
			k = k + j * l;
		}
		
		xx[i - 1] = x[k - 1];
		yy[i - 1] = y[k - 1];
		l = l + 1;
		j = -j;
	}

	i = 0;
	
	do
	{
		i = i + 1;
		z = yy[i];
		
		for(j = 0; j <= i - 1; j ++)
		{
			z = yy[j] + (t - xx[j]) * (yy[j] - z) / (xx[j] - xx[i]);
		}
		
		yy[i] = z;
	}while((i != (m - 1)) && (fabs(yy[i] - yy[i - 1]) > eps));

	return(z);
}

//埃特金等距逐步插值
// x0	-->等距结点中第一个结点值
// h	-->等距结点中步长
// y	-->存放结点yi值的数组
// n	-->结点数量
// t	-->待插值结点的x值
// eps	-->插值的精度要求, 默认=1.0e-6
double KInterpolate::AitkenIsometry(double x0, double h, double* y, int n,double t, double eps)
{
	int i, j, k, m, l;
	double z(0), xx[10], yy[10];

	if (n < 1)
		return z;
	if (n == 1)
	{ 
		z = y[0];
		return z;
	}

	m = 10;
	if (m > n)
		m = n;
	if (t <= x0)
		k = 1;
	else
	{
		if (t >= (x0 + (n - 1) * h))
			k = n;
		else
		{
			k = 1;
			j = n;
			while ((k - j != 1) && (k - j != -1))
			{
				l = (k + j) / 2;
				if (t < (x0 + (l - 1) * h))
					j = l;
				else
					k = l;
			}
			if (fabs(t - (x0 + (l - 1) * h)) > fabs(t - (x0 + (j - 1) * h)))
				k = j;
		}
	}

	j = 1;
	l = 0;
	for (i = 1; i <= m; ++ i)
	{
		k = k + j * l;
		if ((k < 1) || (k > n))
		{
			l = l + 1;
			j = -j;
			k = k + j * l;
		}
		xx[i - 1] = x0 + (k - 1) * h;
		yy[i - 1] = y[k - 1];
		l = l + 1;
		j = -j;
	}

	i = 0;
	do
	{
		i = i + 1; 
		z = yy[i];
		for (j = 0; j <= i - 1; ++ j)
			z = yy[j] + (t - xx[j]) * (yy[j] - z) / (xx[j] - xx[i]);
		yy[i] = z;
	}while ((i != m - 1) && (fabs(yy[i] - yy[i - 1]) > eps));

	return z;
}

//光滑不等距插值
// yi=f(xi)
// 得到子区间[Xk,Xk+1](k=0,1,2,...,n-2)上的三次多项式的系数
//   s(x)=s0+s1(x-xk)+s2(x-xk)^2+s3(x-xk)^3
// x	-->存放结点xi值的数组
// y	-->存放结点yi值的数组
// n	-->结点数量
// k	-->k>=0,表示只计算第k个子区间上的三次多项式系数s0,s1,s2,s3;
//		   k<0, 表示计算指定点t处的函数值f(t),保存在s[4],并计算插值点t所在子区间的多项式系数s0,s1,s2,s3;
// t	-->待插值结点的x值
// s	-->返回三次多项式系数s0,s1,s2,s3
// sn	-->数组s的维数,默认=5
void KInterpolate::AkimaNotIsometry(double* x, double* y, int n, int k, double t, double* s,int sn)
{
	int kk, m, l;
	double u[5], p, q;

	//int n =  y.size();	//给定不等距结点的个数
	
	for(m=0; m<5; m++) s[m] = 0.0;	

	if(n < 1) goto END;
	
	if(n == 1)
	{ 
		s[0] = y[0];
		s[4] = y[0];
		
		goto END;
	}
	
	if(n == 2)
	{
		s[0] = y[0]; 
		s[1] = (y[1] - y[0]) / (x[1] - x[0]);
		
		if(k < 0)
		{
			s[4] = (y[0] * (t - x[1]) - y[1] * (t - x[0])) / (x[0] - x[1]);
		}
		
		goto END;
	}
	
	if(k < 0)
	{
		if(t <= x[1]) kk = 0;
		else 
			if(t >= x[n - 1]) kk = n - 2;
	        else
			{
				kk = 1;
				m = n;
		
				while(((kk - m) != 1) && ((kk - m) != -1))
				{
					l = (kk + m) / 2;
					if(t < x[l - 1]) m = l;
					else kk = l;
				}
			
				kk = kk - 1;
			}
	}
	else kk = k;
	
	if(kk >= n-1) kk = n - 2;
	
	u[2] = (y[kk + 1] - y[kk]) / (x[kk + 1] - x[kk]);
	if(n == 3)
	{
		if(kk == 0)
		{
			u[3] = (y[2] - y[1]) / (x[2] - x[1]);
			u[4] = 2.0 * u[3] - u[2];
			u[1] = 2.0 * u[2] - u[3];
			u[0] = 2.0 * u[1] - u[2];
		}
		else
		{
			u[1] = (y[1] - y[0]) / (x[1] - x[0]);
			u[0] = 2.0 * u[1] - u[2];
			u[3] = 2.0 * u[2] - u[1];
			u[4] = 2.0 * u[3] - u[2];
		}
	}
	else
	{ 
		if(kk <= 1)
		{
			u[3] = (y[kk + 2] - y[kk + 1]) / (x[kk + 2] - x[kk + 1]);
			
			if(kk == 1)
			{
				u[1] = (y[1] - y[0]) / (x[1] - x[0]);
				u[0] = 2.0 * u[1] - u[2];
				
				if(n == 4) u[4] = 2.0 * u[3] - u[2];
				
				else u[4] = (y[4] - y[3]) / (x[4] - x[3]);
			}	
			else
			{
				u[1] = 2.0 * u[2] - u[3];
				u[0] = 2.0 * u[1] - u[2];
				u[4] = (y[3] - y[2]) / (x[3] - x[2]);
			}
		}
		else if(kk >= (n - 3))
		{
			u[1]  = (y[kk] - y[kk - 1]) / (x[kk] - x[kk - 1]);
			
			if(kk == (n - 3))
			{
				u[3] = (y[n - 1] - y[n - 2]) / (x[n - 1]- x[n - 2]);
				u[4] = 2.0 * u[3] - u[2];
				
				if(n == 4) u[0] = 2.0 * u[1] - u[2];
				
				else u[0] = (y[kk - 1] - y[kk - 2]) / (x[kk - 1] - x[kk - 2]);
			}
			else
			{
				u[3] = 2.0 * u[2] - u[1];
				u[4] = 2.0 * u[3] - u[2];
				u[0] = (y[kk - 1] - y[kk - 2]) / (x[kk - 1] - x[kk - 2]);
			}
		}
		else
		{
			u[1] = (y[kk] - y[kk - 1]) / (x[kk] - x[kk - 1]);
			u[0] = (y[kk - 1] - y[kk - 2]) / (x[kk - 1] - x[kk - 2]);
			u[3] = (y[kk + 2] - y[kk + 1]) / (x[kk + 2] - x[kk + 1]);
			u[4] = (y[kk + 3] - y[kk + 2]) / (x[kk + 3] - x[kk + 2]);
		}
	}
	
	s[0] =  fabs(u[3] - u[2]);
	s[1] =  fabs(u[0] - u[1]);
	
	if(fabs(s[0]) <1.0e-12 && fabs(s[1]) < 1.0e-12)	//s[0]==0 && s[1]==0
	//if(FloatEqual(s[0],0) && FloatEqual(s[1],0))
		p = (u[1] + u[2]) / 2.0;
	
	else p = (s[0] * u[1] + s[1] * u[2]) / (s[0] + s[1]);

	s[0] = fabs(u[3] - u[4]);
	s[1] = fabs(u[2] - u[1]);

	if(fabs(s[0]) <1.0e-12 && fabs(s[1]) < 1.0e-12)	//s[0]==0 && s[1]==0
	//if(FloatEqual(s[0],0) && FloatEqual(s[1],0))
		q = (u[2] + u[3]) / 2.0;
	
	else q = (s[0] * u[2] + s[1] * u[3]) / (s[0] + s[1]);
	
	s[0] = y[kk];
	s[1] = p;
	s[3] = x[kk + 1] - x[kk];
	s[2] = (3.0 * u[2] - 2.0 * p - q) / s[3];
	s[3] = (q + p - 2.0 * u[2]) / (s[3] * s[3]);
	
	if(k < 0)
	{ 
		p = t - x[kk];
		s[4] = s[0] + s[1] * p + s[2] * p * p + s[3] * p * p * p;
	}

END: ;
	/*int kk, m, l;
	double u[5], p, q;

	for(m=0; m<5; m++) s[m] = 0.0;	

	if(n < 1) goto END;
	
	if(n == 1)
	{ 
		s[0] = y[0];
		s[4] = y[0];
		
		goto END;
	}
	
	if(n == 2)
	{
		s[0] = y[0]; 
		s[1] = (y[1] - y[0]) / (x[1] - x[0]);
		
		if(k < 0)
		{
			s[4] = (y[0] * (t - x[1]) - y[1] * (t - x[0])) / (x[0] - x[1]);
		}
		
		goto END;
	}
	
	//---确定区间kk
	if(k < 0)	//根据t确定区间
	{
		if(t <= x[1])
		{
			kk = 0;
		}
		else
		{
			if(t >= x[n - 1])
			{
				kk = n - 2;
			}
	        else
			{
				kk = 1;
				m = n;
		
				while(((kk - m) != 1) && ((kk - m) != -1))
				{
					l = (kk + m) / 2;
					if(t < x[l - 1]) m = l;
					else kk = l;
				}
			
				kk = kk - 1;
			}
		}
	}
	else	//k>=0
	{
		kk = k;
	}
	
	//---计算
	if(kk >= n-1) kk = n - 2;
	
	u[2] = (y[kk + 1] - y[kk]) / (x[kk + 1] - x[kk]);
	if(n == 3)
	{
		if(kk == 0)
		{
			u[3] = (y[2] - y[1]) / (x[2] - x[1]);
			u[4] = 2.0 * u[3] - u[2];
			u[1] = 2.0 * u[2] - u[3];
			u[0] = 2.0 * u[1] - u[2];
		}
		else
		{
			u[1] = (y[1] - y[0]) / (x[1] - x[0]);
			u[0] = 2.0 * u[1] - u[2];
			u[3] = 2.0 * u[2] - u[1];
			u[4] = 2.0 * u[3] - u[2];
		}
	}
	else
	{ 
		if(kk <= 1)
		{
			u[3] = (y[kk + 2] - y[kk + 1]) / (x[kk + 2] - x[kk + 1]);
			
			if(kk == 1)
			{
				u[1] = (y[1] - y[0]) / (x[1] - x[0]);
				u[0] = 2.0 * u[1] - u[2];
				
				if(n == 4) u[4] = 2.0 * u[3] - u[2];
				
				else u[4] = (y[4] - y[3]) / (x[4] - x[3]);
			}	
			else
			{
				u[1] = 2.0 * u[2] - u[3];
				u[0] = 2.0 * u[1] - u[2];
				u[4] = (y[3] - y[2]) / (x[3] - x[2]);
			}
		}
		else if(kk >= (n - 3))
		{
			u[1]  = (y[kk] - y[kk - 1]) / (x[kk] - x[kk - 1]);
			
			if(kk == (n - 3))
			{
				u[3] = (y[n - 1] - y[n - 2]) / (x[n - 1]- x[n - 2]);
				u[4] = 2.0 * u[3] - u[2];
				
				if(n == 4) u[0] = 2.0 * u[1] - u[2];
				
				else u[0] = (y[kk - 1] - y[kk - 2]) / (x[kk - 1] - x[kk - 2]);
			}
			else
			{
				u[3] = 2.0 * u[2] - u[1];
				u[4] = 2.0 * u[3] - u[2];
				u[0] = (y[kk - 1] - y[kk - 2]) / (x[kk - 1] - x[kk - 2]);
			}
		}
		else
		{
			u[1] = (y[kk] - y[kk - 1]) / (x[kk] - x[kk - 1]);
			u[0] = (y[kk - 1] - y[kk - 2]) / (x[kk - 1] - x[kk - 2]);
			u[3] = (y[kk + 2] - y[kk + 1]) / (x[kk + 2] - x[kk + 1]);
			u[4] = (y[kk + 3] - y[kk + 2]) / (x[kk + 3] - x[kk + 2]);
		}
	}
	
	s[0] =  fabs(u[3] - u[2]);
	s[1] =  fabs(u[0] - u[1]);
	
	if(fabs(s[0]) <EPSILON && fabs(s[1]) < EPSILON)	//s[0]==0 && s[1]==0
		p = (u[1] + u[2]) / 2.0;
	
	else p = (s[0] * u[1] + s[1] * u[2]) / (s[0] + s[1]);

	s[0] = fabs(u[3] - u[4]);
	s[1] = fabs(u[2] - u[1]);

	if(fabs(s[0]) <EPSILON && fabs(s[1]) < EPSILON)	//s[0]==0 && s[1]==0
		q = (u[2] + u[3]) / 2.0;
	
	else q = (s[0] * u[2] + s[1] * u[3]) / (s[0] + s[1]);
	
	s[0] = y[kk];
	s[1] = p;
	s[3] = x[kk + 1] - x[kk];
	s[2] = (3.0 * u[2] - 2.0 * p - q) / s[3];
	s[3] = (q + p - 2.0 * u[2]) / (s[3] * s[3]);
	
	if(k < 0)	//计算t处的值
	{ 
		p = t - x[kk];
		s[4] = s[0] + s[1] * p + s[2] * p * p + s[3] * p * p * p;
	}

END: ;
*/
}

//阿克玛光滑等距插值
// yi=f(xi)
// 得到子区间[Xk,Xk+1](k=0,1,2,...,n-2)上的三次多项式的系数
//   s(x)=s0+s1(x-xk)+s2(x-xk)^2+s3(x-xk)^3
// x0	-->等距结点中第一个结点值
// h	-->等距结点中步长
// y	-->存放结点yi值的数组
// n	-->结点数量
// k	-->k>=0,表示只计算第k个子区间上的三次多项式系数s0,s1,s2,s3;
//		   k<0, 表示计算指定点t处的函数值f(t),保存在s[4],并计算插值点t所在子区间的多项式系数s0,s1,s2,s3;
// t	-->待插值结点的x值
// s	-->返回三次多项式系数s0,s1,s2,s3
// sn	-->数组s的维数,默认=5
void KInterpolate::AkimaIsometry(double x0, double h, double* y, int n, int k, double t, double* s,int sn)
{
	int kk, m, l;
    double u[5], p, q;

	//int n =  y.size();	//给定等距结点的个数
    
	for(m=0; m<5; m++) s[m] = 0.0;
    
	if(n < 1) goto END;

    if(n == 1) 
	{
		s[0] = y[0];
		s[4] = y[0]; 
		goto END;
	}

    if(n == 2)
	{
		s[0] = y[0];
		s[1] = (y[1] - y[0]) / h;
        
		if(k < 0) s[4] = (y[1] * (t - x0) - y[0] * (t - x0 - h)) / h;
        
		goto END;
	}
    
	if(k < 0)
	{
		if(t <= x0 + h) kk = 0;
		else 
			if(t >= x0 + (n - 1) * h) kk = n - 2;
			else
			{
				kk = 1;
				m = n;

				while(((kk - m) != 1) && ((kk - m) != -1))
				{
					l = (kk + m) / 2;
                
					if(t < x0 + (l - 1) * h) m = l;
					else kk = l;
				}
            
				kk = kk - 1;
			}
	}
	else kk = k;
  
	if(kk >= n - 1) kk = n - 2;
    
	u[2] = (y[kk + 1]- y [kk]) / h;
	
	if(n == 3)
	{
		if(kk == 0)
		{
			u[3] = (y[2] - y[1]) / h;
            u[4] = 2.0 * u[3] - u[2];
            u[1] = 2.0 * u[2] - u[3];
            u[0] = 2.0 * u[1] - u[2];
		}
        
		else
		{ 
			u[1] = (y[1] - y[0]) / h;
            u[0] = 2.0 * u[1] - u[2];
            u[3] = 2.0 * u[2] - u[1];
            u[4] = 2.0 * u[3] - u[2];
		}
	}  
	else
	{
		if(kk <= 1)
		{
			u[3] = (y[kk + 2] -y[kk + 1]) / h;
            
			if(kk == 1)
			{
				u[1] = (y[1] - y[0]) / h;
                u[0] = 2.0 * u[1] - u[2];
                
				if(n == 4) u[4] = 2.0 * u[3] - u[2];
				else u[4] = (y[4] - y[3]) / h;
			}
            else
			{ 
				u[1] = 2.0 *u[2] - u[3];
                u[0] = 2.0 *u[1] - u[2];
                u[4] = (y[3] - y[2]) / h;
			}
		}
		else if(kk >= (n - 3))
		{
			u[1] = (y[kk] - y[kk - 1]) / h;
            
			if(kk == (n - 3))
			{
				u[3] = (y[n - 1] - y[n - 2]) / h;
                u[4] = 2.0 * u[3] - u[2];
                
				if(n == 4) u[0] = 2.0 * u[1] - u[2];
                else u[0] = (y[kk - 1] - y[kk - 2]) / h;
			}   
			else
			{
				u[3] = 2.0 * u[2] - u[1];
                u[4] = 2.0 * u[3] - u[2];
                u[0] = (y[kk - 1] - y[kk - 2]) / h;
			}
		}
		else
		{
			u[1] = (y[kk] - y[kk - 1]) / h;
            u[0] = (y[kk - 1] - y[kk - 2]) / h;
            u[3] = (y[kk + 2] - y[kk + 1]) / h;
            u[4] = (y[kk + 3] - y[kk + 2]) / h;
		}
	}
     
	s[0] = fabs(u[3] - u[2]);
    s[1] = fabs(u[0] - u[1]);
    
	if(fabs(s[0])<1.0e-12 && fabs(s[1])<1.0e-12)	//s[0]==0 && s[1]==0
		p = (u[1] + u[2]) / 2.0;
	else
		p = (s[0] * u[1] + s[1] * u[2]) / (s[0] + s[1]);
    
	s[0] = fabs(u[3] - u[4]);
    s[1] = fabs(u[2] - u[1]);
    
	if(fabs(s[0])<1.0e-12 && fabs(s[1])<1.0e-12)	//s[0]==0 && s[1]==0
		q = (u[2] + u[3]) / 2.0;
	else
		q = (s[0] * u[2] + s[1] * u[3]) / (s[0] + s[1]);
	  
	s[0] = y[kk];
    s[1] = p;
    s[3] = h;
    s[2] = (3.0 * u[2] - 2.0 * p - q) / s[3];
    s[3] = (q+p - 2.0 * u[2]) / (s[3] * s[3]);
    
	if(k < 0)
	{
		p = t - (x0 + kk * h);
        s[4] = s[0] + s[1] * p + s[2] * p * p + s[3] * p * p * p;
	}
    
END: ;
}

//第一种边界条件的三次样条函数插值、微商与积分
// x	-->存放结点xi值的数组,长度=n
// y	-->存放结点yi值的数组,长度=n
// dy	-->double型数组,长度=n
//		   调用时，dy[0]存放给定的左端点处的一阶导数,dy[n-1]存放给定的右端点处的一阶导数
//		   返回时，存放n个给定的结点处的一阶导数
// ddy	-->double型数组,长度=n
//		   返回时，存放n个给定的结点处的二阶导数
// n	-->结点数量

// t	-->待插值结点的x值的数组,长度=m
// z	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的函数值
// dz	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的一阶导数
// ddz	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的二阶导数
// 返回:
//		区间上的积分值
double KInterpolate::SplineBoundary1(double* x, double* y, double* dy, double* ddy, int n,
			double* t, double* z, double* dz,double* ddz,int m)
{
	int i, j;
    double h0, h1, alpha, beta, g;

	//int n =  y.size();	//数组y的长度(元素个数)，给定结点个数
	//int m =  t.size();	//指定插值点的个数
    
	double* s = new double[n];

    s[0] = dy[0];
	dy[0] = 0.0;
    h0 = x[1] - x[0];

    for(j = 1; j < n - 1; j ++)
	{
		h1 = x[j + 1] - x[j];
        alpha = h0 / (h0 + h1);
        
		beta = (1.0 - alpha) * (y[j] - y[j - 1]) / h0;
        beta = 3.0 * (beta + alpha * (y[j + 1] - y[j]) / h1);
        
		dy[j] = -alpha / (2.0 + (1.0 - alpha) * dy[j - 1]);
        s[j] = (beta - (1.0 - alpha) * s[j - 1]);
        s[j] = s[j] / (2.0 + (1.0 - alpha) * dy[j - 1]);
        
		h0 = h1;
	}
    
	for(j = n - 2; j >= 0; j --)
	{
		dy[j] = dy[j] * dy[j + 1] + s[j];
	}

    for(j = 0; j < n - 1; j ++) 
	{
		s[j] = x[j + 1] - x[j];
	}
    
	for(j = 0 ; j < n - 1; j ++)
	{
		h1 = s[j] * s[j];
        ddy[j] = 6.0 * (y[j + 1] - y[j]) / h1 - 2.0 * (2.0 * dy[j] + dy[j + 1]) / s[j];
	}

    h1 = s[n - 2] * s[n - 2];
    ddy[n - 1] = 6.0 * (y[n - 2] - y[n - 1]) / h1 + 2.0 * (2.0 * dy[n - 1] 
		         + dy[n - 2]) / s[n - 2];
    g = 0.0;
    
	for(i = 0;i < n - 1; i ++)
	{
		h1 = 0.5 * s[i] * (y[i] + y[i + 1]);
        h1 = h1 - s[i] * s[i] * s[i] * (ddy[i] + ddy[i + 1]) / 24.0;
        g = g + h1;
	}
    
	for(j = 0; j <= m - 1; j ++)
	{
		if(t[j] >= x[n - 1]) i = n - 2;
        else
		{
			i = 0;
            while(t[j] > x[i + 1]) i ++;
		}
        
		h1 = (x[i + 1] - t[j]) / s[i];
        h0 = h1 * h1;
        
		z[j] = (3.0 * h0 - 2.0 * h0 * h1) * y[i];
        z[j] = z[j] + s[i] * (h0 - h0 * h1) * dy[i];
        
		dz[j] = 6.0 * (h0 - h1) * y[i] / s[i];
        dz[j] = dz[j] + (3.0 * h0 - 2.0 * h1) * dy[i];
        
		ddz[j] = (6.0 - 12.0 * h1) * y[i] / (s[i] * s[i]);
        ddz[j] = ddz[j] + (2.0 - 6.0 * h1) * dy[i] / s[i];
        
		h1 = (t[j] - x[i]) / s[i];
        h0 = h1 * h1;
        
		z[j] = z[j] + (3.0 * h0 -2.0 * h0 * h1) * y[i + 1];
        z[j] = z[j] - s[i] * (h0 - h0 * h1) * dy[i + 1];
        
		dz[j] = dz[j] - 6.0 * (h0 - h1) * y[i + 1] / s[i];
        dz[j] = dz[j] + (3.0 * h0 - 2.0 * h1) * dy[i + 1];
        
		ddz[j] = ddz[j] + (6.0 - 12.0 * h1) * y[i + 1] / (s[i] * s[i]);
        ddz[j] = ddz[j] - (2.0 - 6.0 * h1) * dy[i + 1] / s[i];
	}
    
	//---
	delete []s;
	//---
	return(g);
}

//第二种边界条件的三次样条函数插值、微商与积分
// x	-->存放结点xi值的数组,长度=n
// y	-->存放结点yi值的数组,长度=n
// dy	-->double型数组,长度=n
//		   返回时，存放n个给定的结点处的一阶导数
// ddy	-->double型数组,长度=n
//		   调用时，ddy[0]存放给定的左端点处的二阶导数,ddy[n-1]存放给定的右端点处的二阶导数
//		   返回时，存放n个给定的结点处的二阶导数
// n	-->结点数量

// t	-->待插值结点的x值的数组,长度=m
// z	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的函数值
// dz	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的一阶导数
// ddz	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的二阶导数
// 返回:
//		区间上的积分值
double KInterpolate::SplineBoundary2(double* x, double* y, double* dy, double* ddy, int n,
			double* t, double* z, double* dz,double* ddz,int m)
{
	int i, j;
    double h0, h1, alpha, beta, g;

	//int n =  y.size();	//数组y的长度(元素个数)，给定结点个数
	//int m =  t.size();	//指定插值点的个数
    
	double* s = new double[n];
    
	dy[0] = -0.5;
    h0 = x[1] - x[0];
    s[0] = 3.0 * (y[1] - y[0]) / (2.0 * h0) - ddy[0] * h0 / 4.0;
    
	for(j = 1; j < n - 1; j ++)
	{
		h1 = x[j + 1] - x[j];
        alpha = h0 / (h0 + h1);
        
		beta = (1.0 - alpha) * (y[j] - y[j - 1]) / h0;
        beta = 3.0 * (beta + alpha * (y[j + 1] - y[j]) / h1);
        
		dy[j] = -alpha / (2.0 + (1.0 - alpha) * dy[j - 1]);
        s[j] = (beta - (1.0 - alpha) * s[j - 1]);
        s[j] = s[j] / (2.0 + (1.0 - alpha) * dy[j - 1]);
        
		h0 = h1;
	}
    
	dy[n - 1] = (3.0 * (y[n - 1] - y[n - 2]) / h1 + ddy[n - 1] * h1 
		        / 2.0 - s[n - 2]) / (2.0 + dy[n - 2]);
    
	for(j = n - 2; j >= 0; j --)
	{
		dy[j] = dy[j] * dy[j + 1 ] + s[j];
	}
    
	for(j =0; j < n - 1; j ++)
	{
		s[j] = x[j + 1] - x[j];
	}

    for(j =0; j < n - 1; j ++)
	{
		h1 = s[j] * s[j];
        ddy[j] = 6.0 * (y[j + 1] - y [j]) / h1 - 2.0 * (2.0 * dy[j] + dy[j + 1]) / s[j];
	}
    
	h1 = s[n - 2] * s[n - 2];
    ddy[n - 1]= 6.0 * (y[n - 2] - y[n - 1]) / h1 + 2.0 * (2.0 * dy[n - 1] 
		        + dy[n - 2]) / s[n - 2];
    g = 0.0;
    
	for(i = 0; i < n - 1; i ++)
	{
		h1 = 0.5 * s[i] * (y[i] + y[i + 1]);
        h1 = h1 - s[i] * s[i] * s[i] * (ddy[i] + ddy[i + 1]) / 24.0;
        g = g + h1;
	}
    
	for(j = 0; j <= m - 1; j ++)
	{
		if(t[j] >= x[n - 1]) i = n - 2;
        else
		{
			i = 0;
            while(t[j] > x[i + 1]) i = i + 1;
		}
        
		h1 = (x[i + 1] - t[j]) / s[i];
        h0 = h1 * h1;
        
		z[j] = (3.0 * h0 - 2.0 * h0 * h1) * y[i];
        z[j] = z[j] + s[i] * (h0 - h0 * h1) * dy[i];
        
		dz[j] = 6.0 * (h0 - h1) * y[i] / s[i];
        dz[j] = dz[j] + (3.0 * h0 - 2.0 * h1) * dy[i];
        
		ddz[j] = (6.0 - 12.0 * h1) * y[i] / (s[i] * s[i]);
        ddz[j] = ddz[j] + (2.0 - 6.0 * h1) * dy[i] / s[i];
        
		h1 = (t[j] - x[i]) / s[i];
        h0 = h1 * h1;
        
		z[j] = z[j] + (3.0 * h0 - 2.0 * h0 * h1) * y[i + 1];
        z[j] = z[j] - s[i] * (h0 - h0 * h1)* dy[i + 1];
        
		dz[j] = dz[j] - 6.0 * (h0 - h1) * y[i + 1] / s[i];
        dz[j] = dz[j] + (3.0 * h0 - 2.0 * h1) * dy[i + 1];
        
		ddz[j] = ddz[j] + (6.0 - 12.0 * h1) * y[i + 1] / (s[i] * s[i]);
        ddz[j] = ddz[j] - (2.0 - 6.0 * h1) * dy[i + 1] / s[i];
	}
    
	//---
	delete []s;
	//---
    return(g);
}

//第二种边界条件的三次样条函数插值、微商与积分
// x	-->存放结点xi值的数组,长度=n
// y	-->存放结点yi值的数组,长度=n
//         要求y(0)=y(n-1)
// dy	-->double型数组,长度=n
//		   返回时，存放n个给定的结点处的一阶导数
// ddy	-->double型数组,长度=n
//		   返回时，存放n个给定的结点处的二阶导数
// n	-->结点数量

// t	-->待插值结点的x值的数组,长度=m
// z	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的函数值
// dz	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的一阶导数
// ddz	-->double型数组,长度=m
//		   返回时，存放m个指定插值点处的二阶导数
// 返回:
//		区间上的积分值
double KInterpolate::SplineBoundary3(double* x, double* y, double* dy, double* ddy, int n,
			double* t, double* z, double* dz,double* ddz,int m)
{
	int i, j;
    double h0, y0, h1, y1, alpha, beta, u, g;

	//int n =  y.size();	//数组y的长度(元素个数)，给定结点个数
	//int m =  t.size();	//指定插值点的个数
    
	double* s = new double[n];
    
	h0 = x[n -1 ] - x[n - 2];
    y0 = y[n - 1] - y[n - 2];
    
	dy[0] = 0.0;
	ddy[0] = 0.0;
	ddy[n - 1] = 0.0;

    s[0] = 1.0;
	s[n - 1] = 1.0;
    
	for(j = 1; j < n; j ++)
	{
		h1 = h0;
		y1 = y0;
        h0 = x[j] - x[j - 1];
        y0 = y[j] - y[j - 1];
        
		alpha = h1 / (h1 + h0);
        beta = 3.0 * ((1.0 - alpha) * y1 / h1 + alpha * y0 / h0);
        
		if(j < n - 1)
		{
			u = 2.0 + (1.0 - alpha) * dy[j - 1];
            dy[j] = -alpha / u;
            s[j] = (alpha - 1.0) * s[j - 1] / u;
            ddy[j] = (beta - (1.0 - alpha) * ddy[j - 1]) / u;
		}
	}
    
	for(j = n - 2; j >= 1; j--)
	{
		s[j] = dy[j] * s[j + 1] + s[j];
        ddy[j] = dy[j] * ddy[j + 1] + ddy[j];
	}
    
	dy[n-2] = (beta - alpha * ddy[1] - (1.0 - alpha) * ddy[n - 2])
		      / (alpha * s[1] + (1.0 - alpha)* s[n - 2] + 2.0);
    
	for(j = 2; j < n; j ++)
	{
        dy[j-2]=s[j-1]*dy[n-2]+ddy[j-1];
	}
	
	dy[n - 1] = dy[0];
    
	for(j = 0; j < n - 1; j++)
	{
		s[j] = x[j + 1] - x[j];
	}

    for(j = 0; j < n - 1; j++)
	{
		h1 = s[j] * s[j];
        ddy[j] = 6.0 * (y[j + 1] - y[j]) / h1 - 2.0 
			     * (2.0 * dy[j] + dy[j + 1]) / s[j];
	}

    h1 = s[n - 2] * s[n - 2];
    ddy[n - 1] = 6.0 * (y[n - 2] - y[n - 1]) / h1 
		         + 2.0 * (2.0 * dy[n - 1] + dy[n - 2]) / s[n - 2];
    g = 0.0;

	for(i = 0; i < n - 1; i++)
    {
		h1 = 0.5 * s[i] * (y[i] + y[i + 1]);
        h1 = h1 - s[i] * s[i] * s[i] * (ddy[i] + ddy[i + 1]) / 24.0;
        g = g + h1;
	}
    
	for(j = 0; j <= m - 1; j++)
	{
		h0 = t[j];
        
		while(h0 >= x[n - 1])
		{
			h0 = h0 - (x[n - 1] - x[0]);
		}
        while(h0 < x[0])
		{
			h0 = h0 + (x[n - 1] - x[0]);
		}
       
		i = 0;
        
		while(h0 > x[i + 1]) i++;
        
		u = h0;
        h1 = (x[i + 1] -u ) / s[i];
        h0 = h1 * h1;
        
		z[j] = (3.0 * h0 - 2.0 * h0 * h1) * y[i];
        z[j] = z[j] + s[i] * (h0 - h0 * h1) * dy[i];
         
		dz[j] = 6.0 * (h0 - h1) * y[i] / s[i];
        dz[j] = dz[j] + (3.0 * h0 - 2.0 * h1) * dy[i];
        
		ddz[j] = (6.0 - 12.0 * h1) * y[i] / (s[i] * s[i]);
        ddz[j] = ddz[j] + (2.0 - 6.0 * h1) * dy[i] / s[i];
        
		h1 = (u - x[i]) / s[i];
        h0 = h1 * h1;
        
		z[j] = z[j] + (3.0 * h0 - 2.0 * h0 * h1) * y[i + 1];
        z[j] = z[j] - s[i] * (h0 - h0 * h1) * dy[i + 1];
        
		dz[j] = dz[j] - 6.0 * (h0 - h1) * y[i + 1] / s[i];
        dz[j] = dz[j] + (3.0 * h0 - 2.0 * h1) * dy[i + 1];
        
		ddz[j] = ddz[j] + (6.0 - 12.0 * h1) * y[i + 1] / (s[i] * s[i]);
        ddz[j] = ddz[j] - (2.0 - 6.0 * h1) * dy[i + 1] / s[i];
	}
    
	//---
	delete []s;
	//---
	return(g);
}

//二元三点插值
//给定矩形域上n*m个结点(xi,yj),(i=0,1,2,...,n-1) (j=0,1,2,...,m-1)
//的函数值zij=z(xi,yj),使用二元三点插值公式计算指定插值点(u,v)处的函数值w=z(u,v)
// x	-->double 型数组,长度=n,存放x方向的n个坐标
// y	-->double 型数组,长度=m,存放y方向的m个坐标
// z	-->n*m矩阵,存放结点上的函数值
// u	-->指定插值点x方向坐标
// v	-->指定插值点y方向坐标
double KInterpolate::InterpV2P3(double*x,int n,double* y,int m,KMatrix z,double u,double v)
{
	int nn(3),mm,ip,iq,i,j,k,l;
    double b[3],h,w;

	//int n =  x.size();	//给定结点X方向上的坐标个数
	//int m =  y.size();	//给定结点Y方向上的坐标个数
    
	if(n < 4)
	{
		ip = 0;
		nn = n;
	}
    else if(u <= x[1]) ip = 0;
    else if(u >= x[n - 2]) ip = n - 3;
    else
	{
		i = 1;
		j = n;
        
		while( ((i - j) != 1) && ((i - j) != -1) )
		{
			l = (i + j) / 2;
            
			if(u < x[l - 1]) j = l;
            else i = l;
		}
        
		if(fabs(u - x[i - 1]) < fabs(u - x[j - 1])) ip = i - 2;
        else ip = i-1;
      }

    mm = 3;
    
	if(m < 4)
	{
		iq = 0;
		mm = m;
	}
    else if(v <= y[1]) iq = 0;
    else if(v >= y[m - 2]) iq = m - 3;
    else
	{
		i = 1;
		j = m;
        
		while( ((i - j) != 1) && ((i - j) != -1) )
		{
			l = (i + j) / 2;
            if(v < y[l - 1]) j = l;
            else i = l;
		}
        
		if(fabs(v - y[i - 1]) < fabs(v - y[j - 1])) iq = i - 2;
        else iq = i - 1;
	}
    
	for(i = 0; i < nn; i ++)
	{
		b[i] = 0.0;

        for(j = 0; j < mm; j ++)
		{
			h = z(ip + i,iq +j);

            for(k = 0; k < mm; k ++)
			{
				if(k != j)
					h = h * (v - y[iq + k]) / (y[iq + j] - y[iq + k]);
			}
			
			b[i] = b[i] + h;
		}
	}
    
	w = 0.0;
    
	for(i = 0; i < nn; i ++)
	{
		h = b[i];
        
		for(j = 0; j < nn; j ++)
		{
			if(j != i)
				h = h * (u - x[ip + j]) / (x[ip + i] - x[ip + j]);
		}
		
		w = w + h;
	}
    
	return(w);
}

//二元全区间插值
//给定矩形域上n*m个结点(xi,yj),(i=0,1,2,...,n-1) (j=0,1,2,...,m-1)
//的函数值zij=z(xi,yj),使用二元三点插值公式计算指定插值点(u,v)处的函数值w=z(u,v)
// x	-->double 型数组,长度=n,存放x方向的n个坐标
// y	-->double 型数组,长度=m,存放y方向的m个坐标
// z	-->n*m矩阵,存放结点上的函数值
// u	-->指定插值点x方向坐标
// v	-->指定插值点y方向坐标
double KInterpolate::InterpV2Whole(double*x,int n,double* y,int m,KMatrix z,double u,double v)
{
	
	int ip, ipp, i, j, l, iq, iqq, k;
    double h, w;

	double* b = new double[10];

	//int n =  x.size();	//给定结点X方向上的坐标个数
	//int m =  y.size();	//给定结点Y方向上的坐标个数

	if(u<x[0] || fabs(u-x[0])<1.0e-12)	//u==x[0]
	{
		ip = 1;
		ipp = 4;
	}
    else if(u>x[n-1] || fabs(u-x[n-1])<1.0e-12)	//u==x[n-1]
	{
		ip = n - 3;
		ipp = n;
	}
    else
	{
		i = 1;
		j = n;
        
		while(((i - j) != 1) && ((i - j) != -1))
		{
			l = (i + j) / 2;
            
			if(u < x[l - 1]) j = l;
            else i = l;
		}
        
		ip = i - 3; 
		ipp = i + 4;
	}
    
	if(ip < 1) ip = 1;
    if(ipp > n) ipp = n;
    if(v < y[0] || fabs(v-y[0])<1.0e-12)	//v=y[0]
	{ 
		iq = 1;
		iqq = 4; 
	}
    else if(v > y[m - 1] || fabs(v-y[m - 1])<1.0e-12)	//v==y[m-1]
	{
		iq = m - 3;
		iqq = m;
	}
    else
	{
		i = 1;
		j =m;
        
		while(((i - j) != 1) && ((i - j) != -1))
		{
			l = (i + j) / 2;
            
			if(v <y [l - 1]) j = l;
            else i = l;
		}
        
		iq = i - 3;
		iqq = i + 4;
	}
    
	if(iq < 1) iq = 1;
    if(iqq > m) iqq = m;
    
	for(i = ip - 1; i < ipp; i ++)
	{
		b[i -ip + 1] = 0.0;
        
		for(j = iq - 1; j < iqq; j ++)
		{
			h = z(i,j);
            
			for(k = iq - 1; k < iqq; k ++)
			{
				if(k != j)
					h = h * (v - y[k]) / (y[j] - y[k]);
			}
			
			b[i- ip + 1] = b[i - ip + 1] + h;
		}
	}
    
	w = 0.0;
    
	for(i = ip - 1; i < ipp; i ++)
	{
		h = b[i - ip + 1];
        
		for(j = ip - 1; j < ipp; j ++)
		{
			if(j != i)
				h = h * (u - x[j]) / (x[i] - x[j]);
		}
        
		w =w + h;
	}
    
	//---
	delete []b;
	//---
	return(w);
}
