#include "StdAfx.h"
#include "KMersiData.h"
#include "KMersiDataProcessor.h"

namespace Agmrs { namespace Mersi
{

	KMersiData::KMersiData(void)
	{
		this->SetType(AgmrsDataSource::Mersi);

		m_pDataProcessor = new KMersiDataProcessor();
		m_pDataProcessor->SetOwner(this);
	}

	KMersiData::~KMersiData(void)
	{
	}

	// ����Ĭ�ϵ���ʾͨ��
	void KMersiData::SetDefaultDisplayChannel()
	{
		KLDFile::SetDefaultDisplayChannel();

		// ������Ϣ
		KDataInfo* pDataInfo = GetDataInfo();

		// ͨ������
		int iChannelNums = pDataInfo->ChannelNumbers();

		if(this->IsDay())
		{// ����
			SetDataChannelDef(3,4,3);
		}
		else
		{// ����
			int ch = this->ChannelCode2Channel(pDataInfo->ChannelCode(0));
			SetDataChannelDef(ch,ch,ch);
		}

		return;
	}

	// ͨ��ת��Ϊͨ������
	int KMersiData::Channel2ChannelCode(int iChannel)
	{
		return iChannel + 50;
	}

	// ͨ������ת��Ϊͨ��
	int KMersiData::ChannelCode2Channel(int iChannelCode)
	{
		return iChannelCode - 50;
	}

	// ͨ��ת��Ϊ�ַ���
	LPCTSTR KMersiData::ChannelCode2String(int iChannelCode)
	{
		KDataInfo* pDataInfo = this->GetDataInfo();
		int iChannelNums = pDataInfo->ChannelNumbers();
		_stprintf(m_szChannel,_T("%2d ͨ��"),ChannelCode2Channel(iChannelCode));
		return m_szChannel;
	}

//---------------------------------------------------------
}}// namespace Agmrs::Mersi - end
//---------------------------------------------------------
