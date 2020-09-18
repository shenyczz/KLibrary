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
		// ע�������ʾ�ص�����
		// ע��ǰ���ԶԻص�������һЩ��ʼ��
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


		// ȡ�ûص�����
		CALLBACK_FUNC CallbackFunc() { return m_pCallbackFunc; }
		// ȡ�ûص���������
		void*& CallbackParam() { return m_pCallbackParam; }
		// �Ƿ�ɾ������ָ��
		BOOL IsDeleteCallbackParam() { return m_bDeleteCallbackParam; }

	protected:
		CALLBACK_FUNC	m_pCallbackFunc;		// �ص�����
		void*			m_pCallbackParam;		// �ص���������
		BOOL			m_bDeleteCallbackParam;	// �Ƿ���ջص���������
	};

//---------------------------------------------------------
}// namespace Objects - end
//---------------------------------------------------------
