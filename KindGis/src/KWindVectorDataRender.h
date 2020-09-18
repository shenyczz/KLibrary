/******************************************************************************
 ** KWindVectorDataRender.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KWindVectorDataRender - �糡ʸ����Ⱦ��
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

#include "KRender.h"

namespace Renders
{

	class KIND_EXT_CLASS KWindVectorDataRender : public KRender
	{
	public:
		KWindVectorDataRender(void);
		virtual ~KWindVectorDataRender(void);

	public:
		virtual void Rendering(HDC hDC, LPVOID lpvData);

	protected:
		// ���Ʒ���
		void DrawBarb(Graphics* pGraphics);
		// ���Ʒ��
		void DrawArrow(Graphics* pGraphics);
		// ��������
		void DrawStreamLine(Graphics* pGraphics);

	private:
		// ��ĳλ�û��Ʒ���
		void _DrawOneBarb(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr);
		// ��ĳλ�û��Ʒ��
		void _DrawOneArrow(Graphics* pGraphics,double x,double y,double dDirection,double dSpeed,COLORREF clr);
		// �������ϻ��Ƽ�ͷ
		void _StreamArrow(Graphics* pGraphics,double xx1,double yy1,double xx2,double yy2,COLORREF clr);

		// ��������Ƕ�
		double AdjustmentAngle(double lon,double lat);
	};

//---------------------------------------------------------
}// namespace Renders - end
//---------------------------------------------------------
