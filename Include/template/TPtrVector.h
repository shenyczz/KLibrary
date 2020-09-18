/******************************************************************************
 ** TPtrVector.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: TPtrVector - ʸ��ģ��
 **  Version: 
 ** Function: 
 **  Explain: TElement ������ʹ�� new �����ڴ�ָ��
 **           �Զ�������������
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
 **			TPtrVector<KObject*> objVector;
 **			KObject* pObject = new KObject();
 **			objVector.Add(pObject);
 **
******************************************************************************/

#pragma once

#include <vector>
using namespace std;

template<typename TElement>
class TPtrVector : public vector<TElement>
{
public:
	TPtrVector(void) {}

	virtual ~TPtrVector(void)
	{
		Clear();
	}

public:
	// ���Ԫ��
	void Add(TElement element)
	{
		push_back(element);
	}

	// �Ƴ�Ԫ��
	void Remove(TElement element)
	{
		iterator itrBeg = begin();
		iterator itrEnd = end();
		for(iterator itr=itrBeg; itr!=itrEnd; itr++)
		{
			TElement temp = (*itr);
			if(temp == element)
			{
				erase(itr);
				delete temp;
				temp = NULL;
				break;
			}
		}
	}

	// ���ȫ��Ԫ��
	void Clear()
	{
		int iCount = size();
		for(int i=0;i<iCount;i++)
		{
			if( (*this)[i] != 0)
			{
				delete (*this)[i];
				(*this)[i] = 0;
			}
		}

		clear();
	}

	// Ԫ������
	int Count()
	{
		return size();
	}

	// Ԫ�ص�����ֵ
	int IndexOf(TElement element)
	{
		int index = -1;

		int loop = 0;
		iterator itrBeg = begin();
		iterator itrEnd = end();
		for(iterator itr=itrBeg; itr!=itrEnd; itr++)
		{
			TElement temp = (*itr);
			if(temp == element)
			{
				index = loop;
				break;
			}

			loop++;
		}

		return index;
	}

	// ȡ��ָ��λ��Ԫ��
	TElement GetAt(int index)
	{
		if(index<0 || index>=(int)size())
			return NULL;

		return at(index);
	}

	// ȡ��ָ��λ��Ԫ��
	BOOL SetAt(int index,TElement element)
	{
		if(index<0 || index>=(int)size())
			return FALSE;

		TElement etmp = GetAt(index);
		if(!etmp)
			return FALSE;

		iterator itrsave;
		iterator itrBeg		= begin();
		iterator itrEnd		= end();
		for(iterator itr=itrBeg; itr!=itrEnd; itr++)
		{
			if(etmp == (*itr))
			{
				itrsave = itr;
				break;
			}
		}

		(*itrsave) = element;
		_delete(etmp);

		return TRUE;
	}

};// template TPtrVector - end
