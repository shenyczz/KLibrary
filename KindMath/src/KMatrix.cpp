#include "StdAfx.h"
#include "KMatrix.h"

///////////////////////////////////////////////////////////////////////////
// ��    ��: KMatrix
// ��    ��: ȱʡ���캯��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix::KMatrix() : m_nRow(0),
					 m_nCol(0),
					 m_Data(NULL)
{
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: KMatrix
// ��    ��: �������캯��
// ��    ��: public
// ��    ��:
//			[i] rhs - �������
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix::KMatrix(const KMatrix& rhs)
{
	m_nRow = rhs.GetRowNum();
	m_nCol = rhs.GetColNum();

	m_Data	= new double[m_nRow*m_nCol];
	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] = rhs.m_Data[i];
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: KMatrix
// ��    ��: ����Ԫ��ȫ0�ľ���
// ��    ��: public
// ��    ��:
//			[i] nRow - ��������
//			[i] nCol - ��������
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix::KMatrix(size_t nRow,size_t nCol)
{
	if(nRow<=0 || nCol<=0) throw;

	m_nRow = nRow;
	m_nCol = nCol;

	m_Data	= new double[m_nRow*m_nCol];

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] = double(0);
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: KMatrix
// ��    ��: ����m*n����
// ��    ��: public
// ��    ��:
//			[i] pData - ��������ָ��
//			[i] nRow - ��������
//			[i] nCol - ��������
//			
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix::KMatrix(double* pData,size_t nRow,size_t nCol)
{
	//KfxAssert(pData);
	//KfxAssert(nRow > 0);
	//KfxAssert(nCol > 0);
	if(pData==NULL || nRow<=0 || nCol<=0) throw;

	m_nRow = nRow;
	m_nCol = nCol;

	m_Data	= new double[m_nRow*m_nCol];

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] = *(pData+i);
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: ~KMatrix
// ��    ��: ��������
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix::~KMatrix()
{
	m_nRow = 0;
	m_nCol = 0;

	if(m_Data)	{delete[] m_Data; m_Data = NULL;}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Empty
// ��    ��: �þ���Ϊ�վ���
// ��    ��: public
// ��    ��:
// ��    ��:
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KMatrix::Empty()
{
	m_nRow = 0;
	m_nCol = 0;
	if(m_Data)
	{
		delete[] m_Data;
		m_Data = NULL;
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: SetSize
// ��    ��: ���þ���ߴ�,��������
// ��    ��: public
// ��    ��:
//			[i] nRow - ������
//			[i] nCol - ������
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KMatrix::SetSize(size_t nRow,size_t nCol)
{
	//KfxAssert(nRow > 0);
	//KfxAssert(nCol > 0);
	if(nRow<=0 || nCol<=0) throw;

	m_nRow = nRow;
	m_nCol = nCol;

	if(m_Data) delete[] m_Data;
	m_Data	= new double[m_nRow*m_nCol];

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] = double(0);
	}
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Resize
// ��    ��: ���þ���ߴ�,���ݱ���
// ��    ��: public
// ��    ��:
//			[i] nRow - ������
//			[i] nCol - ������
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KMatrix::Resize(size_t nRow,size_t nCol)
{
	//KfxAssert(nRow > 0);
	//KfxAssert(nCol > 0);
	if(nRow<=0 || nCol<=0) throw;
	//-----------------------------
	size_t i,j;
	//-----------------------------
	size_t nRowOld = m_nRow;
	size_t nColOld = m_nCol;
	//-----------------------------
	//������ʱ�ڴ汣��ԭ������
	double* pData = new double[nRowOld*nColOld];
	for(i=0;i<nRowOld*nColOld;i++)
	{
		*(pData+i) = m_Data[i];
	}
	//-----------------------------
	//���þ���
	SetSize(nRow,nCol);
	//-----------------------------
	//�ָ�����
	for(i=0;i<m_nRow;i++)
	{
		for(j=0;j<m_nCol;j++)
		{
			if(i<nRowOld && j<nColOld)	//��֤�ָ���������ԭ������ķ�Χ��
			{
				m_Data[i*m_nCol+j] = *(pData+i*nColOld+j);
			}
		}
	}
	//-----------------------------
	//�ڴ����
	delete []pData; pData = NULL;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GenerateIdentityMatrix
// ��    ��: ����n�׵�λ��
// ��    ��: public
// ��    ��:
//			[i] n - ��λ��������
//
// ��    ��: true/false
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
bool KMatrix::GenerateIdentityMatrix(size_t n)
{
	if(n<=0)	return false;

	SetSize(n,n);

	for(size_t i=0;i<n;i++)
	{
		for(size_t j=0;j<n;j++)
		{
			if(i==j)
				m_Data[i*n+j]=double(1);
			else
				m_Data[i*n+j]=double(0);
		}
	}
	//---
	return true;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GenerateSymmetry3
// ��    ��: �������Գ���
// ��    ��: public
// ��    ��:
//			[i] b - nά����,�洢���Գ����е����Խ���Ԫ��
//			[i] c - nά����,�洢���Գ����еĴζԽ���Ԫ��
//			[i] n - ����ά��
//
// ��    ��: true/false
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
bool KMatrix::GenerateSymmetry3(double* b,double* c,size_t n)
{
	if(n<=0) return false;

	Empty();
	SetSize(n,n);

	for(size_t i=0;i<n;i++)
	{
		for(size_t j=0;j<n;j++)
		{
			if(i==j)
			{
				m_Data[i*n+j] = b[i];
			}
			else if(abs(int(i-j)) ==1 )
			{
				size_t m=min(i,j);
				m_Data[i*n+j] = c[m];
			}
			else
			{
				m_Data[i*n+j] = double(0);
			}
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: GenerateToeplitz
// ��    ��: �����в�����(Toeplitz)���� 
// ��    ��: public
// ��    ��:
//			[i] tu - ���в����ľ����ϰ�������
//			[i] tl - ���в����ľ����°�������
//			[i] n -  ����ά��
//
// ��    ��: true/false
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
bool KMatrix::GenerateToeplitz(double* tu,double* tl,size_t n)
{
	if(n<=0) return false;

	if(fabs(tu[0]) < 1.0e-12)	return false;	//tr[0] != 0

	Empty();
	SetSize(n,n);

	for(size_t i=0;i<n;i++)
	{
		for(size_t j=0;j<n;j++)
		{
			if(i<=j)
				m_Data[i*n+j]=tu[j-i];
			else	//i>j
				m_Data[i*n+j]=tl[i-j];
		}
	}

	return true;
}

//{---����������------------------------

//[����������] -> ����ֵ
KMatrix& KMatrix::operator = (const KMatrix& rhs)
{
	if(*this == rhs)		return *this;	//������ֱ�ӷ���

	//---
	SetSize(rhs.m_nRow,rhs.m_nCol);

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] = rhs.m_Data[i];
	}
	//---
	return *this;
}

//[����������] -> ��������
KMatrix KMatrix::operator + (const double dValue)
{
	KMatrix mat(*this);
	//KfxAssert(mat.m_nRow > 0);
	//KfxAssert(mat.m_nCol > 0);
	if(mat.m_nRow<=0 || mat.m_nCol<=0) throw;

	for(size_t i=0;i<mat.m_nRow*mat.m_nCol;i++)
	{
		mat.m_Data[i] += dValue;
	}
	//---
	return mat;
}

//[����������] -> ��������
KMatrix& KMatrix::operator += (const double dValue)
{
	//KfxAssert(m_nRow > 0);
	//KfxAssert(m_nCol > 0);
	if(m_nRow<=0 || m_nCol<=0) throw;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] += dValue;
	}
	//---
	return *this;
}

//[����������] -> ��������
KMatrix KMatrix::operator - (const double dValue)
{
	KMatrix mat(*this);

	//KfxAssert(mat.m_nRow > 0);
	//KfxAssert(mat.m_nCol > 0);
	if(mat.m_nRow<=0 || mat.m_nCol<=0) throw;

	for(size_t i=0;i<mat.m_nRow*mat.m_nCol;i++)
	{
		mat.m_Data[i] -= dValue;
	}
	//---
	return mat;
}

//[����������] -> ��������
KMatrix& KMatrix::operator -= (const double dValue)
{
	//KfxAssert(m_nRow > 0);
	//KfxAssert(m_nCol > 0);
	if(m_nRow<=0 || m_nCol<=0) throw;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] -= dValue;
	}
	//---
	return *this;
}

//[����������] -> ��������
KMatrix KMatrix::operator * (const double dValue)
{
	KMatrix mat(*this);
	//KfxAssert(mat.m_nRow > 0);
	//KfxAssert(mat.m_nCol > 0);
	if(mat.m_nRow<=0 || mat.m_nCol<=0) throw;

	for(size_t i=0;i<mat.m_nRow*mat.m_nCol;i++)
	{
		mat.m_Data[i] *= dValue;
	}
	//---
	return mat;
}

//[����������] -> ��������
KMatrix& KMatrix::operator *= (const double dValue)
{
	//KfxAssert(m_nRow > 0);
	//KfxAssert(m_nCol > 0);
	if(m_nRow<=0 || m_nCol<=0) throw;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] *= dValue;
	}
	//---
	return *this;
}

//[����������] -> ��������
KMatrix KMatrix::operator / (const double dValue)
{
	if(m_nRow<=0 || m_nCol<=0) throw;
	if(fabs(dValue)<1.0e-12) throw;

	KMatrix mat(*this);

	for(size_t i=0;i<mat.m_nRow*mat.m_nCol;i++)
	{
		mat.m_Data[i] /= dValue;
	}
	//---
	return mat;
}

