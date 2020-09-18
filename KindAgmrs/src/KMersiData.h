#pragma once

#include "KLDFile.h"

namespace Agmrs { namespace Mersi
{

	class KIND_EXT_CLASS KMersiData : public KLDFile
	{
	public:
		KMersiData(void);
		virtual ~KMersiData(void);

	public:
		// ȡ���б���ֵ
		virtual void* GetThreshold(){ return GetMersiThreshold(); }

	public:
		// ����Ĭ�ϵ���ʾͨ��
		virtual void SetDefaultDisplayChannel();
		// ͨ����ת��Ϊͨ������
		virtual int Channel2ChannelCode(int iChannel);
		// ͨ������ת��Ϊͨ����
		virtual int ChannelCode2Channel(int iChannelCode);
		// ͨ������ת��Ϊͨ������
		virtual LPCTSTR ChannelCode2String(int iChannelCode);
	};

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------
