/******************************************************************************
 ** KGlobal.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KGlobal - ȫ����
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: ������.֣�� (shenyczz@163.com)
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
	// ��������(��1970.1.1���������)ת����������������
	static void JulianDate2GMTDate(long lJDate,int* iYear,int* iMonth,int* iDay);

	// ������ʱ��ת����24Сʱ��ʱ��
	static void JulianTime2GMTTime(long lJTime,int* iHour,int* iMinute,int* iSecond);

public:
	static char g_szReturn[2049];
	static wchar_t g_wszReturn[2049];
};
