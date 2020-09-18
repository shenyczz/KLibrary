#include "StdAfx.h"
#include "KLinearEquation.h"

KLinearEquation::KLinearEquation(void)
{
}

//=============================================================================
// ��    ��: TotalChoiceGauss
// ��    ��: public
//
// ��    ��: ȫѡ��Ԫ��˹��ȥ��
//
// ��    ��:
//			 [i] a - n*n����,��ŷ�����ϵ������,����ʱ�����ƻ�
//			 [i,o] b - n*1����,��ŷ������Ҷ˵ĳ�������;����ʱ��ŷ�����Ľ�����
//
// ��    ��: 
//			 1 - �ɹ�
//			 FALSE - ʧ��
//
// ע    ��: 
// ����n��δ֪��n�����̵����Է�����
// Ax = b
//
//=============================================================================

int KLinearEquation::TotalChoiceGauss(KMatrix& a, KMatrix& b)
{
	int i,j,k;

	long double MaxValue, tmp;		//��¼��Ԫ����ֵ
	int l(1), is;
    bool yn;
    
	int n = a.GetColNum();			//���������
	
	int* js = new int[n];			//���滻��λ��
    
	for( k = 0; k < n - 1; k++)	//ȫѡ��Ԫ
	{	
		MaxValue = 0.0;				//��������Ԫ����ֵ��������ֵ
		        
		for(i = k; i < n; i++)
			for(j = k; j < n; j++)
            {		
				tmp = fabs(a(i, j));	//��a(i,j)����ֵ
				if(tmp > MaxValue)	//����һ���������Ԫ
				{ 
					MaxValue = tmp;	//��������Ԫ����ֵ
					js[k] = j;		//����Ԫ������
					is = i;			//����Ԫ������
				}
            }
			
		yn = fabs(MaxValue) < 1.0e-12;	//MaxValue == 0

        if(yn) l = 0;				//��ԪΪ0
		else
		{
			if(js[k] != k)			//����
			{
				a.MatrixSwapCol(k,js[k],0);	//����a��k��js[k]��

				//for(i = 0; i < n; i++) swap(a(i, k), a(i, js[k]));	//����a��k��js[k]��
			}
								
			if(is != k)				//����
			{ 
				a.MatrixSwapRow(k,is,0);
				//for (j = k; j < n; j++)	swap(a(k, j), a(is, j));	//����a��k��is��

				//swap(b[k], b[is]);	//�������ұߵ�kԪ�����isԪ�ؽ���
				b.MatrixSwapRow(k,is,0);
			}
		}
        
		if(l == 0)					//��������(��ԪΪ0)
		{
			printf("fail 1\n");
            return 0;				// ���ʧ�ܣ�����0ֵ
		}
        
		MaxValue =  fabs(a(k, k));

        for(j = k + 1; j < n; j++)	a(k, j) /= a(k, k); //MaxValue;
        
		b(k,0) /= a(k, k); //MaxValue;
        for(i = k + 1; i < n; i++)
		{
			for(j = k + 1; j < n; j++)
			{
                a(i, j) = a(i, j) - a(i, k) * a(k, j);
			}
            
			b(i,0) = b(i,0) - a(i, k) * b(k,0);
		}
	}
    
	MaxValue = fabs(a((n - 1), (n - 1)));	//��Ԫ

	yn = fabs(MaxValue) < 1.0e-12;	//MaxValue == 0
    if(yn)							//��ԪΪ0
	{
		//cout<<"fail 2"<<endl;
        return(0);					//����Ϊ������,���ʧ�ܣ�����0ֵ
	}

	b(n - 1,0) /= a((n - 1), (n - 1));//��ⷽ�����ұ�X�Ľ�

    for(i = n - 2; i >= 0; i--)		//�ش�����
	{
		double t = 0.0;
        
		for(j = i + 1; j < n; j++)	t = t + a(i, j) * b(j,0);
        
		b(i,0) = b(i,0) - t;
	}
    
	js[n - 1] = n - 1;				//X���һ��Ԫ�ز��û�
    for(k = n - 2; k >= 0; k --)	//k���Դ�n-2��ʼ
	{
		if(js[k] != k)				//����X��Ԫ��λ��(��ȫѡ���в�����)
		{
			//swap(b[k], b[js[k]]);
			b.MatrixSwapRow(k,js[k],0);
		}
	}

	//---
		delete []js;
	//---
    
	return(1);						//���������ɹ���
}

//
// ����n��δ֪��n�����̵����Է�����
// AX = B
// A	-->n*n����,��ŷ�����ϵ������,����ʱ�����ƻ�
// X	-->n*m
// B	-->n*m����,��ŷ������Ҷ˵ĳ�������;����ʱ��ŷ�����Ľ�����
//=======================================================
// ��    ��: 
// ��    ��: public
//
// ��    ��: ȫѡ��Ԫ��˹-Լ����ȥ��
//
// ��    ��:
//			 [i] a - n*n����,��ŷ�����ϵ������,����ʱ�����ƻ�
//			 [i,o] b - n*1����,��ŷ������Ҷ˵ĳ�������;����ʱ��ŷ�����Ľ�����
//
// ��    ��: 
//			 TRUE - �ɹ�
//			 FALSE - ʧ��
//
// ע    ��: 
// ����n��δ֪��n�����̵����Է�����
// AX = B
//
//=======================================================

