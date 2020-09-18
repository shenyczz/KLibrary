#pragma once

#include "KRender.h"

namespace Renders
{

	class KIND_EXT_CLASS KBlackboardRender : public KRender
	{
	public:
		KBlackboardRender(void);
		virtual ~KBlackboardRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	protected:
		// ªÊ÷∆∂‘œÛ
		void DrawObjects(Graphics* pGraphics, KObjectPtrCollection* pObjectPtrCollection);

	private:
		void drawPoint(Graphics* pGraphics, KPoint* pPoint);
		void drawLine(Graphics* pGraphics, KLine* pLine);
		void drawRect(Graphics* pGraphics, KRect* pRect);
		void drawEllipse(Graphics* pGraphics, KEllipse* pEllipse);
		void drawPolyline(Graphics* pGraphics, KPolyline* pPolyline);
		void drawPolygon(Graphics* pGraphics, KPolygon* pPolygon);
		void drawCurve(Graphics* pGraphics, KCurve* pCurve);
		void drawLabel(Graphics* pGraphics, KLabel* pLabel);
		void drawLegend(Graphics* pGraphics, KLegend* pLegend);

		Image* buildLegendImage(KLegend* pLegend);

	protected:
		Image* m_pImage;
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
