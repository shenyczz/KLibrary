/******************************************************************************
 ** T3DMemAllocator.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: T3DMemAllocator - 三维内存分配器类模板
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

	//1. Declare Template Class
		typedef T3DMemAllocator <float>	CFloat3DmemAllocator;
	//2. Allocate Memory
		CFloat3DmemAllocator::AllocMemory3D(_Ty ** &ppp,int nx,int ny,int nz);
	//3. Free Memory
		CFloat3DmemAllocator::FreeMemory3D(_Ty **&ppp);

******************************************************************************/

#ifndef __INCLUDED_TEMPLATE_3DMEMALLOCATOR_H__
#define __INCLUDED_TEMPLATE_3DMEMALLOCATOR_H__

template <class _Ty>
class T3DMemAllocator
{
public:
	T3DMemAllocator()
		:m_pv(0)
		,m_lays(0)
		,m_rows(0)
		,m_cols(0) 
	{
	}

	virtual ~T3DMemAllocator()
	{
		_deletea(m_pv);
	}

public:
	bool AllocMemory3D(int lays,int rows,int cols)
	{
		m_pv = new _Ty [lays*rows*cols];
		if(!m_pv)
			return false;

		memset(m_pv,0,sizeof(_Ty)*lays*rows*cols);

		m_lays = lays;
		m_rows = rows;
		m_cols = cols;

		return true;
	}

	void FreeMemory3D()
	{
		_deletea(m_pv);
	}

public:
	_Ty* GetData(){ return m_pv; }

public:
	//[操作符重载] -> 获取元素(i,j,k)的值,左值函数(可写入)
	_Ty& operator()(int lay,int row, int col)
	{
		if(lay<0 || lay>=m_lays)
			return *m_pv;

		if(row<0 || row>=m_rows)
			return *m_pv;

		if(col<0 || col>=m_cols)
			return *m_pv;

		return *( m_pv + (lay*m_rows+row)*m_cols+col );
	}

	//[操作符重载] -> 获取元素(i,j,k)的值,右值函数(只读)
	const _Ty operator()(int lay,int row, int col) const
	{
		if(lay<0 || lay>=m_lays)
			return *m_pv;

		if(row<0 || row>=m_rows)
			return *m_pv;

		if(col<0 || col>=m_cols)
			return *m_pv;

		return *( m_pv + (lay*m_rows+row)*m_cols+col );
	}

private:
	_Ty * m_pv;
	int m_lays;
	int m_rows;
	int m_cols;
};

typedef T3DMemAllocator<BYTE>		KByte3DMemAllocator;
typedef T3DMemAllocator<WORD>		KWord3DMemAllocator;
typedef T3DMemAllocator<SHORT>		KShort3DMemAllocator;
typedef T3DMemAllocator<LONG>		KLong3DMemAllocator;
typedef T3DMemAllocator<FLOAT>		KFloat3DMemAllocator;
typedef T3DMemAllocator<DOUBLE>		KDouble3DMemAllocator;

#endif // #ifndef __INCLUDED_TEMPLATE_3DMEMALLOCATOR_H__
