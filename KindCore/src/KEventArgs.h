/******************************************************************************
 ** KEventArgs.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KEventArgs
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 
 **
 ** Modifier: 
 ** DateTime: 
 **  Explain: 
 **
 **    Usage: 
 **
******************************************************************************/

#pragma once

#include "KObject.h"

namespace Events
{
//---------------------------------------------------------
}// namespace Events - end
//---------------------------------------------------------

namespace Events
{

	class KIND_EXT_CLASS KEventArgs
	{
	public:
		KEventArgs()
		{
			m_pSource = NULL;
		}
		KEventArgs(const KEventArgs& other)
		{
			m_pSource = other.m_pSource;
		}
		KEventArgs(void* pSource)
		{
			m_pSource = pSource;
		}
		virtual ~KEventArgs(void)
		{

		}

	public:
		// 取得源
		void* GetSource() { return m_pSource; }

	protected:
		// 源
		void* m_pSource;	// 不负责内存管理
	};

//---------------------------------------------------------
}// namespace Events - end
//---------------------------------------------------------
