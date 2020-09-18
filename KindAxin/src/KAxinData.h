/******************************************************************************
 ** KAxinData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KAxinData
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


class KIND_EXT_CLASS KAxinData : public KCustomDataObject
{
public:
	enum _THIS_
	{
		eDataStyle_Marker			= 0x1000,
		eDataStyle_Grid_Contour		= 0x1001,
		eDataStyle_Grid_Image		= 0x1002,
	};

public:
	KAxinData(void);
	virtual ~KAxinData(void);

public:
	// 构建调色板
	void BuildPalette(LPCTSTR lpszFile = NULL);

	// 取得产品调色板
	KPalette* GetProductPalette(int iProductCode);
	// 取得产品数据信息
	KDataInfo* GetProductDataInfo() { return m_pProductDataInfo; }

	// 填充数据信息
	virtual void FillDataInfo(LPVOID lpvData) {}

protected:
	// 产品数据信息
	KDataInfo* m_pProductDataInfo;

public:
	static BOOL IsAxinData(LPCTSTR lpszFile);
	static int GetFormatCode(LPCTSTR lpszFile);
	static BOOL GetDateTime(LPCTSTR lpszFile, DATETIME* pDatm);
	static BOOL GetFileDataInfo(LPCTSTR lpszFile, KDataInfo* pDataInfo);

private:
	static BOOL GetFileDataInfo_bin(LPCTSTR lpszFile, KDataInfo* pDataInfo);
	static BOOL GetFileDataInfo_txt(LPCTSTR lpszFile, KDataInfo* pDataInfo);

public:
	static BOOL CreateFile(LPCTSTR lpszFile, BYTE* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateFile(LPCTSTR lpszFile, char* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateFile(LPCTSTR lpszFile, WORD* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateFile(LPCTSTR lpszFile, SHORT* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateFile(LPCTSTR lpszFile, DWORD* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateFile(LPCTSTR lpszFile, LONG* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateFile(LPCTSTR lpszFile, float* pData, KDataInfo* pDataInfo, KPalette* pPalette);
	static BOOL CreateFile(LPCTSTR lpszFile, double* pData, KDataInfo* pDataInfo, KPalette* pPalette);

private:
	static BOOL CreateFile(LPCTSTR lpszFile, void* pData, KDataInfo* pDataInfo, KPalette* pPalette);

public:
	static BOOL ExportEnviHdr(LPCTSTR lpszFile,KDataInfo* pDataInfo);
};

