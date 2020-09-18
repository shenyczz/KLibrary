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
		// ���Ԫ��
		virtual void Add(const T& item)
		{
			this->push_back(item);
		}
		// �Ƴ�Ԫ��(OL)
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
		// �Ƴ�ָ��λ��Ԫ��(OL)
		virtual bool RemoveAt(int index)
		{
			return Remove(GetAt(index));
		}
		// ����Ԫ��
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
		// ȡ��Ԫ�ص�����ֵ
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
		// ����ָ��Ԫ��
		virtual bool Contains(const T& item)
		{
			return IndexOf(item)>=0;
		}
		// ����ָ��λ��Ԫ��(OL)
		virtual bool SetAt(int index, const T& item)
		{
			if(index<0 || index>=(int)this->size())
				return false;

			this->at(index) = item;
			return true;
		}
		// ȡ��ָ��λ��Ԫ��
		virtual const T& GetAt(int index) const
		{
			return this->at(index);
		}
		virtual T& GetAt(int index)
		{
			return this->at(index);
		}
		// ȡ��Ԫ������
		virtual int Count() const
		{
			return this->size();
		}
		// �������Ԫ��(OL)
		virtual void Clear()
		{
			this->clear();
		}
		// �Ƿ�ռ���
		virtual bool IsEmpty()
		{
			return this->empty();
		}

		// ����
		virtual T& operator[](int index)
		{
			return this->at(index);
		}
	};

//---------------------------------------------------------
}// namespace Collections - end
//---------------------------------------------------------
