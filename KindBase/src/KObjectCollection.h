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
#include "TCollection.h"

class KIND_EXT_CLASS KObjectCollection : public KObject
{
public:
	KObjectCollection(void);
	virtual ~KObjectCollection(void);

public:
	// ���Ԫ��
	virtual void Add(const KObject& object) { m_pCollection->Add(object); }
	// �Ƴ�Ԫ��
	virtual bool Remove(const KObject& object) { return m_pCollection->Remove(object); }
	// �Ƴ�ָ��λ��Ԫ��
	virtual bool RemoveAt(int index) { return m_pCollection->RemoveAt(index); }
	// ����Ԫ��
	virtual bool Insert(int index, const KObject& object) { return m_pCollection->Insert(index,object); }
	// ȡ��Ԫ�ص�����ֵ
	virtual int IndexOf(const KObject& object) { return m_pCollection->IndexOf(object); }
	// ����ָ��Ԫ��
	virtual bool Contains(const KObject& object) { return m_pCollection->Contains(object); }
	// ����ָ��λ��Ԫ��
	virtual bool SetAt(int index, const KObject& object) { return m_pCollection->SetAt(index,object); }
	// ȡ��ָ��λ��Ԫ��
	virtual const KObject& GetAt(int index) const { return m_pCollection->GetAt(index); }
	virtual KObject& GetAt(int index) { return m_pCollection->GetAt(index); }
	// ȡ��Ԫ������
	virtual int Count() const  { return m_pCollection->Count(); }
	// �������Ԫ��
	virtual void Clear() { m_pCollection->Clear(); }
	// �Ƿ�ռ���
	virtual bool IsEmpty() { return m_pCollection->IsEmpty(); }
	// ����
	virtual KObject& operator[](int index) { return (*m_pCollection)[index]; }

protected:
	TCollection<KObject>* m_pCollection;
};
