#include "StdAfx.h"
#include "KRegression.h"
#include "KStatistic.h"

KRegression::KRegression(void)
{
}

//=======================================================
// ��    ��: RegressionLinear1D
// ��    ��: public
//
// ��    ��: һԪ���Իع����
//
// ��    ��:
//			 [i] x - һά����,����Ϊn,����Ա���x��n��ȡֵ
//			 [i] y - һά����,����Ϊn,������Ա���x��Ӧ���������y�Ĺ۲�ֵ
//			 [o] a - һά����,����Ϊ2
//				 a[0]���ػع�ϵ��a0(�ؾ�)
//				 a[1]���ػع�ϵ��a1(б��)
//			 [o] dt - һά����,����Ϊ6
//				 dt[0]����ƫ��ƽ����
//				 dt[1]����ƽ����׼ƫ��
//				 dt[2]���ػع�ƽ����
//				 dt[3]�������ƫ��
//				 dt[4]������Сƫ��
//				 dt[5]����ƫ��ƽ��ֵ
//
// ��    ��: 
//
// ע    ��: y = a0 + a1*x
// 
//
//=======================================================

void KRegression::Linear1D(double* x,double* y,int n,double a[2],double dt[6])
{
	int i;

	double e(0), f(0), u(0),umax(0), umin(1.0e+30);
    double p(0),q(0);

	//xƽ��ֵ
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

//��Ԫ���Իع����(m������n���۲�ֵ,n>=4)
//y=a0x0+a1x1+...+a(m-1)x(m-1)+am
// xy	-->����,���(m+1)*n;
//         ��i�д�ŵ�i���Ա���xi��n���۲�ֵ(i=0,1,...,m-1)
//         ��m��(���һ��)����������y��n���۲�ֵ
//
//         ����ʱxy�����ݽ����ƻ�
//         ��0��xy(i,0)���m+1���ع�ϵ��a0,a1,...,a(m)  (i=0,1,2,...,m)
//         ��1��xy(i,1)���m���Ա�����ƫ���ϵ��
//         ��2��xy(i,2)���4����ֵ,
//              xy(0,2)���ƫ��ƽ����
//              xy(1,2)���ƽ����׼ƫ��
//              xy(2,2)��Ÿ����ϵ��
//              xy(3,2)��Żع�ƽ����
//=======================================================
// ��    ��: 
// ��    ��: public
//
// ��    ��: 
//
// ��    ��:
//			 [i] iRChannel - ��ɫͨ��
//
// ��    ��: 
//			 TRUE - �ɹ�
//			 FALSE - ʧ��
//
// ע    ��: 
//
//=======================================================

void KRegression::LinearMD(KMatrix& xy)
{
	int i,j;

	double r(0),s(0);
    double q(0), e(0), u(0);
	double yy(0);
	double dt[4];

	int m = xy.GetRowNum()-1;			//�Ա�������
	int n = xy.GetColNum();				//�۲����ݵ�����

	//Ϊ����ƽ���������Գ����������麯��׼��������������matrix����
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

	//����ƽ���������Գ�����������ĺ���
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

//�𲽻ع����
// xy	-->����,���(n+1)*k��nΪ��ѡ����������kΪ��������
//         ǰn�д��Ա�������X��k�ι۲�ֵ,���һ��Ϊ�����Y��k�ι۲�ֵ
//          
// f1	-->��Ҫѡ������ʱ�����Լ����F-�ֲ�ֵ
// f2	-->��Ҫ�޳�����ʱ�����Լ����F-�ֲ�ֵ
//
//����:
// b	-->double������,����Ϊn+1��
//         ���ػع鷽���и����ӵĻع�ϵ����������b0,b1,...,bn
//         ע��bnΪ������
// xx	-->double������,����Ϊn+1��
//         ����ǰn����������n���Ա������ӵ�����ƽ��ֵ����n+1������Ϊy������ƽ��ֵ
// v	-->double������,����Ϊn+1��
//         ����ǰn���������ظ����ӵ�ƫ�ع�ƽ����vi�����һ���������زв�ƽ����q
// s	-->double������,����Ϊn+1��
//         ����ǰn���������ظ����ӻع�ϵ���ı�׼ƫ��si��
//		   ���һ���������زв�ƽ���͹��Ƶı�׼ƫ��s(��������ƫ������)
//
// ye	-->double������,����Ϊk��
//         �����������������ֵ��k������ֵei(��Ӧ��k���۲�ֵ)
// yr	-->double������,����Ϊk��
//         �����������k���۲�ֵ�Ĳв�
// r	-->����,���(n+1)*(n+1)��
//         �������յĹ�񻯵�ϵ�������
// dt	-->double������,����Ϊ2��
//         dt[0]���ظ����ϵ��c��dt[1]����F-����ֵ
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
	
	KMatrix x(xy.MatrixTranspose());	//�Ѻ�������ת������������

	int k = x.GetRowNum();			//�۲����(��������)
	int n = x.GetColNum()-1;		//�Ա���x�ĸ���(��ѡ����)

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
