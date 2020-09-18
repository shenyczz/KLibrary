#pragma once

#include "ApiCommon.h"
#include "Wsr98dStructs.h"

namespace Wsr98dApi{}
using namespace Wsr98dApi;

namespace Wsr98dApi
{

	MASC_API bool _STDCALL RPG_LoadData(LPCTSTR lpszFile);

	MASC_API HBITMAP _STDCALL RPG_BuildImage(float fZoom, bool bDrawCoordinate, bool bDrawInfoArea);
	MASC_API HBITMAP _STDCALL RPG_ToImageColor(void);
	MASC_API HBITMAP _STDCALL RPG_ToImageGray(void);

	//---------------------------------------------------------
	//序号:
	//功能:	RPG数据转换为格点数据
	//参数:	pData - 引用指针
	//		radarGridInfo - 雷达格点信息
	//返回:
	//说明:	RPG_LoadData()之后使用
	//用法:
	//
	//[DllImport(dll_name, CharSet = CharSet.Auto)]
	//public static extern bool RPG_ToGrid(out IntPtr p, out RadarGridInfo radarGridInfo);
	//---------------------------------------------------------
	MASC_API int _STDCALL RPG_ToGrid(double*& pData, RadarGridInfo& radarGridInfo);


}
