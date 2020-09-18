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
		// ���Ԫ��
		virtual void Add(KGeometry* pGeometry) { m_pCollection->Add((KGeometry*)pGeometry); }
		// �Ƴ�Ԫ��(��ͬ)
		virtual bool Remove(KGeometry* pGeometry) { return m_pCollection->Remove((KGeometry*)pGeometry); }
		// �Ƴ�ָ��λ��Ԫ��(��ͬ)
		virtual bool RemoveAt(int index) { return m_pCollection->RemoveAt(index); }
		// ����Ԫ��
		virtual bool Insert(int index, KGeometry* pGeometry) { return m_pCollection->Insert(index,(KGeometry*)pGeometry); }
		// ȡ��Ԫ�ص�����ֵ
		virtual int IndexOf(KGeometry* pGeometry) { return m_pCollection->IndexOf((KGeometry*)pGeometry); }
		// ����ָ��Ԫ��
		virtual bool Contains(KGeometry* pGeometry) { return m_pCollection->Contains((KGeometry*)pGeometry); }
		// ����ָ��λ��Ԫ��(��ͬ)
		virtual bool SetAt(int index, KGeometry* pGeometry) { return m_pCollection->SetAt(index,(KGeometry*)pGeometry); }
		// ȡ��ָ��λ��Ԫ��
		virtual const KGeometry* GetAt(int index) const { return m_pCollection->GetAt(index); }
		// ȡ��ָ��λ��Ԫ��
		virtual KGeometry* GetAt(int index) { return m_pCollection->GetAt(index); }
		// ȡ��Ԫ������
		virtual int Count() const  { return m_pCollection->Count(); }
		// �������Ԫ��(��ͬ)
		virtual void Clear() { m_pCollection->Clear(); }
		// �Ƿ�ռ���
		virtual bool IsEmpty() { return m_pCollection->IsEmpty(); }
		// ����
		virtual KGeometry* operator[](int index) { return (*m_pCollection)[index]; }

	protected:
		TPtrCollection<KGeometry*>* m_pCollection;
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
