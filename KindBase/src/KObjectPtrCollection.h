/******************************************************************************
 ** KObjectPtrCollection.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KObjectPtrCollection
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
#include "TPtrCollection.h"

class KIND_EXT_CLASS KObjectPtrCollection : public KObject
{
public:
	KObjectPtrCollection(void);
	virtual ~KObjectPtrCollection(void);

public:
	// 添加元素
	virtual void Add(KObject* pObject) { m_pCollection->Add(pObject); }
	// 移除元素(不同)
	virtual bool Remove(KObject* pObject) { return m_pCollection->Remove(pObject); }
	// 移除指定位置元素(不同)
	virtual bool RemoveAt(int index) { return m_pCollection->RemoveAt(index); }
	// 插入元素
	virtual bool Insert(int index, KObject* pObject) { return m_pCollection->Insert(index,pObject); }
	// 取得元素的索引值
	virtual int IndexOf(KObject* pObject) { return m_pCollection->IndexOf(pObject); }
	// 包含指定元素
	virtual bool Contains(KObject* pObject) { return m_pCollection->Contains(pObject); }
	// 设置指定位置元素(不同)
	virtual bool SetAt(int index, KObject* pObject) { return m_pCollection->SetAt(index,pObject); }
	// 取得指定位置元素
	virtual const KObject* GetAt(int index) const { return m_pCollection->GetAt(index); }
	// 取得指定位置元素
	virtual KObject* GetAt(int index) { return m_pCollection->GetAt(index); }
	// 取得元素数量
	virtual int Count() const  { return m_pCollection->Count(); }
	// 清除所有元素(不同)
	virtual void Clear() { m_pCollection->Clear(); }
	// 是否空集合
	virtual bool IsEmpty() { return m_pCollection->IsEmpty(); }
	// 索引
	virtual KObject* operator[](int index) { return (*m_pCollection)[index]; }

protected:
	TPtrCollection<KObject*>* m_pCollection;
};
