/******************************************************************************
 ** KVirrData.h
 **
 ** Announce: Written by ShenYongchen(shenyczz@163.com),ZhengZhou,HeNan.
 **           All rights reserved.
 **
 **     Name: KVirrData
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

#include "KLDFile.h"

namespace Agmrs { namespace Virr
{

	class KIND_EXT_CLASS KVirrData : public KLDFile
	{
	public:
		KVirrData(void);
		virtual ~KVirrData(void);

	public:
		// ȡ���б���ֵ
		virtual void* GetThreshold(){ return GetVirrThreshold(); }

	public:
		// ����Ĭ�ϵ���ʾͨ��
		virtual void SetDefaultDisplayChannel();
		// ͨ����ת��Ϊͨ������
		virtual int Channel2ChannelCode(int iChannel);
		// ͨ������ת��Ϊͨ����
		virtual int ChannelCode2Channel(int iChannelCode);
		// ͨ��ת��Ϊ�ַ���
		virtual LPCTSTR ChannelCode2String(int iChannelCode);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Virr - end
//---------------------------------------------------------
