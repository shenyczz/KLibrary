/******************************************************************************
 ** TPtrCollection.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: TPtrCollection<T>
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

#include "TCollection.h"

namespace Collections
{

	template <class T>
	class TPtrCollection : public TCollection<T>
	{
	public:
		TPtrCollection() : TCollection<T>() {}
		virtual ~TPtrCollection() { Clear(); }

	public:
		// �Ƴ�Ԫ��
		virtual bool Remove(const T& item)
		{
			vector<T>::iterator itrBeg = this->begin();
			vector<T>::iterator itrEnd = this->end();
			for(vector<T>::iterator itr = itrBeg; itr != itrEnd; itr++)
			{
				T temp = (*itr);
				if(temp == item)
				{
					this->erase(itr);
					delete temp;
					temp = NULL;
					return true;;
				}
			}

			return false;
		}
		// �Ƴ�ָ��λ��Ԫ��
		virtual bool RemoveAt(int index)
		{
			T item = GetAt(index);
			if(item==0) return false;
			else return Remove(item);
		}
		// ����ָ��λ��Ԫ��
		virtual bool SetAt(int index,const T& item)
		{
			if(index<0 || index>=(int)this->size())
				return false;

			T temp = GetAt(index);
			if(!temp)
				return false;

			vector<T>::iterator itrsave;
			vector<T>::iterator itrBeg		= this->begin();
			vector<T>::iterator itrEnd		= this->end();
			for(vector<T>::iterator itr=itrBeg; itr!=itrEnd; itr++)
			{
				if(temp == (*itr))
				{
					itrsave = itr;
					break;
				}
			}

			(*itrsave) = item;
			_delete(temp);

			return true;
		}
		// ���ȫ��Ԫ��
		virtual void Clear()
		{
			int count = this->size();
			for(int i=0;i<count;i++)
			{
				if( (*this)[i] != 0)
				{
					delete (*this)[i];
					(*this)[i] = 0;
				}
			}
			this->clear();
		}
	};

//---------------------------------------------------------
}// namespace Collections - end
//---------------------------------------------------------
