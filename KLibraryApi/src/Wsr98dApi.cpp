// Wsr98dApi.cpp : 定义 DLL 应用程序的导出函数。

#include "stdafx.h"
#include "Wsr98dApi.h"


namespace Wsr98dApi
{

	MASC_API bool _STDCALL RPG_LoadData(LPCTSTR lpszFile)
	{
		bool bret = false;

		bret = g_RpgData.LoadData(lpszFile) ? true : false;

		return bret;
	}

	MASC_API HBITMAP _STDCALL RPG_BuildImage(float fZoom, bool bDrawCoordinate, bool bDrawInfoArea)
	{
		HBITMAP hBitmap = NULL;

		if (g_RpgData.BuildImage(fZoom, bDrawCoordinate, bDrawInfoArea))
		{
			hBitmap = g_RpgData.GetImageHandle();
		}

		return hBitmap;
	}
	MASC_API HBITMAP _STDCALL RPG_ToImageColor()
	{
		HBITMAP hBitmap = NULL;

		if (g_RpgData.BuildImage(1, FALSE, FALSE))
		{
			hBitmap = g_RpgData.GetImageHandle();
		}

		return hBitmap;
	}
	MASC_API HBITMAP _STDCALL RPG_ToImageGray()
	{
		return g_RpgData.ToGrayImage();
	}

	MASC_API int _STDCALL RPG_ToGrid(double*& pData, RadarGridInfo& radarGridInfo)
	{
		if (g_RpgData.IsEmpty())
			return -1;

		KDataInfo dataInfo;
		if (!g_RpgData.ToGrid(pData, dataInfo, 0, 15))
			return -2;

		//{格点数据信息
		radarGridInfo.xNum		= dataInfo.xNum;
		radarGridInfo.xMin		= dataInfo.xMin;
		radarGridInfo.xMax		= dataInfo.xMax;
		radarGridInfo.xInterval = dataInfo.xInterval;

		radarGridInfo.yNum		= dataInfo.yNum;
		radarGridInfo.yMin		= dataInfo.yMin;
		radarGridInfo.yMax		= dataInfo.yMax;
		radarGridInfo.yInterval = dataInfo.yInterval;

		radarGridInfo.wYear		= dataInfo.wYear;
		radarGridInfo.wMonth	= dataInfo.wMonth;
		radarGridInfo.wDay		= dataInfo.wDay;
		radarGridInfo.wHour		= dataInfo.wHour;
		radarGridInfo.wMinute	= dataInfo.wMinute;
		radarGridInfo.wSecond	= dataInfo.wSecond;
		//}格点数据信息

		return 0;
	}









}//
