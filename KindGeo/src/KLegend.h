/******************************************************************************
 ** KLegend.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KLegend
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

#include "KPoint.h"

namespace Geometries
{

	class KIND_EXT_CLASS KLegend : public KGeometry
	{
	public:
		KLegend(void);
		virtual ~KLegend(void);

	public:
		void SetPoint(KPoint point) { m_point = point; }
		const KPoint& GetPoint() { return m_point; }

		LPCTSTR GetUnit() { return m_strUnit; }
		void SetUnit(LPCTSTR lpszUnit) { m_strUnit = lpszUnit; }

		// 调色板
		KPalette* GetPalette() { return m_pPalette; }
		void SetPalette(KPalette* pPalette) { _delete(m_pPalette); m_pPalette = pPalette; }

	public:	// 属性
		// Width - 宽度
		__declspec(property(get=getWidth, put=putWidth)) int Width;
		int getWidth() { return m_iWidth; }
		void putWidth(int value) { m_iWidth = value;}

		// Height - 高度
		__declspec(property(get=getHeight, put=putHeight)) int Height;
		int getHeight() { return m_iHeight; }
		void putHeight(int value) { m_iHeight = value;}

		// Margin - 页边空白
		__declspec(property(get=getMargin, put=putMargin)) int Margin;
		int getMargin() { return m_iMargin; }
		void putMargin(int value) { m_iMargin = value;}

		// ItemWidth - 条目宽度
		__declspec(property(get=getItemWidth, put=putItemWidth)) int ItemWidth;
		int getItemWidth() { return m_iItemWidth; }
		void putItemWidth(int value) { m_iItemWidth = value;}

		// ItemHeight - 条目宽度
		__declspec(property(get=getItemHeight, put=putItemHeight)) int ItemHeight;
		int getItemHeight() { return m_iItemHeight; }
		void putItemHeight(int value) { m_iItemHeight = value;}

		// ItemGap - 条目间隙
		__declspec(property(get=getItemGap, put=putItemGap)) int ItemGap;
		int getItemGap() { return m_iItemGap; }
		void putItemGap(int value) { m_iItemGap = value;}

		// UnitWidth - 标注单位宽度
		__declspec(property(get=getUnitWidth, put=putUnitWidth)) int UnitWidth;
		int getUnitWidth() { return m_iUnitWidth; }
		void putUnitWidth(int value) { m_iUnitWidth = value;}

	protected:
		KPoint m_point;
		// 标注单位
		KString m_strUnit;
		KPalette* m_pPalette;

	private:
		int m_iWidth;			// 图例图片宽度
		int m_iHeight;			// 图例图片高度

		int m_iMargin;			// 页边空白
		int m_iItemWidth;		// 图例条目宽度
		int m_iItemHeight;		// 图例条目高度
		int m_iItemGap;			// 图例条目间隙

		int m_iUnitWidth;		// 标注单位宽度
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
