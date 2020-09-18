/******************************************************************************
 ** TCollection.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: TCollection<T>
 **  Version: 
 ** Function: 
 **  Explain: 
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
 **
******************************************************************************/

#pragma once

namespace Collections
{

	template <class T>
	class TCollection : public vector<T>
	{
	public:
		TCollection(void) : vector<T>()
		{
		}
		TCollection(const TCollection& other) : vector<T>(other)
		{
		}

		virtual ~TCollection(void)
		{
			Clear();
			//_delete(m_pVector);
		}

	public:
		// 添加元素
		virtual void Add(const T& item)
		{
			this->push_back(item);
		}
		// 移除元素(OL)
		virtual bool Remove(const T& item)
		{
			iterator itr_end = this->end();
			for(iterator itr = this->begin(); itr != itr_end; itr++)
			{
				T temp = (*itr);
				if(temp == item)
				{
					this->erase(itr);
					return true;
				}
			}

			return false;
		}
		// 移除指定位置元素(OL)
		virtual bool RemoveAt(int index)
		{
			return Remove(GetAt(index));
		}
		// 插入元素
		virtual bool Insert(int index, const T& item)
		{
			if(index>=0 && index<(int)this->size())
			{
				int loop = 0;
				vector<T>::iterator itr_end = this->end();
				for(vector<T>::iterator itr = this->begin(); itr != itr_end; itr++)
				{
					if(index==loop++)
					{
						this->insert(itr,item);
						return true;
					}
				}
			}

			return false;
		}
		// 取得元素的索引值
		virtual int IndexOf(const T& item)
		{
			int loop = -1;
			vector<T>::iterator itr_end = this->end();
			for(vector<T>::iterator itr = this->begin(); itr != itr_end; itr++)
			{
				loop++;
				T temp = (*itr);
				if(temp == item)
					return loop;
			}
			return -1;
		}
		// 包含指定元素
		virtual bool Contains(const T& item)
		{
			return IndexOf(item)>=0;
		}
		// 设置指定位置元素(OL)
		virtual bool SetAt(int index, const T& item)
		{
			if(index<0 || index>=(int)this->size())
				return false;

			this->at(index) = item;
			return true;
		}
		// 取得指定位置元素
		virtual const T& GetAt(int index) const
		{
			return this->at(index);
		}
		virtual T& GetAt(int index)
		{
			return this->at(index);
		}
		// 取得元素数量
		virtual int Count() const
		{
			return this->size();
		}
		// 清除所有元素(OL)
		virtual void Clear()
		{
			this->clear();
		}
		// 是否空集合
		virtual bool IsEmpty()
		{
			return this->empty();
		}

		// 索引
		virtual T& operator[](int index)
		{
			return this->at(index);
		}
	};

//---------------------------------------------------------
}// namespace Collections - end
//---------------------------------------------------------