int KLinearEquation::TotalChoiceGaussJordan(KMatrix& a, KMatrix& b)
{
	long double MaxValue, tmp;	//��Ԫ����ֵ
	int l(1), k, i, j, is;
	bool yn;
	
    int n = a.GetColNum();		//���������
	int m = b.GetColNum();		//�������Ҷ˳��������ĸ���

	int* js = new int[n];			//���滻��λ��
	
	for(k = 0; k < n; k++)
	{
		MaxValue = long double(0.0);
        
		for(i = k; i < n; i++)
			for(j = k; j < n; j++)
            {
				tmp = fabs( a(i, j) );
				if(tmp > MaxValue)
				{ 
					MaxValue = tmp;
					js[k] = j;
					is = i;
				}
            }
		
		yn = fabs(MaxValue) < 1.0e-12;	//MaxValue == 0
		if(yn) l = 0; 		
		else
		{
			if(js[k] != k)	//������
			{
				a.MatrixSwapCol(k,js[k],0);
				//for(i = 0; i < n; i++)	swap(a(i, k), a(i, js[k]));
			}
				
			if(is != k)	//������
			{
				//for(j = k; j < n; j++)	swap(a(k, j), a(is, j));
				a.MatrixSwapRow(k,is,0);
				
				//for(j = 0; j < m; j++)	swap(b(k, j), b(is, j));
				b.MatrixSwapRow(k,is,0);
			}
		}

		if(l == 0)
		{
			//cout<<"fail"<<endl;
			return 0;
		}
		
		for(j = k + 1; j < n; j++)
			a(k, j) /= a(k, k);

		for(j = 0; j < m; j++)
			b(k, j) /= a(k, k);				

		for(j = k + 1; j < n; j++)
			for(i = 0; i < n; i++)
				if(i != k)
					a(i, j) -= a(i, k) * a(k, j);

		for(j = 0; j < m; j++)
			for(i = 0; i< n; i++)
				if(i != k)
					b(i, j) -= a(i, k) * b(k, j);					
	}

    for(k = n - 1; k >= 0; k --)
	{
		if(js[k] != k)	//������
		{
			//for(j = 0 ; j < m;  j++) swap(b(k, j), b(js[k], j));
			b.MatrixSwapRow(k,js[k],0);
		}

	}

	//---
	delete []js;
	//---
	return 1;				//�ɹ�����
}

//=======================================================
// ��    ��: SymmetryRegularEuationSquareRoot
// ��    ��: public
//
// ��    ��: ���Գ������������ƽ������
//
// ��    ��:
//			 [i,o] a - n*n����,���ϵ������(�Գ���������)
//					   ����ʱ,�������ǲ��ִ�ŷֽ��� U ����
//			 [i,o] d - n*m����,��ŷ������Ҷ�m�鳣������
//					   ����ʱ,��ŷ������m���
//
// ��    ��: 
//			 TRUE - �ɹ�
//			 FALSE - ʧ��
//
// ע    ��: 
//
//=======================================================
int KLinearEquation::SymmetryRegularEuationSquareRoot(KMatrix& a, KMatrix& d)
{
	int i, j, k;

	if(!a.IsSymmetryRegular())	//�б����a�Ƿ�Գ�����
		return(-1);						//����a���Գ�����
    
	int n = a.GetColNum();				//���������
	int m = d.GetColNum();				//�������ұ߳��������ĸ���
	
	a(0,0) = sqrt( a(0, 0) );
	
    for(j = 1; j < n; j++)
	{
		a(0, j) /= a(0, 0);
	}

	for(i = 1; i < n; i++)
	{
        for(j = 1; j <= i; j++)
		{
            a(i, i) = a(i, i) - a((j - 1), i) * a((j - 1), i);
		}
    
		a(i, i) = sqrt(a(i, i));
        if(i != (n - 1))
		{
			for(j = i + 1; j < n; j++)
			{
                for(k = 1; k <= i; k++)
					a(i, j) -= a((k - 1), i) * a((k - 1), j);
                
				a(i, j) /= a(i, i);
			}
		}
	}

	for(j = 0; j < m; j++)
	{
		d(0, j) = d(0, j) / a(0, 0);
        for(i = 1; i < n; i++)
		{
            for(k = 1; k <= i; k++)
			{
				d(i, j) -= a((k - 1), i) * d((k - 1), j);
			}
            
			d(i, j) /= a(i, i);
		}
	}

	for(j = 0; j < m; j++)
	{
        d((n - 1), j) = d((n - 1), j) / a((n - 1), (n - 1));
		
		for(k = n - 1; k >= 1; k --)
		{
		    for(i = k; i < n; i++)
			{
		        d((k - 1), j) -= a((k - 1), i) * d(i, j);
			}
        
            d((k - 1), j) /= a((k - 1), (k - 1));			
		}
	}

	return (1);	//���гɹ�����������
}
