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
 **   Author: ������.֣�� (shenyczz@163.com)
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
		// ȡ��Դ
		void* GetSource() { return m_pSource; }

	protected:
		// Դ
		void* m_pSource;	// �������ڴ����
	};

//---------------------------------------------------------
}// namespace Events - end
//---------------------------------------------------------
