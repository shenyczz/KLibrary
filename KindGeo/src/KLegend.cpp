#include "StdAfx.h"
#include "KLegend.h"

namespace Geometries
{

	KLegend::KLegend(void)
	{
		SetType(GeometryType_Legend);
		m_strUnit = _T("");
		m_pPalette = NULL;

		m_iWidth = 100;			// ͼ��ͼƬ���
		m_iHeight = 0;			// ͼ��ͼƬ�߶�(�Զ�����)

		m_iMargin		= 5;		// ҳ�߿հ�
		m_iItemWidth	= 20;		// ͼ����Ŀ���
		m_iItemHeight	= 12;		// ͼ����Ŀ�߶�
		m_iItemGap		= 2;		// ͼ����Ŀ��϶

		m_iUnitWidth	= 24;		// ��ע��λ���
	}

	KLegend::~KLegend(void)
	{
		_delete(m_pPalette);
	}

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
