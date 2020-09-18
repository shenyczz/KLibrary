/******************************************************************************
 ** KRadarDataOperator.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.
              All rights reserved.

 **     Name: KRadarDataOperator - 雷达数据操作类
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

class KIND_EXT_CLASS KRadarDataOperator : public KDataProcessor
{
public:
	KRadarDataOperator(void);
	virtual ~KRadarDataOperator(void);

public:
	// 儒略日期(从1970.1.1至今的天数)转换到格林威治日期
	void JulianDate2GMTDate(LONG lJDate,int* iYear,int* iMonth,int* iDay);
	// 儒略秒时间转换到24小时制时间
	void JulianTime2GMTTime(LONG lJTime,int* iHour,int* iMinute,int* iSecond);

public:
	// 设置/获取文件指针
	void SetFilePtr(FILE* fp)	{ m_fp = fp; }
	FILE* GetFilePtr()			{ return m_fp; }

	// 设置/获取文件指针偏移地址
	void SetFilePtrOffset(LONG lOffset)	{ m_lOffset = lOffset; }
	LONG GetFilePtrOffset()				{ return m_lOffset; }

protected:
	// 文件指针
	FILE* m_fp;
	// 文件指针偏移
	LONG m_lOffset;
	//灰度图
	KDib m_dibGray;

};
