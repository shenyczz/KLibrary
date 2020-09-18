#pragma once

#include "KGeometry.h"

namespace Geometries
{

	class KIND_EXT_CLASS KGeometryPtrCollection : public KGeometry
	{
	public:
		KGeometryPtrCollection(void);
		virtual ~KGeometryPtrCollection(void);

	public:
		// 添加元素
		virtual void Add(KGeometry* pGeometry) { m_pCollection->Add((KGeometry*)pGeometry); }
		// 移除元素(不同)
		virtual bool Remove(KGeometry* pGeometry) { return m_pCollection->Remove((KGeometry*)pGeometry); }
		// 移除指定位置元素(不同)
		virtual bool RemoveAt(int index) { return m_pCollection->RemoveAt(index); }
		// 插入元素
		virtual bool Insert(int index, KGeometry* pGeometry) { return m_pCollection->Insert(index,(KGeometry*)pGeometry); }
		// 取得元素的索引值
		virtual int IndexOf(KGeometry* pGeometry) { return m_pCollection->IndexOf((KGeometry*)pGeometry); }
		// 包含指定元素
		virtual bool Contains(KGeometry* pGeometry) { return m_pCollection->Contains((KGeometry*)pGeometry); }
		// 设置指定位置元素(不同)
		virtual bool SetAt(int index, KGeometry* pGeometry) { return m_pCollection->SetAt(index,(KGeometry*)pGeometry); }
		// 取得指定位置元素
		virtual const KGeometry* GetAt(int index) const { return m_pCollection->GetAt(index); }
		// 取得指定位置元素
		virtual KGeometry* GetAt(int index) { return m_pCollection->GetAt(index); }
		// 取得元素数量
		virtual int Count() const  { return m_pCollection->Count(); }
		// 清除所有元素(不同)
		virtual void Clear() { m_pCollection->Clear(); }
		// 是否空集合
		virtual bool IsEmpty() { return m_pCollection->IsEmpty(); }
		// 索引
		virtual KGeometry* operator[](int index) { return (*m_pCollection)[index]; }

	protected:
		TPtrCollection<KGeometry*>* m_pCollection;
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
