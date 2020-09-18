/******************************************************************************
 ** KStationData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KStationData
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

class KIND_EXT_CLASS KStationData : public KAxinData
{
public:
	KStationData(void);
	virtual ~KStationData(void);

public:
	// 清除数据并回收内存
	virtual void Empty();
	virtual BOOL IsEmpty();

protected:
	// 装载数据 - ASC文件
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

public:
	// 构造等值线
	BOOL BuildContours();

	// 是否追踪等值线
	BOOL IsTarceContour();

	// 取得站点要素数量
	int GetElementNumber();

public:
	// 等值线是否准备好
	BOOL IsContourReady() { return m_bContourReady; }

	// 等值线是否重新构造
	BOOL IsRebuildContour() { return m_bRebuildContour; }
	void EnableRebuildContour(BOOL bRebuildContour = TRUE) { m_bRebuildContour = bRebuildContour; }

	// 当前要素索引
	int& CurrentElementIndex() { return m_iCurrentElementIndex; }

	// 取得等值线构造器
	KContourBuilder* GetContourBuilder() { return m_pContourBuilder; }

	// 取得站点信息集合
	KObjectPtrCollection* GetStationInfoPtrCollection() { return m_pStationInfoPtrCollection; }

	// 插值记录最大值
	int& InterpolationRecordMax() { return m_InterpolationRecordMax; };

protected:
	// 计算绑定矩形
	void ComputeExtents();

	// 查找SNYC数据极小值和极大值
	void LookupExtremum();

protected:
	// 等值线是否准备好
	BOOL m_bContourReady;
	// 等值线是否重新追踪
	BOOL m_bRebuildContour;
	// 当前要素索引
	int m_iCurrentElementIndex;
	// 等值线构造器
	KContourBuilder* m_pContourBuilder;
	// 对象集合
	KObjectPtrCollection* m_pStationInfoPtrCollection;

	// 插值记录最大值
	int m_InterpolationRecordMax;

public:
	static BOOL SaveData(LPCTSTR pszFile,
		KDataInfo* pDataInfo,
		KObjectPtrCollection* pStationInfoPtrCollection,
		int iPrecision = 8,
		int iDecimal = 3);
};

