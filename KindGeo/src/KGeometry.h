/******************************************************************************
 ** KGeometry.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KGeometry
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

namespace Geometries
{
//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------

namespace Geometries
{

	enum GeometryType : unsigned short
	{
		// 几何图形
		GeometryType_Geometry	= 3000,

		GeometryType_Coordinate,			// 坐标

		GeometryType_Point,					// 点
		GeometryType_Line,					// 线
		GeometryType_Surface,				// 面
		GeometryType_Volume,				// 体

		GeometryType_Rect,					// 矩形
		GeometryType_Ellipse,				// 椭圆
		GeometryType_Polyline,				// 折线
		GeometryType_Polygon,				// 多边形
		GeometryType_Curve,					// 光滑曲线
		GeometryType_Contour,				// 等值线

		GeometryType_Label,					// 标注
		GeometryType_Legend,				// 图例

		GeometryType_GeometryCollection	= 3500,
		GeometryType_CoordinateCollection,	// 坐标集

		GeometryType_PointCollection,		// 点集
		GeometryType_LineCollection,		// 线集
		GeometryType_SurfaceCollection,		// 面集
		GeometryType_VolumeCollection,		// 体集

		GeometryType_RectCollection,		// 矩形集
		GeometryType_EllipseCollection,		// 椭圆集
		GeometryType_PolylineCollection,	// 折线集
		GeometryType_PolygonCollection,		// 多边形集
		GeometryType_CurveCollection,		// 光滑曲线集
		GeometryType_ContourCollection,		// 等值线集
	};

	class KIND_EXT_CLASS KGeometry : public KObject
	{
	public:
		KGeometry(void)
		{
			this->SetType(GeometryType_Geometry);

			m_clr			= 0;
			m_iLineWidth	= 1;
			m_iLineStyle	= PS_SOLID;
			m_iFillType		= -1;
			m_clrFill		= RGB(0,0,222);
			m_iHatchStyle	= 0;
			m_iTransparent	= 0;
		}
		KGeometry(const KGeometry& geometry) : KObject(geometry)
		{
			m_clr			= geometry.m_clr;
			m_iLineWidth	= geometry.m_iLineWidth;
			m_iLineStyle	= geometry.m_iLineStyle;
			m_iFillType		= geometry.m_iFillType;
			m_clrFill		= geometry.m_clrFill;
			m_iHatchStyle	= geometry.m_iHatchStyle;
			m_iTransparent	= geometry.m_iTransparent;
		}
		virtual ~KGeometry(void)
		{
		}

	public:
		// 颜色
		void SetColor(COLORREF clr) { m_clr = clr; }
		const COLORREF& GetColor() { return m_clr; }

		// 线宽
		void SetLineWidth(int iLineWidth) { m_iLineWidth = iLineWidth; }
		const int& GetLineWidth() { return m_iLineWidth; }

		// 线型
		void SetLineStyle(int iLineStyle) { m_iLineStyle = iLineStyle; } 
		const int& GetLineStyle() { return m_iLineStyle; }

		// Fill Type
		void SetFillType(int iFillType) { m_iFillType = iFillType; }
		const int& GetFillType() { return m_iFillType; }

		// Fill Color
		void SetFillColor(COLORREF clr) { m_clrFill = clr; }
		const COLORREF& GetFillColor() { return m_clrFill; }

		// HatchStyle
		void SetHatchStyle(int iHatchStyle) { m_iHatchStyle = iHatchStyle; }
		const int& GetHatchStyle() { return m_iHatchStyle; }

		// Transparent
		void SetTransparency(int iTransparent) { m_iTransparent = iTransparent; }
		const int& GetTransparency() { return m_iTransparent; }

	protected:
		COLORREF m_clr;			// 颜色
		int m_iLineWidth;		// 线宽
		int m_iLineStyle;		// 线型

		int m_iFillType;		// 填充类型

		COLORREF m_clrFill;		// 填充颜色
		int m_iHatchStyle;		// 影线风格

		int m_iTransparent;		// 透明度
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
