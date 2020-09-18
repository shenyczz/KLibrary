#ifndef __INCLUDED_KLIBRARY_H__
#define __INCLUDED_KLIBRARY_H__

#include "KLibrary.inl"

// 使用 STL、Windows 标准库
//
#include "KindResource.h"		// depend on: Independent
#include "KindBase.h"			// depend on: Independent
#include "KindCore.h"			// depend on: KindBase
#include "KindWin32.h"			// depend on: KindBase
#include "KindGeo.h"			// depend on: KindCore

// 数学
//
#include "KindMath.h"			// depend on: KindGeo

// 数据
//
#include "KindData.h"			// depend on: KindBase, KindCore, KindWin32, KindGeo
#include "KindAxin.h"			// depend on: KindMath, KindData
#include "KindAgmrs.h"			// depend on: KindData
#include "KindDiamond.h"		// depend on: KindData
#include "KindRadar.h"			// depend on: KindMath,KindData

// 使用 MFC 库
//
#include "KindMfc.h"			// depend on: KindBase, KindCore
#include "KindProvider.h"		// depend on: KindMfc, KindAgmrs, KindDiamond, KindLeap, KindRadar
#include "KindGis.h"			// depend on: KindProvider

#include "KLibraryApi.h"

#endif // #ifndef __INCLUDED_KLIBRARY_H__
