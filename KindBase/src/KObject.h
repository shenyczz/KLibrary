/******************************************************************************
 ** KObject.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KObject
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

#include "KExtents.h"
#include "KString.h"

class KIND_EXT_CLASS KObject
{
public:
	KObject(void);
	KObject(const KObject& object);
	KObject(LPCTSTR LpszName);
	virtual ~KObject(void);

public:
	// ӵ����
	void* GetOwner() { return m_pOwner; }
	void SetOwner(void* pOwner) { m_pOwner = pOwner; }

	// ID
	virtual void SetId(int id) { m_Id = id; }
	virtual int GetId() const { return m_Id; }

	// Name
	virtual void SetName(LPCTSTR pszName) { m_Name = pszName; }
	virtual KString GetName() { return m_Name; }

	// Type
	virtual void SetType(int type) { m_Type = type; }
	virtual int GetType() const { return m_Type; }

	// Flag
	virtual void SetFlag(int flag) { m_Flag = flag; }
	virtual int GetFlag() const { return m_Flag; }

	// Select
	virtual void Select(bool bSelected = true) { m_bSelected = bSelected; }
	virtual bool IsSelected() { return m_bSelected; }

	// Extents
	virtual void SetExtents(const KExtents& extents) { m_Extents = extents; }
	virtual const KExtents& GetExtents() { return m_Extents; }

	// ExtentsClient
	KExtents& GetExtentsClient() { return m_ExtentsClient; }
	void SetExtentsClient(KExtents extentsClient) { m_ExtentsClient = extentsClient; }

public:
	virtual bool operator ==(const KObject& other);
	virtual bool operator !=(const KObject& other);

protected:
	void*		m_pOwner;			// ӵ����

	int			m_Id;				// ��ʶ
	int			m_Type;				// ����
	KString		m_Name;				// ����
	DWORD		m_Flag;				// ��־

	bool		m_bSelected;		// ѡ��
	KExtents	m_Extents;			// ��Χ��(��������)
	KExtents	m_ExtentsClient;	// ��Χ��(��Ļ����)

};
