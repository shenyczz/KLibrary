#include "StdAfx.h"
#include "KBytesOrder.h"

KBytesOrder::KBytesOrder(void)
{
}

KBytesOrder::~KBytesOrder(void)
{
}

// ��β�ֽں�Сβ�ֽ��໥ת��
void KBytesOrder::ReverseBytes(short* iValue)
{
	union
	{
		BYTE a[2];
		short n;
	} u,v;

	u.n = *iValue;
	      
	v.a[0] = u.a[1];
	v.a[1] = u.a[0];

	*iValue = v.n;
}

// ��β�ֽں�Сβ�ֽ��໥ת��
void KBytesOrder::ReverseBytes(unsigned short* iValue)
{
	union
	{
		BYTE a[2];
		unsigned short n;
	} u,v;

	u.n = *iValue;
	      
	v.a[0] = u.a[1];
	v.a[1] = u.a[0];

	*iValue = v.n;
}

// ��β�ֽں�Сβ�ֽ��໥ת��
// bMidEndian = TRUE ��ʾ��β�ֽ� Ĭ��=FALSE
void KBytesOrder::ReverseBytes(long* lValue,BOOL bMidEndian/*=FALSE*/)
{
	union
	{
		BYTE a[4];
		long n;
	} u,v;

	u.n = *lValue;
	      
	if(bMidEndian)
	{
		v.a[0] = u.a[1];
		v.a[1] = u.a[0];
		v.a[2] = u.a[3];
		v.a[3] = u.a[2];   
	}
	else
	{
		v.a[0] = u.a[3];
		v.a[1] = u.a[2];
		v.a[2] = u.a[1];
		v.a[3] = u.a[0];   
	}

	*lValue = v.n;
}

// ��β�ֽں�Сβ�ֽ��໥ת��
// bMidEndian = TRUE ��ʾ��β�ֽ� Ĭ��=FALSE
void KBytesOrder::ReverseBytes(unsigned long* lValue,BOOL bMidEndian/*=FALSE*/)
{
	union
	{
		BYTE a[4];
		unsigned long n;
	} u,v;

	u.n = *lValue;
	      
	if(bMidEndian)
	{
		v.a[0] = u.a[1];
		v.a[1] = u.a[0];
		v.a[2] = u.a[3];
		v.a[3] = u.a[2];   
	}
	else
	{
		v.a[0] = u.a[3];
		v.a[1] = u.a[2];
		v.a[2] = u.a[1];
		v.a[3] = u.a[0];   
	}

	*lValue = v.n;
}
