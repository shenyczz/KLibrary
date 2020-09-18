/******************************************************************************
 ** KAxinData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDiamondData
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

class KIND_EXT_CLASS KDiamondData : public KCustomDataObject
{
public:
	KDiamondData(void);
	virtual ~KDiamondData(void);

public:
	// 构建调色板
	void BuildPalette(LPCTSTR lpszFile = NULL);

protected:
	// 填充数据信息
	virtual void FillDataInfo(LPVOID lpvData) {}

public:
	static bool IsDiamondData(LPCTSTR lpszFile);
	static int GetFormatCode(LPCTSTR lpszFile);
};

