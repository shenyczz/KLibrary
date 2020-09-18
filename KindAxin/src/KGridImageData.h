/******************************************************************************
 ** KGridImageData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridImageData
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

class KIND_EXT_CLASS KGridImageData : public KGridData
{
public:
	KGridImageData(void);
	virtual ~KGridImageData(void);

public:
	// 文件头
	PCHAR FileHeader() { return m_pFileHeader; }
	
public:
	// 取得数据偏移地址
	DWORD GetDataOffset() { return m_dwDataOffset; }

protected:
	// 装载数据
	virtual BOOL LoadData_bin(LPCTSTR lpszFile);

private:
	// 填充数据信息
	void FillDataInfo(LPVOID lpvData);

protected:
	// 文件头
	PCHAR m_pFileHeader;

	// 数据偏移
	DWORD m_dwDataOffset;

	// 调色板集合
	TPtrCollection<KPalette*>* m_pPaletteCollection;
};
