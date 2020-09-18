/******************************************************************************
 ** KColortableData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KColortableData
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

class KIND_EXT_CLASS KColortableData : public KAxinData
{
public:
	KColortableData(void);
	virtual ~KColortableData(void);

public:
	// 填充数据信息
	virtual void FillDataInfo(LPVOID lpvData);

protected:
	// 装载数据 - ASC文件
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);
};

