#ifndef __INCLUDED_KindBase_H__
#define __INCLUDED_KindBase_H__

//-----------------------------------------------------------------------------
#include "KindBase.inl"

//---------------------------------------------------------
//                全局类定义
//---------------------------------------------------------
#include "KGlobal.h"				// 全局静态类
#include "KBytesOrder.h"			// 字节序静态类

#include "KExtents.h"				// 区块

#include "KObject.h"				// 对象
#include "KString.h"				// 字符串
#include "KException.h"				// 异常

#include "KObjectCollection.h"		// 栈对象搜集
#include "KObjectPtrCollection.h"	// 堆对象搜集

#include "KGridInfo.h"				// 网格信息
#include "KDataInfo.h"				// 数据信息
#include "KStation.h"				// 站点
#include "KStationInfo.h"			// 站点信息

//---------------------------------------------------------
//                接口定义
//---------------------------------------------------------

//---------------------------------------------------------
//                模板定义
//---------------------------------------------------------
#include "TMatrix.h"			// 数学矩阵
#include "TCollection.h"		// 对象搜集
#include "TPtrCollection.h"		// 对象指针搜集
#include "TEventHandler.h"		// 事件处理器

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		KIND_BASE_DLL_NAME
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindBase_H__
