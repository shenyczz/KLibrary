/******************************************************************************
 ** KLayerProperty.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLayerProperty - 图层属性
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

namespace Layers
{
//---------------------------------------------------------
}// namespace Layers - end
//---------------------------------------------------------

namespace Layers
{

	// 图层属性控制
	enum LayerProperties : DWORD
	{
		eLayerProperty_None = 0,

		// 杂项
		eMisce = 0x1000,
			eMisce_Name,
			eMisce_Extents,
			eMisce_DateTime,
			eMisce_ElementNumber,

		// 站点
		eStation = eMisce,
		// 经度
		eLongtitude = eMisce,
		// 纬度
		eLatetitude = eMisce,
		// 数据尺寸
		eDataSize	= eMisce,

		// 通道
		eChannel = 0x4000,
			eRChannel						= eChannel + 0x0001,		// 0x4001
			eGChannel						= eChannel + 0x0002,		// 0x4002
			eBChannel						= eChannel + 0x0004,		// 0x4004

		// 地理信息
		eGeoInfo = 0x8000,

		// 控制
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

			// 矢量控制
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

			eControl_LogFont,											// 逻辑字体
			eControl_CurrentElementIndex,								// 当前站点索引

		// 图例
		eLegend = 0x20000000,
			eLegend_WindBarbColor			= eLegend + 0x0101,			// 0x20000001
			eLegend_WindArrowColor			= eLegend + 0x0102,			// 0x20000002
			eLegend_WindStreanLineColor		= eLegend + 0x0104,			// 0x20000004

		// 几何图形对象
		eGeometry = 0x40000000,
			eGeometry_Color					= eGeometry + 0x01,			// 0x40000001
			eGeometry_Label					= eGeometry + 0x02,			// 0x40000002
			eGeometry_LogFont				= eGeometry + 0x04,			// 0x40000004
			eGeometry_LineWidth				= eGeometry + 0x08,			// 0x40000008
			eGeometry_LineStyle				= eGeometry + 0x10,			// 0x40000010
			eGeometry_FillType				= eGeometry + 0x20,			// 0x40000020
			eGeometry_FillColor				= eGeometry + 0x21,			// 0x40000021 单色画刷使用(FillType=0)
			eGeometry_HatchStyle			= eGeometry + 0x22,			// 0x40000022 影线画刷使用(FillType=1)
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

	// 风场矢量
	enum WindVector : WORD
	{
		WindVector_None			= 0x00,		// 不显示
		WindVector_Barb			= 0x01,		// 风羽
		WindVector_Arrow		= 0x02,		// 风矢
		WindVector_StreamLine	= 0x04,		// 流线
	};

	// 字体类型
	enum FontType
	{
		FontType_Regular    = 0,	// 规则
		FontType_Bold       = 1,	// 粗体
		FontType_Italic     = 2,	// 斜体
		FontType_Underline  = 4,	// 下划线
		FontType_Strikeout  = 8,	// 删除线
	};

	class KIND_EXT_CLASS KLayerProperty : public KProperty
	{
	public:
		KLayerProperty(void);
		virtual ~KLayerProperty(void);

	public:
		// 复位图层属性
		void Reset();

	public:
		//-------------------------------------------------
		// 线型
		int& LineStyle() { return m_iLineStyle; }
		// 线宽
		int& LineWidth() { return m_iLineWidth; }
		// 透明度(%)
		int& Transparency() { return m_iTransparency; }
		const int& GetTransparency() { return m_iTransparency; }
		void SetTransparency(int iTransparency) { m_iTransparency = iTransparency; }

		// 颜色
		DWORD GetColor() const { return m_dwColor; }
		void SetColor(DWORD dwColor) { m_dwColor = dwColor; }
		DWORD& Color() { return m_dwColor; }

		// 是否允许删除
		bool IsAllowDelete() { return m_bAllowDelete; }
		void AllowDelete(bool bAllowDelete = true) { m_bAllowDelete = bAllowDelete; }

		// 是否允许移动
		bool IsAllowMove() { return m_bAllowMove; }
		void AllowMove(bool bAllowMove = true) { m_bAllowMove = bAllowMove; }

		// 是否可用
		bool IsDisable() { return m_bDisable; }
		void Disable(bool bDisable = true) { m_bDisable = bDisable; }

		// 是否可见
		bool IsVisible() { return m_bVisible; }
		void EnableVisible(bool bVisible = true) { m_bVisible = bVisible; }

		// 是否剪切
		bool IsClip() { return m_bClip; }
		void EnableClip(bool bClip = true) { m_bClip = bClip; }

		// 是否填充
		bool IsFill() { return m_bFill; }
		void EnableFill(bool bFill = true) { m_bFill = bFill; }

		// 是否标注
		bool IsLabel() { return m_bLabel; }
		void EnableLabel(bool bLabel = true) { m_bLabel = bLabel; }

		// 是否彩虹线
		bool IsRainbow() { return m_bRainbow; }
		void EnableRainbow(bool bRainbow = true) { m_bRainbow = bRainbow; }

		// 坐标显示属性
		bool IsShowCoordinate() const { return m_bShowCoordinate; }
		void EnableShowCoordinate(bool bShowCoordinate = true) { m_bShowCoordinate = bShowCoordinate; }

		// 格点数据显示属性
		bool IsShowData() const { return m_bShowData; }
		void EnableShowData(bool bShowData = true) { m_bShowData = bShowData; }

		// 站点名称显示属性
		bool IsShowStationName() const { return m_bShowStationName; }
		void EnableShowStationName(bool bShowStationName = true) { m_bShowStationName = bShowStationName; }

		// 是否参与剪切路径构造
		bool IsBuildClipPath() const { return m_bBuildClipPath; }
		void EnableBuildClipPath(bool bBuildClipPath = true) { m_bBuildClipPath = bBuildClipPath; }

		// 阈值
		double& ThresholdMin() { return m_dThreshold_min; }
		double& ThresholdMax() { return m_dThreshold_max; }

		// 曲线张力系数
		double& Tension() { return m_dTension; }
		//-------------------------------------------------
		// 风场矢量
		WORD& WindVector()				{ return m_WindVector; }
		// 矢量图形颜色
		COLORREF& BarbColor()			{ return m_clrBarb; }
		COLORREF& ArrowColor()			{ return m_clrArrow; }
		COLORREF& StreamLineColor()		{ return m_clrStreamLine; }
		// 矢量步长
		int& BarbStep()					{ return m_iBarbStep;};
		int& ArrowStep()				{ return m_iArrowStep; }
		int& StreamLineStep()			{ return m_iStreamLineStep; }
		// 风钩
		bool IsShowBarb() const { return (m_WindVector & WindVector_Barb) ? true : false;}
		void EnableBarb(bool bBarb = true)
		{
			if(bBarb)	m_WindVector |= WindVector_Barb;
			else		m_WindVector &= ~(WindVector_Barb);
		}
		// 风矢
		bool IsShowArrow() const { return (m_WindVector & WindVector_Arrow) ? true : false;}
		void EnableArrow(BOOL bArrow = TRUE)
		{
			if(bArrow)	m_WindVector |= WindVector_Arrow;
			else		m_WindVector &= ~(WindVector_Arrow);
		}
		// 流线
		bool IsShowStreamLine() const { return (m_WindVector & WindVector_StreamLine) ? true : false;}
		void EnableStreamLine(BOOL bStreamLine = TRUE)
		{
			if(bStreamLine)	m_WindVector |= WindVector_StreamLine;
			else			m_WindVector &= ~(WindVector_StreamLine);
		}
		//-------------------------------------------------
		// 逻辑字体
		LOGFONT& LogFont()				{ return m_lf; }
		// 字体颜色
		COLORREF& FontColor()			{ return m_clrFont; }
		//-------------------------------------------------
		KProvider* GetProvider()		{ return m_pProvider; }
		void SetProvider(KProvider* pProvider)	{ _delete(m_pProvider) m_pProvider = pProvider; }
		//-------------------------------------------------

	public:
		// 取得图层范围
		virtual const KExtents& GetExtents();

	protected:
		void Init();

	protected:
		// 线型 
		// PS_SOLID            0
		// PS_DASH             1       /* -------  */
		// PS_DOT              2       /* .......  */
		// PS_DASHDOT          3       /* _._._._  */
		// PS_DASHDOTDOT       4       /* _.._.._  */
		int m_iLineStyle;

		// 线宽
		int m_iLineWidth;

		// 透明度(%)
		int m_iTransparency;

		// 颜色
		DWORD m_dwColor;

		// 是否允许删除
		bool m_bAllowDelete;
		// 是否允许移动
		bool m_bAllowMove;
		// 是否不可用
		bool m_bDisable;
		// 是否可见
		bool m_bVisible;

		// 是否剪切
		bool m_bClip;
		// 是否填充
		bool m_bFill;
		// 是否标注
		bool m_bLabel;
		// 是否彩虹线
		bool m_bRainbow;
		// 是否显示坐标
		bool m_bShowCoordinate;
		// 是否显示格点数据
		bool m_bShowData;
		// 是否显示站点名称
		bool m_bShowStationName;

		// 阈值
		double m_dThreshold_min;
		double m_dThreshold_max;

		// 曲线张力系数
		double m_dTension;

		// 数据提供者
		KProvider* m_pProvider;

	protected:	// 风场矢量
		// 风场显示类型
		// 0:不显示
		// 1:		风羽
		// 2:		风矢
		// 4:		流线
		// 3=1+2:	风羽+风矢
		// 5=1+4:	风羽+流线
		// 6=2+4:	风矢+流线
		// 7=1+2+4: 风羽+风矢+流线
		WORD m_WindVector;
		// 矢量图形颜色
		COLORREF m_clrBarb;			// 风羽颜色
		COLORREF m_clrArrow;		// 风矢颜色
		COLORREF m_clrStreamLine;	// 流线颜色
		// 矢量步长
		int m_iBarbStep;			// 风羽步长
		int m_iArrowStep;			// 风矢步长
		int m_iStreamLineStep;		// 流线步长

	protected:
		LOGFONT m_lf;				// 逻辑字体
		COLORREF m_clrFont;			// 字体颜色

	protected:
		bool m_bBuildClipPath;			// 是否参与剪切路径构造
	};

//---------------------------------------------------------
}// namespace Layers - end
//---------------------------------------------------------
