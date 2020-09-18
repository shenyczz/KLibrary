#include "StdAfx.h"
#include "KRegression.h"
#include "KStatistic.h"

KRegression::KRegression(void)
{
}

//=======================================================
// 名    称: RegressionLinear1D
// 访    问: public
//
// 功    能: 一元线性回归分析
//
// 参    数:
//			 [i] x - 一维数组,长度为n,存放自变量x的n个取值
//			 [i] y - 一维数组,长度为n,存放与自变量x对应的随机变量y的观测值
//			 [o] a - 一维数组,长度为2
//				 a[0]返回回归系数a0(截距)
//				 a[1]返回回归系数a1(斜率)
//			 [o] dt - 一维数组,长度为6
//				 dt[0]返回偏差平方和
//				 dt[1]返回平均标准偏差
//				 dt[2]返回回归平方和
//				 dt[3]返回最大偏差
//				 dt[4]返回最小偏差
//				 dt[5]返回偏差平均值
//
// 返    回: 
//
// 注    释: y = a0 + a1*x
// 
//
//=======================================================

void KRegression::Linear1D(double* x,double* y,int n,double a[2],double dt[6])
{
	int i;

	double e(0), f(0), u(0),umax(0), umin(1.0e+30);
    double p(0),q(0);

	//x平均值
	double xavg = KStatistic::RsAvg(x,n);
	double yavg = KStatistic::RsAvg(y,n);
    
	q = 0;
    for(i=0; i<n; i++)
    {
		q = x[i] - xavg;
		e += q * q;
        f += q * (y[i]-yavg);
    }

    a[1] = f / e;
	a[0] = yavg -a[1] * xavg;
	//--------------------------------------------
	p = 0;
	q = 0;
	u = 0;
    for(i=0; i<n; i++)
    {
		double s = a[1] * x[i] + a[0];

        q += (y[i]-s) * (y[i]-s);
        p += (s-yavg) * (s-yavg);
        e = fabs(y[i]-s);
        if (e>umax) umax=e;
        if (e<umin) umin=e;
        u += e/n;
    }
    dt[0]=q;
    dt[1]=sqrt(q/n);
	dt[2]=p;
    dt[3]=umax;
	dt[4]=umin;
	dt[5]=u;
}

//多元线性回归分析(m个变量n个观测值,n>=4)
//y=a0x0+a1x1+...+a(m-1)x(m-1)+am
// xy	-->矩阵,体积(m+1)*n;
//         第i行存放第i个自变量xi的n个观测值(i=0,1,...,m-1)
//         第m行(最后一行)存放随机变量y的n个观测值
//
//         返回时xy的内容将被破坏
//         第0列xy(i,0)存放m+1个回归系数a0,a1,...,a(m)  (i=0,1,2,...,m)
//         第1列xy(i,1)存放m个自变量的偏相关系数
//         第2列xy(i,2)存放4个数值,
//              xy(0,2)存放偏差平方和
//              xy(1,2)存放平均标准偏差
//              xy(2,2)存放复相关系数
//              xy(3,2)存放回归平方和
//=======================================================
// 名    称: 
// 访    问: public
//
// 功    能: 
//
// 参    数:
//			 [i] iRChannel - 红色通道
//
// 返    回: 
//			 TRUE - 成功
//			 FALSE - 失败
//
// 注    释: 
//
//=======================================================

