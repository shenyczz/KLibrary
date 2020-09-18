#include "StdAfx.h"
#include "KMouseEvent.h"

namespace Events
{

	KMouseEvent::KMouseEvent(const KMouseEventArgs& mouseEventArgs)
	{
		m_MouseEventArgs = mouseEventArgs;
	}

	KMouseEvent::~KMouseEvent(void)
	{
	}

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------
