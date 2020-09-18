#include "StdAfx.h"
#include "KGlobal.h"

// ���徲̬����
char KGlobal::g_szReturn[2049] = "";
wchar_t KGlobal::g_wszReturn[2049] = L"";

KGlobal::KGlobal(void)
{
}

KGlobal::~KGlobal(void)
{
}

// ��������(��1970.1.1���������)ת����������������
void KGlobal::JulianDate2GMTDate(long lJDate,int* iYear,int* iMonth,int* iDay)
{
	int JulianDate = lJDate;
	int JLDAYN = JulianDate+2440587;		// 

	int L   = JLDAYN + 68569 ;
	int N   = 4 * L / 146097 ;
		L  = L - (146097 * N + 3) / 4 ;

	int I   = 4000 * (L + 1) / 1461001 ;
		L   = L - 1461 * I / 4 + 31 ;
	int J   = 80 * L / 2447 ;

	int	day  = L - 2447 * J / 80 ;

		L  = J / 11 ;

	int	month  = J + 2 - 12 * L ;
	int	year  = 100 * (N - 49) + I + L ;

	*iYear = year;
	*iMonth = month;
	*iDay = day;
}

// ������ʱ��ת����24Сʱ��ʱ��
void KGlobal::JulianTime2GMTTime(long lJTime,int* iHour,int* iMinute,int* iSecond)
{
	int Seconds = lJTime;
	//--------------------------------------------
	int hour = Seconds/3600;
	int minute = (Seconds - hour*3600)/60 ;
	int seconds = Seconds - hour*3600 - minute*60 ;
	//--------------------------------------------
	*iHour = hour;
	*iMinute = minute;
	*iSecond = seconds;
}
