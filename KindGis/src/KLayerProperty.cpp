#include "StdAfx.h"
#include "KLayerProperty.h"

namespace Layers
{

	KLayerProperty::KLayerProperty(void) : KProperty(), m_pProvider(NULL)
	{
		this->Init();
	}

	KLayerProperty::~KLayerProperty(void)
	{
		_delete(m_pProvider);
	}

	// 复位
	void KLayerProperty::Reset()
	{
		this->Init();
	}

	// 初始化
	void KLayerProperty::Init()
	{
		//-------------------------------------------------
		m_iLineStyle		= 0;
		m_iLineWidth		= 1;
		m_iTransparency		= 0;

		m_dwColor			= RGB(0,0,0);


		m_bAllowDelete		= true;
		m_bAllowMove		= true;
		m_bDisable			= false;
		m_bVisible			= true;
		m_bClip				= false;
		m_bFill				= false;
		m_bLabel			= true;
		m_bRainbow			= true;
		m_bShowCoordinate	= false;
		m_bShowData			= false;
		m_bShowStationName	= false;
		m_bBuildClipPath	= false;

		m_dThreshold_min	= 0;
		m_dThreshold_max	= 0;

		m_dTension = 0.3;
		//-------------------------------------------------
		// 风场矢量
		m_WindVector		= WindVector_Barb | WindVector_Arrow | WindVector_StreamLine;
		// 矢量图形颜色
		m_clrBarb			= RGB(0,0,0);
		m_clrArrow			= RGB(0,255,255);
		m_clrStreamLine		= RGB(0,0,255);
		// 矢量步长
		m_iBarbStep			= 2;
		m_iArrowStep		= 2;
		m_iStreamLineStep	= 2;
		//-------------------------------------------------
		// 逻辑字体
		HFONT hFont = (HFONT)::GetStockObject(DEFAULT_GUI_FONT);
		::GetObject(hFont,sizeof(LOGFONT),&m_lf);
		_tcscpy(m_lf.lfFaceName,_T("宋体"));
		m_lf.lfHeight = -(LONG)(1.0 * 12.0 / 9 * 9);

		m_clrFont = RGB(0,0,128);
		//-------------------------------------------------
	}

	// 取得图层范围(数据提供者的范围)
	const KExtents& KLayerProperty::GetExtents()
	{
		if(m_pProvider)
		{
			// 设置图层范围
			this->SetExtents(m_pProvider->GetExtents());
		}

		return KObject::GetExtents();
	}

//---------------------------------------------------------
}// namespace Layers - end
//---------------------------------------------------------
