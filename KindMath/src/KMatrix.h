/******************************************************************************
 ** Header File: KMatrix.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KMatrix - 实数矩阵
 ** Version:
 ** Function:

 ** Author: ShenYongchen(shenyczz@163.com) [申永辰]
 ** DateTime: 2000 -
 ** Web Site:

 ** Mender:
 ** DateTime:
 ** Explain:

 ** Usage:

	//1. 
		use

******************************************************************************/

#ifndef __INCLUDED_MATRIX_H__
#define __INCLUDED_MATRIX_H__

class KIND_EXT_CLASS KMatrix  
{
public:	//构造函数和析构函数
	//----------------------------------------------------------
	KMatrix();
	KMatrix(const KMatrix& rhs);
	KMatrix(size_t nRow,size_t nCol);
	KMatrix(double* pData,size_t nRow,size_t nCol);
	//----------------------------------------------------------
	virtual ~KMatrix();
	//----------------------------------------------------------

public:	//操作符重载
	//----------------------------------------------------------
	//矩阵与矩阵自反+ - *   += -= *=
	KMatrix operator + (const KMatrix& rhs);	//矩阵相加
	KMatrix operator - (const KMatrix& rhs);	//矩阵相减
	KMatrix operator * (const KMatrix& rhs);	//矩阵相乘

	KMatrix& operator += (const KMatrix& rhs);	//矩阵相加
	KMatrix& operator -= (const KMatrix& rhs);	//矩阵相减
	KMatrix& operator *= (const KMatrix& rhs);	//矩阵相乘
	//----------------------------------------------------------
	//矩阵与数的自反+ - * /   += -= *= /=
	KMatrix operator + (const double dValue);	//矩阵数加
	KMatrix operator - (const double dValue);	//矩阵数减
	KMatrix operator * (const double dValue);	//矩阵数乘
	KMatrix operator / (const double dValue);	//矩阵数除

