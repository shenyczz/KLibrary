/******************************************************************************
 ** TPtrMap.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: TPtrMap - 映射模板
 **  Version: 
 ** Function: 
 **  Explain: TElement 必须是使用 new 分配内存指针
 **           自动管理垃圾回收
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
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

#ifndef __INCLUDED_TEMPLATE_MAP_H__
#define __INCLUDED_TEMPLATE_MAP_H__

#include <map>
using namespace std;

template<typename TKey, typename TValue>
class TPtrMap : public map<TKey, TValue>
{
public:
	TPtrMap(void) {}

	virtual ~TPtrMap(void)
	{
		Clear();
	}

public:
	// 添加元素
	void Add(TKey key, TValue value)
	{
		insert(value_type(key,value));
	}

	// 移除元素
	void Remove(TKey key)
	{
		iterator itr = find(key);
		if(itr != end())
		{
			TValue value = (*itr).second;
			_delete(value);
			(*itr).second = 0;
			erase(itr);
		}
	}

	// 移除元素
	void Remove(TValue value)
	{
		iterator itr;
		iterator itr_end = end();
		for(itr=begin(); itr!=itr_end; itr++)
		{
			if((*itr).second == value)
				break;
		}

		if(itr != itr_end)
		{
			TValue value = (*itr).second;
			_delete(value);
			(*itr).second = 0;
			erase(itr);
		}
	}

	// 元素数量
	int Count()
	{
		return count();
	}

	// 查找
	TValue Find(TKey key)
	{
		return (*this)[key];
	}

	// 清除全部元素
	void Clear()
	{
		iterator itr_end = end();
		for(iterator itr = begin(); itr != itr_end; itr++)
		{
			if( (*itr).second != 0 )
			{
				delete (*itr).second;
				(*itr).second = 0;
			}
		}

		clear();
	}
};

#endif // #ifndef __INCLUDED_TEMPLATE_MAP_H__
