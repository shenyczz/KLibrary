/******************************************************************************
 ** KShapeDataRender.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KShapeDataRender
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

#include "KRender.h"

namespace Renders
{

	class KIND_EXT_CLASS KShapeDataRender : public KRender
	{
	public:
		KShapeDataRender(void);
		virtual ~KShapeDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	protected:
		void DrawGeometry(KGeometry* pGeometry, KRow* pRow, ShapeType eShapeType);

		void DrawPointCollection(KGeometry* pGeometry,KRow* pRow);
		void DrawPolylineCollection(KGeometry* pGeometry,KRow* pRow);
		void DrawPolygonCollection(KGeometry* pGeometry,KRow* pRow);

		void DrawPoint(KGeometry* pGeometry,KRow* pRow);
		void DrawPolyline(KGeometry* pGeometry,KRow* pRow);
		void DrawPolygon(KGeometry* pGeometry,KRow* pRow);

	private:
		Graphics* m_pGraphics;
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
