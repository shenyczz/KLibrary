#ifndef __INCLUDED_KLIBRARY_INL__
#define __INCLUDED_KLIBRARY_INL__

// KIND_EXT_CLASS
//
//
#if defined (KINDBASE_EXPORTS)			// KindBase 
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDCORE_EXPORTS)		// KindCore
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDWIN32_EXPORTS)		// KindWin32
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDIMAGE_EXPORTS)		// KindImage
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDNETWORK_EXPORTS)		// KindNetwork
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDRESOURCE_EXPORTS)	// KindResource
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDGEO_EXPORTS)			// KindGeo
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDMATH_EXPORTS)		// KindMath
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDPROJ_EXPORTS)		// KindProj
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDMETEO_EXPORTS)		// KindMeteo
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDDATA_EXPORTS)		// KindData
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDPALETTE_EXPORTS)		// KindPalette
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDSHAPE_EXPORTS)		// KindShape
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDGPS_EXPORTS)			// KindGps
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDRADAR_EXPORTS)		// KindRadar
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDSATELLITE_EXPORTS)	// KindSatellite
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDSNYC_EXPORTS)		// KindSnyc
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDDIAMOND_EXPORTS)		// KindDiamond
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDAGMRS_EXPORTS)		// KindAgmrs
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDAXIN_EXPORTS)		// KindAxin
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDMFC_EXPORTS)			// KindMfc
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDPROVIDER_EXPORTS)	// KindProvider
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDGIS_EXPORTS)			// KindGis
	#define KIND_EXT_CLASS __declspec(dllexport)
#elif defined (KINDVISION3D_EXPORTS)	// KindVision3D
	#define KIND_EXT_CLASS __declspec(dllexport)
#else
	#define KIND_EXT_CLASS //__declspec(dllimport)
#endif

// Windows 
//
#include <windows.h>

// stdlib
//
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// TCHAT
//
#include <tchar.h>

// Gdiplus
//
#include <comdef.h>						// Native C++ compiler COM support
#include <Gdiplus.h>
#pragma comment (lib, "Gdiplus.lib" )
using namespace Gdiplus;

// STL
//
#include <map>
#include <list>
#include <vector>
#include <string>
#include <algorithm>
using namespace std;

#ifdef UNICODE
	#define _tstring wstring
#else
	#define _tstring string
#endif


// 自定义
//
#include "KindDefs.inl"

// 自定义模板
//
#include "TPtrMap.h"
#include "TPtrList.h"
#include "TPtrVector.h"

#include "T2DMemory.h"
#include "T3DMemory.h"

#include "T2DMemAllocator.h"
#include "T3DMemAllocator.h"

//#include "TMatrix.h"

#pragma warning (disable : 4251)
#pragma warning (disable : 4275)
#pragma warning (disable : 4482)
#pragma warning (disable : 4996)		// unsafe function
#pragma warning( disable : 4290)
// warning C4290: 忽略 C++ 异常规范，但指示函数不是 __declspec(nothrow)

#pragma pack(1)
#pragma pack()

//---------------------------------------------------------
#define TOSTRING(s) s

#define link_message	"Automatically linking with "

#define DLL_NAME		"KLib"		// 动态库名称
#define dll_separator	""			// 分隔符,比如"_"
#define dll_version		"120"		// 动态库版本，建议和MFC库版本一致
#define dll_ext			".dll"		// 动态库扩展名
#define lib_ext			".lib"		// 连接库扩展名

// 运行环境标识
#ifdef _DEBUG
	#if defined(_UNICODE)
		#define dll_env "ud"		// Unicode Debug
	#else
		#define dll_env "d"			// Debug
	#endif
#else
	#if defined(_UNICODE)
		#define dll_env "ur"		// Unicode Release
	#else
		#define dll_env "r"			// Release
	#endif
#endif

#ifdef WIN32
	#define Platform_Architecture ""	// 平台结构：WIN32=>32，WIN64=>64
#endif

// 例子
//#define SFX_DLL	KLib_120_ur32.dll

#define SFX_DLL	\
	TOSTRING(DLL_NAME)\
	TOSTRING(dll_separator)\
	TOSTRING(dll_version)\
	TOSTRING(dll_separator)\
	TOSTRING(dll_env)\
	TOSTRING(Platform_Architecture)\
	TOSTRING(dll_ext)
#define SFX_LIB \
	TOSTRING(DLL_NAME)\
	TOSTRING(dll_separator)\
	TOSTRING(dll_version)\
	TOSTRING(dll_separator)\
	TOSTRING(dll_env)\
	TOSTRING(Platform_Architecture)\
	TOSTRING(lib_ext)

//---------------------------------------------------------
#define KIND_BASE_DLL_NAME	"KindBase"	// 在KindBase.h中使用
#define KIND_CORE_DLL_NAME	"KindCore"
//---------------------------------------------------------


#endif	//#ifndef __INCLUDED_KLIBRARY_INL__