//[����������] -> ��������
KMatrix& KMatrix::operator /= (const double dValue)
{
	//KfxAssert(m_nRow > 0);
	//KfxAssert(m_nCol > 0);
	//KfxAssert(dValue > 1.0e-12);
	if(m_nRow<=0 || m_nCol<=0) throw;
	if(fabs(dValue)<1.0e-12) throw;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] /= dValue;
	}
	//---
	return *this;
}

//[����������] -> �������
KMatrix KMatrix::operator + (const KMatrix& rhs)
{
	KMatrix mat(*this);

	//������Ȳ������
	if(mat.m_nRow != rhs.m_nRow || mat.m_nCol != rhs.m_nCol) throw;


	for(size_t i=0;i<mat.m_nRow*mat.m_nCol;i++)
	{
		mat.m_Data[i] += rhs.m_Data[i];
	}
	//---
	return mat;
}

//[����������] -> �������
KMatrix& KMatrix::operator += (const KMatrix& rhs)
{
	//������Ȳ������
	if(m_nRow != rhs.m_nRow || m_nCol != rhs.m_nCol) throw;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] += rhs.m_Data[i];
	}
	//---
	return *this;
}

//[����������] -> �������
KMatrix KMatrix::operator - (const KMatrix& rhs)
{
	KMatrix mat(*this);
	//������Ȳ������
	if(mat.m_nRow != rhs.m_nRow || mat.m_nCol != rhs.m_nCol) throw;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		mat.m_Data[i] = m_Data[i]-rhs.m_Data[i];
	}
	//---
	return mat;
}

//[����������] -> �������
KMatrix& KMatrix::operator -= (const KMatrix& rhs)
{
	//������Ȳ������
	if(m_nRow != rhs.m_nRow || m_nCol != rhs.m_nCol) throw;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		m_Data[i] -= rhs.m_Data[i];
	}
	//---
	return *this;
}

//[����������] -> �������
KMatrix KMatrix::operator * (const KMatrix& rhs)
{
	KMatrix mOut;
	KMatrix lhs(*this);
	_MatrixMultiply(lhs,rhs,mOut);
	//---
	return mOut;
}

//[����������] -> �������
KMatrix& KMatrix::operator *= (const KMatrix& rhs)
{
	KMatrix mOut;
	KMatrix lhs(*this);
	_MatrixMultiply(lhs,rhs,mOut);
	*this=mOut;
	//---
	return *this;
}

//[����������] -> ������ + ��(����)
KMatrix KMatrix::operator + ()
{
	KMatrix mat(*this);
	return mat;
}

//[����������] -> ������ - ��(����)
KMatrix KMatrix::operator - ()
{
	if(m_nRow<=0 || m_nCol<=0) throw;

	KMatrix mat(*this);

	for(size_t i=0;i<mat.m_nRow*mat.m_nCol;i++)
	{
		mat.m_Data[i] = -mat.m_Data[i];
	}
	//---
	return mat;
}

//[����������] -> ��ȡ��ά����Ԫ��(i,j)��ֵ,��ֵ����(��д��)
double& KMatrix::operator () (size_t nRow,size_t nCol)
{
	if(nRow<0 || nRow>=m_nRow) throw;
	if(nCol<0 || nCol>=m_nCol) throw;

	return m_Data[nRow*m_nCol+nCol];
}

//[����������] -> ��ȡ��ά����Ԫ��(i,j)��ֵ,��ֵ����(ֻ��)
const double KMatrix::operator () (size_t nRow,size_t nCol) const
{
	if(nRow<0 || nRow>=m_nRow) throw;
	if(nCol<0 || nCol>=m_nCol) throw;

	return m_Data[nRow*m_nCol+nCol];
}

//[����������] -> �������
bool KMatrix::operator == (const KMatrix& rhs) const	
{
	if(m_nRow != rhs.m_nRow)
		return false;

	if(m_nCol != rhs.m_nCol)
		return false;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		if(m_Data[i] != rhs.m_Data[i])
			return false;
	}
	//---
	return true;

}

//[����������] -> �������
bool KMatrix::operator != (const KMatrix& rhs) const	
{
	if(m_nRow != rhs.m_nRow)
		return true;

	if(m_nCol != rhs.m_nCol)
		return true;

	for(size_t i=0;i<m_nRow*m_nCol;i++)
	{
		if(m_Data[i] != rhs.m_Data[i])
			return true;
	}
	//---
	return false;

}

