/******************************************************************************
 ** TPtrList.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: TPtrList - ��ģ��
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
 **			TPtrList<KObject*> objList;
 **			KObject* pObject = new KObject();
 **			objList.Add(pObject);
 **
******************************************************************************/

#ifndef __INCLUDED_TEMPLATE_LIST_H__
#define __INCLUDED_TEMPLATE_LIST_H__

#include <list>
using namespace std;

template<typename TElement>
class TPtrList : public list<TElement>
{
public:
	TPtrList(void) {}

	virtual ~TPtrList(void)
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
		iterator itr_end = end();
		for(iterator itr=begin(); itr!=itr_end; itr++)
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

	// ���ȫ��Ԫ��
	void Clear()
	{
		iterator itr;
		iterator iend = end();
		for(itr = begin(); itr != iend; itr++)
		{
			if( (*itr) != 0 )
			{
				delete (*itr);
				(*itr) = 0;
			}
		}

		clear();
	}
};

#endif // #ifndef __INCLUDED_TEMPLATE_LIST_H__
