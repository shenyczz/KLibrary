#pragma once

#include "KDiamondData.h"

class KIND_EXT_CLASS KDiamondGridData : public KDiamondData
{
public:
	KDiamondGridData(void);
	virtual ~KDiamondGridData(void);

public:
	// 构造等值线
	virtual BOOL BuildContours();

public:
	// 取得等值线构造器
	KContourBuilder* GetContourBuilder() { return m_pContourBuilder; }

public:
	// 经纬度坐标转换为格点坐标
	BOOL LonLat2xy(double lon,double lat,int& x,int& y);
	// 格点坐标转换为经纬度坐标
	BOOL xy2LonLat(int x,int y,double& lon,double& lat);

	double GetGridValue(int row,int col);
	BOOL SetGridValue(int row,int col,double value);

	double GetGridValue(double lon,double lat);
	BOOL SetGridValue(double lon,double lat,double value);

protected:
	// 装载数据 - ASC文件
	virtual BOOL LoadData_txt(LPCTSTR lpszFile);

protected:
	// 查找数据极小值和极大值
	void LookupExtremum();

protected:
	// 等值线构造器
	KContourBuilder* m_pContourBuilder;
};

