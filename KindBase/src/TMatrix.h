/******************************************************************************
 ** TMatrix.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: TMatrix
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: ������.֣�� (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#pragma warning( disable : 4290 )
// warning C4290: ���� C++ �쳣�淶����ָʾ�������� __declspec(nothrow)

template<typename T>
class TMatrix
{
public:
	TMatrix(void) throw()
		: m_nCol(0), m_nRow(0), m_pData(NULL)
	{
	}
	TMatrix(const TMatrix& rhs) throw()
		: m_nCol(0), m_nRow(0), m_pData(NULL)
	{
		m_nCol = rhs.m_nCol;
		m_nRow = rhs.m_nRow;
		int nTotalSize = m_nCol * m_nRow;
		m_pData = new T[nTotalSize];
		memcpy(m_pData, rhs.m_pData, sizeof(T) * nTotalSize);
	}
	TMatrix(int Rows, int Cols) throw()
		: m_nCol(Rows), m_nRow(Cols), m_pData(NULL)
	{
		int nTotalSize = Cols * Rows;
		m_pData = new T[nTotalSize];
		memset(m_pData, 0, sizeof(T) * nTotalSize);
	}
	TMatrix(int Rows, int Cols, T* pData) throw()
		: m_nCol(Rows), m_nRow(Cols), m_pData(NULL)
	{
		int nTotalSize = Cols * Rows;
		m_pData = new T[nTotalSize];
		memcpy(m_pData, pData, sizeof(T) * nTotalSize);
	}

	virtual ~TMatrix(void)
	{
		Empty();
	}

public:
	// �þ���Ϊ�վ���
	TMatrix& Empty()
	{
		m_nRow = 0;
		m_nCol = 0;
		if(m_pData)
		{
			delete[] m_pData;
			m_pData = NULL;
		}

		return *this;
	}
	// ����
	TMatrix& Zero() throw()
	{
		memset(m_pData, 0, sizeof(T) * m_nCol * m_nRow);
		return *this;
	}
	// ���óߴ�
	void SetDimension(int nRows, int nCols) throw()
	{
		Empty();
		m_nCol = nCols;
		m_nRow = nRows;
		int nTotalSize = m_nCol * m_nRow;
		m_pData = new T[nTotalSize];
		memset(m_pData, 0, sizeof(T) * nTotalSize);
	}
	// ȡ�óߴ�
	void GetDimension(int& nRows, int& nCols) throw()
	{
		nCols = m_nCol;
		nRows = m_nRow;
	}
	// ������
	int Width() const throw()
	{
		return m_nCol;
	}
	// ����߶�
	int Height() const throw()
	{ 
		return m_nRow;
	}
	// ��������
	int Cols() const throw()
	{
		return m_nCol;
	}
	// ��������
	int Rows() const throw()
	{ 
		return m_nRow;
	}
	//��������
	int GetRowNum() const throw()
	{
		return m_nRow;
	}
	//��������
	int GetColNum() const throw()
	{
		return m_nCol;
	}

public:
	//�Ƿ�վ���
	bool IsEmpty() const throw()
	{
		return (m_nRow==0 || m_nCol==0 || m_pData==NULL);
	}
	//�Ƿ���
	bool IsPhalanx() const throw()
	{
		return (!IsEmpty() && (m_nRow == m_nCol));
	}
	//�Ƿ�������
	bool IsRegular() const throw()
	{
		int i,j,k;

		// ������������Ƿ���
		if(!IsPhalanx())
			return false;

		for(k = 0; k < m_nRow; k ++)
		{
			if( abs( ((*this)(k, k))) < 1.0e-12	|| ((*this)(k, k)) < 0)	//(k,k) <= 0
				return false;	//�Խ�Ԫ������0��������������
		}
		//---
		T det;
		for(k = 2; k <= m_nRow; k++)
		{
			TMatrix m(k, k);	//����һmatrix����

			for( i=0; i<k; i++)
			{
				for( j=0; j<k; j++)
				{
					m(i, j) = (*this)(i, j);	//��ʼ��
				}
			}

			det = m.Determinant();				// ˳������ʽ��ֵ
		
			if(abs(det)<1.0e-12 || det < 0.0)	//ldDet <= 0
				return false;					//����������
		}
		//---
		return true;
	}
	//�Ƿ�Գ���
	bool IsSymmetry() const throw()
	{
		if(!IsPhalanx())	// �����Ƿ���
			return false;
		for(int i=0;i<m_nRow;i++)
		{
			for(int j=0;j<m_nCol;j++)
			{
				T v1 = m_pData[i*m_nCol+j];	// (i,j)
				T v2 = m_pData[j*m_nCol+i];	// (j,i)
				if(v1!=v2)
					return false;
			}
		}
		return true;
	}
	//�Ƿ�Գ�������
	bool IsSymmetryRegular() const throw()
	{
		return IsSymmetry() && IsRegular();
	}
	//�Ƿ��в�����(Toeplitz)����
	bool IsToeplitz() const throw()
	{
		if(!IsPhalanx())	return false;	//���Ƿ���

		size_t nRank = m_nRow;	//ȡ�þ����

		TMatrix m(*this);

		if(abs(m(0,0))<1.0e-12)
			return false;	//m(0,0) != 0

		for(size_t i=0;i<nRank-1;i++)
		{
			for(size_t j=0;j<nRank-1;j++)
			{
				T dTemp = m(i,j) - m(i+1,j+1);
				if(abs(dTemp)>1.0e-12)	//m(i,j) != m(i+1,j+1)
					return false;
			}
		}
		//---
		return true;
	}
	//�Ƿ����Գ���
	bool IsSymmetry3() const throw()
	{
		if(!IsSymmetry())	return false;	//���ǶԳ���

		size_t nRank = m_nRow;	//�������

		for(size_t i=0;i<nRank;i++)
		{
			for(size_t j=0;j<nRank;j++)
			{
				if(abs(int(i-j)) > 1)
				{
					T dTemp = (T)( (*this)(i,j) );
					if(abs(dTemp)>1.0e-12)	//dTemp!=0
					{
						return false;
					}
				}
			}
		}
		return true;
	}

public:
	// ����ʽֵ(ȫѡ��Ԫ�����������ʽ��ֵ)
	T Determinant() const throw()
	{
		size_t i(0), j(0), k(0);

		// ���Ƿ���
		if(!IsPhalanx())
			return T(0);

		// ����һ Matrix ����
		TMatrix m(*this);

		T maxValue(0), tmp(0);

		size_t stRank = Rows();				// ��������
		size_t iSign(0), jSign(0);			// ��Ԫ��λ�ñ�־
		int	nSgn = 1;						// ����

		T det(1);							// ����ʽ��ֵ

		// ȫѡ��Ԫ
		for(k = 0; k < stRank - 1; k++)
		{
			maxValue = T(0);
			for(i = k; i < stRank; i ++)
			{
				for(j = k; j < stRank; j++)
				{
					tmp = abs( m(i, j) );
					if(tmp > maxValue)
					{
						// ������Ԫֵ��λ��
						maxValue = tmp;
						iSign = i;
						jSign = j;
					}
				}
			}
		
			// ����ֵ���Ԫ��Ϊ0, ����ʽҲΪ0
			if(abs(maxValue)<1.0e-12)
				return T(0);

			//����ֵ���Ԫ�ز��ڵ�ǰ��
			if(iSign != k)
			{
				// �任����ʽ����
				nSgn = -nSgn;
				// ������
				for( j = k; j < stRank; j ++)
				{
					// ���� (k,j) �� (iSign, j)
					tmp = m(k, j);
					m(k, j) =  m(iSign, j);
					m(iSign, j) = tmp;
				}
			}
			// ����ֵ���Ԫ�ز��ڵ�ǰ��
			if(jSign != k)
			{
				// �任����ʽ����
				nSgn = -nSgn;
				// ������
				for( i = k; i < stRank; i ++)
				{
					// ���� (i, jSign) �� (i, k)
					tmp = m(i,jSign);
					m(i,jSign) = m(i,k);
					m(i,k) = tmp;
				}
			}

			// �Խ�Ԫ���
			det *= m(k, k);
			for(i = k + 1; i < stRank; i ++)
			{
				if( abs(m(k, k)) < 1.0e-12 )	// ==0
					throw;

				// ��Ԫ����
				T d = m(i, k) / m(k, k);
				for(j = k + 1; j < stRank; j ++)	// ����Ԫ�·�Ԫ����Ϊ0
				{
					m(i, j) -= d * m(k, j);	// ��ǰ��Ԫ����������Ԫ�����任
				}
			}
		}

		return det * nSgn * m(stRank - 1, stRank - 1);	// ��������ʽ��ֵ
	}
	// �������
	unsigned int Rank() const throw()
	{
		size_t i,j,k;
		size_t iSign, jSign;					//��Ԫ��λ�ñ�־

		size_t nRank = 0;						//��������
		size_t stRow = m_nRow;					//ȡ��������
		size_t stCol = m_nCol;					//ȡ��������
		size_t minColRow = min(stRow, stCol);	//ȡ�л�����Сֵ

		// ���� TMatrix ������this��ʼ��
		TMatrix m(*this);

		T maxValue(0), tmp(0);
		for(k = 0; k < minColRow; k ++)	// ȫѡ��Ԫ
		{
			maxValue = T(0);
			for(i = k; i < stRow; i ++)
			{
				for(j = k; j < stCol; j ++)
				{
					tmp= abs(m(i, j));	//��m(i,j)����ֵ
					if(tmp > maxValue)
					{
						maxValue = tmp;
						iSign = i;					//������Ԫλ��
						jSign = j;
					}
				}
			}

			//����Ԫ�ؾ���ֵ�����Ϊ0
			if(abs(maxValue)<1.0e-12) // maxValue == 0
				break;	//return nRank;
			else				
				nRank++;		//����Ԫ�ؾ���ֵ����߲�Ϊ0�������ȼ�1

			if(k ==(minColRow - 1))		//�ѵ����һ��(��)
				break;	//return nRank;
		
			if(iSign != k)				//��Ԫ���ڵ�ǰ��
			{
				for( j = k; j < stCol; j ++)	//������Ԫ��
				{
					swap(m(k, j), m(iSign, j));
				}
			}

			if(jSign != k)				//��Ԫ���ڵ�ǰ��
			{
				for(size_t i = k; i < stRow; i ++)	//������Ԫ��
				{
					swap(m(i, jSign), m(i, k));
				}
			}

			for(i = k + 1; i < stRow; i ++)
			{
				T d = m(i, k) / m(k, k);		//��Ԫ����
				for(size_t j = k + 1; j < stCol; j ++)
				{
					m(i, j) -= d * m(k, j);		//��ǰ��Ԫ������Ԫ�����任
				}
			}
		}

		return nRank;
	}
	// ����ת��
	TMatrix& Transpose() throw()
	{
		if(!IsEmpty())
		{
			TMatrix m(*this);
			for(int i=0;i<m_nRow;i++)
			{
				for(int j=0;j<m_nCol;j++)
				{
					(*this)(i,j) = m(j,i);
				}
			}
		}

		return *this;
	}
	// �������
	TMatrix& Inversion() throw()
	{
		TMatrix mtmp(*this);

		if(mtmp.IsSymmetryRegular())	// ����ǶԳ���������
		{
			*this = mtmp.Inversion_SymmetryRegular();
		}
		else if(mtmp.IsToeplitz())
		{
			*this = mtmp.Inversion_ToeplitzTrench();
		}
		else
		{
			*this = mtmp.Inversion_gs();
		}

		return (*this);
	}
	// n�׵�λ��
	TMatrix& Identity(int n) throw()
	{
		if(n<=0)
			return this->Empty();

		this->SetDimension(n,n);
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(i==j)
					m_pData[i*n+j] = T(1);
				else
					m_pData[i*n+j] = T(0);
			}
		}

		return *this;
	}
	//---------------------------------------------------------
	// Name:     SwapRow
	// Function: �����������
	// Parameters:
	//		[i] r1 - �к�1(��1��ʼ)
	//		[1] r1 - �к�2(��1��ʼ)
	//
	// Return: ����
	//
	// Comment: �кŴ�1��ʼ
	//
	//---------------------------------------------------------
	TMatrix& SwapRow(int r1,int r2) throw(KException)
	{
		int nRow = m_nRow;	//����
		int nCol = m_nCol;	//����

		if(r1 == r2)
			return *this;

		r1--;
		r2--;
		if(r1<0 || r1>=nRow || r2<0 || r2>=nRow)
		{
			throw KException(_T("The row number is out of range!"));
		}

		for(int j=0;j<nCol;j++)	//��ѭ��
		{
			T dTemp		= m_pData[r1*nCol+j];
			m_pData[r1*nCol+j]	= m_pData[r2*nCol+j];	// ��*���� + ��
			m_pData[r2*nCol+j]	= dTemp;
		}

		return *this;
	}
	//---------------------------------------------------------
	// Name:     SwapCol
	// Function: �����������
	// Parameters:
	//		[i] c1 - �к�1(��1��ʼ)
	//		[1] c2 - �к�2(��1��ʼ)
	//
	// Return: ����
	//
	// Comment: �кŴ�1��ʼ
	//
	//---------------------------------------------------------
	TMatrix& SwapCol(int c1,int c2) throw(KException)
	{
		int nRow = m_nRow;	//����
		int nCol = m_nCol;	//����

		if(c1 == c2)
			return *this;

		c1--;
		c2--;
		if(c1<0 || c1>=nCol || c2<0 || c2>=nCol)
		{
			throw KException(_T("The column number is out of range!"));
		}

		for(int i=0;i<nRow;i++)	//��ѭ��
		{
			T dTemp		= m_pData[i*nCol+c1];
			m_pData[i*nCol+c1]	= m_pData[i*nCol+c2];	// ��*���� + ��
			m_pData[i*nCol+c2]	= dTemp;
		}

		return *this;
	}
	// �������Գ���
	// ��    ��:
	//	[i] b - nά����,�洢���Գ����е����Խ���Ԫ��
	//	[i] c - nά����,�洢���Գ����еĴζԽ���Ԫ��
	//	[i] n - ����ά��
	TMatrix& Symmetry3(T* b, T* c, int n) throw()
	{
		if(n<=0)
			return this->Empty();

		this->SetDimension(n,n);
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(i==j)
				{
					m_pData[i*n+j] = b[i];
				}
				else if(abs(int(i-j)) ==1 )
				{
					int m = min(i,j);
					m_pData[i*n+j] = c[m];
				}
				else
				{
					m_pData[i*n+j] = T(0);
				}
			}
		}

		return *this;
	}
	// �����в�����(Toeplitz)���� 
	// ��    ��:
	//	[i] tu - ���в����ľ����ϰ�������
	//	[i] tl - ���в����ľ����°�������
	//	[i] n -  ����ά��
	TMatrix& Toeplitz(T* tu, T* tl, int n) throw()
	{
		if(n<=0 || abs(tu[0]) < 1.0e-12)
			return this->Empty();

		this->SetDimension(n,n);
		for(int i=0;i<n;i++)
		{
			for(int j=0;j<n;j++)
			{
				if(i<=j)
					m_pData[i*n+j] = tu[j-i];
				else	//i>j
					m_pData[i*n+j] = tl[i-j];
			}
		}

		return *this;
	}
	//---------------------------------------------------------
	// Name:     Cholesky
	// Function: �Գ������������˹��(Cholesky)�ֽ⼰��������ʽֵ
	// Parameters:
	//		[o] mout	- ����ֽ�õ���ML��������
	//
	// Return: ����ʽֵ
	//
	// Comment: 
	//
	//---------------------------------------------------------
	T Cholesky(TMatrix& mout) throw()
	{
		size_t i,j,k;

		// ���ǶԳ�������
		if(!IsSymmetryRegular())
			return T(0);

		// �������
		size_t stRank = m_nCol;
		TMatrix m(*this);		// ����һmatrix������this��ʼ��
		T det = m(0,0);			// ��ʼ����ʽֵ

		m(0,0) = sqrt(m(0,0)); 
		for( i=1; i<stRank; i++)
		{
			m(i, 0) /= m(0, 0);
		}

		for( j=1; j<stRank; j++)
		{
			for( k=0; k<j; k++)
			{
				m(j,j) = m(j,j) - m(j,k) * m(j,k);
			}

			det *= m(j,j);
			m(j,j) = sqrt(m(j,j));

			for(i=j+1; i<stRank; i++)
			{
				for(k=0; k<j; k++)
				{
					m(i,j) = m(i,j) -m(i,k) * m(j,k);
				}
				m(i,j) /= m(j,j);
			}
		}

		for(i=0; i<stRank-1; i++)
		{
			for(j=i+1; j<stRank; j++)
			{
				m(i,j)=0;
			}
		}

		mout = m;	// ����Cholesky��
		return det;		// ��������ʽֵ
	}
	//---------------------------------------------------------
	// Name:     LU
	// Function: ����ֽ� - LU
	// Parameters:
	//		[o] lhs - ����LU�ֽ�����L��
	//		[o] uhs - ����LU�ֽ�����U��
	//
	// Return: 
	//		true - �ɹ�
	//		false - ʧ��
	//
	// Comment: ����ֽ� (decomposition, factorization)�ǽ�����
	//			���Ϊ��������ĳ˻����ɷ�Ϊ���Ƿֽ⡢���ȷֽ⡢
	//			QR�ֽ⡢Jordan�ֽ��SVD������ֵ���ֽ�ȣ�
	//			�����������֣�
	//				1)���Ƿֽⷨ (Triangular Factorization),�ֳ�LU�ֽ�
	//				2)QR �ֽⷨ (QR Factorization)
	//				3)����ֵ�ֽⷨ (Singular Value Decompostion)
	//
	// [���Ƿֽⷨ]
	// �ǽ�ԭ���� (square) ����ֽ��һ���������ξ����������(permuted)
	// ���������ξ����һ���������ξ��������ķֽⷨ�ֳ�ΪLU�ֽⷨ��
	// ������;��Ҫ�ڼ�һ������������ʽֵ�ļ�����̣��󷴾��󣬺�������������顣
	// ����Ҫע�����ַֽⷨ���õ������������ξ��󲢷�Ψһ��
	// �����ҵ�������ͬ ��һ�����������ξ��󣬴��������ξ������Ҳ��õ�ԭ����
	//---------------------------------------------------------
	bool LU(TMatrix& lhs, TMatrix& uhs) throw()
	{
		int i,j,k;

		// ���Ƿ���
		if(this->IsPhalanx())
			return false;

		// �������
		int stRank = m_nCol;

		TMatrix m(*this);
		lhs.SetDimension(stRank,stRank);
		uhs.SetDimension(stRank,stRank);
		for( k=0; k<stRank-1; k++)
		{
			// ��ԪΪ0,��������
			if( abs(m(k,k)) < 1.0e-12)
				return false;

			for( i=k+1; i<stRank; i++)
			{
				m(i,k) /= m(k,k);
			}

			for(i=k+1; i<stRank; i++)
			{
				for(j=k+1; j<stRank; j++)
				{
					m(i,j)=m(i,j)-m(i,k)*m(k,j);
				}
			}
		}

		//���ϡ���������ֵ
		for( i=0; i<stRank; i++)
		{
			for( j=0; j<i; j++)
			{
				lhs(i,j) = m(i,j);
				uhs(i,j) = 0.0;
			}

			lhs(i,i) = T(1);
			uhs(i,i) = m(i,i);

			for(j=i+1; j<stRank; j++)
			{
				lhs(i,j) = T(0);
				uhs(i,j) = m(i,j);
			}
		}

		return true;
	}
	//---------------------------------------------------------
	// Name:     QR
	// Function: ����ֽ� - QR
	// Parameters:
	//		[o] qhs - ����QR�ֽ���m*m��������Q
	//		[o] rhs - ����QR�ֽ���m*n�����Ǿ���R
	//
	// Return: 
	//		true - �ɹ�
	//		false - ʧ��
	//
	// Comment: ����ֽ� (decomposition, factorization)�ǽ�����
	//			���Ϊ��������ĳ˻����ɷ�Ϊ���Ƿֽ⡢���ȷֽ⡢
	//			QR�ֽ⡢Jordan�ֽ��SVD������ֵ���ֽ�ȣ�
	//			�����������֣�
	//				1)���Ƿֽⷨ (Triangular Factorization),�ֳ�LU�ֽ�
	//				2)QR �ֽⷨ (QR Factorization)
	//				3)����ֵ�ֽⷨ (Singular Value Decompostion)
	//
	// [QR�ֽⷨ]
	// �ǽ�����ֽ��һ�����������������������ξ���,���Գ�ΪQR�ֽⷨ,
	// ����������������ͨ�÷���Q�йء�
	//---------------------------------------------------------
	bool QR(TMatrix& qhs, TMatrix& rhs) throw()
	{
		int i,j,k;

		int stRow = m_nRow;	// ��������
		int stCol = m_nCol;	// ��������

		// �в���С����
		if(stRow < stCol)
			return false;

		rhs = TMatrix(*this);
		qhs.SetDimension(stRow,stRow);

		for( i=0; i<stRow; i++)
		{
			for( j=0; j<stRow; j++)
			{ 
				if(i==j)	qhs(i,j) = T(1);
				else		qhs(i,j) = T(0);
			}
		}

		int nn = stCol;
		if(stRow == stCol)
			nn = stRow - 1;

		for( k=0; k<nn; k++)
		{
			T u = T(0);
			for( i = k; i < stRow; i++)
			{ 
				T w = abs( rhs(i,k) );
				if(w > u) u = w;
			}

			T alpha = T(0);
			for(i = k; i < stRow; i++)
			{
				T t = rhs(i,k) / u;
				alpha = alpha + t * t;
			}

			if( rhs(k,k) > T(0) )
			{
				u = -u;
			}

			alpha = u * sqrt(alpha);
			if(abs(alpha) < 1.0e-12)
				return false;	// alpha == 0

			u = sqrt(T(2) * alpha * (alpha - rhs(k,k)));
			if( abs(u) > 1.0e-12)	//u>0.0
			{ 
				rhs(k,k) = (rhs(k,k) - alpha) / u;
				for(i=k+1; i<stRow; i++)
				{
            		rhs(i,k) /= u;
				}
				for( j=0; j<stRow; j++)
				{
					T t = T(0);
					for(int jj=k; jj<stRow; jj++)
					{
						t += rhs(jj,k) * qhs(jj,j);
					}

					for(i=k; i<stRow; i++)
					{
						qhs(i,j) -= T(2.0) * t * rhs(i,k);
					}
				}
				for(j=k+1; j<stCol; j++)
				{ 
					T t = T(0);
            
					for(int jj=k; jj<stRow; jj++)
					{
						t += rhs(jj,k) * rhs(jj,j);
					}

					for(i=k; i<stRow; i++)
					{
            			rhs(i,j) -= T(2.0) * t * rhs(i,k);
					}
				}

				rhs(k,k) = alpha;
				for(i=k+1; i<stRow; i++)
				{
				  rhs(i,k) = T(0);
				}
			}
		}

		for(i=0; i<stRow-1; i++)
		{
			for(j=i+1; j<stRow;j++)
			{
				swap(qhs(i,j), qhs(j,i));
			}
		}

		return true;
	}
	//---------------------------------------------------------
	// Name:     SVD
	// Function: ����ֽ� - SVD
	// Parameters:
	//			[o] u - (m*m) �����������U
	//			[o] v - (n*n) �����������V
	//			[o] s - (m*n) ��Խ������θ�������ֵ(�Էǵ�����������)������Ԫ��Ϊ0
	//
	// Return: 
	//		true - �ɹ�
	//		false - ʧ��
	//
	// Comment: ����ֽ� (decomposition, factorization)�ǽ�����
	//			���Ϊ��������ĳ˻����ɷ�Ϊ���Ƿֽ⡢���ȷֽ⡢
	//			QR�ֽ⡢Jordan�ֽ��SVD������ֵ���ֽ�ȣ�
	//			�����������֣�
	//				1)���Ƿֽⷨ (Triangular Factorization),�ֳ�LU�ֽ�
	//				2)QR �ֽⷨ (QR Factorization)
	//				3)����ֵ�ֽⷨ (Singular Value Decompostion)
	//
	// [SVD�ֽⷨ]
	// ����һ����������ֽⷨ��SVD����ɿ��ķֽⷨ����������QR�ֽ�
	// ��Ҫ���Ͻ�ʮ���ļ���ʱ�䡣[U,S,V]=svd(A)������U��V�������
	// �໥�������󣬶�S����һ�ԽǾ��� 
	// ��QR�ֽⷨ��ͬ�ߣ�ԭ����A����Ϊ��������
	// ʹ��SVD�ֽⷨ����;�ǽ���Сƽ����������ѹ����
	//---------------------------------------------------------
	bool SVD(TMatrix& uhs, TMatrix& vhs, TMatrix& shs) throw()
	{
		int loop = 60;			// ��������
		T eps = T(0.000001);	// ����Ҫ��

		int i,j;
		int kk,mm,nn,m1,ks,ka;
		int it(loop);
		T d,dd,t,sm,sm1,em1,sk,ek,b,c,shh;

		int m = m_nRow;
		int n = m_nCol;

		if(m<=0 || n<=0)
			return false;

		TMatrix matEmpty;		//�վ���
		shs = *this;
		uhs.SetDimension(m,m);	// ��������v�ĳߴ�
		vhs.SetDimension(n,n);	// ��������v�ĳߴ�

		for( j=0; j<m; j++)
		{
			uhs(j, m-1) = T(0);
		}

		if(m > n)
			ka = (int)m + 1;
		else
			ka = (int)n + 1;

		//---------------------------------------
		T* s	= new T[ka];
		T* e	= new T[ka];
		T* w	= new T[ka];
		T* fg	= new T[2];
		T* cs	= new T[2];
		//---------------------------------------

		int k = n;
		if(m-1<n)	k = m-1;

		int l = m;
		if(n-2<m)	l = n-2;

		if(l<0)		l = 0;

		int ll = k;
		if(l>k)		ll = l;

		if(ll>=1)
		{
			for(kk=1; kk<=ll; kk++)
			{
				if(kk<=k)
				{
					d = T(0);
					for(i=kk; i<=m; i++)
					{
						d += shs(i-1,kk-1) * shs(i-1,kk-1);
					}

					s[kk-1] = sqrt(d);

					if(abs(s[kk-1])>1.0e-12)	//s[kk-1] != 0
					{
						if(abs(shs(kk-1,kk-1))>1.0e-12)	//shs(kk-1,kk-1)!=0.0
						{
							s[kk-1] = abs(s[kk-1]);
							if(shs(kk-1,kk-1)<T(0))
								s[kk-1] = -s[kk-1];
						}

						for(i=kk; i<=m; i++)
						{
							shs(i-1,kk-1) = shs(i-1,kk-1) / s[kk-1];
						}

						shs(kk-1,kk-1) = T(1) + shs(kk-1,kk-1);
					}

					s[kk-1] = -s[kk-1];
				}// if(kk<=k)

				if(n>=kk+1)
				{
					for(j=kk+1; j<=n; j++)
					{
						if((kk<=k)&&(fabs(s[kk-1])>1.0e-12))	//s[kk-1]!=0.0
						{
							d = T(0);
							for(i=kk; i<=m; i++)
							{
								d = d + shs(i-1,kk-1) * shs(i-1,j-1);
							}

							d = -d / shs(kk-1,kk-1);

							for(i=kk; i<=m; i++)
							{
								shs(i-1,j-1) += d * shs(i-1,kk-1);
							}
						}

						e[j-1] = shs(kk-1,j-1);
					}
				}// if(n>=kk+1)

				if(kk<=k)
				{
					for(i=kk; i<=m; i++)
					{
						uhs(i-1,kk-1) = shs(i-1,kk-1);
					}
				}

				if(kk<=l)
				{ 
					d = T(0);
					for(i=kk+1; i<=n; i++)
					{
					  d += e[i-1] * e[i-1];
					}
					e[kk-1] = sqrt(d);

					if(abs(e[kk-1])>1.0e-12)	//e[kk-1]!=0.0
					{
						if(abs(e[kk])>1.0e-12)	//e[kk]!=0.0
						{
							e[kk-1] = abs(e[kk-1]);
							if(e[kk]<0.0)
							{
								e[kk-1] = -e[kk-1];
							}
						}

						for(i=kk+1; i<=n; i++)
						{
						  e[i-1] = e[i-1] / e[kk-1];
						}
						e[kk] = T(1) + e[kk];
					}
					e[kk-1]=-e[kk-1];

					if((kk+1<=m)&&(fabs(e[kk-1])>1.0e-12))	//e[kk-1]!=0.0
					{ 
						for(i=kk+1; i<=m; i++)
						{
							w[i-1] = T(0);
						}

						for(j=kk+1; j<=n; j++)
						{
						  for(i=kk+1; i<=m; i++)
						  {
							w[i-1] = w[i-1] + e[j-1] * shs(i-1,j-1);
						  }
						}

						for(j=kk+1; j<=n; j++)
						{
						  for(i=kk+1; i<=m; i++)
						  {
							  shs(i-1,j-1) -= w[i-1] * e[j-1] / e[kk];
						  }
						}
					}

					for(i=kk+1; i<=n; i++)
					{
						vhs(i-1,kk-1) = e[i-1];
					}

				}// if(kk<=l)
			}// next(kk)
		}// if(ll>=1)

		mm = n;
		if(m+1<n)	mm		= m + 1;
		if(k<n)		s[k]	= shs(k,k);
		if(m<mm)	s[mm-1] = T(0);
		if(l+1<mm)	e[l]	= shs(l,mm-1);

		e[mm-1] = T(0);
		nn = m;
		if(m>n) nn = n;

		if(nn>=k+1)
		{
			for(j=k+1; j<=nn; j++)
			{
				for(i=1; i<=m; i++)
				{
					uhs(i-1,j-1) = T(0);
				}
				uhs(j-1,j-1) = T(1);
			}
		}

		if(k>=1)
		{
			for(ll=1; ll<=k; ll++)
			{
				kk = k - ll + 1;
				if( abs(s[kk-1]) > 1.0e-12)
				{
					if(nn>=kk+1)
					{
						for(j=kk+1; j<=nn; j++)
						{
							d = T(0);
							for(i=kk; i<=m; i++)
							{
								d = d + uhs(i-1,kk-1) * uhs(i-1,j-1) / uhs(kk-1,kk-1);
							}

							d = -d;
							for(i=kk; i<=m; i++)
							{
								uhs(i-1,j-1) = uhs(i-1,j-1) + d * uhs(i-1,kk-1);
							}
						}
					}// if(nn>=kk+1)

					for(i=kk; i<=m; i++)
					{
						uhs(i-1,kk-1) = -uhs(i-1,kk-1);
					}
					uhs(kk-1,kk-1) = T(1) + uhs(kk-1,kk-1);

					if(kk-1>=1)
					{
						for(i=1; i<kk; i++)
						{
							uhs(i-1,kk-1) = T(0);
						}
					}
				}//if( abs(s[kk-1]) > 1.0e-12)
				else
				{ 
					for(i=1; i<=m; i++)
					{
						uhs(i-1,kk-1) = T(0);
					}
					uhs(kk-1,kk-1) = T(1);
				}
			}// next(ll)
		}// if(k>=1)

		for(ll=1; ll<=n; ll++)
		{
			kk = n - ll + 1; 
			if((kk<=l)&&(e[kk-1]!=0.0))
			{
				for(j=kk+1; j<=n; j++)
				{
					d = T(0);
					for(i=kk+1; i<=n; i++)
					{
						d += vhs(i-1,kk-1) * vhs(i-1,j-1) / vhs(kk,kk-1);
					}

					d = -d;
					for(i=kk+1; i<=n; i++)
					{
						vhs(i-1,j-1) = vhs(i-1,j-1) + d * vhs(i-1,kk-1);
					}
				}
			}// if((kk<=l)&&(e[kk-1]!=0.0))

			for(i=1; i<=n; i++)
			{
				vhs(i-1,kk-1) = T(0);
			}
			vhs(kk-1,kk-1) = T(1);

		}// next ll

		for(i=1; i<=m; i++)
		{
			for(j=1; j<=n; j++)
			{
				shs(i-1,j-1) = T(0);
			}
		}

		m1 = mm; 
		it = loop;	// ��������
		while(1)
		{
			if(mm==0)
			{
				msv_1(shs,e,s,vhs,(int)m,(int)n);
				//-----------
				delete[] s;
				delete[] e;
				delete[] w;
				delete[] fg;
				delete[] cs;
				//-----------
				return true;
			}

			kk = mm - 1;
			while((kk!=0)&&(fabs(e[kk-1])>1.0e-12))	//e[kk-1]!=0
			{
				d = abs(s[kk-1]) + abs(s[kk]);
				dd = abs(e[kk-1]);
				if(dd>eps*d)	kk = kk - 1;
				else			e[kk-1] = T(0);
			}

			if(kk==mm-1)
			{
				kk = kk + 1;
				if(s[kk-1]<T(0))
				{
					s[kk-1] = -s[kk-1];
					for(i=1; i<=n; i++)
					{
						vhs(i-1,kk-1)= -vhs(i-1,kk-1);
					}
				}

				while((kk!=m1)&&(s[kk-1]<s[kk]))
				{
					d = s[kk-1]; 
					s[kk-1] = s[kk];
					s[kk] = d;
					if(kk<n)
					{
						for(i=1; i<=n; i++)
						{
							d = vhs(i-1,kk-1);
							vhs(i-1,kk-1) = vhs(i-1,kk); 
							vhs(i-1,kk) = d;
						}
					}

					if(kk<m)
					{
						for(i=1; i<=m; i++)
						{ 
							d = uhs(i-1,kk-1);
							uhs(i-1,kk-1) = uhs(i-1,kk);
							uhs(i-1,kk) = d;
						}
					}
					kk = kk + 1;
				}// while((kk!=m1)&&(s[kk-1]<s[kk]))

				it = loop;
				mm = mm-1;

			}// if(kk==mm-1)
			else
			{ 
				ks=mm;
				while((ks>kk)&&(fabs(s[ks-1])!=0.0))
				{
					d = T(0);
					if(ks!=mm)		d += abs(e[ks-1]);
					if(ks!=kk+1)	d += abs(e[ks-2]);

					dd = abs(s[ks-1]);

					if(dd>eps*d)	ks = ks - 1;
					else			s[ks-1] = T(0);
				}

				if(ks==kk)
				{ 
					kk = kk + 1;
					d = abs(s[mm-1]);

					t = abs(s[mm-2]);
					if(t>d) d = t;

					t = abs(e[mm-2]);
					if(t>d) d = t;

					t = abs(s[kk-1]);
					if(t>d) d = t;

					t = abs(e[kk-1]);
					if(t>d) d = t;

					sm  = s[mm-1] / d;
					sm1 = s[mm-2] / d;
					em1 = e[mm-2] / d;
					sk  = s[kk-1] / d;
					ek  = e[kk-1] / d;

					b = ((sm1+sm) * (sm1-sm) + em1*em1) / T(2);
					c = sm * em1;
					c = c * c;
					shh = T(0);

					if((b!=T(0))||(c!=T(0)))
					{
						shh = sqrt(b * b + c);
						if(b<0.0) shh = -shh;
						shh = c / (b + shh);
					}

					fg[0] = (sk+sm) * (sk-sm) - shh;
					fg[1] = sk * ek;

					for(i=kk; i<=mm-1; i++)
					{
						msv_2(fg,cs);
						if(i!=kk) e[i-2] = fg[0];

						fg[0]  = cs[0] * s[i-1] + cs[1] * e[i-1];
						e[i-1] = cs[0] * e[i-1] - cs[1] * s[i-1];
						fg[1]  = cs[1] * s[i];
						s[i]   = cs[0] * s[i];
						if((cs[0]!=1.0)||(cs[1]!=0.0))
						{
							for(j=1; j<=n; j++)
							{
								d = cs[0] * vhs(j-1,i-1) + cs[1] * vhs(j-1,i);
								vhs(j-1,i) = -cs[1] * vhs(j-1,i-1) + cs[0] * vhs(j-1,i);
								vhs(j-1,i-1) = d;
							}
						}

						msv_2(fg,cs);
						s[i-1] = fg[0];
						fg[0]  = cs[0] * e[i-1] + cs[1] * s[i];
						s[i]   =-cs[1] * e[i-1] + cs[0] * s[i];
						fg[1]  = cs[1] * e[i];
						e[i]   = cs[0] * e[i];

						if(i<m)
						{
							if((cs[0]!=T(1.0))||(cs[1]!=T(0.0)))
							{
								for(j=1; j<=m; j++)
								{ 
									d = cs[0] * uhs(j-1,i-1) + cs[1] * uhs(j-1,i);
									uhs(j-1,i) = -cs[1] * uhs(j-1,i-1) + cs[0] * uhs(j-1,i);
									uhs(j-1,i-1) = d;
								}
							}
						}

					}// next i

					e[mm-2] = fg[0];
					it = it-1;

				}// if(ks==kk)
				else
				{ 
					if(ks==mm)
					{
						kk = kk + 1;
						fg[1] = e[mm-2]; 
						e[mm-2] = T(0);
						for(ll=kk; ll<=mm-1; ll++)
						{
							i = mm + kk - ll - 1;
							fg[0] = s[i-1];
							msv_2(fg,cs);
							s[i-1] = fg[0];

							if(i!=kk)
							{
								fg[1]  = -cs[1] * e[i-2];
								e[i-2] =  cs[0] * e[i-2];
							}

							if((cs[0]!=1.0)||(cs[1]!=0.0))
							{
								for(j=1; j<=n; j++)
								{ 
									d = cs[0] * vhs(j-1,i-1) + cs[1] * vhs(j-1,mm-1);
									vhs(j-1,mm-1) = -cs[1] * vhs(j-1,i-1) + cs[0] * vhs(j-1,mm-1);
									vhs(j-1,i-1) = d;
								}
							}
						}
					}//if(ks==mm)
					else
					{ 
						kk = ks + 1;
						fg[1] = e[kk-2];
						e[kk-2] = T(0);

						for(i=kk; i<=mm; i++)
						{
							fg[0] = s[i-1];
							msv_2(fg,cs);
							s[i-1] = fg[0];
							fg[1] = -cs[1] * e[i-1];
							e[i-1] = cs[0] * e[i-1];

							if((cs[0]!=1.0)||(cs[1]!=0.0))
							{
								for(j=1; j<=m; j++)
								{
									d = cs[0] * uhs(j-1,i-1) + cs[1] * uhs(j-1,kk-2);
									uhs(j-1,kk-2) = -cs[1] * uhs(j-1,i-1) + cs[0] * uhs(j-1,kk-2);
									uhs(j-1,i-1) = d;
								}
							}
						}//for(i=kk; i<=mm; i++)
					}
				}

			}// else - if(kk==mm-1)

		}// while(1)

		//---------------------------------------
		delete[] s;
		delete[] e;
		delete[] w;
		delete[] fg;
		delete[] cs;
		//---------------------------------------

		return true;
	}
	//---------------------------------------------------------
	// Name:     SVD_GI
	// Function: ����A(m*n)�Ĺ����������ֵ�ֽ� - SVD_GI
	// Parameters:
	//			[o] ap - (n*m) ���A�Ĺ�����A+ 
	//			[o] u - (m*m) �����������U
	//			[o] v - (n*n) �����������Vt
	//			[o] s - (m*n) �Խ������θ�������ֵ(�Էǵ�����������)������Ԫ��Ϊ0
	//
	// Return: 
	//		true - �ɹ�
	//		false - ʧ��
	//
	// Comment: Singular Value Decompostion Generalized Inversion
	//---------------------------------------------------------
	bool SVD_GI(TMatrix& ap, TMatrix& u, TMatrix& v, TMatrix& s) throw()
	{
		int i, j, k(0);
		int l, ka;

		int m = m_nRow;
		int n = m_nCol;
		T eps = T(0.000001);	// ����Ҫ��

		if(m > n)
			ka = m + 1;
		else
			ka = n + 1;

		//TMatrix mat = TMatrix(*this);
		////��������ap,u,v�ĳߴ�
		//ap.SetDimension(n,m);
		//u.SetDimension(m,m);
		//v.SetDimension(n,n);
		//s.SetDimension(m,n);

		if(!this->SVD(u,v,s))
			return false;

		j = (m < n) ? m : n;
		j -= 1;

		while((k<=j)&&(abs(s(k,k))>1.0e-12))	// s(k,k)!=0
		{
			k = k + 1;
		}

		k = k - 1;

		for( i=0; i<n; i++)
		{
			for(j=0; j<m; j++)
			{
				ap(i,j) = T(0);
				for(l=0; l<=k; l++)
				{
					ap(i,j) = ap(i,j) + v(l,i) * u(j,l) / s(l,l);
				}
			}
		}

		return true;
	}
	//---------------------------------------------------------
	// Name:     HessenbergTransform
	// Function: ��һ��ʵ����(n*n)��Ϊ���겮����(��H��)
	// Parameters:
	//		[o] h - ���겮����
	// Return: true/false
	// Comment: 
	//---------------------------------------------------------
	bool HessenbergTransform(TMatrix& h) throw()
	{
		int i,j,k;

		// �����Ƿ���
		if(!this->IsPhalanx())
			return false;

		// �������
		int stRank = m_nRow;

		T t;
		h = TMatrix(*this);
		for( k=1; k<stRank-1; k++)
		{
			T d = T(0);
			for( j=k; j<stRank; j++)
			{ 
				t = h(j,k-1);
				if(abs(t) > abs(d))
				{
					d = t; i = j;
				}
			}

			if(abs(d)>1.0e-12)	// d != 0
			{
				if(i!=k)
				{
					for(j=k-1; j<stRank; j++)	//����i k ��
						swap(h(i,j), h(k,j));
					for(j=0; j<stRank; j++)		//����i k ��
						swap(h(j,i), h(j,k));
				}

				for(i=k+1; i<stRank; i++)
				{ 
					t = h(i,k-1) / d; 
					h(i,k-1) = T(0);

					for(j=k; j<stRank; j++)
						h(i,j) = h(i,j) - t * h(k,j);
					for(j=0; j<stRank; j++)
						h(j,k) = h(j,k) + t * h(j,i);
				}
			}//if(abs(d)>1.0e-12)	// d != 0
		}//next k

		return true;
	}
	//---------------------------------------------------------
	// Name:     HouseholderTransform
	// Function: �ú�˹�ɶ���(Householder)�任����n��ʵ�Գ���Լ��
	//			 Ϊ�Գ����Խ���
	// Parameters:
	//		[o] q - n*n ���غ�˹�ɶ��±任�ĳ˻�����Q
	//		[o] b - nά����ָ��,���ضԳ����Խ�������Խ���Ԫ��
	//		[o] c - nά����ָ��,���ضԳ����Խ���ĴζԽ���Ԫ��
	// Return: true/false
	// Comment: 
	//---------------------------------------------------------
	bool HouseholderTransform(TMatrix& q, T* b, T* c) throw()
	{
		// �����ǶԳ���
		if(!IsSymmetry())
			return false;

		int i,j, k;
		T h, f, g, h2;

		// �������
		int stRank = m_nRow;

		q = *this;
		for( i=stRank-1; i>=1; i--)
		{
			h = T(0);
			if(i>1)
			{
			  for(k=0; k<i; k++) h += q(i,k) * q(i,k);
			}

			if(abs(h)<1.0e-12)		//h == 0
			{
				c[i] = T(0.0);
				if(i==1) c[i] = q(i,i-1);
				b[i] = T(0.0);
			}
			else
			{
				c[i] = sqrt(h);
				if(q(i,i-1)>0.0) c[i] = -c[i];
				h -= q(i,i-1) * c[i];
				q(i,i-1) = q(i,i-1) - c[i];
				f = T(0.0);

				for(j=0; j<i; j++)
				{ 
					q(j,i) /= h;
					g = T(0.0);
					for(k=0; k<=j; k++) g += q(j,k) * q(i,k);
					if(j+1<i)
					{
					  for(k=j+1; k<i; k++) g += q(k,j) * q(i,k);
					}
					c[j] = g / h;
					f = f + g * q(j,i);
				}

				h2 = f / (h+h);
				for(j=0; j<i; j++)
				{ 
					f = q(i,j);
					g = c[j] - h2 * f;
					c[j] = g;
					for(k=0; k<=j; k++)
						q(j,k) = q(j,k)- f * c[k] - g * q(i,k);
				}
				b[i]=h;
			}
		}//next i

		for(i=0; i<stRank-1; i++)
		{
			c[i] = c[i+1];
		}

		c[stRank-1] = T(0.0);
		b[0] = T(0.0);
		for(i=0; i<stRank; i++)
		{
			if((b[i]!=0.0)&&(i-1>=0))
			{
				for(j=0; j<i; j++)
				{ 
					g = T(0.0);
					for(k=0; k<i; k++) g += q(i,k) * q(k,j);
					for(k=0; k<i; k++) q(k,j) = q(k,j)- g * q(k,i);
				}
			}

			b[i] = q(i,i);
			q(i,i) = T(1.0);
			if(i-1>=0)
			{
				for(j=0; j<i; j++)
				{ 
					q(i,j) = T(0.0);
					q(j,i) = T(0.0);
				}
			}
		}

		return true;
	}
	//---------------------------------------------------------
	// Name:     EVV_Jacobi
	// Function: ʵ�Գ���(n*n)����ֵ�����������ſɱȷ�
	// Parameters:
	//		[o] lmd - n*n ���Խ��ߴ��n������ֵ
	//		[o] v - n*n �������������,j��������������lmd(j,j)������ֵ��Ӧ
	// Return: true/false
	// Comment: 
	//---------------------------------------------------------
	bool EVV_Jacobi(TMatrix& lmd, TMatrix& v) throw()
	{
		// ���ǶԳ���
		if(!IsSymmetry())
			return false;

		int i, j, p, q, l(1);
		T fm,cn,sn,omega,x,y,d;
		int stRank = m_nCol;		// �������

		T eps = (T)1.0e-6;
		int loop = 60;

		lmd = *this;
		v.SetDimension(stRank,stRank);
		for( i=0; i<stRank; i++)
		{
			v(i,i) = T(1);
			for(j=0; j<stRank; j++)
			{
				if(i!=j) v(i,j) = T(0);
			}
		}

		while(1)
		{
			fm = T(0);
			for(i=1; i<stRank; i++)
			{
				for(j=0; j<i; j++)
				{
					d = abs(lmd(i,j));
					if((i!=j)&&(d>fm))
					{
						fm = d; p = i; q = j;
					}
				}
			}

			if(fm<eps)
				return true;
			if((int)l>loop)
				return false;

			l = l+1;
			x = -lmd(p,q);
			y = (lmd(q,q) - lmd(p,p)) / T(2);
			omega = x / sqrt(x*x+y*y);
			if(y<0.0) omega = -omega;
			sn = T(1) + sqrt(T(1) - omega * omega);
			sn = omega / sqrt(T(2) * sn);
			cn = sqrt(T(1) - sn * sn);
			fm = lmd(p,p);
			lmd(p,p) = fm*cn*cn + lmd(q,q)*sn*sn + lmd(p,q)*omega;
			lmd(q,q) = fm*sn*sn + lmd(q,q)*cn*cn - lmd(p,q)*omega;
			lmd(p,q) = T(0); 
			lmd(q,p) = T(0);

			for(j=0; j<stRank; j++)
			{
				if((j!=p)&&(j!=q))
				{
					fm = lmd(p,j);
					lmd(p,j) = fm*cn + lmd(q,j)*sn;
					lmd(q,j) =-fm*sn + lmd(q,j)*cn;
				}
			}
			for(i=0; i<stRank; i++)
			{
				if((i!=p)&&(i!=q))
				{ 
					fm = lmd(i,p);
					lmd(i,p) = fm*cn + lmd(i,q)*sn;
					lmd(i,q) =-fm*sn + lmd(i,q)*cn;
				}
			}
			for(i=0; i<stRank; i++)
			{
				fm = v(i,p);
				v(i,p) = fm*cn + v(i,q)*sn;
				v(i,q) =-fm*sn + v(i,q)*cn;
			}

		}// while(1)

		return true;
	}
	//---------------------------------------------------------
	// Name:     EVV_JacobiBarrier
	// Function: ʵ�Գ�������ֵ(n*n)�����������ſɱȹ��ط�
	// Parameters:
	//		[o] lmd - n*n ���Խ��ߴ��n������ֵ
	//		[o] v - n*n �������������,j��������������lmd(j,j)������ֵ��Ӧ
	// Return: true/false
	// Comment: 
	//---------------------------------------------------------
	bool EVV_JacobiBarrier(TMatrix& lmd, TMatrix& v) throw()
	{
		// ���Գ�
		if(!IsSymmetry())
			return false;

		int i,j, p, q, stRank;
		T fm, cn, sn, omega, x, y, d;

		T eps = (T)1.0e-6;

		stRank = m_nRow;		//�������
		lmd = *this;
		v.SetDimension(stRank,stRank);
		for( i=0; i<stRank; i++)
		{ 
			v(i,i) = T(1);
			for(j=0; j<stRank; j++)
			{
			  if(i!=j) v(i,j) = T(0);
			}
		}

		T ff(0);
		for(i=1; i<stRank; i++)
		{
			for(j=0; j<i; j++)
			{
				d = lmd(i,j);
				ff += d*d;
			}
		}

		ff = sqrt(ff*T(2));

loop0:
		ff = ff / stRank;

loop1:
		for(i=1; i<stRank; i++)
		{
			for(j=0; j<i; j++)
			{ 
				d = abs(lmd(i,j));
				if(d>ff)
				{ 
					p=i;
					q=j;
					goto loop;
				}
			}
		}
		if(ff<eps) return true;	//�ɹ�����
		goto loop0;

loop:
		x = -lmd(p,q);
		y = (lmd(q,q) - lmd(p,p)) / T(2);
		omega = x / sqrt(x*x + y*y);
		if(y<T(0)) omega = -omega;
		sn = T(1) + sqrt(T(1) - omega*omega);
		sn = omega / sqrt(T(2)*sn);
		cn = sqrt(T(1)-sn*sn);
		fm = lmd(p,p);
		lmd(p,p) = fm*cn*cn + lmd(q,q)*sn*sn + lmd(p,q)*omega;
		lmd(q,q) = fm*sn*sn + lmd(q,q)*cn*cn - lmd(p,q)*omega;
		lmd(p,q) = T(0);
		lmd(q,p) = T(0);

		for(j=0; j<stRank; j++)
		{
			if((j!=p)&&(j!=q))
			{
				fm = lmd(p,j);
				lmd(p,j) = fm*cn + lmd(q,j)*sn;
				lmd(q,j) =-fm*sn + lmd(q,j)*cn;
			}
		}

		for(i=0; i<stRank; i++)
		{
			if((i!=p)&&(i!=q))
			{
				fm = lmd(i,p);
				lmd(i,p) = fm*cn + lmd(i,q)*sn;
				lmd(i,q) =-fm*sn + lmd(i,q)*cn;
			}
		}

		for(i=0; i<stRank; i++)
		{ 
			fm = v(i,p);
			v(i,p) = fm*cn + v(i,q)*sn;
			v(i,q) =-fm*sn + v(i,q)*cn;
		}
		goto loop1;

		return true;
	}
	//---------------------------------------------------------
	// Name:     EVV_RealSymmetryTriangleQR
	// Function: ʵ�Գ����Խ���ȫ������ֵ����������QR��
	// Parameters:
	//		[i] b - ����ָ��,���n�׶Գ����Խ�������Խ���Ԫ��
	//		[i] c - ����ָ��,���n�׶Գ����Խ���ĴζԽ���Ԫ��
	//		[i,o] q - n*n 
	//		    ����ŵ�λ����,�򷵻�ʱ���n�ױ�ʵ�Գ����Խ��������������
	//          �������MatrixTransformHouseholder()���ɵ�ʵ�Գ���A�ĳ˻�����Q,
	//          �򷵻�ʱ���ʵ�Գ���A������������
	//          ����,q�ĵ�j��Ϊ������b�е�j������ֵ��Ӧ����������
	// Return: true/false
	// Comment: 
	//---------------------------------------------------------
	bool EVV_RealSymmetryTriangleQR(T* b,T* c, TMatrix& q) throw()
	{
		if(q.IsPhalanx())
			return false;	//qӦ��n*n����

		int i,j, k, m, it, stRank;
		T h, g, p, r, e, s, d(0), f(0);

		int loop = 60;
		T eps = (T)1.0e-6;

		stRank = m_nCol;		// �������

		c[stRank-1] = T(0);
		for( j=0; j<stRank; j++)
		{
			it = 0;
			h = eps*(abs(b[j])+fabs(c[j]));
			if(h>d) d = h;
			m = j;
			while((m<stRank)&&(abs(c[m])>d)) m++;

			if(m!=j)
			{
				do
				{
					// ����,��������������û�дﵽ����Ҫ��
					if(it==loop)
						return false;

					it++;
					g = b[j];
					p = (b[j+1]-g) / (T(2)*c[j]);
					r = sqrt(p*p+T(1));

					if(p>0.0 || fabs(p)<1.0e-12)	//p==0
						b[j] = c[j] / (p+r);
					else
						b[j] = c[j] / (p-r);

					h = g - b[j];
					for(i=j+1; i<stRank; i++)
						b[i] = b[i] - h;

					f = f + h; 
					p = b[m]; 
					e = T(1); 
					s = T(0);

					for(i=m-1; i>=j; i--)
					{
						g = e * c[i];
						h = e * p;

						if(abs(p)>=abs(c[i]))
						{
							e = c[i] / p;
							r = sqrt(e*e+T(1));
							c[i+1] = s*p*r;
							s = e / r; 
							e = T(1) / r;
						}
						else
						{
							e = p / c[i]; 
							r = sqrt(e*e+T(1));
							c[i+1] = s * c[i] * r;
							s = T(1) / r; 
							e = e / r;
						}
					}

					for(i=m-1; i>=j; i--)
					{
						p = e * b[i] - s * g;
						b[i+1] = h + s * (e*g + s*b[i]);
						for(k=0; k<stRank; k++)
						{ 
							h = q(k,i+1);
							q(k,i+1) = s*q(k,i) + e*h;
							q(k,i) = e*q(k,i) - s*h;
						}
					}

					c[j] = s*p;
					b[j] = e*p;

				}while(abs(c[j])>d);

			}//if(m!=j)

			b[j] = b[j] + f;

		}// next j

		for(i=0; i<stRank; i++)
		{
			k = i;
			p = b[i];

			if(i+1<stRank)
			{
				j = i + 1;
				while((j<stRank)&&(b[j]<=p))
				{
					k = j;
					p = b[j];
					j = j+1;
				}
			}

			if(k!=i)
			{ 
				b[k] = b[i]; 
				b[i] = p;
				for(j=0; j<stRank; j++)
				{
					p = q(j,i);
					q(j,i) = q(j,k);
					q(j,k) = p;
				}
			}

		}//next i

		return true;
	}

public:
	// ���������� ()
	T& operator ()(int nRow, int nCol) const throw(KException)
	{
		if(nCol <0 || nCol >= m_nCol || nRow < 0 || nRow >= m_nRow)
		{
			TCHAR buffer[MAX_PATH];
			_stprintf(buffer, _T("Index out of bound : row %d, col %d"), nRow, nCol);
			throw KException(buffer);
		}

		return m_pData[nCol + nRow * m_nCol];
	}
	//�Ծ��󸺺�(-)
	TMatrix& operator -() throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] = 0 - m_pData[i];
		}
		return *this;
	}

	// ���������� +
	TMatrix& operator +(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] = m_pData[i] + value;
		}
		return *this;
	}
	TMatrix& operator +(const TMatrix& rhs) throw(KException)
	{
		if(m_nCol != rhs.GetWidth() || m_nRow != rhs.GetHeight())
		{
			throw KException(_T("Different dimension in argument"));
		}

		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] = m_pData[i] + rhs.m_pData[i];
		}
		return *this;
	}

	// ���������� -
	TMatrix& operator -(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalByte; i++)
		{
			m_pData[i] = m_pData[i] - value;
		}
		return *this;
	}
	TMatrix& operator -(const TMatrix& rhs) throw(KException)
	{
		if(m_nCol != rhs.GetWidth() || m_nRow != rhs.GetHeight())
		{
			throw KException(_T("Different dimension in argument"));
		}

		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] = m_pData[i] - rhs.m_pData[i];
		}

		return *this;
	}

	// ���������� +=
	TMatrix& operator +=(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] += value;
		}
		return *this;
	}
	TMatrix& operator +=(const TMatrix& rhs) throw(KException)
	{
		if(m_nCol != rhs.GetWidth() || m_nRow != rhs.GetHeight())
		{
			throw KException(_T("Different dimension in argument"));
		}

		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] += rhs.m_pData[i];
		}

		return *this;
	}

	// ���������� -=
	TMatrix& operator -=(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] -= value;
		}
		return *this;
	}
	TMatrix& operator -=(const TMatrix& rhs) throw(KException)
	{
		if(m_nCol != rhs.GetWidth() || m_nRow != rhs.GetHeight())
		{
			throw KException(_T("Different dimension in argument"));
		}

		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] -= rhs.m_pData[i];
		}

		return *this;
	}

	// ���������� *
	TMatrix& operator *(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] = m_pData[i] * value;
		}
		return *this;
	}
	TMatrix& operator *(const TMatrix& rhs) throw(KException)
	{
		KMatrix lhs(*this);
		MatrixMultiply(lhs,rhs,*this);
		return *this;
	}

	// ���������� *=
	TMatrix& operator *=(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] *= value;
		}
		return *this;
	}
	TMatrix& operator *=(const TMatrix& rhs) throw(KException)
	{
		KMatrix lhs(*this);
		MatrixMultiply(lhs,rhs,*this);
		return *this;
	}

	// ���������� /
	TMatrix& operator /(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] = m_pData[i] / value;
		}
		return *this;
	}
	//TMatrix& operator /(const TMatrix& rhs) throw(KException); // �൱�� * �����

	// ���������� /=
	TMatrix& operator /=(const T value) throw()
	{
		int nTotalSize = m_nCol * m_nRow;
		for(int i=0; i<nTotalSize; i++)
		{
			m_pData[i] /= value;
		}
		return *this;
	}
	TMatrix& operator /=(const TMatrix& rhs) throw(KException)
	{
	}

	// ���������� ==
	bool operator ==(const TMatrix& rhs) const throw()
	{
		if(m_nRow != rhs.m_nRow)
			return false;

		if(m_nCol != rhs.m_nCol)
			return false;

		for(size_t i=0; i<m_nRow*m_nCol; i++)
		{
			if(m_pData[i] != rhs.m_pData[i])
				return false;
		}

		return true;
	}
	// ���������� !=
	bool operator !=(const TMatrix& rhs) const throw()
	{
		if(m_nRow != rhs.m_nRow)
			return true;

		if(m_nCol != rhs.m_nCol)
			return true;

		for(size_t i=0; i<m_nRow*m_nCol; i++)
		{
			if(m_pData[i] != rhs.m_pData[i])
				return true;
		}

		return false;
	}

public:
private:
	// ��ֵ����
	void swap(T& t1, T& t2) const throw()
	{
		T temp = t1;
		t1 = t2;
		t2 = temp;
	}
	// ȫѡ��Ԫ��˹-Լ��(Gauss-Jordan)���������
	TMatrix& Inversion_gs() throw()
	{
		size_t i,j,k;

		// rhs���Ƿ���
		if(!this->IsPhalanx())
		{
			this->Empty();
			return *this;
		}

		size_t stRank = m_nRow;	// �������
		size_t* is = new size_t[stRank];	//�н�����Ϣ
		size_t* js = new size_t[stRank];	//�н�����Ϣ

		TMatrix m(*this);					//����һmatrix����
		T maxValue(0), tmp(0);
		for(k = 0; k < stRank; k++) // ȫѡ��Ԫ
		{
			maxValue = T(0);
			for(i = k; i < stRank; i ++)
			{
				for(j = k; j < stRank; j ++)
				{
					tmp = abs(m(i, j));	//��m(i,j)����ֵ 
					if(tmp > maxValue)				//��Ԫ���ڶԽ�����
					{
						maxValue = tmp;
						is[k] = i;					//��¼��Ԫ����
						js[k] = j;					//��¼��Ԫ����
					}
				}
			}

			// ��ԪΪ0����������
			if(abs(maxValue)<1.0e-12)
			{
				this->Empty();
				return *this;
			}

			// ��Ԫ���ڵ�ǰ��
			if(is[k] != k)
			{
				// ������Ԫ��
				for(j = 0; j < stRank; j ++)
				{
					swap(m(k, j), m(is[k], j));
				}
			}

			// ��Ԫ���ڵ�ǰ��
			if(js[k] != k)
			{
				// ������Ԫ��
				for(i = 0; i < stRank; i ++)
				{
					swap(m(i, k), m(i, js[k]));
				}
			}

			// ��Ԫ����
			m(k, k) = T(1) / m(k, k);
		}

		// ����
		for(k = 0; k < stRank; k++)
		{
			for( j = 0; j < stRank; j ++)
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
					swap(m(r, j), m(js[r], j));
			if((int)is[r] != r)
				for(size_t i = 0; i < stRank; i ++)
					swap(m(i, r), m(i, is[r]));
		}


		*this = m;

		delete[] is;
		delete[] js;

		return *this;
	}
	// �á�����ѭ�����±�ŷ�����Գ�����������
	TMatrix& Inversion_SymmetryRegular() throw()
	{
		size_t i,j,k;
		TMatrix mInversion;		//����һ�������

		// ���ǶԳ�������
		if(!this->IsSymmetryRegular())
		{
			this->Empty();
			return *this;
		}

		size_t stRank = m_nRow;	// �������
		TMatrix msr(*this);			//����һmatrix������this��ʼ��

		T *b = new T[stRank];
		T w,g;
		for(k=0; k<stRank;k++)
		{
			w = msr(0,0);
			size_t m = stRank - k -1;
			for(i = 1; i < stRank; i++)
			{
				g = msr(i, 0);
				b[i] = g / w;

				if(i <= m)
					b[i] = -b[i];
			}

			for(i = 1; i < stRank; i++)
			{
				for( j = 1; j <= i; j ++)
				{
					msr((i-1),(j-1)) = msr(i, j) + g * b[j];
				}
			}
		}

		for(k=0; k<stRank;k++)
		{
			msr(stRank-1, stRank-1) = T(1) / w;
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

		*this = msr;

		delete[] b;
		return *this;
	}
	// ������(Trench)�����в�����(Toeplitz)������
	TMatrix& Inversion_ToeplitzTrench() throw()
	{
		size_t i,j,k;

		// �����в�����
		if(!this->IsToeplitz())
		{
			this->Empty();
			return *this;
		}

		TMatrix rhs(*this);
		size_t stRank = rhs.Cols();	// �������

		T* t	= new T[stRank];
		T* tuo	= new T[stRank];

		tuo[0] = T(0);
		for(size_t n=0;n<stRank;n++)
		{
			t[n]=rhs(0,n);
			if(n>0) tuo[n] = rhs(n,0);
		}

		T* c = new T[stRank];
		T* p = new T[stRank];
		T* r = new T[stRank];

		T a		= t[0]; 
		c[0]	= tuo[1]/t[0];
		r[0]	= t[1]/t[0];

		TMatrix b(rhs);
		for( k=0; k<=stRank-3; k++)
		{
			T s = T(0);
			for( j=1; j<=k+1; j++)
			{
				s = s + c[k+1-j] * tuo[j];
			}

			s = (s - tuo[k+2]) / a;

			for( i=0; i<=k; i++)
			{
				p[i] = c[i] + s * r[k-i];
			}

			c[k+1] = -s;

			s = T(0);

			for(j=1; j<=k+1; j++)
			{
				s = s + r[k+1-j] * t[j];
			}

			s = (s - t[k+2]) / a;
			for(i=0; i<=k; i++)
			{
				r[i] = r[i] + s * c[k-i];
				c[k-i] = p[k-i];
			}
			r[k+1] = -s;
			a = T(0);
			for(j=1; j<=k+2; j++)
			{
				a = a + t[j] * c[j-1];
			}
			a = t[0] - a;

			if(abs(a)<1.0e-12)	//a==0
			{
				delete[] t;
				delete[] tuo;

				delete[] c;
				delete[] p;
				delete[] r;

				this->Empty();
				return *this;	
			}

		}// next k

		b(0,0) = T(1) / a;
		for( i=0; i<stRank-1; i++)
		{	
			//k = i+1;
			b(0, i+1) = -r[i] / a;
			b(i+1, 0) = -c[i] / a;
		}

		for(i=0; i<stRank-1; i++)
		{
			for( j=0; j<stRank-1; j++)
			{
				b(i+1, j+1) = b(i,j) - c[i] * b(0,j+1) + c[stRank-j-2] * b(0,stRank-i-1);
			}
		}

		*this = b;

		delete[] t;
		delete[] tuo;
		delete[] c;
		delete[] p;
		delete[] r;

		return *this;
	}

	// һ��ʵ���������ֵ�ֽ⸨������
	void msv_1(TMatrix& a, T* e, T* s, TMatrix& v, int m, int n) throw()
	{
		int i,j;
		T d;

		if(m>=n)	i = n;
		else		i = m;

		for(j=1; j<i; j++)
		{
			a(j-1,j-1) = s[j-1];
			a(j-1,j) = e[j-1];
		}

		a(i-1,i-1) = s[i-1];
		if(m<n)
			a(i-1,i) = e[i-1];

		for(i=1; i<n; i++)
		{
			for(j=i+1; j<=n; j++)
			{
				d = v(i-1,j-1);
				v(i-1,j-1) = v(j-1,i-1); 
				v(j-1,i-1) = d;
			}
		}

		// FUN_END
	}
	//һ��ʵ���������ֵ�ֽ⸨������
	void msv_2(T* fg, T* cs) throw()
	{
		T r,d;

		r = abs(fg[0]) + abs(fg[1]);
		if(abs(r)<1.0e-12)	// r==0.0
		{
			cs[0] = T(1);
			cs[1] = T(0);
			d     = T(0);
		}
		else
		{
			d = sqrt(fg[0] * fg[0] + fg[1] * fg[1]);
			if( abs(fg[0]) > abs(fg[1]) )
			{
				d = abs(d);
				if(fg[0]<0.0) d = -d;
			}

			if(abs(fg[1])>abs(fg[0]) || abs(abs(fg[1])-abs(fg[0]))<1.0e-12	)
			{ 
				d = abs(d);
				if(fg[1]<0.0) d = -d;
			}

			cs[0] = fg[0] / d; 
			cs[1] = fg[1] / d;
		}

		r = T(1);
		if(abs(fg[0])>abs(fg[1]))
		{
			r = cs[1];
		}
		else if(abs(cs[0])>1.0e-12)	//cs[0] != 0
		{
			r = T(1) / cs[0];
		}

		fg[0] = d;
		fg[1] = r;

		// FUN_END
	}

protected:
	T* m_pData;
	int m_nCol;
	int m_nRow;

public:
	// �����
	static void MatrixMultiply(const TMatrix& lhs, const TMatrix& rhs, TMatrix& out)  throw(KException)
	{
		// ���� == ����
		// �϶���߾�����������ұ߾�����������
		if(lhs.Cols() != rhs.Rows())
		{
			throw KException(_T("Dimension error."));
		}

		// ���ɾ����¶���
		// ��lhs������Ϊ���������
		// ��rhs��������Ϊ���������
		out.SetDimension(lhs.Rows(), rhs.Cols());

		for(size_t i = 0; i < lhs.Rows(); i ++)			// ���������
		{
			for(size_t j = 0; j < rhs.Cols(); j ++)		// �Ҿ�������
			{
				mOut(i, j) = T(0);		//����ֵ0
				for(size_t k = 0; k < lhs.Cols(); k ++)	//����
				{
					out(i, j) += lhs(i, k) * rhs(k, j);
				}
			}
		}

		return;
	}

};

///=============================================================================

///=============================================================================
