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
		// ȡ��ͼ������
		void SetLayerProperty(KLayerProperty* pLayerProperty) { m_pLayerProperty = pLayerProperty; }
		KLayerProperty* GetLayerProperty() const { return m_pLayerProperty; }

		// ������ɫ
		//void SetPenColor(COLORREF clr) { m_clrPen = clr; }
		//COLORREF GetPenColor() { return m_clrPen; }

	protected:
		// ���Ƶ�ֵ��
		void DrawContour(Graphics* pGraphics, KContour* pContour);
		// ����ֵ��
		void FillContour(Graphics* pGraphics, KContour* pContour);
		// ��ע��ֵ��
		void LabelContour(Graphics* pGraphics, KContour* pContour);

		// ��������
		void DrawCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr, BOOL bBold);
		// ��������
		void FillCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr);
		// ��ע����
		void LabelCurve(Graphics* pGraphics, KCurve* pCurve, COLORREF clr);

		// ��ע��ֵ
		void LabelValue(Graphics* pGraphics, double X, double Y, double V, COLORREF clr);

	protected:
		// ͼ������
		KLayerProperty* m_pLayerProperty;

		HDC m_hDC;
		LPVOID m_lpvData;

		COLORREF m_clrPen;	// Ĭ�ϻ�����ɫ
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