void KRegression::LinearMD(KMatrix& xy)
{
	int i,j;

	double r(0),s(0);
    double q(0), e(0), u(0);
	double yy(0);
	double dt[4];

	int m = xy.GetRowNum()-1;			//自变量个数
	int n = xy.GetColNum();				//观测数据的组数

	//为调用平方根法求解对称正定方程组函数准备，其两参数须matrix类型
	KMatrix b((m+1),(m+1));
	KMatrix aa(m+1,1);

	double* v = new double[m+1];
	double* y = new double[n];

	for(i=0;i<m+1;i++)
	{
		v[i]=0.0;
	}
	for(i=0;i<n;i++)
	{
		y[i]=xy(m,i);
		if(i<=4) dt[i]=0.0;
	}

    int mm = m + 1;
	b(m, m) = n;

    for( j=0; j<m; j++)
    {
		double p(0);
        for(int i=0; i<n; i++)	p = p + xy(j,i);
		b(m,j) = p;
		b(j,m) = p;
    }
	//=====================
	//b.Trace();
 	//=====================
   for(i=0; i<m; i++)
      for(j=i; j<m; j++)
      {
		  double p(0);
          for(int k=0; k<n; k++)	p=p+xy(i,k)*xy(j,k);
		  b(j,i) = p;
		  b(i,j) = p;

      }
	//=====================
	//b.Trace();
	//b.IsSymmetry();
	//b.IsRegular();
 	//=====================

	aa(m,0) = 0.0;
    for(i=0; i<n; i++) aa(m,0) += y[i];
    for(i=0; i<m; i++)
    {
		aa(i,0) = 0.0;
		for(j=0; j<n; j++)	aa(i,0) = aa(i,0) + xy(i,j) * y[j];
    }
	//=====================
	//aa.Trace();
 	//=====================

	//调用平方根法求解对称正定方程组的函数
    if(KLinearEquation::SymmetryRegularEuationSquareRoot(b,aa)<1)
	{
		//TRACE("Matrix is not Symmetry and Regular!\n");
		goto END;
	}
    
    for (i=0; i<n; i++)	yy = yy + y[i] / n;
    for(i=0; i<n; i++)
    {
		double p = aa(m,0);
		for(j=0; j<m; j++)	p = p + aa(j,0) * xy(j,i);
        q=q+(y[i]-p)*(y[i]-p);
        e=e+(y[i]-yy)*(y[i]-yy);
        u=u+(yy-p)*(yy-p);
    }
    s = sqrt(q/n);
    r = sqrt(1.0-q/e);

    for(j=0; j<m; j++)
    {
		double p(0);
        for (i=0; i<n; i++)
        {
			double pp = aa(m,0);
            for(int k=0; k<m; k++)
              if(k!=j) pp = pp + aa(k,0) * xy(k,i);
            p = p + (y[i] - pp) * (y[i] - pp);
        }
        v[j] = sqrt(1.0 - q / p);
    }
    dt[0] = q;
	dt[1] = s;
	dt[2] = r;
	dt[3] = u;

END:
	//---
	v[m]=0.0;
	xy.SetSize(m+1,n);
	for(i=0;i<m+1;i++)
	{
		xy(i,0)=aa(i,0);
		xy(i,1)=v[i];
		if(i<=4)	xy(i,2)=dt[i];
	}
	//---
	delete []v;
	delete []y;
	//---
	return;
}

