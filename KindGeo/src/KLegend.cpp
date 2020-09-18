#include "StdAfx.h"
#include "KLegend.h"

namespace Geometries
{

	KLegend::KLegend(void)
	{
		SetType(GeometryType_Legend);
		m_strUnit = _T("");
		m_pPalette = NULL;

		m_iWidth = 100;			// 图例图片宽度
		m_iHeight = 0;			// 图例图片高度(自动计算)

		m_iMargin		= 5;		// 页边空白
		m_iItemWidth	= 20;		// 图例条目宽度
		m_iItemHeight	= 12;		// 图例条目高度
		m_iItemGap		= 2;		// 图例条目间隙

		m_iUnitWidth	= 24;		// 标注单位宽度
	}

	KLegend::~KLegend(void)
	{
		_delete(m_pPalette);
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
