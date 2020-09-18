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
		// 鼠标标志
		UINT GetFlag() { return m_Flag; }
		// 当前鼠标所在点的坐标(像素)
		POINT GetPoint() { return m_Point; }
		// 获取当前交互工具的子工具类型。
		int& GetTool() { return m_Tool; }
		// 取得鼠标选中/盘旋的对象
		void* GetTarget() { return m_pTarget; }

	protected:
		UINT m_Flag;
		POINT m_Point;
		int m_Tool;
		void* m_pTarget;	// 目标(不负责内存管理)
	};

//---------------------------------------------------------
}// namespace Events - end
//---------------------------------------------------------
