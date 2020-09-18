/******************************************************************************
 ** KShapeRender.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KShapeRender
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

#include "KLayerProperty.h"

namespace Renders
{
//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------

namespace Renders
{

	class KIND_EXT_CLASS KRender : public KProgressObject
	{
	public:
		KRender(void);
		virtual ~KRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData) = 0;

	public:
		// 取得图层属性
		void SetLayerProperty(KLayerProperty* pLayerProperty) { m_pLayerProperty = pLayerProperty; }
		KLayerProperty* GetLayerProperty() const { return m_pLayerProperty; }

		// 画笔颜色
		//void SetPenColor(COLORREF clr) { m_clrPen = clr; }
		//COLORREF GetPenColor() { return m_clrPen; }

	protected:
		// 绘制等值线
		void DrawContour(Graphics* pGraphics, KContour* pContour);
		// 填充等值线
		void FillContour(Graphics* pGraphics, KContour* pContour);
		// 标注等值线
		void LabelContour(Graphics* pGraphics, KContour* pContour);

		// 绘制曲线
		void DrawCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr, BOOL bBold);
		// 绘制曲线
		void FillCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr);
		// 标注曲线
		void LabelCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr);

		// 标注数值
		void LabelValue(Graphics* pGraphics, double X, double Y, double V, COLORREF clr);

	protected:
		// 图层属性
		KLayerProperty* m_pLayerProperty;

		HDC m_hDC;
		LPVOID m_lpvData;

		COLORREF m_clrPen;	// 默认画笔颜色
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
