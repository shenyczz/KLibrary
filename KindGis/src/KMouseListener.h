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

#include "KMouseEvent.h"

namespace Mouse
{

	class KIND_EXT_CLASS KMouseListener
	{
	public:
		KMouseListener(void);
		virtual ~KMouseListener(void);

	public:
		// 鼠标左键被按下。
		void OnLButtonDown(KMouseEvent* pMouseEvent);

		// 鼠标左键被抬起。
		void OnLButtonUp(KMouseEvent* pMouseEvent);

		// 鼠标左键被双击。
		void OnLButtonDBLClick(KMouseEvent* pMouseEvent);

		// 鼠标右键被按下。
		void OnRButtonDown(KMouseEvent* pMouseEvent);

		// 鼠标右键被抬起。
		void OnRButtonUp(KMouseEvent* pMouseEvent);

		// 鼠标右键被双击。
		void OnRButtonDBLClick(KMouseEvent* pMouseEvent);

		// 鼠标移动
		void OnMouseMove(KMouseEvent* pMouseEvent);

		// 鼠标拖动
		void OnMouseDrag(KMouseEvent* pMouseEvent);

		// 取消操作。
		void OnCancel();

		// 鼠标盘旋在对象上方
		void OnMouseHover(KMouseEvent* pMouseEvent);

	public:
		// 鼠标事件
		KMouseEvent* GetMouseEvent() { return m_pMouseEvent; }
		//void SetMouseEvent(KMouseEvent* pMouseEvent) { _delete(m_pMouseEvent); m_pMouseEvent = pMouseEvent; }
		// 取得最近一次鼠标位置
		POINT GetLastPoint() { return m_lastPoint; }
		// 取得鼠标左键首次按下时位置
		POINT GetLButtonPoint() { return m_LButtonPoint; }

		// 图形绘制完成
		BOOL IsFinishedGeometry() { return m_bFinishedGeometry; }

		// 取得点序列
		vector<POINT>* GetPointSequence() { return &m_PointSequence; }

	private:
		POINT m_lastPoint;		// 鼠标最后位置
		POINT m_LButtonPoint;	// 鼠标左键首次按下时位置

		POINT m_pointPrv;

		// 图形绘制完成
		bool m_bFinishedGeometry;

		vector<POINT> m_PointSequence;

		KMouseEvent* m_pMouseEvent;	// 不负责内存管理
	};

	typedef map<int,KMouseListener*> KMouseListenerMap;

//---------------------------------------------------------
}// namespace Mouse - end
//---------------------------------------------------------
