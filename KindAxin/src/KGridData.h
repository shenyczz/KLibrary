/******************************************************************************
 ** KGridData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridData
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

class KIND_EXT_CLASS KGridData : public KAxinData
{
public:
	KGridData(void);
	virtual ~KGridData(void);

public:
	// 查找数据极小值和极大值
	virtual void LookupExtremum();

public:
	// 经纬度坐标转换为格点坐标
	BOOL LonLat2xy(double lon,double lat,int& x,int& y);
	// 格点坐标转换为经纬度坐标
	BOOL xy2LonLat(int x,int y,double& lon,double& lat);

	double GetGridValue(int row,int col);
	BOOL SetGridValue(int row,int col,double value);

	double GetGridValue(double lon,double lat);
	BOOL SetGridValue(double lon,double lat,double value);

public:
	// 导出 ENVI 文件头数据
	BOOL ExportEnviHdr(LPCTSTR lpszFile);

	// 导出 ArcGis bmp & bmpw
	BOOL ExportArcGis_bmp_bmpw(LPCTSTR lpszFile);
};

