/******************************************************************************
 ** Header File: KMatrix.h

 ** Announce:
          if it works, I know it was written by
             ShenYongchen(shenyczz@163.com),live in ZhengZhou,HeNan.
          if not, then I don't know nothing.

 ** Name: KMatrix - ʵ������
 ** Version:
 ** Function:

 ** Author: ShenYongchen(shenyczz@163.com) [������]
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
public:	//���캯������������
	//----------------------------------------------------------
	KMatrix();
	KMatrix(const KMatrix& rhs);
	KMatrix(size_t nRow,size_t nCol);
	KMatrix(double* pData,size_t nRow,size_t nCol);
	//----------------------------------------------------------
	virtual ~KMatrix();
	//----------------------------------------------------------

public:	//����������
	//----------------------------------------------------------
	//����������Է�+ - *   += -= *=
	KMatrix operator + (const KMatrix& rhs);	//�������
	KMatrix operator - (const KMatrix& rhs);	//�������
	KMatrix operator * (const KMatrix& rhs);	//�������

	KMatrix& operator += (const KMatrix& rhs);	//�������
	KMatrix& operator -= (const KMatrix& rhs);	//�������
	KMatrix& operator *= (const KMatrix& rhs);	//�������
	//----------------------------------------------------------
	//�����������Է�+ - * /   += -= *= /=
	KMatrix operator + (const double dValue);	//��������
	KMatrix operator - (const double dValue);	//��������
	KMatrix operator * (const double dValue);	//��������
	KMatrix operator / (const double dValue);	//��������

	KMatrix& operator += (const double dValue);	//��������
	KMatrix& operator -= (const double dValue);	//��������
	KMatrix& operator *= (const double dValue);	//��������
	KMatrix& operator /= (const double dValue);	//��������
	//----------------------------------------------------------
	//�Ծ���(ÿ��Ԫ��)���� + �� - ��
	KMatrix operator + ();	//������ - ��
	KMatrix operator - ();	//������ - ��
	//----------------------------------------------------------
	//����ֵ =
	KMatrix& operator = (const KMatrix& rhs);	//����ֵ
	//----------------------------------------------------------
	//ȡ(i,j)��ֵ
	double& operator () (size_t nRow,size_t nCol);
	const double operator () (size_t nRow,size_t nCol) const;
	//----------------------------------------------------------
	//����Ƚ�
	bool operator == (const KMatrix& rhs) const;	//�������
	bool operator != (const KMatrix& rhs) const;	//�������
	//----------------------------------------------------------

public:	//��������
	//----------------------------------------------------------
	//��������
	size_t GetRowNum() const		{return m_nRow;}
	//��������
	size_t GetColNum() const 		{return m_nCol;}
	//----------------------------------------------------------
	//�Ƿ�վ���
	bool IsEmpty() const;
	//�Ƿ���
	bool IsPhalanx() const;
	//�Ƿ�Գ�
	bool IsSymmetry() const;
	//�Ƿ����Գ���
	bool IsSymmetry3() const;
	//�Ƿ�����
	bool IsRegular() const;
	//�Ƿ�Գ�����
	bool IsSymmetryRegular() const;
	//�Ƿ��в�����(Toeplitz)����
	bool IsToeplitz() const;
	//----------------------------------------------------------

public:	//�������
	//----------------------------------------------------------
	double GetAt(size_t nRow,size_t nCol) const			{return m_Data[nRow*m_nCol+nCol];}
	void SetAt(size_t nRow,size_t nCol,double dValue)	{m_Data[nRow*m_nCol+nCol]=dValue;}
	//----------------------------------------------------------
	//�þ���Ϊ�վ���
	void Empty();
	//���þ���ߴ�
	void SetSize(size_t nRow,size_t nCol);
	//���þ���ߴ�
	void Resize(size_t nRow,size_t nCol);
	//----------------------------------------------------------
	//�����в�����(Toeplitz)���� 
	bool GenerateToeplitz(double* tu,double* tl,size_t n);
	//�������Գ���
	bool GenerateSymmetry3(double* b,double* c,size_t n);
	//����n�׵�λ��
	bool GenerateIdentityMatrix(size_t n);
	//----------------------------------------------------------

public:	//����任 MatrixTransform
	//----------------------------------------------------------
	//�����н���
	KMatrix& MatrixSwapRow(size_t r1,size_t r2,int base=1);
	//�����н���
	KMatrix& MatrixSwapCol(size_t c1,size_t c2,int base=1);
	//����ת��
	//KMatrix& MatrixTranspose();
	//const KMatrix MatrixTranspose() const;
	KMatrix MatrixTranspose() const;
	//----------------------------------------------------------
	//�������
	//�������ͱ����Ǹ�����
	//������ɹ�����һ���վ���
	KMatrix MatrixInversion() const;
	//ȫѡ��Ԫ��˹-Լ��(Gauss-Jordan)���������
	//�������ͱ����Ǹ�����
	//������ɹ�����һ���վ���
	KMatrix MatrixInversionGS();
	//�á�����ѭ�����±�ŷ�����Գ�����������
	//�������ͱ����Ǹ�����
	//������ɹ�����һ���վ���
	KMatrix MatrixInversionSymmetryRegular() const;
	//������(Trench)�����в�����(Toeplitz)������,�������ͱ����Ǹ�����
	//������ɹ�����һ���վ���
	KMatrix MatrixInversionToeplitzTrench() const;
	//----------------------------------------------------------
	//��˹�ɶ��±任��Լ���Գ���Ϊ�Գ����Խ���
	bool MatrixTransformHouseholder(KMatrix& q, double* b,  double* c);
	//��һ��ʵ����Ϊ���겮����(��H��)
	bool MatrixTransformHessenberg(KMatrix& h);
	//----------------------------------------------------------

public:
	//----------------------------------------------------------
	//�������
	size_t MatrixRank();
	//���������ʽ
	double MatrixDeterminant();
	//ʵ����LU�ֽ�(��������Ǹ�����)
	int MatrixLU(KMatrix& lhs, KMatrix& uhs);
	//�ú�˹�ɶ���(Householder)�任��һ��m*n�׵�ʵ�������QR�ֽ�
	int MatrixQR(KMatrix& MQ, KMatrix& MR);
	//�Գ������������˹��(Cholesky)�ֽ⼰��������ʽֵ,�����뷵��ֵ���ͱ����Ǹ�����
	double MatrixCholesky(KMatrix& ML);
	//һ��ʵ���������ֵ�ֽ�
	KMatrix MatrixSingularValue(KMatrix& u,KMatrix& v,double eps=1.0e-6,int loop=60);
	//�����������ֵ�ֽ�
	KMatrix MatrixSingularValueGeneralizedInversion(KMatrix& ap, KMatrix& u, KMatrix& v,double eps=1.0e-6);
	//----------------------------------------------------------
	//ʵ�Գ�������ȫ������ֵ����������QR��
	int MatrixEigenvalueVectorRealSymmetryTriangleQR(double* b,double* c, KMatrix& q, double eps=1.0e-6, int loop=60);
	//ʵ�Գ�������ֵ�����������ſɱȷ�
	int MatrixEigenvalueVectorRealSymmetryJacobi(KMatrix& lmd,KMatrix& v,double eps=1.0e-6, int loop=60);
	//ʵ�Գ�������ֵ�����������ſɱȹ��ط�
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
	//����˷�
	void _MatrixMultiply(const KMatrix& lhs, const KMatrix& rhs,KMatrix& mOut);
	//----------------------------------------------------------
	//һ��ʵ���������ֵ�ֽ⸨������
	void _MSV_1(KMatrix& a, double* e, double* s,	KMatrix& v, int m, int n);
	//һ��ʵ���������ֵ�ֽ⸨������
	void _MSV_2(double* fg, double* cs);
	//----------------------------------------------------------
	void _swap(double& d1,double& d2);
	//----------------------------------------------------------
};

#endif // #ifndef __INCLUDED_MATRIX_H__
