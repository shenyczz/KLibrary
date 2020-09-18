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

#include "KRender.h"

namespace Renders
{

	class KIND_EXT_CLASS KDiamondGridDataRender : public KRender
	{
	public:
		KDiamondGridDataRender(void);
		virtual ~KDiamondGridDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	private:
		// 显示格点数据
		void drawGridValue(Graphics* pGraphics);
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
