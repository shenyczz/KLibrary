/******************************************************************************
 ** KBlackboardData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KBlackboardData
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

#include "KAxinData.h"

class KIND_EXT_CLASS KBlackboardData : public KAxinData
{
public:
	KBlackboardData(void);
	virtual ~KBlackboardData(void);

public:
	// �Ƿ��
	virtual BOOL IsEmpty();
	// ȡ�÷�Χ
	virtual const KExtents& GetExtents();

public:
	KObjectPtrCollection* GetObjectPtrCollection() { return m_pObjectPtrCollection; }

public:
	// ��Ӷ���
	void AddObject(KObject* pObject);
	// �Ƴ�����
	void RemoveObject(KObject* pObject);
	// ѡ������
	void SelectAll(bool bSelect);
	// ѡ�����
	void Select(KObject* pObject, bool bSelect);

	// ȡ�����λ���·��Ķ���(��������)(��ʱδ��)
	KObject* GetObject(KPoint point);
	// ȡ�����λ���·��Ķ���(��Ļ����)
	KObject* GetObject(POINT point);
	// ȡ��ѡ�еĶ���
	KObject* GetSelectedObject();

	// ����ָ����ǵ�ͼ��
	KObject* FindObject(DWORD dwFlag);

protected:
	// ����װ������
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

private:
	// ���󼯺�
	KObjectPtrCollection* m_pObjectPtrCollection;
};

