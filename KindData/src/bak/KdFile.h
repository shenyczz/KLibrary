/******************************************************************************
 ** KdFile.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KdFile
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

#include "KCustomDataObject.h"

class KIND_EXT_CLASS KdFile : public KCustomDataObject
{
public:
	KdFile(void);
	virtual ~KdFile(void);

public:
	// 清除数据并回收内存
	virtual void Empty();
	//本类数据是否清空(根据需要在派生类中根据需要取舍条件)
	virtual BOOL IsEmpty();

public:
	// 文件头
	void* FileHeader() { return m_pFileHeader; }

	// 数据信息块
	DMB* DMBPtr() { return (DMB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_DMB);}
	// 扩展信息块
	EMB* EMBPtr() { return (EMB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_EMB); }
	// 注释数据块
	CDB* CDBPtr() { return (CDB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_CDB); }
	// 数据描述块
	DDB* DDBPtr() { return (DDB*)((char*)m_pFileHeader + KD_BLOCK_OFFSET_DDB); }

protected:
	// 数据转换为颜色
	virtual COLORREF* Data2Colors(COLORREF clrBack) { return NULL; }

protected:
	// 文件头
	void* m_pFileHeader;

	// 颜色数组指针
	COLORREF* m_pColors;

public: // static function
	// 是否 KD 文件
	static BOOL IsKdFile(LPCTSTR lpszFile);

	static BOOL CreateKdFile(LPCTSTR lpszFile, BYTE* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, char* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, WORD* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, SHORT* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, DWORD* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, LONG* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, float* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile(LPCTSTR lpszFile, double* pData, KDataInfo* pDataInfo, KPalette* pPalette);

private:
	static BOOL IsKdFile_bin(LPCTSTR lpszFile);
	static BOOL IsKdFile_txt(LPCTSTR lpszFile);

	static BOOL CreateKdFile(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile_easy(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateKdFile_hard(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette);
};

