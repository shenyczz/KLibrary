/******************************************************************************
 ** KRpgDataRender.h

 ** Announce: if it works, I know it was written by
                ShenYongchen(shenyczz@163.com),lives in ZhengZhou,HeNan.
              if not, then I don't know nothing.

 **     Name: KRpgDataRender - 雷达产品数据 Rpg 图层绘制
 **  Version: 
 ** Function: 
 **  Explain: 

 **   Author: 申永辰.郑州 (shenyczz@163.com)
 ** DateTime: 2000 -
 ** Web Site: 

 ** Modifier: 
 ** DateTime: 
 **  Explain: 

 **    Usage: 

              // 1. 
              use

******************************************************************************/
#pragma once

#include "KRender.h"

namespace Renders
{

	class KIND_EXT_CLASS KRpgDataRender : public KRender
	{
	public:
		KRpgDataRender();
		virtual ~KRpgDataRender(void);

	public:
		// 绘制图层
		virtual void Rendering(HDC hDC,LPVOID lpvData);
		private:
			void render_RadialImage(HDC hDC,LPVOID lpvData);
			void render_RasterImage(HDC hDC,LPVOID lpvData);
			void render_LinkedContourVectorImage(HDC hDC,LPVOID lpvData);
			void render_GeographicAlphaImage(HDC hDC,LPVOID lpvData);
			void render_NonGeographicAlphaImage(HDC hDC,LPVOID lpvData);
			void render_AlphanumericImage(HDC hDC,LPVOID lpvData);
			void render_RasterImageSection(HDC hDC,LPVOID lpvData);
			void render_RasterImageSlice(HDC hDC,LPVOID lpvData);

	private:
		// 绘制极坐标
		void DrawPolar(Graphics* pGraphics);
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
