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

		// ��ɫ��
		KPalette* GetPalette() { return m_pPalette; }
		void SetPalette(KPalette* pPalette) { _delete(m_pPalette); m_pPalette = pPalette; }

	public:	// ����
		// Width - ���
		__declspec(property(get=getWidth, put=putWidth)) int Width;
		int getWidth() { return m_iWidth; }
		void putWidth(int value) { m_iWidth = value;}

		// Height - �߶�
		__declspec(property(get=getHeight, put=putHeight)) int Height;
		int getHeight() { return m_iHeight; }
		void putHeight(int value) { m_iHeight = value;}

		// Margin - ҳ�߿հ�
		__declspec(property(get=getMargin, put=putMargin)) int Margin;
		int getMargin() { return m_iMargin; }
		void putMargin(int value) { m_iMargin = value;}

		// ItemWidth - ��Ŀ���
		__declspec(property(get=getItemWidth, put=putItemWidth)) int ItemWidth;
		int getItemWidth() { return m_iItemWidth; }
		void putItemWidth(int value) { m_iItemWidth = value;}

		// ItemHeight - ��Ŀ���
		__declspec(property(get=getItemHeight, put=putItemHeight)) int ItemHeight;
		int getItemHeight() { return m_iItemHeight; }
		void putItemHeight(int value) { m_iItemHeight = value;}

		// ItemGap - ��Ŀ��϶
		__declspec(property(get=getItemGap, put=putItemGap)) int ItemGap;
		int getItemGap() { return m_iItemGap; }
		void putItemGap(int value) { m_iItemGap = value;}

		// UnitWidth - ��ע��λ���
		__declspec(property(get=getUnitWidth, put=putUnitWidth)) int UnitWidth;
		int getUnitWidth() { return m_iUnitWidth; }
		void putUnitWidth(int value) { m_iUnitWidth = value;}

	protected:
		KPoint m_point;
		// ��ע��λ
		KString m_strUnit;
		KPalette* m_pPalette;

	private:
		int m_iWidth;			// ͼ��ͼƬ���
		int m_iHeight;			// ͼ��ͼƬ�߶�

		int m_iMargin;			// ҳ�߿հ�
		int m_iItemWidth;		// ͼ����Ŀ���
		int m_iItemHeight;		// ͼ����Ŀ�߶�
		int m_iItemGap;			// ͼ����Ŀ��϶

		int m_iUnitWidth;		// ��ע��λ���
	};

//---------------------------------------------------------
}// namespace Geometries - end
//---------------------------------------------------------
