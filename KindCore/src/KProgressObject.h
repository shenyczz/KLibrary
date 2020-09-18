/******************************************************************************
 ** KProgressObject.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KProgressObject
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

#include "KObject.h"

namespace Objects
{

	class KIND_EXT_CLASS KProgressObject : public KObject
	{
	public:
		KProgressObject(void)
			:KObject()
			,m_pCallbackFunc(NULL)
			,m_pCallbackParam(NULL)
			,m_bDeleteCallbackParam(FALSE)
		{
		}

		KProgressObject(const KProgressObject& obj)
			:KObject(obj)
			,m_pCallbackFunc(obj.m_pCallbackFunc)
			,m_pCallbackParam(obj.m_pCallbackParam)
			,m_bDeleteCallbackParam(obj.m_bDeleteCallbackParam)

		{
		}

		KProgressObject(LPCTSTR LpszName)
			:KObject(LpszName)
			,m_pCallbackFunc(NULL)
			,m_pCallbackParam(NULL)
			,m_bDeleteCallbackParam(FALSE)
		{
		}

		virtual ~KProgressObject(void)
		{
			if(m_bDeleteCallbackParam)
				_delete(m_pCallbackParam);

			m_pCallbackFunc = NULL;
			m_pCallbackParam = NULL;
		}

	public:
		// 注册进度显示回调函数
		// 注册前可以对回调参数作一些初始化
		void RegiestCallbackFunc(CALLBACK_FUNC pCallbackFunc, void* pCallbackParam, BOOL bDeleteCallbackParam = FALSE)
		{
			if(m_bDeleteCallbackParam && m_pCallbackParam)
			{
				delete m_pCallbackParam;
				m_pCallbackParam = NULL;
			}

			m_pCallbackFunc = pCallbackFunc;
			m_pCallbackParam = pCallbackParam;
			m_bDeleteCallbackParam = bDeleteCallbackParam;
		}


		// 取得回调函数
		CALLBACK_FUNC CallbackFunc() { return m_pCallbackFunc; }
		// 取得回调函数参数
		void*& CallbackParam() { return m_pCallbackParam; }
		// 是否删除参数指针
		BOOL IsDeleteCallbackParam() { return m_bDeleteCallbackParam; }

	protected:
		CALLBACK_FUNC	m_pCallbackFunc;		// 回调函数
		void*			m_pCallbackParam;		// 回调函数参数
		BOOL			m_bDeleteCallbackParam;	// 是否回收回调函数参数
	};

//---------------------------------------------------------
}// namespace Objects - end
//---------------------------------------------------------
