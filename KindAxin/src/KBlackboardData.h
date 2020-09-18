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

#include "KAxinData.h"

class KIND_EXT_CLASS KBlackboardData : public KAxinData
{
public:
	KBlackboardData(void);
	virtual ~KBlackboardData(void);

public:
	// 是否空
	virtual BOOL IsEmpty();
	// 取得范围
	virtual const KExtents& GetExtents();

public:
	KObjectPtrCollection* GetObjectPtrCollection() { return m_pObjectPtrCollection; }

public:
	// 添加对象
	void AddObject(KObject* pObject);
	// 移除对象
	void RemoveObject(KObject* pObject);
	// 选择所有
	void SelectAll(bool bSelect);
	// 选择对象
	void Select(KObject* pObject, bool bSelect);

	// 取得鼠标位置下方的对象(世界坐标)(暂时未用)
	KObject* GetObject(KPoint point);
	// 取得鼠标位置下方的对象(屏幕坐标)
	KObject* GetObject(POINT point);
	// 取得选中的对象
	KObject* GetSelectedObject();

	// 查找指定标记的图形
	KObject* FindObject(DWORD dwFlag);

protected:
	// 重载装载数据
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

private:
	// 对象集合
	KObjectPtrCollection* m_pObjectPtrCollection;
};

