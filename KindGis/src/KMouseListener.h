/******************************************************************************
 ** KMouseListener.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KMouseListener
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

#include "KMouseEvent.h"

namespace Mouse
{

	class KIND_EXT_CLASS KMouseListener
	{
	public:
		KMouseListener(void);
		virtual ~KMouseListener(void);

	public:
		// �����������¡�
		void OnLButtonDown(KMouseEvent* pMouseEvent);

		// ��������̧��
		void OnLButtonUp(KMouseEvent* pMouseEvent);

		// ��������˫����
		void OnLButtonDBLClick(KMouseEvent* pMouseEvent);

		// ����Ҽ������¡�
		void OnRButtonDown(KMouseEvent* pMouseEvent);

		// ����Ҽ���̧��
		void OnRButtonUp(KMouseEvent* pMouseEvent);

		// ����Ҽ���˫����
		void OnRButtonDBLClick(KMouseEvent* pMouseEvent);

		// ����ƶ�
		void OnMouseMove(KMouseEvent* pMouseEvent);

		// ����϶�
		void OnMouseDrag(KMouseEvent* pMouseEvent);

		// ȡ��������
		void OnCancel();

		// ��������ڶ����Ϸ�
		void OnMouseHover(KMouseEvent* pMouseEvent);

	public:
		// ����¼�
		KMouseEvent* GetMouseEvent() { return m_pMouseEvent; }
		//void SetMouseEvent(KMouseEvent* pMouseEvent) { _delete(m_pMouseEvent); m_pMouseEvent = pMouseEvent; }
		// ȡ�����һ�����λ��
		POINT GetLastPoint() { return m_lastPoint; }
		// ȡ���������״ΰ���ʱλ��
		POINT GetLButtonPoint() { return m_LButtonPoint; }

		// ͼ�λ������
		BOOL IsFinishedGeometry() { return m_bFinishedGeometry; }

		// ȡ�õ�����
		vector<POINT>* GetPointSequence() { return &m_PointSequence; }

	private:
		POINT m_lastPoint;		// ������λ��
		POINT m_LButtonPoint;	// �������״ΰ���ʱλ��

		POINT m_pointPrv;

		// ͼ�λ������
		bool m_bFinishedGeometry;

		vector<POINT> m_PointSequence;

		KMouseEvent* m_pMouseEvent;	// �������ڴ����
	};

	typedef map<int,KMouseListener*> KMouseListenerMap;

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------
