/******************************************************************************
 ** KRadarData.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRadarData - 雷达数据类
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              //1. 
              use

******************************************************************************/

#pragma once

#include "KRadarDataOperator.h"

class KIND_EXT_CLASS KRadarData : public KCustomDataObject
{
public:
	KRadarData(void);
	virtual ~KRadarData(void);

public:
	// 清空数据
	virtual void Empty();

	// 装载数据
	virtual BOOL LoadData(LPCTSTR lpszFile,LPVOID lpvData = 0);

	// 保存图像
	virtual BOOL SaveImage(LPCTSTR lpszFile);

	// 初始化颜色表
	virtual void InitColorTable(int iProductCode);


public:
	// 是否空数据集
	BOOL IsEmpty() { return m_bEmpty; }

	// 取得位图句柄
	void SetImageHandle(HBITMAP hBitmap);
	HBITMAP GetImageHandle() { return m_hBitmap; }

	// 设置/获取图像背景颜色
	void SetImageBkColor(COLORREF clr) { m_clrBack = clr; }
	COLORREF GetImageBkColor() { return m_clrBack; }

	// 设置图像尺寸
	void SetImageSize(DWORD dwSize) { m_dwWidth = m_dwHeight = dwSize; }

	// 设置/获取图像宽度
	void SetImageWidth(DWORD dwWidth) { m_dwWidth = dwWidth; }
	DWORD GetImageWidth() { return m_dwWidth; }

	// 设置/获取图像高度
	void SetImageHeight(DWORD dwHeight) { m_dwHeight = dwHeight; }
	DWORD GetImageHeight() { return m_dwHeight; }

	// 取得图例
	Legend* GetLegend() { return &m_Legend[0]; }
	Legend* GetLegend(int index) { return &m_Legend[index]; }

	// 设置/获取雷达型号
	void SetRadarType(RadarType eRadarType) { m_eRadarType = eRadarType; }
	RadarType GetRadarType() { return m_eRadarType; }

	// 获取雷达信息
	RadarInfo* GetRadarInfo() { return &m_RadarInfo; }

public:
	// 日期
	void SetDate(sfx_uint16 year,sfx_uint16 month,sfx_uint16 day);
	void GetDate(sfx_uint16 &year,sfx_uint16 &month,sfx_uint16 &day);

	// 时间
	void SetTime(sfx_uint16 hour,sfx_uint16 minute,sfx_uint16 second);
	void GetTime(sfx_uint16 &hour,sfx_uint16 &minute,sfx_uint16 &second);

	// 日期 时间
	void SetDateTime(sfx_uint16 year,sfx_uint16 month,sfx_uint16 day,sfx_uint16 hour,sfx_uint16 minute,sfx_uint16 second);
	void GetDateTime(sfx_uint16 &year,sfx_uint16 &month,sfx_uint16 &day,sfx_uint16 &hour,sfx_uint16 &minute,sfx_uint16 &second);

	// 雷达数据操作器
	void SetRadarDataOperator(KRadarDataOperator* pRadarDataOperator) { m_pRadarDataOperator = pRadarDataOperator; }
	KRadarDataOperator* GetRadarDataOperator() { return m_pRadarDataOperator; }

protected:
	// 填写雷达信息
	virtual void FillRadarInfo(RadarInfo* pRadarInfo);

protected:
	// 日期
	sfx_uint16 m_iYear;
	sfx_uint16 m_iMonth;
	sfx_uint16 m_iDay;

	// 时间
	sfx_uint16 m_iHour;
	sfx_uint16 m_iMinute;
	sfx_uint16 m_iSecond;

	// 雷达型号
	RadarType m_eRadarType;

	// 雷达信息
	RadarInfo m_RadarInfo;

	// 图像
	HBITMAP		m_hBitmap;		// 位图句柄
	COLORREF	m_clrBack;		// 图像的背景颜色
	DWORD		m_dwWidth;		// 图像宽度
	DWORD		m_dwHeight;		// 图像高度

	// 颜色表和图例
	COLORREF m_ColorTable[256];	// 256色颜色表
	Legend m_Legend[256];		// 图例

	// 是否空数据集
	BOOL m_bEmpty;

	// 计量单位
	TCHAR m_szUnit[32];

	// 操作器
	KRadarDataOperator* m_pRadarDataOperator;

	public:
		static bool IsRadarData(LPCTSTR lpszFile);
};
