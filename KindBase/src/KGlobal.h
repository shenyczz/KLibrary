/******************************************************************************
 ** KGlobal.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KGlobal - 全局类
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

class KIND_EXT_CLASS KGlobal
{
protected:
	KGlobal(void);
	virtual ~KGlobal(void);

public:
	// 儒略日期(从1970.1.1至今的天数)转换到格林威治日期
	static void JulianDate2GMTDate(long lJDate,int* iYear,int* iMonth,int* iDay);

	// 儒略秒时间转换到24小时制时间
	static void JulianTime2GMTTime(long lJTime,int* iHour,int* iMinute,int* iSecond);

public:
	static char g_szReturn[2049];
	static wchar_t g_wszReturn[2049];
};
