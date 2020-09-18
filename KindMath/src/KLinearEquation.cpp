#include "StdAfx.h"
#include "KLinearEquation.h"

KLinearEquation::KLinearEquation(void)
{
}

//=============================================================================
// 名    称: TotalChoiceGauss
// 访    问: public
//
// 功    能: 全选主元高斯消去法
//
// 参    数:
//			 [i] a - n*n矩阵,存放方程组系数矩阵,返回时将被破坏
//			 [i,o] b - n*1矩阵,存放方程组右端的常数向量;返回时存放方程组的解向量
//
// 返    回: 
//			 1 - 成功
//			 FALSE - 失败
//
// 注    释: 
// 含有n个未知量n个方程的线性方程组
// Ax = b
//
//=============================================================================

int KLinearEquation::TotalChoiceGauss(KMatrix& a, KMatrix& b)
{
	int i,j,k;

	long double MaxValue, tmp;		//记录主元绝对值
	int l(1), is;
    bool yn;
    
	int n = a.GetColNum();			//方程组阶数
	
	int* js = new int[n];			//保存换列位置
    
	for( k = 0; k < n - 1; k++)	//全选主元
	{	
		MaxValue = 0.0;				//给保存主元绝对值变量赋初值
		        
		for(i = k; i < n; i++)
			for(j = k; j < n; j++)
            {		
				tmp = fabs(a(i, j));	//求a(i,j)绝对值
				if(tmp > MaxValue)	//发现一个更大的主元
				{ 
					MaxValue = tmp;	//保存新主元绝对值
					js[k] = j;		//新主元所在列
					is = i;			//新主元所在行
				}
            }
			
		yn = fabs(MaxValue) < 1.0e-12;	//MaxValue == 0

        if(yn) l = 0;				//主元为0
		else
		{
			if(js[k] != k)			//换列
			{
				a.MatrixSwapCol(k,js[k],0);	//交换a的k和js[k]列

				//for(i = 0; i < n; i++) swap(a(i, k), a(i, js[k]));	//交换a的k和js[k]列
			}
								
			if(is != k)				//换行
			{ 
				a.MatrixSwapRow(k,is,0);
				//for (j = k; j < n; j++)	swap(a(k, j), a(is, j));	//交换a的k和is行

				//swap(b[k], b[is]);	//方程组右边第k元素与第is元素交换
				b.MatrixSwapRow(k,is,0);
			}
		}
        
		if(l == 0)					//矩阵奇异(主元为0)
		{
			printf("fail 1\n");
            return 0;				// 求解失败，返回0值
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
    
	MaxValue = fabs(a((n - 1), (n - 1)));	//主元

	yn = fabs(MaxValue) < 1.0e-12;	//MaxValue == 0
    if(yn)							//主元为0
	{
		//cout<<"fail 2"<<endl;
        return(0);					//矩阵为奇异阵,求解失败，返回0值
	}

	b(n - 1,0) /= a((n - 1), (n - 1));//求解方程组右边X的解

    for(i = n - 2; i >= 0; i--)		//回代过程
	{
		double t = 0.0;
        
		for(j = i + 1; j < n; j++)	t = t + a(i, j) * b(j,0);
        
		b(i,0) = b(i,0) - t;
	}
    
	js[n - 1] = n - 1;				//X最后一个元素不用换
    for(k = n - 2; k >= 0; k --)	//k可以从n-2开始
	{
		if(js[k] != k)				//交换X的元素位置(由全选换列产生的)
		{
			//swap(b[k], b[js[k]]);
			b.MatrixSwapRow(k,js[k],0);
		}
	}

	//---
		delete []js;
	//---
    
	return(1);						//方程组求解成功！
}

//
// 含有n个未知量n个方程的线性方程组
// AX = B
// A	-->n*n矩阵,存放方程组系数矩阵,返回时将被破坏
// X	-->n*m
// B	-->n*m矩阵,存放方程组右端的常数向量;返回时存放方程组的解向量
//=======================================================
// 名    称: 
// 访    问: public
//
// 功    能: 全选主元高斯-约当消去法
//
// 参    数:
//			 [i] a - n*n矩阵,存放方程组系数矩阵,返回时将被破坏
//			 [i,o] b - n*1矩阵,存放方程组右端的常数向量;返回时存放方程组的解向量
//
// 返    回: 
//			 TRUE - 成功
//			 FALSE - 失败
//
// 注    释: 
// 含有n个未知量n个方程的线性方程组
// AX = B
//
//=======================================================

int KLinearEquation::TotalChoiceGaussJordan(KMatrix& a, KMatrix& b)
{
	long double MaxValue, tmp;	//主元绝对值
	int l(1), k, i, j, is;
	bool yn;
	
    int n = a.GetColNum();		//方程组阶数
	int m = b.GetColNum();		//方程组右端常数向量的个数

	int* js = new int[n];			//保存换列位置
	
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
			if(js[k] != k)	//交换列
			{
				a.MatrixSwapCol(k,js[k],0);
				//for(i = 0; i < n; i++)	swap(a(i, k), a(i, js[k]));
			}
				
			if(is != k)	//交换行
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
		if(js[k] != k)	//交换行
		{
			//for(j = 0 ; j < m;  j++) swap(b(k, j), b(js[k], j));
			b.MatrixSwapRow(k,js[k],0);
		}

	}

	//---
	delete []js;
	//---
	return 1;				//成功返回
}

//=======================================================
// 名    称: SymmetryRegularEuationSquareRoot
// 访    问: public
//
// 功    能: 求解对称正定方程组的平方根法
//
// 参    数:
//			 [i,o] a - n*n矩阵,存放系数矩阵(对称正定矩阵)
//					   返回时,其上三角部分存放分解后的 U 矩阵
//			 [i,o] d - n*m矩阵,存放方程组右端m组常数向量
//					   返回时,存放方程组的m组解
//
// 返    回: 
//			 TRUE - 成功
//			 FALSE - 失败
//
// 注    释: 
//
//=======================================================
int KLinearEquation::SymmetryRegularEuationSquareRoot(KMatrix& a, KMatrix& d)
{
	int i, j, k;

	if(!a.IsSymmetryRegular())	//判别矩阵a是否对称正定
		return(-1);						//矩阵a不对称正定
    
	int n = a.GetColNum();				//方程组阶数
	int m = d.GetColNum();				//方程组右边常数向量的个数
	
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

	return (1);	//运行成功，正常返回
}
