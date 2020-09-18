/******************************************************************************
 ** KWindVectorData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KWindVectorData - 风场矢量数据
 **  Version: 
 ** Function: 根据风场数据的U、V分量绘制风场流线等
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

class KIND_EXT_CLASS KWindVectorData : public KGridData
{
public:
	KWindVectorData(void);
	virtual ~KWindVectorData(void);

protected:
	// 重载装载数据
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

private:
	// 查找找网格数据极值
	void LookupExtremum();
};

