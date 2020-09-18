/******************************************************************************
 ** KObjectCollection.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KObjectCollection
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

#include "KObject.h"
#include "TCollection.h"

class KIND_EXT_CLASS KObjectCollection : public KObject
{
public:
	KObjectCollection(void);
	virtual ~KObjectCollection(void);

public:
	// 添加元素
	virtual void Add(const KObject& object) { m_pCollection->Add(object); }
	// 移除元素
	virtual bool Remove(const KObject& object) { return m_pCollection->Remove(object); }
	// 移除指定位置元素
	virtual bool RemoveAt(int index) { return m_pCollection->RemoveAt(index); }
	// 插入元素
	virtual bool Insert(int index, const KObject& object) { return m_pCollection->Insert(index,object); }
	// 取得元素的索引值
	virtual int IndexOf(const KObject& object) { return m_pCollection->IndexOf(object); }
	// 包含指定元素
	virtual bool Contains(const KObject& object) { return m_pCollection->Contains(object); }
	// 设置指定位置元素
	virtual bool SetAt(int index, const KObject& object) { return m_pCollection->SetAt(index,object); }
	// 取得指定位置元素
	virtual const KObject& GetAt(int index) const { return m_pCollection->GetAt(index); }
	virtual KObject& GetAt(int index) { return m_pCollection->GetAt(index); }
	// 取得元素数量
	virtual int Count() const  { return m_pCollection->Count(); }
	// 清除所有元素
	virtual void Clear() { m_pCollection->Clear(); }
	// 是否空集合
	virtual bool IsEmpty() { return m_pCollection->IsEmpty(); }
	// 索引
	virtual KObject& operator[](int index) { return (*m_pCollection)[index]; }

protected:
	TCollection<KObject>* m_pCollection;
};
