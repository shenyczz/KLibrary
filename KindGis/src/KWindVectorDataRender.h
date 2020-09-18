/******************************************************************************
 ** KWindVectorDataRender.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KWindVectorDataRender - 风场矢量渲染器
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

	class KIND_EXT_CLASS KWindVectorDataRender : public KRender
	{
	public:
		KWindVectorDataRender(void);
		virtual ~KWindVectorDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	protected:
		// 绘制风羽
		void DrawBarb(Graphics* pGraphics);
		// 绘制风箭
		void DrawArrow(Graphics* pGraphics);
		// 绘制流线
		void DrawStreamLine(Graphics* pGraphics);

	private:
		// 在某位置绘制风羽
		void _DrawOneBarb(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr);
		// 在某位置绘制风箭
		void _DrawOneArrow(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr);
		// 在流线上绘制箭头
		void _StreamArrow(Graphics* pGraphics,double xx1,double yy1,double xx2,double yy2,COLORREF clr);

		// 调整风向角度
		double AdjustmentAngle(double lon,double lat);
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
