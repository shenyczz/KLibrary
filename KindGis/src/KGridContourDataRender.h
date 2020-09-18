/******************************************************************************
 ** KGridContourDataRender.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGridContourDataRender
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

#include "KRender.h"

namespace Renders
{

	class KIND_EXT_CLASS KGridContourDataRender : public KRender
	{
	public:
		KGridContourDataRender(void);
		virtual ~KGridContourDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	private:
		// ��ʾ�������
		void drawGridValue(Graphics* pGraphics);
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
