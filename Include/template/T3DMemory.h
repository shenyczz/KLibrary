/******************************************************************************
 ** T3DMemory.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: T3DMemory - 三维内存类模板
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
		T3DMemory<float>	f3dMemory;
	//2. Allocate Memory
		f3dMemory.AllocMemory(l,r,c);
	//3. Free Memory
		f3dMemory.FreeMemory();

******************************************************************************/

#ifndef __INCLUDED_TEMPLATE_T3DMEMORY_H__
#define __INCLUDED_TEMPLATE_T3DMEMORY_H__

template <class _Ty>
class T3DMemory
{
public:
	T3DMemory()
		:m_pv(0)
		,m_lays(0)
		,m_rows(0)
		,m_cols(0) 
	{
	}

	virtual ~T3DMemory()
	{
		_deletea(m_pv);
	}

public:
	bool AllocMemory(int lays,int rows,int cols)
	{
		if(lays<=0 || rows<=0 || cols<=0)
			return false;

		m_pv = new _Ty [lays*rows*cols];
		if(!m_pv)
			return false;

		memset(m_pv,0,sizeof(_Ty)*lays*rows*cols);

		m_lays = lays;
		m_rows = rows;
		m_cols = cols;

		return true;
	}

	void FreeMemory()
	{
		_deletea(m_pv);
	}

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

		//return *(m_pv + (lay*m_rows + row)*m_cols + col);
		return *(m_pv + lay*m_rows*m_cols + row*m_cols + col);
	}

	//[操作符重载] -> 获取元素(i,j,k)的值,右值函数(只读)
	const _Ty& operator()(int lay,int row, int col) const
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

typedef T3DMemory<BYTE>		KByte3DMemory;
typedef T3DMemory<WORD>		KWord3DMemory;
typedef T3DMemory<SHORT>	KShort3DMemory;
typedef T3DMemory<LONG>		KLong3DMemory;
typedef T3DMemory<FLOAT>	KFloat3DMemory;
typedef T3DMemory<DOUBLE>	KDouble3DMemory;

#endif // #ifndef __INCLUDED_TEMPLATE_T3DMEMORY_H__
