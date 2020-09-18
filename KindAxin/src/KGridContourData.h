/******************************************************************************
 ** KGridContourData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridContourData
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

#include "KGridData.h"

class KIND_EXT_CLASS KGridContourData : public KGridData
{
public:
	KGridContourData(void);
	virtual ~KGridContourData(void);

public:
	// 构造等值线
	virtual BOOL BuildContours();

public:
	// 取得等值线构造器
	KContourBuilder* GetContourBuilder() { return m_pContourBuilder; }

protected:
	// 装载数据 - ASC文件
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

protected:
	// 等值线构造器
	KContourBuilder* m_pContourBuilder;
};

