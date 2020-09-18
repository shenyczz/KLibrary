/******************************************************************************
 ** ICollection<T>.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: ICollection<T>
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

template <typename TSender, typename TEventArgs>
class TEventHandler
{
public:
	TEventHandler() {}
	virtual ~TEventHandler() {}

protected:
	TSender m_Sender;
	TEventArgs m_EventArgs;
};