//}---���������ؽ���---------------

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixSwapRow
// ��    ��: �����н���
// ��    ��: public
// ��    ��:
//			[i] r1	- �к�1
//			[i] r2	- �к�2
//			[i] base - 1��ʾ�±��1��ʼ, 0��ʾ�±��0��ʼ,Ĭ��=1
//
// ��    ��: 
//			�����������
//
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix& KMatrix::MatrixSwapRow(size_t r1,size_t r2,int base)
{
	size_t nRow = GetRowNum();	//����
	size_t nCol = GetColNum();	//����

	//---����ϰ��
	if(base)	{r1--;	r2--;}

	if(r1>=nRow || r2>=nRow) throw;

	for(size_t j=0;j<nCol;j++)	//��ѭ��
	{
		double dTemp		= m_Data[r1*nCol+j];
		m_Data[r1*nCol+j]	= m_Data[r2*nCol+j];	// ��*���� + ��
		m_Data[r2*nCol+j]	= dTemp;
	}

	return *this;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixSwapCol
// ��    ��: �����н���
// ��    ��: public
// ��    ��:
//			[i] c1	- �к�1
//			[i] c2	- �к�2
//			[i] base - 1��ʾ�±��1��ʼ, 0��ʾ�±��0��ʼ,Ĭ��=1
//
// ��    ��: 
//			�����������
//
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix& KMatrix::MatrixSwapCol(size_t c1,size_t c2,int base)
{
	size_t nRow = GetRowNum();	//����
	size_t nCol = GetColNum();	//����

	//---����ϰ��
	if(base)	{c1--;	c2--;}

	if(c1>=nCol || c2>=nCol) throw;

	for(size_t i=0;i<nRow;i++)	//��ѭ��
	{
		double dTemp		= m_Data[i*nCol+c1];
		m_Data[i*nCol+c1]	= m_Data[i*nCol+c2];	// ��*���� + ��
		m_Data[i*nCol+c2]	= dTemp;
	}

	return *this;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixTranspose
// ��    ��: ����ת��(���ı�ԭ����)
// ��    ��: public
// ��    ��:
// ��    ��: 
//			�������
//
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix KMatrix::MatrixTranspose() const
{
	KMatrix mOut;

	if(IsEmpty())	return mOut;

	KMatrix mm(*this);

	size_t nRowNew = mm.m_nCol;	//��������������
	size_t nColNew = mm.m_nRow;	//��������������

	mOut.SetSize(nRowNew,nColNew);	//��������

	for(size_t i=0;i<nRowNew;i++)
	{
		for(size_t j=0;j<nColNew;j++)
		{
			//m_Data[i*nColNew+j]=mm(j,i);
			mOut(i,j)=mm(j,i);
		}
	}
	//---
	return mOut;
}

/*
///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixTranspose
// ��    ��: ����ת��(�ı�ԭ����)
// ��    ��: public
// ��    ��:
// ��    ��: 
//			�������
//
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
KMatrix& KMatrix::MatrixTranspose()
{
	if(IsEmpty())	return *this;

	KMatrix mm(*this);

	size_t nRowNew = m_nCol;	//��������������
	size_t nColNew = m_nRow;	//��������������

	SetSize(nRowNew,nColNew);	//��������

	for(size_t i=0;i<nRowNew;i++)
	{
		for(size_t j=0;j<nColNew;j++)
		{
			m_Data[i*nColNew+j]=mm(j,i);
		}
	}
	//---
	return *this;
}
*/

//�Ƿ�վ���
bool KMatrix::IsEmpty() const
{
	return (m_nRow==0 || m_nCol==0 || m_Data==NULL);
}

//�Ƿ���
bool KMatrix::IsPhalanx() const
{
	return (!IsEmpty() && (m_nRow == m_nCol));
}

//�Ƿ�Գ���
bool KMatrix::IsSymmetry() const
{
	if(!IsPhalanx())	return false;	//�����Ƿ���

	for(size_t i=0;i<GetRowNum();i++)
	{
		for(size_t j=0;j<GetColNum();j++)
		{
			if( fabs(GetAt(i,j) - GetAt(j,i)) > 1.0e-12 )	// (i,j) == (j,i)
				return false;
		}
	}
	//---
	return true;
}

//�Ƿ����Գ���
bool KMatrix::IsSymmetry3() const
{
	if(!IsSymmetry())	return false;	//���ǶԳ���

	size_t nRank = GetRowNum();	//�������

	for(size_t i=0;i<nRank;i++)
	{
		for(size_t j=0;j<nRank;j++)
		{
			if(abs(int(i-j)) > 1)
			{
				double dTemp = GetAt(i,j);
				if(fabs(dTemp)>1.0e-12)	//dTemp!=0
				{
					return false;
				}
			}
		}
	}
	return true;
}

//�Ƿ�������
bool KMatrix::IsRegular() const
{
	size_t k;

	if(!IsPhalanx())	return false;	//������������Ƿ�

	for(k = 0; k < GetRowNum(); k ++)
	{
		if( fabs(GetAt(k, k)) < 1.0e-12	|| GetAt(k, k) < 0)	//(k,k) <= 0
			return false;	//�Խ�Ԫ������0��������������
	}
	//---
	long double ldDet;
	for(k = 2; k <= GetRowNum(); k++)
	{
		KMatrix m(k, k);	//����һmatrix����

		for(size_t i=0; i<k; i++)
		{
			for(size_t j=0; j<k; j++)
			{
				m(i, j) = GetAt(i, j);	//��ʼ��
			}
		}

		//ldDet = MatrixDeterminant(m);				// ˳������ʽ��ֵ
		ldDet = m.MatrixDeterminant();				// ˳������ʽ��ֵ
		
		if(fabs(ldDet)<1.0e-12 || ldDet < 0.0)	//ldDet <= 0
			return false;					//����������
	}
	//---
	return true;
}

//�Ƿ�Գ�������
bool KMatrix::IsSymmetryRegular() const
{
	return IsSymmetry() && IsRegular();
}

//�Ƿ��в�����(Toeplitz)����
bool KMatrix::IsToeplitz() const
{
	if(!IsPhalanx())	return false;	//���Ƿ���

	size_t nRank = m_nRow;	//ȡ�þ����

	KMatrix m(*this);

	if(fabs(m(0,0))<1.0e-12)	return false;	//m(0,0) != 0

	for(size_t i=0;i<nRank-1;i++)
	{
		for(size_t j=0;j<nRank-1;j++)
		{
			double dTemp = m(i,j) - m(i+1,j+1);
			if(fabs(dTemp)>1.0e-12)	//m(i,j) != m(i+1,j+1)
				return false;
		}
	}
	//---
	return true;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixDeterminant
// ��    ��: ȫѡ��Ԫ�����������ʽ��ֵ
// ��    ��: public
// ��    ��:
// ��    ��: ����ʽ��ֵ
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
double KMatrix::MatrixDeterminant()
{
	if(!IsPhalanx())
		return double(0);			//���Ƿ���

	KMatrix m(*this);	//����һ KMatrix ����

	size_t i(0),j(0);
	double  MaxValue, tmp;
	size_t stRank = GetRowNum();	// �������


	size_t iSign, jSign;				// ��Ԫ��λ�ñ�־
	double Det(1);						// ����ʽ��ֵ
	int	nSgn = 1;						// ����
	for(size_t k = 0; k < stRank - 1; k ++)	// ȫѡ��Ԫ
	{	
		MaxValue = 0.0;
		for(i = k; i < stRank; i ++)
		{
			for(size_t j = k; j < stRank; j ++)
			{
				tmp = fabs(m(i, j));		//��m(i,j)����ֵ
				if(tmp > MaxValue)
				{
					MaxValue = tmp;
					iSign = i;			//������Ԫλ��
					jSign = j;
				}
			}
		}
		
		if(fabs(MaxValue)<1.0e-12)	//����ֵ���Ԫ��Ϊ0������ʽҲΪ0
			return double(0);

		if(iSign != k)			//����ֵ���Ԫ�ز��ڵ�ǰ��
		{
			nSgn = -nSgn;		//�任����ʽ����
			for(size_t j = k; j < stRank; j ++)		//������
			{
				//tmp=m(k, j);		//===test
				//tmp=m(iSign, j);	//===test
				_swap(m(k, j), m(iSign, j));
				//tmp=m(k, j);		//===test
			}
		}

		if(jSign != k)			//����ֵ���Ԫ�ز��ڵ�ǰ��
		{
			nSgn = -nSgn;		//�任����ʽ����
			for(size_t i = k; i < stRank; i ++)		//������
			{
				//tmp=m(i, jSign);		//===test
				//tmp=m(i, k);	//===test
				_swap(m(i, jSign), m(i, k));
				//tmp=m(i, k);	//===test
			}
		}

		//tmp=m(k, k);	//===test
		Det *= m(k, k);					//�Խ�Ԫ���
		for(i = k + 1; i < stRank; i ++)
		{
			if(fabs(m(k, k))<1.0e-12)
				throw;

			long double d(m(i, k) / m(k, k));		//��Ԫ����
			for(size_t j = k + 1; j < stRank; j ++)	//����Ԫ�·�Ԫ����Ϊ0
			{
				m(i, j) -= d * m(k, j);	//��ǰ��Ԫ����������Ԫ�����任
			}
		}
	}

	//----------------------------------------------------
	return Det * nSgn * m(stRank - 1, stRank - 1);	//��������ʽ��ֵ
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixRank
// ��    ��: ��������: ȫѡ��Ԫ��˹(Gauss)��Ԫ��
// ��    ��: public
// ��    ��:
// ��    ��: �������
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
size_t KMatrix::MatrixRank()
{
	size_t i,j,k;

	size_t nRank = 0;						//��������

	size_t iSign, jSign;					//��Ԫ��λ�ñ�־

	size_t stRow = GetRowNum();			//ȡ��������
	size_t stCol = GetColNum();			//ȡ��������

	size_t ColRowMin = min(stRow, stCol);	//ȡ�л�����Сֵ

	KMatrix m(*this);				//����һ KMatrix ������this��ʼ��

	for(k = 0; k < ColRowMin; k ++)	// ȫѡ��Ԫ
	{
		long double MaxValue = 0;
		for(i = k; i < stRow; i ++)
		{
			for(j = k; j < stCol; j ++)
			{
				long double tmp(fabs(m(i, j)));	//��m(i,j)����ֵ
				if(tmp > MaxValue)
				{
					MaxValue = tmp;
					iSign = i;					//������Ԫλ��
					jSign = j;
				}
			}
		}

		//����Ԫ�ؾ���ֵ�����Ϊ0
		if(fabs(MaxValue)<1.0e-12) //MaxValue == double(0)
			break;	//return nRank;
		else				
			nRank++;		//����Ԫ�ؾ���ֵ����߲�Ϊ0�������ȼ�1

		if(k ==(ColRowMin - 1))		//�ѵ����һ��(��)
			break;	//return nRank;
		
		if(iSign != k)				//��Ԫ���ڵ�ǰ��
		{
			for(size_t j = k; j < stCol; j ++)	//������Ԫ��
				_swap(m(k, j), m(iSign, j));
		}

		if(jSign != k)				//��Ԫ���ڵ�ǰ��
		{
			for(size_t i = k; i < stRow; i ++)	//������Ԫ��
				_swap(m(i, jSign), m(i, k));
		}

		for(i = k + 1; i < stRow; i ++)
		{
			const double d(m(i, k) / m(k, k));		//��Ԫ����
			for(size_t j = k + 1; j < stCol; j ++)	
				m(i, j) -= d * m(k, j);		//��ǰ��Ԫ������Ԫ�����任
		}
	}
	//-----------------------------------------
	return nRank;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixInversion
// ��    ��: �������
// ��    ��: public
// ��    ��:
// ��    ��: ������
// ע    ��: 
//			�������ͱ����Ǹ�����
//			������ɹ�����һ���վ���
//			���ı�ԭ����
///////////////////////////////////////////////////////////////////////////
KMatrix KMatrix::MatrixInversion() const
{
	KMatrix mInversion;		//����һ�������

	KMatrix rhs(*this);

	if(rhs.IsSymmetryRegular())	//����ǶԳ���������
	{
		mInversion = rhs.MatrixInversionSymmetryRegular();
	}
	else if(rhs.IsToeplitz())
	{
		mInversion = rhs.MatrixInversionToeplitzTrench();
	}
	else
	{
		mInversion = rhs.MatrixInversionGS();
	}
	//----------------------------------
	return mInversion;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixInversionGS
// ��    ��: ȫѡ��Ԫ��˹-Լ��(Gauss-Jordan)���������
// ��    ��: public
// ��    ��:
// ��    ��: ������
// ע    ��: 
//			�������ͱ����Ǹ�����
//			������ɹ�����һ���վ���
//			���ı�ԭ����
///////////////////////////////////////////////////////////////////////////
KMatrix KMatrix::MatrixInversionGS()
{
	size_t i,j,k;

	KMatrix mInversion;
	KMatrix rhs(*this);

	if(!rhs.IsPhalanx())	return mInversion;		//rhs���Ƿ���

	size_t stRank = rhs.GetColNum();	// �������

	size_t* is = new size_t[stRank];	//�н�����Ϣ
	size_t* js = new size_t[stRank];	//�н�����Ϣ
	
	KMatrix m(rhs);					//����һmatrix����

	for(k = 0; k < stRank; k++)
	{	// ȫѡ��Ԫ
		double MaxValue(0);
		for(i = k; i < stRank; i ++)
		{
			for(j = k; j < stRank; j ++)
			{
				double tmp(fabs(m(i, j)));	//��m(i,j)����ֵ
				if(tmp > MaxValue)				//��Ԫ���ڶԽ�����
				{
					MaxValue = tmp;
					is[k] = i;					//��¼��Ԫ����
					js[k] = j;					//��¼��Ԫ����
				}
			}
		}
		
		if(fabs(MaxValue)<1.0e-12) 
			return mInversion;						//��ԪΪ0����������
		
		if(is[k] != k)							//��Ԫ���ڵ�ǰ��
		{
			for(size_t j = 0; j < stRank; j ++)	//������Ԫ��
				_swap(m(k, j), m(is[k], j));
		}

		if(js[k] != k)							//��Ԫ���ڵ�ǰ��
		{
			for(size_t i = 0; i < stRank; i ++)	//������Ԫ��
				_swap(m(i, k), m(i, js[k]));
		}

		m(k, k) = 1.0 / m(k, k);				//��Ԫ����
		for(size_t j = 0; j < stRank; j ++)
			if(j != k)
				m(k, j) *= m(k, k);

		for(i = 0; i < stRank; i ++)
			if(i != k)
				for(size_t j = 0; j < stRank; j ++)	
					if(j != k)
						m(i, j) = m(i, j) - m(i, k) * m(k, j);

		for(i = 0; i < stRank; i ++)
			if(i != k)
				m(i, k) = -m(i, k) * m(k, k);
	}

	for(int r = (int)stRank - 1; r >= 0; r--)
	{
		if((int)js[r] != r)
			for(size_t j = 0; j < stRank; j ++)
				_swap(m(r, j), m(js[r], j));
		if((int)is[r] != r)
			for(size_t i = 0; i < stRank; i ++)
				_swap(m(i, r), m(i, is[r]));
	}

	mInversion = m;
	//------------------
	delete[] is;
	delete[] js;
	//------------------
	return mInversion;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixInversionSymmetryRegular
// ��    ��: �á�����ѭ�����±�ŷ�����Գ�����������
// ��    ��: public
// ��    ��:
// ��    ��: ������
// ע    ��: 
//			�������ͱ����Ǹ�����
//			������ɹ�����һ���վ���
//			���ı�ԭ����
///////////////////////////////////////////////////////////////////////////
KMatrix KMatrix::MatrixInversionSymmetryRegular() const
{
	size_t i,j,k;

	KMatrix mInversion;		//����һ�������

	KMatrix rhs(*this);
	if(!rhs.IsSymmetryRegular())	return mInversion;	//rhs���ǶԳ�������

	size_t stRank = rhs.GetColNum();	// �������

	KMatrix msr(rhs);			//����һmatrix������rhs��ʼ��

	double *b = new double[stRank];

	for(k=0; k<stRank;k++)
	{
		double w = msr(0,0);
		size_t m = stRank - k -1;
		for(i = 1; i < stRank; i++)
		{
			double g = msr(i, 0);
			b[i] = g / w;

			if(i <= m) b[i] = -b[i];

			for(size_t j = 1; j <= i; j ++)
			{
				msr((i-1),(j-1)) = msr(i, j) + g * b[j];
			}
		}

		msr(stRank-1, stRank-1) = 1.0 / w;
		for(i = 1; i < stRank; i ++)
		{
			msr(stRank-1,(i-1)) =  b[i];
		}
	}

	for(i = 0; i < stRank-1; i ++)
	{
		for(j = i+1; j < stRank; j ++)
		{
			msr(i,j) = msr(j, i);
		}
	}

	//----------------------------------
	mInversion = msr;
	//----------------------------------
	delete[] b;
	//----------------------------------
	return mInversion;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixInversionToeplitzTrench
// ��    ��: ������(Trench)�����в�����(Toeplitz)������
// ��    ��: public
// ��    ��:
// ��    ��: ������
// ע    ��: 
//			�������ͱ����Ǹ�����
//			������ɹ�����һ���վ���
//			���ı�ԭ����
///////////////////////////////////////////////////////////////////////////
KMatrix KMatrix::MatrixInversionToeplitzTrench() const
{
	size_t i,j,k;

	KMatrix mInversion;		//����һ�������

	KMatrix rhs(*this);
	if(!rhs.IsToeplitz())	return mInversion;	//�����в�����

	size_t stRank = rhs.GetColNum();	// �������

	double* t	= new double[stRank];
	double* tuo	= new double[stRank];
	tuo[0]=0.0;
	for(size_t n=0;n<stRank;n++)
	{
		t[n]=rhs(0,n);
		if(n>0)
			tuo[n]=rhs(n,0);
	}

	double* c	= new double[stRank];
	double* p	= new double[stRank];
	double* r	= new double[stRank];

	double a=t[0]; 
	c[0]=tuo[1]/t[0];
	r[0]=t[1]/t[0];

	KMatrix b(rhs);
	for( k=0; k<=stRank-3; k++)
	{
		double s=0.0;
		for( j=1; j<=k+1; j++)
		{
			s=s+c[k+1-j]*tuo[j];
		}

		s=(s-tuo[k+2])/a;
		for( i=0; i<=k; i++)
		{
			p[i]=c[i]+s*r[k-i];
		}

        c[k+1]=-s;
        s=0.0;
        for(j=1; j<=k+1; j++)
		{
			s=s+r[k+1-j]*t[j];
		}

        s=(s-t[k+2])/a;
        for(i=0; i<=k; i++)
        {
			r[i]=r[i]+s*c[k-i];
            c[k-i]=p[k-i];
        }

        r[k+1]=-s;
		a=0.0;
        for(j=1; j<=k+2; j++)
		{
			a=a+t[j]*c[j-1];
		}

        a=t[0]-a;
		if(fabs(a)<1.0e-12)	//a==0
		{
			delete[] t;
			delete[] tuo;

			delete[] c;
			delete[] p;
			delete[] r;

			return mInversion;	
		}
	}//next k

    b(0,0)=1.0/a;
    for( i=0; i<stRank-1; i++)
    {	
		k=i+1;
        b(0, k)=-r[i]/a;
		b(i+1,0)=-c[i]/a;
    }

    for(i=0; i<stRank-1; i++)
	{
		for(size_t j=0; j<stRank-1; j++)
		{
			b(i+1, j+1)=b(i,j)-c[i]*b(0,j+1)+c[stRank-j-2]*b(0,stRank-i-1);
		}
	}

	rhs = b;
	mInversion = rhs;
	//-------------------
	delete[] t;
	delete[] tuo;

	delete[] c;
	delete[] p;
	delete[] r;
	//----------------------------------
	return mInversion;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixLU
// ��    ��: ʵ����LU�ֽ�
// ��    ��: public
// ��    ��:
//			[o] lhs	- ����LU�ֽ�����L��
//			[o] uhs	- ����LU�ֽ�����U��
//
// ��    ��: 
//			-1	- ���Ƿ���
//			 0	- ��ԪΪ0
//			 1	- OK
//
// ע    ��: 
//			A=LU
//			�������ͱ����Ǹ�����
//			������ɹ�����һ���վ���
//			���ı�ԭ����
//			���㷨û��ʹ��ѡ��Ԫ��������ֵ���㲻�ȶ�
///////////////////////////////////////////////////////////////////////////
int KMatrix::MatrixLU(KMatrix& lhs, KMatrix& uhs)
{
	size_t i,j,k;

	if(!IsPhalanx())	return -1;	//���Ƿ���

	size_t stRank = GetColNum();	// �������

	lhs.SetSize(stRank,stRank);		//
	uhs.SetSize(stRank,stRank);		//
	
	KMatrix m(*this);

	for( k=0; k<stRank-1; k++)
	{
		if(fabs(m(k,k)) < 1.0e-12)	return 0;	//��ԪΪ0,��������
		
		for( i=k+1; i<stRank; i++)
			m(i,k) /= m(k,k);
		for(i=k+1; i<stRank; i++)
			for(size_t j=k+1; j<stRank; j++)
				m(i,j)=m(i,j)-m(i,k)*m(k,j);
	}
	//���ϡ���������ֵ
	for( i=0; i<stRank; i++)
	{
		for( j=0; j<i; j++)
		{
			lhs(i,j)=m(i,j);
			uhs(i,j)=0.0;
		}
		lhs(i,i)=1.0;
		uhs(i,i)=m(i,i);
		for(j=i+1; j<stRank; j++)
		{
			lhs(i,j)=0.0;
			uhs(i,j)=m(i,j);
		}
	}
	
	return 1;		//�ֽ�ɹ�
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixQR
// ��    ��: ʵ����QR�ֽ�(�ú�˹�ɶ���(Householder)�任��һ��m*n�׵�ʵ�������QR�ֽ�)
// ��    ��: public
// ��    ��:
//			[o] mQ	- ����QR�ֽ���m*m��������Q
//			[o] mR	- ����QR�ֽ���m*n�����Ǿ���R
//
// ��    ��: 
//			 0	- �������С����
//			 1	- OK
//
// ע    ��: 
//			m>=n
//
///////////////////////////////////////////////////////////////////////////
int KMatrix::MatrixQR(KMatrix& mQ, KMatrix& mR)
{
	size_t i,j,k;

	KMatrix rhs(*this);

	size_t stRow = rhs.GetRowNum();	// ��������
	size_t stCol = rhs.GetColNum();	// ��������
	
	mQ.SetSize(stRow,stRow);
	if(stRow < stCol)	return 0;					//�в���С����

	for( i=0; i<stRow; i++)
	{
		for( j=0; j<stRow; j++)
		{ 
			if(i==j)
				mQ(i,j)=1.0;
			else
				mQ(i,j)=0.0;
		}
	}

	size_t nn=stCol;
    
	if(stRow == stCol) nn=stRow-1;

	for( k=0; k<nn; k++)
    {
		double u=0.0;

        for( i = k; i < stRow; i++)
        { 
			double w = fabs(rhs(i,k));
            if(w > u) u = w;
        }
        double alpha=0.0;
        for(i = k; i < stRow; i++)
        {
			double t=rhs(i,k)/u;
			alpha=alpha+t*t;
		}
        
		if(rhs(k,k)>0.0) u=-u;
        
		alpha=u*sqrt(alpha);
        
		if(fabs(alpha) <1.0e-12) return false;	//alpha=0.0

        u=sqrt(2.0*alpha*(alpha-rhs(k,k)));

        if(fabs(u) > 1.0e-12)	//u>0.0
        { 
			rhs(k,k)=(rhs(k,k)-alpha)/u;
            
			for(i=k+1; i<stRow; i++)
            	rhs(i,k) /= u;

			for( j=0; j<stRow; j++)
            {
				double t=0.0;

                for(size_t jj=k; jj<stRow; jj++)
					t=t+rhs(jj,k)*mQ(jj,j);

                for(i=k; i<stRow; i++)
					mQ(i,j)=mQ(i,j)-2.0*t*rhs(i,k);
            }
            
			for(j=k+1; j<stCol; j++)
            { 
				double t=0.0;
            
				for(size_t jj=k; jj<stRow; jj++)
					t=t+rhs(jj,k)*rhs(jj,j);

                for(i=k; i<stRow; i++)
            		rhs(i,j)=rhs(i,j)-2.0*t*rhs(i,k);
			}

            rhs(k,k)=alpha;

            for(i=k+1; i<stRow; i++)
              rhs(i,k)=0.0;
          }
    }

	for(i=0; i<stRow-1; i++)
	{
		for(size_t j=i+1; j<stRow;j++)
				_swap(mQ(i,j), mQ(j,i));
	}

	mR = rhs;
	//----------------
    return 1;

}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixCholesky
// ��    ��: �Գ������������˹��(Cholesky)�ֽ⼰��������ʽֵ
// ��    ��: public
// ��    ��:
//			[o] ML	- ����ֽ�õ���ML��������
//
// ��    ��: ����ʽֵ
//
// ע    ��: 
//
///////////////////////////////////////////////////////////////////////////
double KMatrix::MatrixCholesky(KMatrix& ML)
{
	size_t i,j,k;

	if(!IsSymmetryRegular())	return double(0);	//���ǶԳ�������


	size_t stRank = GetColNum();	// �������

	KMatrix m(*this);				//����һmatrix������rhs��ʼ��

	long double Det = m(0,0);					//��������ʽֵ
	m(0,0) = sqrt(m(0,0)); 
	
	for( i=1; i<stRank; i++)
		m(i, 0) /= m(0, 0);
	for( j=1; j<stRank; j++)
	{
		for( k=0; k<j; k++)
			m(j,j) = m(j,j) - m(j,k) * m(j,k);
		Det *= m(j,j);
		m(j,j) = sqrt(m(j,j));
		for(i=j+1; i<stRank; i++)
		{
			for(k=0; k<j; k++)
				m(i,j) = m(i,j) -m(i,k) * m(j,k);
			m(i,j) /= m(j,j);
		}
	}
	for(i=0; i<stRank-1; i++)
	    for(j=i+1; j<stRank; j++)
			m(i,j)=0;

	ML = m;			//����Cholesky��
	return Det;		//��������ʽֵ
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixSingularValue
// ��    ��: һ��ʵ����(m*n)������ֵ�ֽ�
// ��    ��: public
// ��    ��:
//			[o] u - (m*m) �����������U
//			[o] v - (n*n) �����������Vt
//			[i] eps - �����ľ���Ҫ��,Ĭ��=0.000001
//			[i] loop - ��������,Ĭ��=60
//
// ��    ��: 
//			�����ص��ǿվ���
//			(m*n)����,��Խ������θ�������ֵ(�Էǵ�����������)������Ԫ��Ϊ0
//
// ע    ��: 
//
///////////////////////////////////////////////////////////////////////////
KMatrix KMatrix::MatrixSingularValue(KMatrix& u,KMatrix& v, double eps,int loop)
{
	size_t j;
	size_t i, it(loop), kk, mm, nn, m1, ks, ka;
    double d,dd,t,sm,sm1,em1,sk,ek,b,c,shh;

	KMatrix matEmpty;	//�վ���
	KMatrix a(*this);

	size_t m = a.GetRowNum();
	size_t n = a.GetColNum();

	u.SetSize(m,m);	//��������u,v�ĳߴ�
	v.SetSize(n,n);	//

	for( j=0; j<m; j++) u(j, m-1) = double(0);

	if(m > n)
		ka = (int)m + 1;
	else
		ka = (int)n + 1;
	
	//---------------------------------------
	double* s	= new double[ka];
	double* e	= new double[ka];
	double* w	= new double[ka];
	double* fg	= new double[2];
	double* cs	= new double[2];
	//---------------------------------------
    
	size_t k = n;
    if(m-1<n) k=m-1;

    size_t l = m;
    if(n-2<m) l=n-2;

    if(l<0) l=0;

    size_t ll=k;
    if(l>k) ll=l;

    if(ll>=1)
	{
		for(kk=1; kk<=ll; kk++)
		{
			if(kk<=k)
			{
				d=0.0;
                for(i=kk; i<=m; i++)
					d=d+a(i-1,kk-1)*a(i-1,kk-1);
                s[kk-1]=sqrt(d);
                if(fabs(s[kk-1])>1.0e-12)	//s[kk-1] != 0
                {
                    if(fabs(a(kk-1,kk-1))>1.0e-12)	//a(kk-1,kk-1)!=0.0
                    {
						s[kk-1]=fabs(s[kk-1]);
                        if(a(kk-1,kk-1)<0.0) s[kk-1]=-s[kk-1];
                    }
                    for(i=kk; i<=m; i++)	a(i-1,kk-1)=a(i-1,kk-1)/s[kk-1];
                    a(kk-1,kk-1)=1.0+a(kk-1,kk-1);
                }
                s[kk-1]=-s[kk-1];
			}

            if(n>=kk+1)
			{
				for(j=kk+1; j<=n; j++)
                {
					if((kk<=k)&&(fabs(s[kk-1])>1.0e-12))	//s[kk-1]!=0.0
                    {
						d=0.0;
                        for(i=kk; i<=m; i++)	d=d+a(i-1,kk-1)*a(i-1,j-1);
                        d=-d/a(kk-1,kk-1);
                        for(i=kk; i<=m; i++)	a(i-1,j-1)=a(i-1,j-1)+d*a(i-1,kk-1);
                    }
                    e[j-1]=a(kk-1,j-1);
                }
			}

            if(kk<=k)
			{
				for(i=kk; i<=m; i++)
					u(i-1,kk-1)=a(i-1,kk-1);
			}

            if(kk<=l)
            { 
				d=0.0;
                for(i=kk+1; i<=n; i++)
                  d=d+e[i-1]*e[i-1];
                e[kk-1]=sqrt(d);
                if(fabs(e[kk-1])>1.0e-12)	//e[kk-1]!=0.0
                {
					if(fabs(e[kk])>1.0e-12)	//e[kk]!=0.0
                    {
						e[kk-1]=fabs(e[kk-1]);
                        if(e[kk]<0.0) e[kk-1]=-e[kk-1];
                    }
                    for(i=kk+1; i<=n; i++)
                      e[i-1]=e[i-1]/e[kk-1];
                    e[kk]=1.0+e[kk];
                }
                e[kk-1]=-e[kk-1];
                if((kk+1<=m)&&(fabs(e[kk-1])>1.0e-12))	//e[kk-1]!=0.0
                { 
					for(i=kk+1; i<=m; i++) w[i-1]=0.0;
                    for(j=kk+1; j<=n; j++)
                      for(i=kk+1; i<=m; i++)
                        w[i-1]=w[i-1]+e[j-1]*a(i-1,j-1);
                    for(j=kk+1; j<=n; j++)
                      for(i=kk+1; i<=m; i++)
                          a(i-1,j-1)=a(i-1,j-1)-w[i-1]*e[j-1]/e[kk];
                }
                for(i=kk+1; i<=n; i++)
                  v(i-1,kk-1)=e[i-1];
            }
		}//next kk
	}//if(ll>=1)

    mm=n;
    if(m+1<n) mm=m+1;
    if(k<n) s[k]=a(k,k);
    if(m<mm) s[mm-1]=0.0;
    if(l+1<mm) e[l]=a(l,mm-1);
    e[mm-1]=0.0;
    nn=m;
    if(m>n) nn=n;

    if(nn>=k+1)
    {
		for(j=k+1; j<=nn; j++)
        {
			for(i=1; i<=m; i++)	u(i-1,j-1)=0.0;
            u(j-1,j-1)=1.0;
        }
    }

    if(k>=1)
    { 
		for(ll=1; ll<=k; ll++)
        {
			kk=k-ll+1;
            if(s[kk-1]!=0.0)
            {
				if(nn>=kk+1)
                  for(j=kk+1; j<=nn; j++)
                  {
					  d=0.0;
                      for(i=kk; i<=m; i++)
						d=d+u(i-1,kk-1)*u(i-1,j-1)/u(kk-1,kk-1);
                      d=-d;
                      for(i=kk; i<=m; i++)
                          u(i-1,j-1)=u(i-1,j-1)+d*u(i-1,kk-1);
                  }
                  for(i=kk; i<=m; i++)
					  u(i-1,kk-1)=-u(i-1,kk-1);
                  u(kk-1,kk-1)=1.0+u(kk-1,kk-1);
                  if(kk-1>=1)
                    for(i=1; i<kk; i++)	u(i-1,kk-1)=0.0;
            }
            else
            { 
				for(i=1; i<=m; i++)	u(i-1,kk-1)=0.0;
                u(kk-1,kk-1)=1.0;
            }
        }
    }

    for(ll=1; ll<=n; ll++)
    { 
		kk=n-ll+1; 
        if((kk<=l)&&(e[kk-1]!=0.0))
        {
			for(j=kk+1; j<=n; j++)
            {
				d=0.0;
                for(i=kk+1; i<=n; i++)
                    d=d+v(i-1,kk-1)*v(i-1,j-1)/v(kk,kk-1);
                d=-d;
                for(i=kk+1; i<=n; i++)
                    v(i-1,j-1)=v(i-1,j-1)+d*v(i-1,kk-1);
            }
        }
        for(i=1; i<=n; i++)	v(i-1,kk-1)=0.0;
        v(kk-1,kk-1)=1.0;
    }

    for(i=1; i<=m; i++)
		for(j=1; j<=n; j++)	a(i-1,j-1)=0.0;

    m1=mm; 
	it=loop;	//��������
	while(1)
	{
		if(mm==0)
        {
			_MSV_1(a,e,s,v,(int)m,(int)n);
			//-----------
			delete[] s;
			delete[] e;
			delete[] w;
			delete[] fg;
			delete[] cs;
			//-----------
			return a;
        }
        if(it==0)
        { 
			_MSV_1(a,e,s,v,(int)m,(int)n);
			//-----------
			delete[] s;
			delete[] e;
			delete[] w;
			delete[] fg;
			delete[] cs;
			//-----------
			return matEmpty;
        }

        kk=mm-1;
		while((kk!=0)&&(fabs(e[kk-1])>1.0e-12))	//e[kk-1]!=0
		{ 
			d=fabs(s[kk-1])+fabs(s[kk]);
			dd=fabs(e[kk-1]);
			if(dd>eps*d) kk=kk-1;
			else e[kk-1]=0.0;
		}

		if(kk==mm-1)
		{
			kk=kk+1;
			if(s[kk-1]<0.0)
			{
				s[kk-1]=-s[kk-1];
				for(i=1; i<=n; i++)		v(i-1,kk-1)=-v(i-1,kk-1);
			}
			while((kk!=m1)&&(s[kk-1]<s[kk]))
			{ 
				d=s[kk-1]; 
				s[kk-1]=s[kk];
				s[kk]=d;
				if(kk<n)
					for(i=1; i<=n; i++)
					{
					  d=v(i-1,kk-1);
					  v(i-1,kk-1)=v(i-1,kk); 
					  v(i-1,kk)=d;
					}
				 if(kk<m)
					  for(i=1; i<=m; i++)
					  { 
						  d=u(i-1,kk-1);
						  u(i-1,kk-1)=u(i-1,kk);
						  u(i-1,kk)=d;
					  }
				kk=kk+1;
            }
            it=loop;
            mm=mm-1;
        }
        else
        { 
			ks=mm;
            while((ks>kk)&&(fabs(s[ks-1])!=0.0))
            {
				d=0.0;
                if(ks!=mm) d=d+fabs(e[ks-1]);
                if(ks!=kk+1) d=d+fabs(e[ks-2]);
                dd=fabs(s[ks-1]);
                if(dd>eps*d) ks=ks-1;
                else s[ks-1]=0.0;
            }
            if(ks==kk)
            { 
				kk=kk+1;
                d=fabs(s[mm-1]);
                t=fabs(s[mm-2]);
                if(t>d) d=t;
                t=fabs(e[mm-2]);
                if(t>d) d=t;
                t=fabs(s[kk-1]);
                if(t>d) d=t;
                t=fabs(e[kk-1]);
                if(t>d) d=t;
                sm=s[mm-1]/d; sm1=s[mm-2]/d;
                em1=e[mm-2]/d;
                sk=s[kk-1]/d; ek=e[kk-1]/d;
                b=((sm1+sm)*(sm1-sm)+em1*em1)/2.0;
                c=sm*em1; c=c*c; shh=0.0;
                if((b!=0.0)||(c!=0.0))
                {
					shh=sqrt(b*b+c);
                    if(b<0.0) shh=-shh;
                    shh=c/(b+shh);
                }
                fg[0]=(sk+sm)*(sk-sm)-shh;
                fg[1]=sk*ek;
                for(i=kk; i<=mm-1; i++)
                { 
					_MSV_2(fg,cs);
                    if(i!=kk) e[i-2]=fg[0];
                    fg[0]=cs[0]*s[i-1]+cs[1]*e[i-1];
                    e[i-1]=cs[0]*e[i-1]-cs[1]*s[i-1];
                    fg[1]=cs[1]*s[i];
                    s[i]=cs[0]*s[i];
                    if((cs[0]!=1.0)||(cs[1]!=0.0))
                      for(j=1; j<=n; j++)
                      {
                          d=cs[0]*v(j-1,i-1)+cs[1]*v(j-1,i);
                          v(j-1,i)=-cs[1]*v(j-1,i-1)+cs[0]*v(j-1,i);
                          v(j-1,i-1)=d;
                      }
                    _MSV_2(fg,cs);
                    s[i-1]=fg[0];
                    fg[0]=cs[0]*e[i-1]+cs[1]*s[i];
                    s[i]=-cs[1]*e[i-1]+cs[0]*s[i];
                    fg[1]=cs[1]*e[i];
                    e[i]=cs[0]*e[i];
                    if(i<m)
                      if((cs[0]!=1.0)||(cs[1]!=0.0))
                        for(j=1; j<=m; j++)
                        { 
                            d=cs[0]*u(j-1,i-1)+cs[1]*u(j-1,i);
                            u(j-1,i)=-cs[1]*u(j-1,i-1)+cs[0]*u(j-1,i);
                            u(j-1,i-1)=d;
                        }
                }
                e[mm-2]=fg[0];
                it=it-1;
            }
            else
            { 
				if(ks==mm)
                {
					kk=kk+1;
                    fg[1]=e[mm-2]; 
					e[mm-2]=0.0;
                    for(ll=kk; ll<=mm-1; ll++)
                    {
						i=mm+kk-ll-1;
                        fg[0]=s[i-1];
                        _MSV_2(fg,cs);
                        s[i-1]=fg[0];
                        if(i!=kk)
                        {
							fg[1]=-cs[1]*e[i-2];
                            e[i-2]=cs[0]*e[i-2];
                        }
                        if((cs[0]!=1.0)||(cs[1]!=0.0))
                          for(j=1; j<=n; j++)
                          { 
                              d=cs[0]*v(j-1,i-1)+cs[1]*v(j-1,mm-1);
                              v(j-1,mm-1)=-cs[1]*v(j-1,i-1)+cs[0]*v(j-1,mm-1);
                              v(j-1,i-1)=d;
                          }
                    }
                }
                else
                { 
					kk=ks+1;
                    fg[1]=e[kk-2];
                    e[kk-2]=0.0;
                    for(i=kk; i<=mm; i++)
                    {
						fg[0]=s[i-1];
                        _MSV_2(fg,cs);
                        s[i-1]=fg[0];
                        fg[1]=-cs[1]*e[i-1];
                        e[i-1]=cs[0]*e[i-1];
                        if((cs[0]!=1.0)||(cs[1]!=0.0))
                          for(j=1; j<=m; j++)
                          {
                              d=cs[0]*u(j-1,i-1)+cs[1]*u(j-1,kk-2);
                              u(j-1,kk-2)=-cs[1]*u(j-1,i-1)+cs[0]*u(j-1,kk-2);
                              u(j-1,i-1)=d;
                          }
                    }
                }
            }
        }
	}//end loop

	//---------------------------------------
	delete[] s;
	delete[] e;
	delete[] w;
	delete[] fg;
	delete[] cs;
	//---------------------------------------
	return a;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixSingularValueGeneralizedInversion
// ��    ��: ����A(m*n)�Ĺ����������ֵ�ֽ�
// ��    ��: public
// ��    ��:
//			[o] ap - (n*m)���A�Ĺ�����A+ 
//			[o] u - (m*m) �����������U
//			[o] v - (n*n) �����������Vt
//			[o] eps - �����ľ���Ҫ��,Ĭ��=1.0e-6
//
// ��    ��: 
//			(m*n)����,��Խ������θ�������ֵ(�Էǵ�����������)������Ԫ��Ϊ0
//			����ʱ���ؿվ���
//
// ע    ��: 
//			���ƻ�ԭ����
//
///////////////////////////////////////////////////////////////////////////
KMatrix KMatrix::MatrixSingularValueGeneralizedInversion(KMatrix& ap, KMatrix& u, KMatrix& v,double eps)
{
	size_t k(0), l, ka;

	KMatrix matEmpty;	//�վ���
	KMatrix a(*this);

	size_t m = a.GetRowNum();
	size_t n = a.GetColNum();

	//��������ap,u,v�ĳߴ�
	ap.SetSize(n,m);
	u.SetSize(m,m);
	v.SetSize(n,n);

	if(m > n)
		ka = m + 1;
	else
		ka = n + 1;

	KMatrix mm=a.MatrixSingularValue(u,v,eps);
    if (mm.IsEmpty())
		return matEmpty;
	else
		a=mm;
  
    size_t j = n;
    if(m < n)
	{
		j = m;
	}
    j = j - 1;

    while((k<=j)&&(fabs(a(k,k))>1.0e-12))	//a(k,k)!=0
	{
		k = k + 1;
	}
    k = k - 1;

    for(size_t i=0; i<n; i++)
	{
		for(j=0; j<m; j++)
		{
			ap(i,j)=0.0;
			for(l=0; l<=k; l++)
			{
				ap(i,j)=ap(i,j)+v(l,i)*u(j,l)/a(l,l);
			}
		}
	}
	//---------------------------------------
	return a;
}

//--------------------------------------------------------
//һ��ʵ���������ֵ�ֽ⸨������
void KMatrix::_MSV_1(KMatrix& a, double* e, double* s,	KMatrix& v, int m, int n)
{
	int i,j;
    double d;
    if(m>=n) i=n;
    else i=m;
    for(j=1; j<i; j++)
    {
		a(j-1,j-1)=s[j-1];
        a(j-1,j)=e[j-1];
    }
    a(i-1,i-1)=s[i-1];
    if(m<n) a(i-1,i)=e[i-1];
    for(i=1; i<n; i++)
    for(j=i+1; j<=n; j++)
    { 
        d=v(i-1,j-1);
		v(i-1,j-1)=v(j-1,i-1); 
		v(j-1,i-1)=d;
    }
}

//һ��ʵ���������ֵ�ֽ⸨������
void KMatrix::_MSV_2(double* fg, double* cs)
{
	double r,d;
	r = fabs(fg[0])+fabs(fg[1]);

	if(fabs(r)<1.0e-12)	// r==0.0
	{
		cs[0]=1.0;
		cs[1]=0.0;
		d=0.0;
	}
	else
	{
		d=sqrt(fg[0]*fg[0]+fg[1]*fg[1]);
        if(fabs(fg[0])>fabs(fg[1]))
        {
			d=fabs(d);
            if(fg[0]<0.0) d=-d;
        }
        if(fabs(fg[1])>fabs(fg[0]) || fabs(fabs(fg[1])-fabs(fg[0]))<1.0e-12	)
        { 
			d=fabs(d);
            if(fg[1]<0.0) d=-d;
        }
        cs[0]=fg[0]/d; 
		cs[1]=fg[1]/d;
	}

    r=1.0;
    if(fabs(fg[0])>fabs(fg[1]))
	{
		r=cs[1];
	}
    else
	{
      if(fabs(cs[0])>1.0e-12)	//cs[0] != 0
		r=1.0/cs[0];
	}

    fg[0]=d;
	fg[1]=r;
}
//--------------------------------------------------------

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixTransformHessenberg
// ��    ��: ��һ��ʵ����(n*n)��Ϊ���겮����(��H��)
// ��    ��: public
// ��    ��:
//			[o] h - ���겮����(��H��)
//
// ��    ��: true/false
//
// ע    ��: 
//			���ƻ�ԭ����
//
///////////////////////////////////////////////////////////////////////////
bool KMatrix::MatrixTransformHessenberg(KMatrix& h)
{
	if(!IsPhalanx())	return false;	//����a���Ƿ���

	KMatrix a(*this);

	size_t i,j,k, stRank;
    double t;
	
	stRank = a.GetColNum();		// �������

    for( k=1; k<stRank-1; k++)
    { 
		double d=0.0;
        for( j=k; j<stRank; j++)
        { 
			t=a(j,k-1);
            if(fabs(t)>fabs(d))
            {
				d=t; 
				i=j;
			}
        }
        if(fabs(d)>1.0e-12)	//d!=0
        { 
			if(i!=k)
            {
				for(j=k-1; j<stRank; j++)	//����i k ��
					_swap(a(i,j), a(k,j));
                for(j=0; j<stRank; j++)		//����i k ��
					_swap(a(j,i), a(j,k));
            }
            for(i=k+1; i<stRank; i++)
            { 
				t=a(i,k-1)/d; 
				a(i,k-1)=0.0;
                for(j=k; j<stRank; j++)
                    a(i,j)=a(i,j)-t*a(k,j);
                for(j=0; j<stRank; j++)
                    a(j,k)=a(j,k)+t*a(j,i);
            }
        }
    }

	h = a;
	return true;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixTransformHouseholder
// ��    ��: �ú�˹�ɶ���(Householder)�任����n��ʵ�Գ���Լ��Ϊ�Գ����Խ���
// ��    ��: public
// ��    ��:
//			[o] q - ���غ�˹�ɶ��±任�ĳ˻�����Q,��� n*n
//			[o] b - nά����ָ��,���ضԳ����Խ�������Խ���Ԫ��
//			[o] c - nά����ָ��,���ضԳ����Խ���ĴζԽ���Ԫ��
//
// ��    ��: 
//			false - �����ǶԳ���
//			true - OK
//
// ע    ��: 
//			���ƻ�ԭ����
//
///////////////////////////////////////////////////////////////////////////
bool KMatrix::MatrixTransformHouseholder(KMatrix& q, double* b, double* c)
{
	if(!IsSymmetry())	return false;	//�����ǶԳ���

	size_t i,j, k, stRank;
    double h, f, g, h2;

	q=*this;
	stRank = q.GetColNum();		// �������

    for( i=stRank-1; i>=1; i--)
    { 
		h=0.0;
        if(i>1)
          for(k=0; k<i; k++)
			  h=h+q(i,k)*q(i,k);
        if(fabs(h)<1.0e-12)		//h == 0
        {
			c[i]=0.0;
            if(i==1) c[i]=q(i,i-1);
            b[i]=0.0;
        }
        else
        { 
			c[i]=sqrt(h);
            if(q(i,i-1)>0.0) c[i]=-c[i];
            h=h-q(i,i-1)*c[i];
            q(i,i-1)=q(i,i-1)-c[i];
            f=0.0;
            for(j=0; j<i; j++)
            { 
				q(j,i) = q(i,j) / h;
                g = 0.0;
                for(k=0; k<=j; k++)
                  g = g + q(j,k) * q(i,k);
                if(j+1<i)
                  for(k=j+1; k<i; k++)
                    g = g + q(k,j) * q(i,k);
                c[j] = g / h;
                f = f + g * q(j,i);
            }
            h2 = f / (h+h);
            for(j=0; j<i; j++)
            { 
				f = q(i,j);
                g = c[j] -h2 * f;
                c[j] = g;
                for(k=0; k<=j; k++)
                    q(j,k)=q(j,k)-f*c[k]-g*q(i,k);
            }
            b[i]=h;
        }
    }
    for(i=0; i<stRank-1; i++) c[i]=c[i+1];
    c[stRank-1]=0.0;
    b[0]=0.0;
    for(i=0; i<stRank; i++)
    { if((b[i]!=0.0)&&(i-1>=0))
          for(j=0; j<i; j++)
          { 
			  g=0.0;
              for(k=0; k<i; k++)
				g=g+q(i,k)*q(k,j);
              for(k=0; k<i; k++)
                q(k,j)=q(k,j)-g*q(k,i);
          }
        b[i]=q(i,i);
		q(i,i)=1.0;
        if(i-1>=0)
          for(j=0; j<i; j++)
          { 
			  q(i,j)=0.0;
			  q(j,i)=0.0;
		  }
    }

	//---
	return true;	//��������
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixEigenvalueVectorRealSymmetryTriangleQR
// ��    ��: ʵ�Գ����Խ���ȫ������ֵ����������QR��
// ��    ��: public
// ��    ��:
//			b - ����ָ��,���n�׶Գ����Խ�������Խ���Ԫ��
//			c - ����ָ��,���n�׶Գ����Խ���ĴζԽ���Ԫ��
//			q - n*n����. 
//		   ����ŵ�λ����,�򷵻�ʱ���n�ױ�ʵ�Գ����Խ��������������
//         �������MatrixTransformHouseholder()���ɵ�ʵ�Գ���A�ĳ˻�����Q,
//         �򷵻�ʱ���ʵ�Գ���A������������
//         ����,q�ĵ�j��Ϊ������b�е�j������ֵ��Ӧ����������
//			eps - ����Ҫ��,Ĭ��=1.0e-6
//			loop - ��������,Ĭ��=60
//
// ��    ��: 
//			<0 - ��������������û�����㾫��Ҫ��
//			 0 - q����n*n����
//			>0 - OK
//
// ע    ��: 
//			���ƻ�ԭ����
//
///////////////////////////////////////////////////////////////////////////
int KMatrix::MatrixEigenvalueVectorRealSymmetryTriangleQR(double* b,double* c, KMatrix& q, double eps, int loop)
{
	if(q.IsPhalanx())	return 0;	//qӦ��n*n����

	size_t i,j, k, m, it, stRank;
    double h, g, p, r, e, s, d(0), f(0);

	stRank = q.GetColNum();		// �������
	//stRank = n;		// �������

	c[stRank-1]=0.0;

    for( j=0; j<stRank; j++)
    { 
		it=0;
        h=eps*(fabs(b[j])+fabs(c[j]));
        if(h>d) d=h;
        m=j;
        while((m<stRank)&&(fabs(c[m])>d)) m++;
        if(m!=j)
        {
			do
            {
				if(it==loop)
                { 
					//cout << "fail" << endl;
                    return -1;				//����,��������������û�дﵽ����Ҫ��
                }
                it++;
                g=b[j];
                p=(b[j+1]-g)/(2.0*c[j]);
                r=sqrt(p*p+1.0);

                if(p>0.0 || fabs(p)<1.0e-12)	//p==0
					b[j]=c[j]/(p+r);
                else
					b[j]=c[j]/(p-r);

                h=g-b[j];
                for(i=j+1; i<stRank; i++)
					b[i]=b[i]-h;
                f=f+h; 
				p=b[m]; 
				e=1.0; 
				s=0.0;
                for(i=m-1; i>=j; i--)
                {
					g=e*c[i];
					h=e*p;
                    if(fabs(p)>=fabs(c[i]))
                    {
						e=c[i]/p;
						r=sqrt(e*e+1.0);
                        c[i+1]=s*p*r;
						s=e/r; 
						e=1.0/r;
                    }
                    else
					{
						e=p/c[i]; 
						r=sqrt(e*e+1.0);
                        c[i+1]=s*c[i]*r;
                        s=1.0/r; 
						e=e/r;
                    }
                    p=e*b[i]-s*g;
                    b[i+1]=h+s*(e*g+s*b[i]);
                    for(k=0; k<stRank; k++)
                    { 
                        h=q(k,i+1);
						q(k,i+1)=s*q(k,i)+e*h;
                        q(k,i)=e*q(k,i)-s*h;
                    }
                }
                c[j]=s*p;
				b[j]=e*p;
            }while(fabs(c[j])>d);
        }
        b[j]=b[j]+f;
    }
    for(i=0; i<stRank; i++)
    { 
		k=i;
		p=b[i];
        if(i+1<stRank)
        {
			j=i+1;
            while((j<stRank)&&(b[j]<=p))
            {
				k=j;
				p=b[j];
				j=j+1;
			}
		}
        if(k!=i)
        { 
			b[k]=b[i]; 
			b[i]=p;
            for(j=0; j<stRank; j++)
            {
                p=q(j,i);
				q(j,i)=q(j,k);
				q(j,k)=p;
            }
        }
    }

	return 1;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixEigenvalueVectorRealSymmetryJacobi
// ��    ��: ʵ�Գ���(n*n)����ֵ�����������ſɱȷ�
// ��    ��: public
// ��    ��:
//			lmd - ����,����ʱΪn*n�׷���,���Խ��ߴ��n������ֵ
//			v - n�׷���,�������������,j��������������lmd(j,j)������ֵ��Ӧ
//			eps - ����Ҫ��,Ĭ��=1.0e-6
//			loop - ��������,Ĭ��=60
//
// ��    ��: 
//		-1	-->��������������û�����㾫��Ҫ��
//		 0	-->���ǶԳ���
//		 1	-->OK
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
int KMatrix::MatrixEigenvalueVectorRealSymmetryJacobi(KMatrix& lmd,KMatrix& v,double eps, int loop)
{
	if(!IsSymmetry())	return 0;	//���ǶԳ���

	size_t i, j, p, q, l(1), stRank;
    double fm,cn,sn,omega,x,y,d;

	lmd = *this;
	stRank = lmd.GetColNum();		// �������

	v.SetSize(stRank,stRank);

    for( i=0; i<stRank; i++)
    {
		v(i,i)=1.0;
        for(j=0; j<stRank; j++)
          if(i!=j) v(i,j)=0.0;
    }
    while(1)
    { 
		fm=0.0;
        for(i=1; i<stRank; i++)
        for(j=0; j<i; j++)
        {
			d=fabs(lmd(i,j));
            if((i!=j)&&(d>fm))
            {
				fm=d; 
				p=i;
				q=j;
			}
        }
        if(fm<eps)
			return(1);
        if((int)l>loop)
			return(-1);
        l=l+1;
        x=-lmd(p,q);
		y=(lmd(q,q)-lmd(p,p))/2.0;
        omega=x/sqrt(x*x+y*y);
        if(y<0.0) omega=-omega;
        sn=1.0+sqrt(1.0-omega*omega);
        sn=omega/sqrt(2.0*sn);
        cn=sqrt(1.0-sn*sn);
        fm=lmd(p,p);
        lmd(p,p)=fm*cn*cn+lmd(q,q)*sn*sn+lmd(p,q)*omega;
        lmd(q,q)=fm*sn*sn+lmd(q,q)*cn*cn-lmd(p,q)*omega;
        lmd(p,q)=0.0; 
		lmd(q,p)=0.0;
        for(j=0; j<stRank; j++)
        if((j!=p)&&(j!=q))
        {
            fm=lmd(p,j);
            lmd(p,j)=fm*cn+lmd(q,j)*sn;
            lmd(q,j)=-fm*sn+lmd(q,j)*cn;
        }
        for(i=0; i<stRank; i++)
          if((i!=p)&&(i!=q))
          { 
              fm=lmd(i,p);
              lmd(i,p)=fm*cn+lmd(i,q)*sn;
              lmd(i,q)=-fm*sn+lmd(i,q)*cn;
          }
        for(i=0; i<stRank; i++)
        {
            fm=v(i,p);
            v(i,p)=fm*cn+v(i,q)*sn;
            v(i,q)=-fm*sn+v(i,q)*cn;
        }
    }

	//---
	return 1;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: MatrixEigenvalueVectorRealSymmetryJacobiBarrier
// ��    ��: ʵ�Գ�������ֵ(n*n)�����������ſɱȹ��ط�
// ��    ��: public
// ��    ��:
//			lmd - n*n����,����ʱ���Խ��߱���n������ֵ
//			v - n*n����,������������. ���е�j��Ϊ��lmd(j,j)����ֵ��Ӧ����������
//			eps - ����Ҫ��,Ĭ��=1.0e-6
//
// ��    ��: 
//		 0	-->���ǶԳ���
//		 1	-->OK
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
int KMatrix::MatrixEigenvalueVectorRealSymmetryJacobiBarrier(KMatrix& lmd,KMatrix& v,double eps)
{
	if(!IsSymmetry())	return 0;	//���Գ�

	size_t i,j, p, q, stRank;
    double fm, cn, sn, omega, x, y, d;

	lmd = *this;
	stRank = lmd.GetColNum();		//�������
	v.SetSize(stRank,stRank);


    for( i=0; i<stRank; i++)
    { 
		v(i,i)=1.0;
        for(j=0; j<stRank; j++)
		{
          if(i!=j) v(i,j)=0.0;
		}
    }
    double ff(0);
    for(i=1; i<stRank; i++)
	{
		for(j=0; j<i; j++)
		{
			d=lmd(i,j);
			ff=ff+d*d;
		}
	}
    ff=sqrt(2.0*ff);

loop0:
    ff=ff/(1.0*stRank);
loop1:
    for(i=1; i<stRank; i++)
	{
        for(j=0; j<i; j++)
        { 
			d=fabs(lmd(i,j));
            if(d>ff)
            { 
				p=i;
				q=j;
                goto loop;
            }
        }
	}
	if(ff<eps) return(1);	//�ɹ�����
    goto loop0;
loop:
    x=-lmd(p,q);
	y=(lmd(q,q)-lmd(p,p))/2.0;
    omega=x/sqrt(x*x+y*y);
    if(y<0.0) omega=-omega;
    sn=1.0+sqrt(1.0-omega*omega);
    sn=omega/sqrt(2.0*sn);
    cn=sqrt(1.0-sn*sn);
    fm=lmd(p,p);
    lmd(p,p)=fm*cn*cn+lmd(q,q)*sn*sn+lmd(p,q)*omega;
    lmd(q,q)=fm*sn*sn+lmd(q,q)*cn*cn-lmd(p,q)*omega;
    lmd(p,q)=0.0;
	lmd(q,p)=0.0;
    for(j=0; j<stRank; j++)
	{
        if((j!=p)&&(j!=q))
        {
            fm=lmd(p,j);
            lmd(p,j)=fm*cn+lmd(q,j)*sn;
            lmd(q,j)=-fm*sn+lmd(q,j)*cn;
        }
	}
    for(i=0; i<stRank; i++)
	{
        if((i!=p)&&(i!=q))
        {
			fm=lmd(i,p);
            lmd(i,p)=fm*cn+lmd(i,q)*sn;
            lmd(i,q)=-fm*sn+lmd(i,q)*cn;
        }
	}
    for(i=0; i<stRank; i++)
    { 
        fm=v(i,p);
        v(i,p)=fm*cn+v(i,q)*sn;
        v(i,q)=-fm*sn+v(i,q)*cn;
    }
    goto loop1;


	return 1;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: _MatrixMultiply
// ��    ��: ����˷�����
// ��    ��: public
// ��    ��:
//			[i] lhs - �����
//			[i] rhs - �Ҿ���
//			[o] mOut - ����˻�
//
// ��    ��: 
//
// ע    ��: 
//
///////////////////////////////////////////////////////////////////////////
void KMatrix::_MatrixMultiply(const KMatrix& lhs, const KMatrix& rhs, KMatrix& mOut)
{	
	//�϶���߾�����������ұ߾�����������
	if(lhs.GetColNum() != rhs.GetRowNum()) throw;	//���� == ����

	//���ɾ����¶�����lhs������Ϊ�������������rhs��������Ϊ���������
	mOut.SetSize(lhs.GetRowNum(),rhs.GetColNum());

	for(size_t i = 0; i < lhs.GetRowNum(); i ++)			//���������
	{
		for(size_t j = 0; j < mOut.GetColNum(); j ++)		//�Ҿ�������
		{
			mOut(i, j) = double(0);		//����ֵ0
			for(size_t k = 0; k < lhs.GetColNum(); k ++)	//����
			{
				mOut(i, j) += lhs(i, k) * rhs(k, j);
			}
		}
	}
	//---
	return;		//���ؽ������
}

//����ֵ
void KMatrix::_swap(double& d1, double& d2)
{
	double dTemp = d1;
	d1=d2;
	d2=dTemp;
}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Trace
// ��    ��: ����׷��
// ��    ��: public
// ��    ��:
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
//void KMatrix::Trace()
//{
//	TRACE("\n----------------------------------------------\n");
//	for(size_t i=0;i<m_nRow;i++)
//	{
//		for(size_t j=0;j<m_nCol;j++)
//		{
//			TRACE("%8.3f ",m_Data[i*m_nCol+j]);
//		}
//		TRACE("\n");
//	}
//	TRACE("----------------------------------------------\n\n");
//}

///////////////////////////////////////////////////////////////////////////
// ��    ��: Output
// ��    ��: ��������������ļ�
// ��    ��: public
// ��    ��:
//			[i] lpszFile - �����ļ���
//
// ��    ��: 
// ע    ��: 
///////////////////////////////////////////////////////////////////////////
void KMatrix::Output(const char* lpszFile)
{
	FILE* fp=fopen(lpszFile,"wt");
	if(!fp)	return;
	//-------------------------
	fprintf(fp,"%d %d\n",m_nRow,m_nCol);

	for(size_t i=0;i<m_nRow;i++)
	{
		for(size_t j=0;j<m_nCol;j++)
		{
			fprintf(fp,"%8.3f ",m_Data[i*m_nCol+j]);
		}
		fprintf(fp,"\n");
	}
	//-------------------------
	fclose(fp);
}
