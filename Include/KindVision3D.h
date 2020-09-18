#ifndef __INCLUDED_KindVision3D_H__
#define __INCLUDED_KindVision3D_H__

//-----------------------------------------------------------------------------
#include "KindVision3D.inl"

#include "CustomVertex.h"

#include "KTarget3D.h"
#include "KRenderableTarget3D.h"
#include "KWorld.h"
#include "KEarth.h"

#include "KCamera.h"
#include "KFrustum.h"
#include "KPoint3D.h"
#include "KQuaternion.h"
#include "KRenderEngine.h"
#include "KVector3.h"
#include "KViewport3D.h"
#include "KMathEngine.h"

#include "KWorldCamera.h"
#include "KEarthCamera.h"

//-----------------------------------------------------------------------------
#ifdef _DEBUG	// Debug

	#if defined(_UNICODE)
		#define _KindVision3D_Comment_ "KindVision3Dud.lib"
		#define _KindVision3D_Message_ "Automatically linking with KindVision3Dud.dll"
	#else
		#define _KindVision3D_Comment_ "KindVision3Dd.lib" 
		#define _KindVision3D_Message_ "Automatically linking with KindVision3Dd.dll"
	#endif

#else			// Release

	#if defined(_UNICODE)
		#define _KindVision3D_Comment_ "KindVision3Du.lib"
		#define _KindVision3D_Message_ "Automatically linking with KindVision3Du.dll"
	#else
		#define _KindVision3D_Comment_ "KindVision3D.lib" 
		#define _KindVision3D_Message_ "Automatically linking with KindVision3D.dll"
	#endif

#endif
//-----------------------------------------------------------------------------
#pragma comment(lib, _KindVision3D_Comment_) 
#pragma message(_KindVision3D_Message_)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindVision3D_H__
