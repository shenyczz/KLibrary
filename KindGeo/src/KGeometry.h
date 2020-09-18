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

namespace Geometries
{
//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------

namespace Geometries
{

	enum GeometryType : unsigned short
	{
		// ����ͼ��
		GeometryType_Geometry	= 3000,

		GeometryType_Coordinate,			// ����

		GeometryType_Point,					// ��
		GeometryType_Line,					// ��
		GeometryType_Surface,				// ��
		GeometryType_Volume,				// ��

		GeometryType_Rect,					// ����
		GeometryType_Ellipse,				// ��Բ
		GeometryType_Polyline,				// ����
		GeometryType_Polygon,				// �����
		GeometryType_Curve,					// �⻬����
		GeometryType_Contour,				// ��ֵ��

		GeometryType_Label,					// ��ע
		GeometryType_Legend,				// ͼ��

		GeometryType_GeometryCollection	= 3500,
		GeometryType_CoordinateCollection,	// ���꼯

		GeometryType_PointCollection,		// �㼯
		GeometryType_LineCollection,		// �߼�
		GeometryType_SurfaceCollection,		// �漯
		GeometryType_VolumeCollection,		// �弯

		GeometryType_RectCollection,		// ���μ�
		GeometryType_EllipseCollection,		// ��Բ��
		GeometryType_PolylineCollection,	// ���߼�
		GeometryType_PolygonCollection,		// ����μ�
		GeometryType_CurveCollection,		// �⻬���߼�
		GeometryType_ContourCollection,		// ��ֵ�߼�
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
		// ��ɫ
		void SetColor(COLORREF clr) { m_clr = clr; }
		const COLORREF& GetColor() { return m_clr; }

		// �߿�
		void SetLineWidth(int iLineWidth) { m_iLineWidth = iLineWidth; }
		const int& GetLineWidth() { return m_iLineWidth; }

		// ����
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
		COLORREF m_clr;			// ��ɫ
		int m_iLineWidth;		// �߿�
		int m_iLineStyle;		// ����

		int m_iFillType;		// �������

		COLORREF m_clrFill;		// �����ɫ
		int m_iHatchStyle;		// Ӱ�߷��

		int m_iTransparent;		// ͸����
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
