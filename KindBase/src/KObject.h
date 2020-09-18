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
	// 拥有者
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
	void*		m_pOwner;			// 拥有者

	int			m_Id;				// 标识
	int			m_Type;				// 类型
	KString		m_Name;				// 名称
	DWORD		m_Flag;				// 标志

	bool		m_bSelected;		// 选择
	KExtents	m_Extents;			// 包围盒(世界坐标)
	KExtents	m_ExtentsClient;	// 包围盒(屏幕坐标)

};
