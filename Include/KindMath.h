#ifndef __INCLUDED_KindMath_H__
#define __INCLUDED_KindMath_H__

//-----------------------------------------------------------------------------
#include "KindMath.inl"

// ������
//
#include "KMath.h"
#include "KRandom.h"
#include "KMatrix.h"
#include "KLinearEquation.h"

#include "KSmooth.h"
#include "KBarnes.h"

#include "KGridDensify.h"
#include "KGridExtract.h"

#include "KV2VInterpolate.h"
#include "KV2GInterpolate.h"
#include "KG2VInterpolate.h"

// ����ͳ��
//
#include "KStatistic.h"
#include "KRegression.h"
#include "KFitting.h"
#include "KPolynomial.h"
#include "KIntegral.h"
#include "KInterpolate.h"
#include "KSFunction.h"

// ��ֵ�߹���
//
#include "KContourBuilder.h"
#include "KQContourBuilder.h"
#include "KTContourBuilder.h"

#include "KContourTracer.h"
#include "KQContourTracer.h"
#include "KTContourTracer.h"

// ��ʽ����
// 
#include "KPhysConst.h"
#include "KFormelParser.h"

//-----------------------------------------------------------------------------
#ifdef DLL_NAME
	#undef DLL_NAME
	#define DLL_NAME		"KindMath"
#endif
//-----------------------------------------------------------------------------
#define LIB_COMMENT			SFX_LIB
#define DISPLAY_MESSAGE		TOSTRING(link_message)SFX_DLL

#pragma comment(lib, LIB_COMMENT) 
#pragma message(DISPLAY_MESSAGE)
//-----------------------------------------------------------------------------

#endif // #ifndef __INCLUDED_KindMath_H__
