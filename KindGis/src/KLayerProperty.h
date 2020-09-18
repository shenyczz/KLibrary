/******************************************************************************
 ** KLayerProperty.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLayerProperty - ͼ������
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

namespace Layers
{
//---------------------------------------------------------
}// namespace Layers - end
//---------------------------------------------------------

namespace Layers
{

	// ͼ�����Կ���
	enum LayerProperties : DWORD
	{
		eLayerProperty_None = 0,

		// ����
		eMisce = 0x1000,
			eMisce_Name,
			eMisce_Extents,
			eMisce_DateTime,
			eMisce_ElementNumber,

		// վ��
		eStation = eMisce,
		// ����
		eLongtitude = eMisce,
		// γ��
		eLatetitude = eMisce,
		// ���ݳߴ�
		eDataSize	= eMisce,

		// ͨ��
		eChannel = 0x4000,
			eRChannel						= eChannel + 0x0001,		// 0x4001
			eGChannel						= eChannel + 0x0002,		// 0x4002
			eBChannel						= eChannel + 0x0004,		// 0x4004

		// ������Ϣ
		eGeoInfo = 0x8000,

		// ����
		eControl = 0x10000000,
			eControl_Clip					= eControl + 0x0001,		// 0x10000001
			eControl_Fill					= eControl + 0x0002,		// 0x10000002
			eControl_Label					= eControl + 0x0004,		// 0x10000004
			eControl_Rainbow				= eControl + 0x0008,		// 0x10000008

			eControl_ShowData				= eControl + 0x0010,		// 0x10000010
			eControl_Transparency			= eControl + 0x0020,		// 0x10000020
			eControl_Color					= eControl + 0x0040,		// 0x10000040
			eControl_ShowCoordinate			= eControl + 0x0080,		// 0x10000080

			eControl_Threshold_min			= eControl + 0x0100,		// 0x10000100
			eControl_Threshold_max			= eControl + 0x0200,		// 0x10000200
			eControl_Visible				= eControl + 0x0400,		// 0x10000400
			eControl_ShowStationName		= eControl + 0x0800,		// 0x10000800
			eControl_CurrentFactor			= eControl + 0x0801,		// 0x10000801
			eControl_Tension				= eControl + 0x0802,		// 0x10000802

			// ʸ������
			eControl_WindBarb				= eControl + 0x10000,		// 0x10010000
			eControl_WindArrow				= eControl + 0x20000,		// 0x10020000
			eControl_WindStreamLine			= eControl + 0x40000,		// 0x10040000
			eControl_0x80000				= eControl + 0x80000,		// 0x10080000

			eControl_WindBarbStep			= eControl + 0x100000,		// 0x10100000
			eControl_WindArrowStep			= eControl + 0x200000,		// 0x10200000
			eControl_WindStreamLineStep		= eControl + 0x400000,		// 0x10400000
			eControl_0x800000				= eControl + 0x800000,		// 0x10800000

			eControl_LineWidth				= eControl + 0x1000000,		// 0x11000000
			eControl_LineStyle				= eControl + 0x2000000,		// 0x12000000

			eControl_LogFont,											// �߼�����
			eControl_CurrentElementIndex,								// ��ǰվ������

		// ͼ��
		eLegend = 0x20000000,
			eLegend_WindBarbColor			= eLegend + 0x0101,			// 0x20000001
			eLegend_WindArrowColor			= eLegend + 0x0102,			// 0x20000002
			eLegend_WindStreanLineColor		= eLegend + 0x0104,			// 0x20000004

		// ����ͼ�ζ���
		eGeometry = 0x40000000,
			eGeometry_Color					= eGeometry + 0x01,			// 0x40000001
			eGeometry_Label					= eGeometry + 0x02,			// 0x40000002
			eGeometry_LogFont				= eGeometry + 0x04,			// 0x40000004
			eGeometry_LineWidth				= eGeometry + 0x08,			// 0x40000008
			eGeometry_LineStyle				= eGeometry + 0x10,			// 0x40000010
			eGeometry_FillType				= eGeometry + 0x20,			// 0x40000020
			eGeometry_FillColor				= eGeometry + 0x21,			// 0x40000021 ��ɫ��ˢʹ��(FillType=0)
			eGeometry_HatchStyle			= eGeometry + 0x22,			// 0x40000022 Ӱ�߻�ˢʹ��(FillType=1)
			eGeometry_Tension				= eGeometry + 0x40,			// 0x40000040
			eGeometry_Transparency			= eGeometry + 0x80,			// 0x40000080

			eGeometry_Legend_Width			= eGeometry + 0x100,		// 0x40000100
			eGeometry_Legend_Height			= eGeometry + 0x101,		// 0x40000101
			eGeometry_Legend_Margin			= eGeometry + 0x102,		// 0x40000102
			eGeometry_Legend_ItemWidth		= eGeometry + 0x104,		// 0x40000104
			eGeometry_Legend_ItemHeight		= eGeometry + 0x108,		// 0x40000108
			eGeometry_Legend_ItemGap		= eGeometry + 0x110,		// 0x40000110
			eGeometry_Legend_UnitWidth		= eGeometry + 0x111,		// 0x40000111

	};

	// �糡ʸ��
	enum WindVector : WORD
	{
		WindVector_None			= 0x00,		// ����ʾ
		WindVector_Barb			= 0x01,		// ����
		WindVector_Arrow		= 0x02,		// ��ʸ
		WindVector_StreamLine	= 0x04,		// ����
	};

	// ��������
	enum FontType
	{
		FontType_Regular    = 0,	// ����
		FontType_Bold       = 1,	// ����
		FontType_Italic     = 2,	// б��
		FontType_Underline  = 4,	// �»���
		FontType_Strikeout  = 8,	// ɾ����
	};

	class KIND_EXT_CLASS KLayerProperty : public KProperty
	{
	public:
		KLayerProperty(void);
		virtual ~KLayerProperty(void);

	public:
		// ��λͼ������
		void Reset();

	public:
		//-------------------------------------------------
		// ����
		int& LineStyle() { return m_iLineStyle; }
		// �߿�
		int& LineWidth() { return m_iLineWidth; }
		// ͸����(%)
		int& Transparency() { return m_iTransparency; }
		const int& GetTransparency() { return m_iTransparency; }
		void SetTransparency(int iTransparency) { m_iTransparency = iTransparency; }

		// ��ɫ
		DWORD GetColor() const { return m_dwColor; }
		void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
		DWORD& Color() { return m_dwColor; }

		// �Ƿ�����ɾ��
		bool IsAllowDelete() { return m_bAllowDelete; }
		void AllowDelete(bool bAllowDelete = true) { m_bAllowDelete = bAllowDelete; }

		// �Ƿ������ƶ�
		bool IsAllowMove() { return m_bAllowMove; }
		void AllowMove(bool bAllowMove = true) { m_bAllowMove = bAllowMove; }

		// �Ƿ����
		bool IsDisable() { return m_bDisable; }
		void Disable(bool bDisable = true) { m_bDisable = bDisable; }

		// �Ƿ�ɼ�
		bool IsVisible() { return m_bVisible; }
		void EnableVisible(bool bVisible = true) { m_bVisible = bVisible; }

		// �Ƿ����
		bool IsClip() { return m_bClip; }
		void EnableClip(bool bClip = true) { m_bClip = bClip; }

		// �Ƿ����
		bool IsFill() { return m_bFill; }
		void EnableFill(bool bFill = true) { m_bFill = bFill; }

		// �Ƿ��ע
		bool IsLabel() { return m_bLabel; }
		void EnableLabel(bool bLabel = true) { m_bLabel = bLabel; }

		// �Ƿ�ʺ���
		bool IsRainbow() { return m_bRainbow; }
		void EnableRainbow(bool bRainbow = true) { m_bRainbow = bRainbow; }

		// ������ʾ����
		bool IsShowCoordinate() const { return m_bShowCoordinate; }
		void EnableShowCoordinate(bool bShowCoordinate = true) { m_bShowCoordinate = bShowCoordinate; }

		// ���������ʾ����
		bool IsShowData() const { return m_bShowData; }
		void EnableShowData(bool bShowData = true) { m_bShowData = bShowData; }

		// վ��������ʾ����
		bool IsShowStationName() const { return m_bShowStationName; }
		void EnableShowStationName(bool bShowStationName = true) { m_bShowStationName = bShowStationName; }

		// �Ƿ�������·������
		bool IsBuildClipPath() const { return m_bBuildClipPath; }
		void EnableBuildClipPath(bool bBuildClipPath = true) { m_bBuildClipPath = bBuildClipPath; }

		// ��ֵ
		double& ThresholdMin() { return m_dThreshold_min; }
		double& ThresholdMax() { return m_dThreshold_max; }

		// ��������ϵ��
		double& Tension() { return m_dTension; }
		//-------------------------------------------------
		// �糡ʸ��
		WORD& WindVector()				{ return m_WindVector; }
		// ʸ��ͼ����ɫ
		COLORREF& BarbColor()			{ return m_clrBarb; }
		COLORREF& ArrowColor()			{ return m_clrArrow; }
		COLORREF& StreamLineColor()		{ return m_clrStreamLine; }
		// ʸ������
		int& BarbStep()					{ return m_iBarbStep;};
		int& ArrowStep()				{ return m_iArrowStep; }
		int& StreamLineStep()			{ return m_iStreamLineStep; }
		// �繳
		bool IsShowBarb() const { return (m_WindVector & WindVector_Barb) ? true : false;}
		void EnableBarb(bool bBarb = true)
		{
			if(bBarb)	m_WindVector |= WindVector_Barb;
			else		m_WindVector &= ~(WindVector_Barb);
		}
		// ��ʸ
		bool IsShowArrow() const { return (m_WindVector & WindVector_Arrow) ? true : false;}
		void EnableArrow(BOOL bArrow = TRUE)
		{
			if(bArrow)	m_WindVector |= WindVector_Arrow;
			else		m_WindVector &= ~(WindVector_Arrow);
		}
		// ����
		bool IsShowStreamLine() const { return (m_WindVector & WindVector_StreamLine) ? true : false;}
		void EnableStreamLine(BOOL bStreamLine = TRUE)
		{
			if(bStreamLine)	m_WindVector |= WindVector_StreamLine;
			else			m_WindVector &= ~(WindVector_StreamLine);
		}
		//-------------------------------------------------
		// �߼�����
		LOGFONT& LogFont()				{ return m_lf; }
		// ������ɫ
		COLORREF& FontColor()			{ return m_clrFont; }
		//-------------------------------------------------
		KProvider* GetProvider()		{ return m_pProvider; }
		void SetProvider(KProvider* pProvider)	{ _delete(m_pProvider) m_pProvider = pProvider; }
		//-------------------------------------------------

	public:
		// ȡ��ͼ�㷶Χ
		virtual const KExtents& GetExtents();

	protected:
		void Init();

	protected:
		// ���� 
		// PS_SOLID            0
		// PS_DASH             1       /* -------  */
		// PS_DOT              2       /* .......  */
		// PS_DASHDOT          3       /* _._._._  */
		// PS_DASHDOTDOT       4       /* _.._.._  */
		int m_iLineStyle;

		// �߿�
		int m_iLineWidth;

		// ͸����(%)
		int m_iTransparency;

		// ��ɫ
		DWORD m_dwColor;

		// �Ƿ�����ɾ��
		bool m_bAllowDelete;
		// �Ƿ������ƶ�
		bool m_bAllowMove;
		// �Ƿ񲻿���
		bool m_bDisable;
		// �Ƿ�ɼ�
		bool m_bVisible;

		// �Ƿ����
		bool m_bClip;
		// �Ƿ����
		bool m_bFill;
		// �Ƿ��ע
		bool m_bLabel;
		// �Ƿ�ʺ���
		bool m_bRainbow;
		// �Ƿ���ʾ����
		bool m_bShowCoordinate;
		// �Ƿ���ʾ�������
		bool m_bShowData;
		// �Ƿ���ʾվ������
		bool m_bShowStationName;

		// ��ֵ
		double m_dThreshold_min;
		double m_dThreshold_max;

		// ��������ϵ��
		double m_dTension;

		// �����ṩ��
		KProvider* m_pProvider;

	protected:	// �糡ʸ��
		// �糡��ʾ����
		// 0:����ʾ
		// 1:		����
		// 2:		��ʸ
		// 4:		����
		// 3=1+2:	����+��ʸ
		// 5=1+4:	����+����
		// 6=2+4:	��ʸ+����
		// 7=1+2+4: ����+��ʸ+����
		WORD m_WindVector;
		// ʸ��ͼ����ɫ
		COLORREF m_clrBarb;			// ������ɫ
		COLORREF m_clrArrow;		// ��ʸ��ɫ
		COLORREF m_clrStreamLine;	// ������ɫ
		// ʸ������
		int m_iBarbStep;			// ���𲽳�
		int m_iArrowStep;			// ��ʸ����
		int m_iStreamLineStep;		// ���߲���

	protected:
		LOGFONT m_lf;				// �߼�����
		COLORREF m_clrFont;			// ������ɫ

	protected:
		bool m_bBuildClipPath;			// �Ƿ�������·������
	};

//---------------------------------------------------------
}// namespace Layers - end
//---------------------------------------------------------
