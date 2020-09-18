/******************************************************************************
 ** KMouseEvent.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KMouseEvent
 **  Version: 
 ** Function: 
 **  Explain: 
 **
 **   Author: …Í”¿≥Ω.÷£÷› (shenyczz@163.com)
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

#include "KMouseEventArgs.h"

namespace Events
{

	class KIND_EXT_CLASS KMouseEvent
	{
	public:
		KMouseEvent(const KMouseEventArgs& mouseEventArgs);
		~KMouseEvent(void);

	public:
		KMouseEventArgs& GetMouseEventArgs() { return m_MouseEventArgs; }

	protected:
		KMouseEventArgs m_MouseEventArgs;
	};

//---------------------------------------------------------
}// namespace Events - end
//---------------------------------------------------------
