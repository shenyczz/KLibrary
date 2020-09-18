/******************************************************************************
 ** KDataObject.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KDataObject
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

#include "KDataProcessor.h"

// 内存分配枚举
enum UVW
{
	eUvw_u = 1,		// U 分量
	eUvw_v = 2,		// V 分量
	eUvw_w = 4,		// W 分量(DWORD)
	eUvw_uv = 3,	// UV
	eUvw_uw = 5,	// UW
	eUvw_vw = 6,	// VW
	eUvw_all = 7,	// UVW
};

class KIND_EXT_CLASS KDataObject : public KProgressObject
{
public:
	KDataObject(void);
	KDataObject(LPCTSTR lpszFile);
	virtual ~KDataObject(void);

public:
	// 清除数据并回收内存
	virtual void Empty();
	//本类数据是否清空(根据需要在派生类中根据需要取舍条件)
	virtual BOOL IsEmpty();

	// 装载数据
	virtual BOOL LoadData(LPCTSTR lpszFile);

	// 保存数据
	virtual BOOL SaveData(LPCTSTR lpszFile);

protected:
	virtual BOOL LoadData_bin(LPCTSTR lpszFile);
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);


public:
	// 生成图像
	virtual BOOL BuildImage(COLORREF clrBack,LPCTSTR lpszFile = NULL);

	// 应用调色板
	virtual BOOL UsePalette(KPalette* pPalette);

	// 无效数据
	BOOL IsInvalidData(double value);
	// 特定值
	BOOL IsSpecialData(double value);

public:
	// 取得数据信息
	KDataInfo* GetDataInfo() { return m_pDataInfo; }
	// 数据处理器
	KDataProcessor* GetDataProcessor() { return m_pDataProcessor; }

	// 文件名
	LPCTSTR GetFileName() { return m_strFileName; }
	void SetFileName(LPCTSTR lpszFile) { m_strFileName = lpszFile; }

	// Gdiplus Image
	Image* GetImage();

	// 调色板
	KPalette* GetPalette() { return m_pPalette; }
	void SetPalette(KPalette* pPalette)	{ _delete(m_pPalette); m_pPalette = pPalette;	}

public:
	//取得x坐标指针
	double* GetXData() { return m_px; }
	//取得x坐标数据
	double GetXData(int i) { return *(m_px+i); }

	//取得y坐标指针
	double* GetYData() { return m_py; }
	//取得y坐标数据
	double GetYData(int i) { return *(m_py+i); }

	//取得z坐标指针
	double* GetZData() { return m_pz; }
	//取得z坐标数据
	double GetZData(int i) { return *(m_pz+i); }

	//取得U数据指针
	double* GetUData() { return m_pu; }
	//取得u坐标数据
	double GetUData(int i) { return *(m_pu+i); }

	//取得V数据指针
	double* GetVData() { return m_pv; }
	//取得v坐标数据
	double GetVData(int i) { return *(m_pv+i); }

	//取得W数据指针
	double* GetWData() { return m_pw; }
	//取得w坐标数据
	double GetWData(int i) { return *(m_pw+i); }

	// 信息
	LPCTSTR GetMessage() { return m_pszMessage; }
	void SetMessage(LPCTSTR lpszMessage)  { KString::SetString(m_pszMessage,lpszMessage); }

	// 数据元素单位
	virtual LPCTSTR GetUnit() { return m_pszUint; }
	void SetUnit(LPCTSTR lpszUnit) { KString::SetString(m_pszUint,lpszUnit); }

public:
	// 查找数据极小值和极大值
	virtual void LookupExtremum() { }

protected:
	// 为数据分配内存
	void PrepareMemory(UVW eUvw);

protected:
	// 填充数据信息
	virtual void FillDataInfo(LPVOID lpvData) = 0;

protected:
	// x坐标指针
	double* m_px;
	// y坐标指针
	double* m_py;
	// z坐标指针
	double* m_pz;

	// 风场U分量指针
	double* m_pu;
	// 风场V分量指针/数据指针
	double* m_pv;
	// DWORD数据指针
	double* m_pw;

	// 数据元素单位
	TCHAR* m_pszUint;
	// 错误信息
	TCHAR* m_pszMessage;

protected:
	// 调色板
	KPalette* m_pPalette;
	// 数据文件名称
	KString m_strFileName;

protected:
	// 数据信息
	KDataInfo* m_pDataInfo;
	// 数据处理器
	KDataProcessor* m_pDataProcessor;
};