//逐步回归分析
// xy	-->矩阵,体积(n+1)*k。n为待选因子数量，k为样本容量
//         前n列存自变量因子X的k次观测值,最后一列为因变量Y的k次观测值
//          
// f1	-->置要选入因子时显著性检验的F-分布值
// f2	-->置要剔出因子时显著性检验的F-分布值
//
//返回:
// b	-->double型数组,长度为n+1。
//         返回回归方程中各因子的回归系数及常数项b0,b1,...,bn
//         注意bn为常数项
// xx	-->double型数组,长度为n+1。
//         其中前n个分量返回n个自变量因子的算术平均值，第n+1个分量为y的算术平均值
// v	-->double型数组,长度为n+1。
//         其中前n个分量返回各因子的偏回归平方和vi，最后一个分量返回残差平方和q
// s	-->double型数组,长度为n+1。
//         其中前n个分量返回各因子回归系数的标准偏差si，
//		   最后一个分量返回残差平方和估计的标准偏差s(均方差无偏估计量)
//
// ye	-->double型数组,长度为k。
//         返回因变量条件期望值的k个估计值ei(对应于k个观测值)
// yr	-->double型数组,长度为k。
//         返回因变量的k个观测值的残差
// r	-->矩阵,体积(n+1)*(n+1)。
//         返回最终的规格化的系数相关阵
// dt	-->double型数组,长度为2。
//         dt[0]返回复相关系数c，dt[1]返回F-检验值
void KRegression::Stepwise(KMatrix& xy,double f1,double f2,
		double* b,double* xx,double* v,double* s,
		double* ye,double* yr,
		KMatrix& r,
		double dt[2],
		double eps)
{
	int i,j;

	int ii,l;
    double phi,sd,fmi,fmx;
	
	KMatrix x(xy.MatrixTranspose());	//把横资料镇转换成竖资料阵

	int k = x.GetRowNum();			//观测次数(样本容量)
	int n = x.GetColNum()-1;		//自变量x的个数(待选因子)

	r.SetSize(n+1,n+1);

    int m = n + 1;
	double q(0);
    for( j=0; j<=n; j++)
    {
		double z(0);
        for(int i=0; i<k; i++)	z = z + x(i,j) / k;
        xx[j] = z;
    }
    for( i=0; i<=n; i++)
      for(j=0; j<=i; j++)
      {
		  double z(0);
          for(ii=0; ii<k; ii++)
			  z = z + (x(ii,i) - xx[i]) * (x(ii,j) - xx[j]);
          r(i,j) = z;
      }
    for(i=0; i<=n; i++)	ye[i]=sqrt(r(i,i));
    for (i=0; i<=n; i++)
      for (j=0; j<=i; j++)
      {
		  r(i,j) = r(i,j) / (ye[i] * ye[j]);
          r(j,i) = r(i,j);
      }
    phi = k - 1.0;
    sd = ye[n] / sqrt(k-1.0);
    int it(1);
    while(it==1)
    {
		it = 0;
		double vmi(1.0e+35), vmx(0);
        int imi(-1), imx(-1);
        for(i=0; i<=n; i++)
        {
			v[i]=0.0;
			b[i]=0.0;
			s[i]=0.0;
		}
        for(i=0; i<n; i++)
          if(r(i,i) > eps || fabs(r(i,i)-eps)<1.0e-12)
           {
			  v[i] = r(i,n) * r(n,i) / r(i,i);
              if (v[i] > 0.0 || fabs(v[i])<1.0e-12)
              { 
				  if(v[i]>vmx)
                  {
					vmx=v[i];
					imx=i;
				  }
              }
              else
              {
				  b[i] = r(i,n) * ye[n] / ye[i];
                  s[i] = sqrt(r(i,i)) * sd / ye[i];
                  if(fabs(v[i])<vmi)
                  {
					  vmi=fabs(v[i]);
					  imi=i;
				  }
              }
          }
        if(phi!=n-1.0)
        {
			double z(0);
            for (i=0; i<n; i++)	z = z + b[i] * xx[i];
            b[n] = xx[n] - z;
			s[n] = sd;
			v[n] = q;
        }
        else
        {
			b[n]=xx[n];
			s[n]=sd;
		}
        fmi = vmi * phi / r(n,n);
        fmx = (phi-1.0) * vmx / (r(n,n)-vmx);
        if((fmi<f2)||(fmx>=f1))
        {
			if(fmi<f2)
            {
				phi=phi+1.0;
				l=imi;
			}
            else
            {
				phi=phi-1.0;
				l=imx;
			}
            for(i=0; i<=n; i++)
              if (i!=l)
                for (j=0; j<=n; j++)
                  if (j!=l)
                    r(i,j)=r(i,j)-(r(l,j)/r(l,l))*r(i,l);
            for(j=0; j<=n; j++)
              if(j!=l)
                r(l,j)=r(l,j)/r(l,l);
            for(i=0; i<=n; i++)
              if(i!=l)
                r(i,l)=-r(i,l)/r(l,l);
            r(l,l)=1.0/r(l,l);
            q=r(n,n)*ye[n]*ye[n];
            sd=sqrt(r(n,n)/phi)*ye[n];
            dt[0]=sqrt(1.0-r(n,n));
            dt[1]=(phi*(1.0-r(n,n)))/((k-phi-1.0)*r(n,n));
            it=1;
        }
    }
    for(i=0; i<k; i++)
    {
		double z(0);
        for(j=0; j<n; j++)	z=z+b[j]*x(i,j);
        ye[i]=b[n]+z; 
		yr[i]=x(i,n)-ye[i];
    }
}