	KMatrix& operator += (const double dValue);	//矩阵数加
	KMatrix& operator -= (const double dValue);	//矩阵数减
	KMatrix& operator *= (const double dValue);	//矩阵数乘
	KMatrix& operator /= (const double dValue);	//矩阵数除
	//----------------------------------------------------------
	//对矩阵(每个元素)赋予 + 或 - 号
	KMatrix operator + ();	//矩阵赋予 - 号
	KMatrix operator - ();	//矩阵赋予 - 号
	//----------------------------------------------------------
	//矩阵赋值 =
	KMatrix& operator = (const KMatrix& rhs);	//矩阵赋值
	//----------------------------------------------------------
	//取(i,j)的值
	double& operator () (size_t nRow,size_t nCol);
	const double operator () (size_t nRow,size_t nCol) const;
	//----------------------------------------------------------
	//矩阵比较
	bool operator == (const KMatrix& rhs) const;	//矩阵相等
	bool operator != (const KMatrix& rhs) const;	//矩阵不相等
	//----------------------------------------------------------

public:	//矩阵属性
	//----------------------------------------------------------
	//矩阵行数
	size_t GetRowNum() const		{return m_nRow;}
	//矩阵列数
	size_t GetColNum() const 		{return m_nCol;}
	//----------------------------------------------------------
	//是否空矩阵
	bool IsEmpty() const;
	//是否方阵
	bool IsPhalanx() const;
	//是否对称
	bool IsSymmetry() const;
	//是否三对称阵
	bool IsSymmetry3() const;
	//是否正定
	bool IsRegular() const;
	//是否对称正定
	bool IsSymmetryRegular() const;
	//是否托伯利兹(Toeplitz)矩阵
	bool IsToeplitz() const;
	//----------------------------------------------------------

public:	//矩阵操作
	//----------------------------------------------------------
	double GetAt(size_t nRow,size_t nCol) const			{return m_Data[nRow*m_nCol+nCol];}
	void SetAt(size_t nRow,size_t nCol,double dValue)	{m_Data[nRow*m_nCol+nCol]=dValue;}
	//----------------------------------------------------------
	//置矩阵为空矩阵
	void Empty();
	//设置矩阵尺寸
	void SetSize(size_t nRow,size_t nCol);
	//重置矩阵尺寸
	void Resize(size_t nRow,size_t nCol);
	//----------------------------------------------------------
	//生成托伯利茨(Toeplitz)矩阵 
	bool GenerateToeplitz(double* tu,double* tl,size_t n);
	//生成三对称阵
	bool GenerateSymmetry3(double* b,double* c,size_t n);
	//生成n阶单位阵
	bool GenerateIdentityMatrix(size_t n);
	//----------------------------------------------------------

public:	//矩阵变换 MatrixTransform
	//----------------------------------------------------------
	//矩阵行交换
	KMatrix& MatrixSwapRow(size_t r1,size_t r2,int base=1);
	//矩阵列交换
	KMatrix& MatrixSwapCol(size_t c1,size_t c2,int base=1);
	//矩阵转置
	//KMatrix& MatrixTranspose();
	//const KMatrix MatrixTranspose() const;
	KMatrix MatrixTranspose() const;
	//----------------------------------------------------------
	//求矩阵逆
	//矩阵类型必须是浮点型
	//如果不成功返回一个空矩阵
	KMatrix MatrixInversion() const;
	//全选主元高斯-约当(Gauss-Jordan)法求矩阵逆
	//矩阵类型必须是浮点型
	//如果不成功返回一个空矩阵
	KMatrix MatrixInversionGS();
	//用“变量循环重新编号法”求对称正定矩阵逆
	//矩阵类型必须是浮点型
	//如果不成功返回一个空矩阵
	KMatrix MatrixInversionSymmetryRegular() const;
	//特兰持(Trench)法求托伯利兹(Toeplitz)矩阵逆,矩阵类型必须是浮点型
	//如果不成功返回一个空矩阵
	KMatrix MatrixInversionToeplitzTrench() const;
	//----------------------------------------------------------
	//豪斯荷尔德变换法约化对称阵为对称三对角阵
	bool MatrixTransformHouseholder(KMatrix& q, double* b,  double* c);
	//将一般实方阵化为赫申伯格阵(上H阵)
	bool MatrixTransformHessenberg(KMatrix& h);
	//----------------------------------------------------------

public:
	//----------------------------------------------------------
	//求矩阵秩
	size_t MatrixRank();
	//求矩阵行列式
	double MatrixDeterminant();
	//实矩阵LU分解(矩阵必须是浮点型)
	int MatrixLU(KMatrix& lhs, KMatrix& uhs);
	//用豪斯荷尔德(Householder)变换对一般m*n阶的实矩阵进行QR分解
	int MatrixQR(KMatrix& MQ, KMatrix& MR);
	//对称正定阵的乔里斯基(Cholesky)分解及求其行列式值,矩阵与返回值类型必须是浮点型
	double MatrixCholesky(KMatrix& ML);
	//一般实矩阵的奇异值分解
	KMatrix MatrixSingularValue(KMatrix& u,KMatrix& v,double eps=1.0e-6,int loop=60);
	//广义逆的奇异值分解
	KMatrix MatrixSingularValueGeneralizedInversion(KMatrix& ap, KMatrix& u, KMatrix& v,double eps=1.0e-6);
	//----------------------------------------------------------
	//实对称三角阵全部特征值及特征向量QR法
	int MatrixEigenvalueVectorRealSymmetryTriangleQR(double* b,double* c, KMatrix& q, double eps=1.0e-6, int loop=60);
	//实对称阵特征值及特征向量雅可比法
	int MatrixEigenvalueVectorRealSymmetryJacobi(KMatrix& lmd,KMatrix& v,double eps=1.0e-6, int loop=60);
	//实对称阵特征值及特征向量雅可比过关法
	int MatrixEigenvalueVectorRealSymmetryJacobiBarrier(KMatrix& lmd,KMatrix& v,double eps=1.0e-6);
	//----------------------------------------------------------

public:
	//void Trace();
	void Output(const char* lpszFile);

protected:

protected:
	size_t				m_nRow;
	size_t				m_nCol;
	double*				m_Data;

private:
	//----------------------------------------------------------
	//矩阵乘法
	void _MatrixMultiply(const KMatrix& lhs, const KMatrix& rhs,KMatrix& mOut);
	//----------------------------------------------------------
	//一般实矩阵的奇异值分解辅助函数
	void _MSV_1(KMatrix& a, double* e, double* s,	KMatrix& v, int m, int n);
	//一般实矩阵的奇异值分解辅助函数
	void _MSV_2(double* fg, double* cs);
	//----------------------------------------------------------
	void _swap(double& d1,double& d2);
	//----------------------------------------------------------
};

#endif // #ifndef __INCLUDED_MATRIX_H__
