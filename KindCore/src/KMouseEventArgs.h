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

#include "KEventArgs.h"

namespace Events
{

	class KIND_EXT_CLASS KMouseEventArgs : public KEventArgs
	{
	public:
		KMouseEventArgs()
			: KEventArgs()
		{
			m_Flag = 0;
			m_Point.x = 0;
			m_Point.y = 0;
			m_Tool = 0;
			m_pTarget = NULL;
		}
		KMouseEventArgs(const KMouseEventArgs& other)
			: KEventArgs(other)
		{
			m_Flag = other.m_Flag;
			m_Point = other.m_Point;
			m_Tool = other.m_Tool;
			m_pTarget = other.m_pTarget;
		}
		KMouseEventArgs(void* pSource, UINT flag, POINT point, int tool, void* pTarget)
			: KEventArgs(pSource)
		{
			m_Flag = flag;
			m_Point = point;
			m_Tool = tool;
			m_pTarget = pTarget;
		}
		virtual ~KMouseEventArgs(void)
		{
		}

	public:
		// ����־
		UINT GetFlag() { return m_Flag; }
		// ��ǰ������ڵ������(����)
		POINT GetPoint() { return m_Point; }
		// ��ȡ��ǰ�������ߵ��ӹ������͡�
		int& GetTool() { return m_Tool; }
		// ȡ�����ѡ��/�����Ķ���
		void* GetTarget() { return m_pTarget; }

	protected:
		UINT m_Flag;
		POINT m_Point;
		int m_Tool;
		void* m_pTarget;	// Ŀ��(�������ڴ����)
	};

//---------------------------------------------------------
}// namespace Events - end
//---------------------------------------------------------
