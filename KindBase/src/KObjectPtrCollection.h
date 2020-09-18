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

#include "KObject.h"
#include "TPtrCollection.h"

class KIND_EXT_CLASS KObjectPtrCollection : public KObject
{
public:
	KObjectPtrCollection(void);
	virtual ~KObjectPtrCollection(void);

public:
	// ���Ԫ��
	virtual void Add(KObject* pObject) { m_pCollection->Add(pObject); }
	// �Ƴ�Ԫ��(��ͬ)
	virtual bool Remove(KObject* pObject) { return m_pCollection->Remove(pObject); }
	// �Ƴ�ָ��λ��Ԫ��(��ͬ)
	virtual bool RemoveAt(int index) { return m_pCollection->RemoveAt(index); }
	// ����Ԫ��
	virtual bool Insert(int index, KObject* pObject) { return m_pCollection->Insert(index,pObject); }
	// ȡ��Ԫ�ص�����ֵ
	virtual int IndexOf(KObject* pObject) { return m_pCollection->IndexOf(pObject); }
	// ����ָ��Ԫ��
	virtual bool Contains(KObject* pObject) { return m_pCollection->Contains(pObject); }
	// ����ָ��λ��Ԫ��(��ͬ)
	virtual bool SetAt(int index, KObject* pObject) { return m_pCollection->SetAt(index,pObject); }
	// ȡ��ָ��λ��Ԫ��
	virtual const KObject* GetAt(int index) const { return m_pCollection->GetAt(index); }
	// ȡ��ָ��λ��Ԫ��
	virtual KObject* GetAt(int index) { return m_pCollection->GetAt(index); }
	// ȡ��Ԫ������
	virtual int Count() const  { return m_pCollection->Count(); }
	// �������Ԫ��(��ͬ)
	virtual void Clear() { m_pCollection->Clear(); }
	// �Ƿ�ռ���
	virtual bool IsEmpty() { return m_pCollection->IsEmpty(); }
	// ����
	virtual KObject* operator[](int index) { return (*m_pCollection)[index]; }

protected:
	TPtrCollection<KObject*>* m_pCollection;
};
